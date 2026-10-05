--[[
bofa_kaizo_tools.lua — mGBA debug/testing tool for the bofa Kaizo hack.

Adapted from EmeraldKaizo's EK.lua (credit: toxicenduser and the EK project,
see Reference/EmeraldKaizo-main/). EK.lua's hardcoded memory addresses are
specific to vanilla Emerald's compiled binary and do NOT work on bofa (bofa's
expansion codebase has a much larger data segment, shifting every EWRAM
address). The addresses below were pulled from THIS repo's own compiled
pokeemerald.elf and will need re-deriving any time the ROM is rebuilt with
code/data changes big enough to shift EWRAM layout (adding/removing globals,
changing struct sizes, etc.) They will NOT shift from ordinary gameplay data
changes (trainers.party edits, map edits) since those don't touch EWRAM
layout -- only C source/struct changes do.

To re-derive after a rebuild that might have shifted things:
    arm-none-eabi-nm pokeemerald.elf | grep -E ' gPlayerParty$| gPlayerPartyCount$| gEnemyParty$| gEnemyPartyCount$| gBattleWeather$| gSpeciesInfo$'
and for the gSpeciesInfo stride/name-offset specifically, see the empirical
derivation method in this session's PROGRESS.md if the struct changes again
(the header's struct size comments are NOT reliable -- this project's
SpeciesInfo struct has been extended well past its stale "/*0xC4*/" comment;
the real stride was found by searching the compiled ROM for a known species'
name and computing table_offset - known_index * candidate_stride).

VERIFIED against the actual running ROM (not just static file analysis): with
mGBA launched via `mGBA.exe -g` and a minimal Python GDB-remote-protocol
client (no gdb binary was available in this environment, so the protocol was
implemented directly -- it's simple enough), confirmed (a) reading 13 bytes
at species index 1's derived name address returns exactly Bulbasaur's
charmap-encoded name, byte for byte, and (b) writing 2 bytes to a party
mon's HP offset and reading it back returns the written value unchanged.
That validates the two riskiest assumptions this script depends on -- the
gSpeciesInfo address math, and the basic emu-style read/write mechanics --
against the real game, not just this reasoning. What's still unverified is
the Lua syntax/runtime itself (no Lua interpreter was available to load this
file and actually call its functions) and mGBA's exact scripting API surface
(clipboard availability, io sandboxing) -- those need an actual Tools >
Scripting session in mGBA.

Requirements: mGBA development build (Tools > Scripting > File > Load script).

Commands (type in the Scripting console):
    sethp(slot, hp)              -- set party mon's current HP (slot 1-6)
    setstatus(slot, name)        -- name: "poison","burn","paralysis","freeze",
                                     "sleep","toxic","frostbite","none"
    setweather(name)             -- name: "rain","sun","sand","hail",
                                     "strongwinds","none"
    printparty()                 -- print party summary to console
    exportparty()                -- export whole party to Showdown format:
                                     prints to console AND writes
                                     bofa_export.txt next to the ROM. Also
                                     TRIES an mGBA clipboard API if one
                                     exists in this build (wrapped in pcall
                                     so it never errors if absent) -- the
                                     file is the guaranteed way to get the
                                     text out; clipboard is a bonus if your
                                     mGBA build has it.
    exportmon(slot)               -- export a single party slot the same way
    heal(slot)                    -- restore one party mon to full HP and cure status
    healall()                     -- heal(slot) for the whole party

HP/status edits change the PARTY copy. Use them outside battle (or before
sending a mon in); a mon already out in battle keeps its in-battle HP/status.

gSpeciesInfo lives in ROM and moves whenever game data changes size, so the
script finds it at load time by searching the ROM for Bulbasaur/Ivysaur/
Venusaur names 260 bytes apart. The hardcoded value below is only a fallback.

Known limitation: species names are read directly from the ROM (works for
every species bofa has, vanilla or newly added). Move names use a hardcoded
vanilla Gen1-3 table (bofa's own additions past move ~354 print as "Move
#N" -- look up the number in include/constants/moves.h). Items/abilities
print as "Item #N"/"Ability #N" (numeric only) -- not implemented with
name tables in this pass, scoped out for time. Nature names are always
correct (computed from personality, not a lookup table).
]]

local ADDR = {
    gPlayerParty      = 0x02035694,
    gPlayerPartyCount = 0x02035691,
    gEnemyParty       = 0x020358ec,
    gEnemyPartyCount  = 0x02035692,
    gBattleWeather    = 0x02000754,
    gSpeciesInfoBase  = 0x08ce6100,   -- fallback; located at load time (see locateSpeciesInfo)
}

local PARTY_MON_SIZE = 100
local SPECIES_INFO_STRIDE = 260
local SPECIES_NAME_OFFSET = 44
local SPECIES_NAME_LEN = 12

-- struct Pokemon field offsets (standard Gen3 layout, unchanged by bofa)
local OFF_PERSONALITY = 0
local OFF_OTID = 4
local OFF_CHECKSUM = 28
local OFF_SUBSTRUCT_START = 32
local OFF_STATUS = 80
local OFF_LEVEL = 84
local OFF_HP = 86
local OFF_MAXHP = 88

local STATUS1 = {
    poison    = 8,     -- 1 << 3
    burn      = 16,    -- 1 << 4
    freeze    = 32,    -- 1 << 5
    paralysis = 64,    -- 1 << 6
    toxic     = 128,   -- 1 << 7
    frostbite = 4096,  -- 1 << 12
}

local B_WEATHER = {
    rain        = 4,     -- 1 << 2, permanent variant, lasts until cleared
    sun         = 128,   -- 1 << 7
    sand        = 32,    -- 1 << 5
    hail        = 1024,  -- 1 << 10
    strongwinds = 2048,  -- 1 << 11
    none        = 0,
}

local NATURE_NAMES = {
    "Hardy","Lonely","Brave","Adamant","Naughty",
    "Bold","Docile","Relaxed","Impish","Lax",
    "Timid","Hasty","Serious","Jolly","Naive",
    "Modest","Mild","Quiet","Bashful","Rash",
    "Calm","Gentle","Sassy","Careful","Quirky",
}

-- Vanilla Gen1-3 move names (index = move ID). bofa's own additions beyond
-- this range (TMs 51-66 and everything from the Gen4-9 movepool expansion)
-- print as "Move #N" instead -- ported from EK.lua's table, which itself
-- only covers this same vanilla range.
local MOVE_NAMES = {
    [0]="-",[1]="Pound",[2]="Karate Chop",[3]="Double Slap",[4]="Comet Punch",[5]="Mega Punch",
    [6]="Pay Day",[7]="Fire Punch",[8]="Ice Punch",[9]="Thunder Punch",[10]="Scratch",
    [11]="Vice Grip",[12]="Guillotine",[13]="Razor Wind",[14]="Swords Dance",[15]="Cut",
    [16]="Gust",[17]="Wing Attack",[18]="Whirlwind",[19]="Fly",[20]="Bind",
    [21]="Slam",[22]="Vine Whip",[23]="Stomp",[24]="Double Kick",[25]="Mega Kick",
    [26]="Jump Kick",[27]="Rolling Kick",[28]="Sand Attack",[29]="Headbutt",[30]="Horn Attack",
    [31]="Fury Attack",[32]="Horn Drill",[33]="Tackle",[34]="Body Slam",[35]="Wrap",
    [36]="Take Down",[37]="Thrash",[38]="Double-Edge",[39]="Tail Whip",[40]="Poison Sting",
    [41]="Twineedle",[42]="Pin Missile",[43]="Leer",[44]="Bite",[45]="Growl",
    [46]="Roar",[47]="Sing",[48]="Supersonic",[49]="Sonic Boom",[50]="Disable",
    [51]="Acid",[52]="Ember",[53]="Flamethrower",[54]="Mist",[55]="Water Gun",
    [56]="Hydro Pump",[57]="Surf",[58]="Ice Beam",[59]="Blizzard",[60]="Psybeam",
    [61]="Bubble Beam",[62]="Aurora Beam",[63]="Hyper Beam",[64]="Peck",[65]="Drill Peck",
    [66]="Submission",[67]="Low Kick",[68]="Counter",[69]="Seismic Toss",[70]="Strength",
    [71]="Absorb",[72]="Mega Drain",[73]="Leech Seed",[74]="Growth",[75]="Razor Leaf",
    [76]="Solar Beam",[77]="Poison Powder",[78]="Stun Spore",[79]="Sleep Powder",[80]="Petal Dance",
    [81]="String Shot",[82]="Dragon Rage",[83]="Fire Spin",[84]="Thunder Shock",[85]="Thunderbolt",
    [86]="Thunder Wave",[87]="Thunder",[88]="Rock Throw",[89]="Earthquake",[90]="Fissure",
    [91]="Dig",[92]="Toxic",[93]="Confusion",[94]="Psychic",[95]="Hypnosis",
    [96]="Meditate",[97]="Agility",[98]="Quick Attack",[99]="Rage",[100]="Teleport",
    [101]="Night Shade",[102]="Mimic",[103]="Screech",[104]="Double Team",[105]="Recover",
    [106]="Harden",[107]="Minimize",[108]="Smokescreen",[109]="Confuse Ray",[110]="Withdraw",
    [111]="Defense Curl",[112]="Barrier",[113]="Light Screen",[114]="Haze",[115]="Reflect",
    [116]="Focus Energy",[117]="Bide",[118]="Metronome",[119]="Mirror Move",[120]="Self-Destruct",
    [121]="Egg Bomb",[122]="Lick",[123]="Smog",[124]="Sludge",[125]="Bone Club",
    [126]="Fire Blast",[127]="Waterfall",[128]="Clamp",[129]="Swift",[130]="Skull Bash",
    [131]="Spike Cannon",[132]="Constrict",[133]="Amnesia",[134]="Kinesis",[135]="Soft-Boiled",
    [136]="Hi Jump Kick",[137]="Glare",[138]="Dream Eater",[139]="Poison Gas",[140]="Barrage",
    [141]="Leech Life",[142]="Lovely Kiss",[143]="Sky Attack",[144]="Transform",[145]="Bubble",
    [146]="Dizzy Punch",[147]="Spore",[148]="Flash",[149]="Psywave",[150]="Splash",
    [151]="Acid Armor",[152]="Crabhammer",[153]="Explosion",[154]="Fury Swipes",[155]="Bonemerang",
    [156]="Rest",[157]="Rock Slide",[158]="Hyper Fang",[159]="Sharpen",[160]="Conversion",
    [161]="Tri Attack",[162]="Super Fang",[163]="Slash",[164]="Substitute",[165]="Struggle",
    [166]="Sketch",[167]="Triple Kick",[168]="Thief",[169]="Spider Web",[170]="Mind Reader",
    [171]="Nightmare",[172]="Flame Wheel",[173]="Snore",[174]="Curse",[175]="Flail",
    [176]="Conversion 2",[177]="Aeroblast",[178]="Cotton Spore",[179]="Reversal",[180]="Spite",
    [181]="Powder Snow",[182]="Protect",[183]="Mach Punch",[184]="Scary Face",[185]="Feint Attack",
    [186]="Sweet Kiss",[187]="Belly Drum",[188]="Sludge Bomb",[189]="Mud-Slap",[190]="Octazooka",
    [191]="Spikes",[192]="Zap Cannon",[193]="Foresight",[194]="Destiny Bond",[195]="Perish Song",
    [196]="Icy Wind",[197]="Detect",[198]="Bone Rush",[199]="Lock-On",[200]="Outrage",
    [201]="Sandstorm",[202]="Giga Drain",[203]="Endure",[204]="Charm",[205]="Rollout",
    [206]="False Swipe",[207]="Swagger",[208]="Milk Drink",[209]="Spark",[210]="Fury Cutter",
    [211]="Steel Wing",[212]="Mean Look",[213]="Attract",[214]="Sleep Talk",[215]="Heal Bell",
    [216]="Return",[217]="Present",[218]="Frustration",[219]="Safeguard",[220]="Pain Split",
    [221]="Sacred Fire",[222]="Magnitude",[223]="Dynamic Punch",[224]="Megahorn",[225]="Dragon Breath",
    [226]="Baton Pass",[227]="Encore",[228]="Pursuit",[229]="Rapid Spin",[230]="Sweet Scent",
    [231]="Iron Tail",[232]="Metal Claw",[233]="Vital Throw",[234]="Morning Sun",[235]="Synthesis",
    [236]="Moonlight",[237]="Hidden Power",[238]="Cross Chop",[239]="Twister",[240]="Rain Dance",
    [241]="Sunny Day",[242]="Crunch",[243]="Mirror Coat",[244]="Psych Up",[245]="Extreme Speed",
    [246]="Ancient Power",[247]="Shadow Ball",[248]="Future Sight",[249]="Rock Smash",[250]="Whirlpool",
    [251]="Beat Up",[252]="Fake Out",[253]="Uproar",[254]="Stockpile",[255]="Spit Up",
    [256]="Swallow",[257]="Heat Wave",[258]="Hail",[259]="Torment",[260]="Flatter",
    [261]="Will-O-Wisp",[262]="Memento",[263]="Facade",[264]="Focus Punch",[265]="Smelling Salts",
    [266]="Follow Me",[267]="Nature Power",[268]="Charge",[269]="Taunt",[270]="Helping Hand",
    [271]="Trick",[272]="Role Play",[273]="Wish",[274]="Assist",[275]="Ingrain",
    [276]="Superpower",[277]="Magic Coat",[278]="Recycle",[279]="Revenge",[280]="Brick Break",
    [281]="Yawn",[282]="Knock Off",[283]="Endeavor",[284]="Eruption",[285]="Skill Swap",
    [286]="Imprison",[287]="Refresh",[288]="Grudge",[289]="Snatch",[290]="Secret Power",
    [291]="Dive",[292]="Arm Thrust",[293]="Camouflage",[294]="Tail Glow",[295]="Luster Purge",
    [296]="Mist Ball",[297]="Feather Dance",[298]="Teeter Dance",[299]="Blaze Kick",[300]="Mud Sport",
    [301]="Ice Ball",[302]="Needle Arm",[303]="Slack Off",[304]="Hyper Voice",[305]="Poison Fang",
    [306]="Crush Claw",[307]="Blast Burn",[308]="Hydro Cannon",[309]="Meteor Mash",[310]="Astonish",
    [311]="Weather Ball",[312]="Aromatherapy",[313]="Fake Tears",[314]="Air Cutter",[315]="Overheat",
    [316]="Odor Sleuth",[317]="Rock Tomb",[318]="Silver Wind",[319]="Metal Sound",[320]="Grass Whistle",
    [321]="Tickle",[322]="Cosmic Power",[323]="Water Spout",[324]="Signal Beam",[325]="Shadow Punch",
    [326]="Extrasensory",[327]="Sky Uppercut",[328]="Sand Tomb",[329]="Sheer Cold",[330]="Muddy Water",
    [331]="Bullet Seed",[332]="Aerial Ace",[333]="Icicle Spear",[334]="Iron Defense",[335]="Block",
    [336]="Howl",[337]="Dragon Claw",[338]="Frenzy Plant",[339]="Bulk Up",[340]="Bounce",
    [341]="Mud Shot",[342]="Poison Tail",[343]="Covet",[344]="Volt Tackle",[345]="Magical Leaf",
    [346]="Water Sport",[347]="Calm Mind",[348]="Leaf Blade",[349]="Dragon Dance",[350]="Rock Blast",
    [351]="Shock Wave",[352]="Water Pulse",[353]="Doom Desire",[354]="Psycho Boost",
}

-- ==========================================================================
-- low-level reads
-- ==========================================================================

local function partyAddr(base, slot)
    return base + (slot - 1) * PARTY_MON_SIZE
end

-- Pure-arithmetic bit operations. mGBA's embedded Lua version is not
-- guaranteed to support native bitwise operators (~, &, |, <<, >>) -- EK.lua
-- itself only ever manually implements XOR for exactly this reason, so
-- these are written the same defensive way and cover AND/OR/shift too,
-- since this script needs all of them for the Gen3 substructure decryption.
local function bxor(a, b)
    local p, c = 1, 0
    while a > 0 and b > 0 do
        local ra, rb = a % 2, b % 2
        if ra ~= rb then c = c + p end
        a, b, p = (a - ra) / 2, (b - rb) / 2, p * 2
    end
    if a < b then a = b end
    while a > 0 do
        local ra = a % 2
        if ra > 0 then c = c + p end
        a, p = (a - ra) / 2, p * 2
    end
    return c
end

local function band(a, b)
    local p, c = 1, 0
    while a > 0 and b > 0 do
        local ra, rb = a % 2, b % 2
        if ra == 1 and rb == 1 then c = c + p end
        a, b, p = (a - ra) / 2, (b - rb) / 2, p * 2
    end
    return c
end

local function rshift(a, n)
    return math.floor(a / (2 ^ n))
end

local substructSelector = {
    [0]={0,1,2,3},[1]={0,1,3,2},[2]={0,2,1,3},[3]={0,3,1,2},[4]={0,2,3,1},[5]={0,3,2,1},
    [6]={1,0,2,3},[7]={1,0,3,2},[8]={2,0,1,3},[9]={3,0,1,2},[10]={2,0,3,1},[11]={3,0,2,1},
    [12]={1,2,0,3},[13]={1,3,0,2},[14]={2,1,0,3},[15]={3,1,0,2},[16]={2,3,0,1},[17]={3,2,0,1},
    [18]={1,2,3,0},[19]={1,3,2,0},[20]={2,1,3,0},[21]={3,1,2,0},[22]={2,3,1,0},[23]={3,2,1,0},
}

-- Reads and decrypts the 4 substructures of a party mon (species, held item,
-- exp, moves/pp, EVs, IVs, ability slot, etc.) Standard Gen3 encryption,
-- unchanged by bofa.
local function readMon(baseAddr, slot)
    local addr = partyAddr(baseAddr, slot)
    local personality = emu:read32(addr + OFF_PERSONALITY)
    local otId = emu:read32(addr + OFF_OTID)
    local key = bxor(personality, otId)
    local pSel = substructSelector[personality % 24]

    local ss = {{}, {}, {}, {}}
    for sub = 1, 4 do
        for i = 0, 2 do
            ss[sub][i] = bxor(emu:read32(addr + OFF_SUBSTRUCT_START + pSel[sub] * 12 + i * 4), key)
        end
    end

    local mon = {}
    mon.personality = personality
    mon.otId = otId
    mon.species = band(ss[1][0], 0xFFFF)
    mon.heldItem = rshift(ss[1][0], 16)
    mon.experience = ss[1][1]
    mon.moves = {
        band(ss[2][0], 0xFFFF),
        rshift(ss[2][0], 16),
        band(ss[2][1], 0xFFFF),
        rshift(ss[2][1], 16),
    }
    mon.hpEV      = band(ss[3][0], 0xFF)
    mon.atkEV     = band(rshift(ss[3][0], 8), 0xFF)
    mon.defEV     = band(rshift(ss[3][0], 16), 0xFF)
    mon.speedEV   = rshift(ss[3][0], 24)
    mon.spatkEV   = band(ss[3][1], 0xFF)
    mon.spdefEV   = band(rshift(ss[3][1], 8), 0xFF)

    local flags = ss[4][1]
    mon.hpIV    = band(flags, 0x1F)
    mon.atkIV   = band(rshift(flags, 5), 0x1F)
    mon.defIV   = band(rshift(flags, 10), 0x1F)
    mon.speedIV = band(rshift(flags, 15), 0x1F)
    mon.spatkIV = band(rshift(flags, 20), 0x1F)
    mon.spdefIV = band(rshift(flags, 25), 0x1F)
    mon.abilityNum = band(rshift(flags, 31), 1)

    mon.status = emu:read32(addr + OFF_STATUS)
    mon.level = emu:read8(addr + OFF_LEVEL)
    mon.hp = emu:read16(addr + OFF_HP)
    mon.maxHp = emu:read16(addr + OFF_MAXHP)
    mon.nature = personality % 25
    return mon
end

-- Gen 3 charmap -> ASCII (enough for species names)
local function decodeChar(b)
    if b >= 0xBB and b <= 0xD4 then return string.char(65 + b - 0xBB) end   -- A-Z
    if b >= 0xD5 and b <= 0xEE then return string.char(97 + b - 0xD5) end   -- a-z
    if b >= 0xA1 and b <= 0xAA then return string.char(48 + b - 0xA1) end   -- 0-9
    local special = { [0x00]=" ", [0xAB]="!", [0xAC]="?", [0xAD]=".", [0xAE]="-",
                      [0xB4]="'", [0xB5]="M", [0xB6]="F", [0xB8]=",", [0xBA]="/",
                      [0xF0]=":", [0x1B]="e" }
    return special[b] or "?"
end

local function encodeName(str)
    local out = {}
    for i = 1, #str do
        local c = str:byte(i)
        if c >= 65 and c <= 90 then out[#out+1] = string.char(0xBB + c - 65)
        elseif c >= 97 and c <= 122 then out[#out+1] = string.char(0xD5 + c - 97) end
    end
    out[#out+1] = string.char(0xFF)
    return table.concat(out)
end

local function locateSpeciesInfo()
    local bulba, ivy, venu = encodeName("Bulbasaur"), encodeName("Ivysaur"), encodeName("Venusaur")
    local CHUNK, OVERLAP = 0x10000, 16
    local romStart, romEnd = 0x08000000, 0x0A000000
    for addr = romStart, romEnd - 1, CHUNK do
        local ok, data = pcall(function() return emu:readRange(addr, CHUNK + OVERLAP) end)
        if not ok or data == nil then break end
        local pos = 1
        while true do
            local i = data:find(bulba, pos, true)
            if not i then break end
            local nameAddr = addr + i - 1
            if emu:readRange(nameAddr + SPECIES_INFO_STRIDE, #ivy) == ivy
               and emu:readRange(nameAddr + 2 * SPECIES_INFO_STRIDE, #venu) == venu then
                return nameAddr - SPECIES_INFO_STRIDE - SPECIES_NAME_OFFSET
            end
            pos = i + 1
        end
    end
    return nil
end

local function getSpeciesName(id)
    if id == 0 then return "None" end
    local nameAddr = ADDR.gSpeciesInfoBase + id * SPECIES_INFO_STRIDE + SPECIES_NAME_OFFSET
    local bytes = emu:readRange(nameAddr, SPECIES_NAME_LEN)
    -- Species names are plain ASCII-range text in this ROM's charmap for the
    -- overwhelming majority of entries (accented/foreign characters are rare
    -- in species names specifically); this direct byte read works for those.
    -- mGBA's readRange returns a Lua string already decoded 1:1 from memory
    -- bytes -- if a name looks wrong/garbled for a specific species, that
    -- species likely uses a special character this simple approach doesn't
    -- handle; cross-check against src/data/pokemon/species_info/*.h.
    local out = {}
    for i = 1, #bytes do
        local b = bytes:byte(i)
        if b == 0xFF then break end
        out[#out+1] = decodeChar(b)
    end
    return table.concat(out)
end

local function getMoveName(id)
    return MOVE_NAMES[id] or ("Move #" .. id)
end

-- ==========================================================================
-- write / mutate functions
-- ==========================================================================

function sethp(slot, hp)
    local addr = partyAddr(ADDR.gPlayerParty, slot)
    emu:write16(addr + OFF_HP, hp)
    console:log(string.format("Set slot %d HP to %d", slot, hp))
end

function setstatus(slot, name)
    local addr = partyAddr(ADDR.gPlayerParty, slot)
    name = string.lower(name)
    local value
    if name == "none" then
        value = 0
    elseif name == "sleep" then
        value = 3 -- 3 turns
    elseif STATUS1[name] then
        value = STATUS1[name]
    else
        console:error("Unknown status: " .. name .. " (use poison/burn/paralysis/freeze/sleep/toxic/frostbite/none)")
        return
    end
    emu:write32(addr + OFF_STATUS, value)
    console:log(string.format("Set slot %d status to %s", slot, name))
end

function setweather(name)
    name = string.lower(name)
    local value = B_WEATHER[name]
    if value == nil then
        console:error("Unknown weather: " .. name .. " (use rain/sun/sand/hail/strongwinds/none)")
        return
    end
    emu:write16(ADDR.gBattleWeather, value)
    console:log("Set battle weather to " .. name .. " (only takes effect during an active battle)")
end

-- ==========================================================================
-- export / display
-- ==========================================================================

local function monToShowdownText(mon, label)
    local lines = {}
    local speciesName = getSpeciesName(mon.species)
    lines[#lines+1] = string.format("%s (%s) @ Item #%d", label or speciesName, speciesName, mon.heldItem)
    lines[#lines+1] = string.format("Ability: Ability #%d", mon.abilityNum)
    lines[#lines+1] = string.format("Level: %d", mon.level)
    lines[#lines+1] = string.format("EVs: %d HP / %d Atk / %d Def / %d SpA / %d SpD / %d Spe",
        mon.hpEV, mon.atkEV, mon.defEV, mon.spatkEV, mon.spdefEV, mon.speedEV)
    lines[#lines+1] = (NATURE_NAMES[mon.nature + 1] or "Hardy") .. " Nature"
    lines[#lines+1] = string.format("IVs: %d HP / %d Atk / %d Def / %d SpA / %d SpD / %d Spe",
        mon.hpIV, mon.atkIV, mon.defIV, mon.spatkIV, mon.spdefIV, mon.speedIV)
    for i = 1, 4 do
        if mon.moves[i] and mon.moves[i] ~= 0 then
            lines[#lines+1] = "- " .. getMoveName(mon.moves[i])
        end
    end
    return table.concat(lines, "\n")
end

local function tryClipboard(text)
    -- Not all mGBA builds expose a scripting clipboard API. Try a few
    -- plausible names and silently continue if none exist -- the file
    -- output below is the guaranteed path to get the text out.
    local candidates = {
        function() return _G.clipboard and _G.clipboard.set and _G.clipboard.set(text) end,
        function() return _G.emu and _G.emu.setClipboardText and _G.emu:setClipboardText(text) end,
        function() return _G.C and _G.C.setClipboard and _G.C.setClipboard(text) end,
    }
    for _, try in ipairs(candidates) do
        local ok, err = pcall(try)
        if ok and err ~= false then
            return true
        end
    end
    return false
end

local function writeExportFile(text)
    local ok, f = pcall(io.open, "bofa_export.txt", "w")
    if ok and f then
        f:write(text)
        f:close()
        return true
    end
    return false
end

function exportmon(slot)
    local mon = readMon(ADDR.gPlayerParty, slot)
    local text = monToShowdownText(mon)
    console:log(text)
    local wroteFile = writeExportFile(text)
    local gotClipboard = tryClipboard(text)
    console:log(string.format("[export: file=%s clipboard=%s]", tostring(wroteFile), tostring(gotClipboard)))
end

function exportparty()
    local count = emu:read8(ADDR.gPlayerPartyCount)
    local chunks = {}
    for slot = 1, count do
        local mon = readMon(ADDR.gPlayerParty, slot)
        if mon.species ~= 0 then
            chunks[#chunks+1] = monToShowdownText(mon)
        end
    end
    local text = table.concat(chunks, "\n\n")
    console:log(text)
    local wroteFile = writeExportFile(text)
    local gotClipboard = tryClipboard(text)
    console:log(string.format("[export: file=bofa_export.txt written=%s clipboard=%s]", tostring(wroteFile), tostring(gotClipboard)))
    if not gotClipboard then
        console:log("(no scripting clipboard API found in this mGBA build -- open bofa_export.txt instead, it's next to wherever mGBA's working directory is, usually the ROM's folder)")
    end
end

function printparty()
    local count = emu:read8(ADDR.gPlayerPartyCount)
    for slot = 1, count do
        local mon = readMon(ADDR.gPlayerParty, slot)
        if mon.species ~= 0 then
            console:log(string.format("Slot %d: %s Lv%d  HP %d/%d  status=0x%X",
                slot, getSpeciesName(mon.species), mon.level, mon.hp, mon.maxHp, mon.status))
        end
    end
end

function heal(slot)
    local addr = partyAddr(ADDR.gPlayerParty, slot)
    emu:write16(addr + OFF_HP, emu:read16(addr + OFF_MAXHP))
    emu:write32(addr + OFF_STATUS, 0)
    console:log(string.format("Healed slot %d (full HP, no status)", slot))
end

function healall()
    for slot = 1, emu:read8(ADDR.gPlayerPartyCount) do heal(slot) end
end

do
    local found = locateSpeciesInfo()
    if found then
        ADDR.gSpeciesInfoBase = found
        console:log(string.format("gSpeciesInfo found at 0x%08X", found))
    else
        console:log(string.format("gSpeciesInfo search failed; using fallback 0x%08X (names may be wrong)", ADDR.gSpeciesInfoBase))
    end
end

console:log("bofa_kaizo_tools.lua loaded. Try: printparty(), exportparty(), sethp(1,1), setstatus(1,'burn'), setweather('rain'), heal(1), healall()")
