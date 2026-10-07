--[[
bofa_kaizo_tools.lua -- mGBA scripting tool for BOFA playtesting.

Same commands (and names) as EmeraldKaizo's EK.lua (credit: the EK project,
see Reference/EmeraldKaizo-main/), rebuilt for BOFA's pokeemerald-expansion
memory layout: 11-bit species/moves, 21-bit experience, 10-bit held items,
expansion names for every move/item/ability.

Load: mGBA (0.10+) > Tools > Scripting > File > Load script > this file.
Type commands into the scripting console. Type help() to list them.

The GENERATED block below (RAM addresses, move/item/ability names, move PP)
comes from the built ROM. After a rebuild that changes C code or data, run:
    python tools/kaizo_lua/gen_tables.py
(Claude does this automatically when it rebuilds BOFA.)

Edits change the PARTY copy of a Pokemon: use them outside battle.
]]

-- BEGIN GENERATED (tools/kaizo_lua/gen_tables.py) -- do not edit by hand
local ADDR = {
    gPlayerParty       = 0x02035694,
    gPlayerPartyCount  = 0x02035691,
    gPokemonStoragePtr = 0x0300722c,
    gBattleWeather     = 0x02000754,
    gSpeciesInfoBase   = 0x08ce83e0,
}
local SPECIES_INFO_STRIDE = 260
local SPECIES_NAME_OFFSET = 44
local MOVE_NAMES = {[0]="-",[1]="Pound",[2]="Karate Chop",[3]="Double Slap",[4]="Comet Punch",[5]="Mega Punch",[6]="Pay Day",[7]="Fire Punch",[8]="Ice Punch",[9]="Thunder Punch",[10]="Scratch",[11]="Vise Grip",[12]="Guillotine",[13]="Razor Wind",[14]="Swords Dance",[15]="Cut",[16]="Gust",[17]="Wing Attack",[18]="Whirlwind",[19]="Fly",[20]="Bind",[21]="Slam",[22]="Vine Whip",[23]="Stomp",[24]="Double Kick",[25]="Mega Kick",[26]="Jump Kick",[27]="Rolling Kick",[28]="Sand Attack",[29]="Headbutt",[30]="Horn Attack",[31]="Fury Attack",[32]="Horn Drill",[33]="Tackle",[34]="Body Slam",[35]="Wrap",[36]="Take Down",[37]="Thrash",[38]="Double-Edge",[39]="Tail Whip",[40]="Poison Sting",[41]="Twineedle",[42]="Pin Missile",[43]="Leer",[44]="Bite",[45]="Growl",[46]="Roar",[47]="Sing",[48]="Supersonic",[49]="Sonic Boom",[50]="Disable",[51]="Acid",[52]="Ember",[53]="Flamethrower",[54]="Mist",[55]="Water Gun",[56]="Hydro Pump",[57]="Surf",[58]="Ice Beam",[59]="Blizzard",[60]="Psybeam",[61]="Bubble Beam",[62]="Aurora Beam",[63]="Hyper Beam",[64]="Peck",[65]="Drill Peck",[66]="Submission",[67]="Low Kick",[68]="Counter",[69]="Seismic Toss",[70]="Strength",[71]="Absorb",[72]="Mega Drain",[73]="Leech Seed",[74]="Growth",[75]="Razor Leaf",[76]="Solar Beam",[77]="Poison Powder",[78]="Stun Spore",[79]="Sleep Powder",[80]="Petal Dance",[81]="String Shot",[82]="Dragon Rage",[83]="Fire Spin",[84]="Thunder Shock",[85]="Thunderbolt",[86]="Thunder Wave",[87]="Thunder",[88]="Rock Throw",[89]="Earthquake",[90]="Fissure",[91]="Dig",[92]="Toxic",[93]="Confusion",[94]="Psychic",[95]="Hypnosis",[96]="Meditate",[97]="Agility",[98]="Quick Attack",[99]="Rage",[100]="Teleport",[101]="Night Shade",[102]="Mimic",[103]="Screech",[104]="Double Team",[105]="Recover",[106]="Harden",[107]="Minimize",[108]="Smokescreen",[109]="Confuse Ray",[110]="Withdraw",[111]="Defense Curl",[112]="Barrier",[113]="Light Screen",[114]="Haze",[115]="Reflect",[116]="Focus Energy",[117]="Bide",[118]="Metronome",[119]="Mirror Move",[120]="Self-Destruct",[121]="Egg Bomb",[122]="Lick",[123]="Smog",[124]="Sludge",[125]="Bone Club",[126]="Fire Blast",[127]="Waterfall",[128]="Clamp",[129]="Swift",[130]="Skull Bash",[131]="Spike Cannon",[132]="Constrict",[133]="Amnesia",[134]="Kinesis",[135]="Soft-Boiled",[136]="High Jump Kick",[137]="Glare",[138]="Dream Eater",[139]="Poison Gas",[140]="Barrage",[141]="Leech Life",[142]="Lovely Kiss",[143]="Sky Attack",[144]="Transform",[145]="Bubble",[146]="Dizzy Punch",[147]="Spore",[148]="Flash",[149]="Psywave",[150]="Splash",[151]="Acid Armor",[152]="Crabhammer",[153]="Explosion",[154]="Fury Swipes",[155]="Bonemerang",[156]="Rest",[157]="Rock Slide",[158]="Hyper Fang",[159]="Sharpen",[160]="Conversion",[161]="Tri Attack",[162]="Super Fang",[163]="Slash",[164]="Substitute",[165]="Struggle",[166]="Sketch",[167]="Triple Kick",[168]="Thief",[169]="Spider Web",[170]="Mind Reader",[171]="Nightmare",[172]="Flame Wheel",[173]="Snore",[174]="Curse",[175]="Flail",[176]="Conversion 2",[177]="Aeroblast",[178]="Cotton Spore",[179]="Reversal",[180]="Spite",[181]="Powder Snow",[182]="Protect",[183]="Mach Punch",[184]="Scary Face",[185]="Feint Attack",[186]="Sweet Kiss",[187]="Belly Drum",[188]="Sludge Bomb",[189]="Mud-Slap",[190]="Octazooka",[191]="Spikes",[192]="Zap Cannon",[193]="Foresight",[194]="Destiny Bond",[195]="Perish Song",[196]="Icy Wind",[197]="Detect",[198]="Bone Rush",[199]="Lock-On",[200]="Outrage",[201]="Sandstorm",[202]="Giga Drain",[203]="Endure",[204]="Charm",[205]="Rollout",[206]="False Swipe",[207]="Swagger",[208]="Milk Drink",[209]="Spark",[210]="Fury Cutter",[211]="Steel Wing",[212]="Mean Look",[213]="Attract",[214]="Sleep Talk",[215]="Heal Bell",[216]="Return",[217]="Present",[218]="Frustration",[219]="Safeguard",[220]="Pain Split",[221]="Sacred Fire",[222]="Magnitude",[223]="Dynamic Punch",[224]="Megahorn",[225]="Dragon Breath",[226]="Baton Pass",[227]="Encore",[228]="Pursuit",[229]="Rapid Spin",[230]="Sweet Scent",[231]="Iron Tail",[232]="Metal Claw",[233]="Vital Throw",[234]="Morning Sun",[235]="Synthesis",[236]="Moonlight",[237]="Hidden Power",[238]="Cross Chop",[239]="Twister",[240]="Rain Dance",[241]="Sunny Day",[242]="Crunch",[243]="Mirror Coat",[244]="Psych Up",[245]="Extreme Speed",[246]="Ancient Power",[247]="Shadow Ball",[248]="Future Sight",[249]="Rock Smash",[250]="Whirlpool",[251]="Beat Up",[252]="Fake Out",[253]="Uproar",[254]="Stockpile",[255]="Spit Up",[256]="Swallow",[257]="Heat Wave",[258]="Hail",[259]="Torment",[260]="Flatter",[261]="Will-O-Wisp",[262]="Memento",[263]="Facade",[264]="Focus Punch",[265]="Smelling Salts",[266]="Follow Me",[267]="Nature Power",[268]="Charge",[269]="Taunt",[270]="Helping Hand",[271]="Trick",[272]="Role Play",[273]="Wish",[274]="Assist",[275]="Ingrain",[276]="Superpower",[277]="Magic Coat",[278]="Recycle",[279]="Revenge",[280]="Brick Break",[281]="Yawn",[282]="Knock Off",[283]="Endeavor",[284]="Eruption",[285]="Skill Swap",[286]="Imprison",[287]="Refresh",[288]="Grudge",[289]="Snatch",[290]="Secret Power",[291]="Dive",[292]="Arm Thrust",[293]="Camouflage",[294]="Tail Glow",[295]="Luster Purge",[296]="Mist Ball",[297]="Feather Dance",[298]="Teeter Dance",[299]="Blaze Kick",[300]="Mud Sport",[301]="Ice Ball",[302]="Needle Arm",[303]="Slack Off",[304]="Hyper Voice",[305]="Poison Fang",[306]="Crush Claw",[307]="Blast Burn",[308]="Hydro Cannon",[309]="Meteor Mash",[310]="Astonish",[311]="Weather Ball",[312]="Aromatherapy",[313]="Fake Tears",[314]="Air Cutter",[315]="Overheat",[316]="Odor Sleuth",[317]="Rock Tomb",[318]="Silver Wind",[319]="Metal Sound",[320]="Grass Whistle",[321]="Tickle",[322]="Cosmic Power",[323]="Water Spout",[324]="Signal Beam",[325]="Shadow Punch",[326]="Extrasensory",[327]="Sky Uppercut",[328]="Sand Tomb",[329]="Sheer Cold",[330]="Muddy Water",[331]="Bullet Seed",[332]="Aerial Ace",[333]="Icicle Spear",[334]="Iron Defense",[335]="Block",[336]="Howl",[337]="Dragon Claw",[338]="Frenzy Plant",[339]="Bulk Up",[340]="Bounce",[341]="Mud Shot",[342]="Poison Tail",[343]="Covet",[344]="Volt Tackle",[345]="Magical Leaf",[346]="Water Sport",[347]="Calm Mind",[348]="Leaf Blade",[349]="Dragon Dance",[350]="Rock Blast",[351]="Shock Wave",[352]="Water Pulse",[353]="Doom Desire",[354]="Psycho Boost",[355]="Roost",[356]="Gravity",[357]="Miracle Eye",[358]="Wake-Up Slap",[359]="Hammer Arm",[360]="Gyro Ball",[361]="Healing Wish",[362]="Brine",[363]="Natural Gift",[364]="Feint",[365]="Pluck",[366]="Tailwind",[367]="Acupressure",[368]="Metal Burst",[369]="U-turn",[370]="Close Combat",[371]="Payback",[372]="Assurance",[373]="Embargo",[374]="Fling",[375]="Psycho Shift",[376]="Trump Card",[377]="Heal Block",[378]="Wring Out",[379]="Power Trick",[380]="Gastro Acid",[381]="Lucky Chant",[382]="Me First",[383]="Copycat",[384]="Power Swap",[385]="Guard Swap",[386]="Punishment",[387]="Last Resort",[388]="Worry Seed",[389]="Sucker Punch",[390]="Toxic Spikes",[391]="Heart Swap",[392]="Aqua Ring",[393]="Magnet Rise",[394]="Flare Blitz",[395]="Force Palm",[396]="Aura Sphere",[397]="Rock Polish",[398]="Poison Jab",[399]="Dark Pulse",[400]="Night Slash",[401]="Aqua Tail",[402]="Seed Bomb",[403]="Air Slash",[404]="X-Scissor",[405]="Bug Buzz",[406]="Dragon Pulse",[407]="Dragon Rush",[408]="Power Gem",[409]="Drain Punch",[410]="Vacuum Wave",[411]="Focus Blast",[412]="Energy Ball",[413]="Brave Bird",[414]="Earth Power",[415]="Switcheroo",[416]="Giga Impact",[417]="Nasty Plot",[418]="Bullet Punch",[419]="Avalanche",[420]="Ice Shard",[421]="Shadow Claw",[422]="Thunder Fang",[423]="Ice Fang",[424]="Fire Fang",[425]="Shadow Sneak",[426]="Mud Bomb",[427]="Psycho Cut",[428]="Zen Headbutt",[429]="Mirror Shot",[430]="Flash Cannon",[431]="Rock Climb",[432]="Defog",[433]="Trick Room",[434]="Draco Meteor",[435]="Discharge",[436]="Lava Plume",[437]="Leaf Storm",[438]="Power Whip",[439]="Rock Wrecker",[440]="Cross Poison",[441]="Gunk Shot",[442]="Iron Head",[443]="Magnet Bomb",[444]="Stone Edge",[445]="Captivate",[446]="Stealth Rock",[447]="Grass Knot",[448]="Chatter",[449]="Judgment",[450]="Bug Bite",[451]="Charge Beam",[452]="Wood Hammer",[453]="Aqua Jet",[454]="Attack Order",[455]="Defend Order",[456]="Heal Order",[457]="Head Smash",[458]="Double Hit",[459]="Roar of Time",[460]="Spacial Rend",[461]="Lunar Dance",[462]="Crush Grip",[463]="Magma Storm",[464]="Dark Void",[465]="Seed Flare",[466]="Ominous Wind",[467]="Shadow Force",[468]="Hone Claws",[469]="Wide Guard",[470]="Guard Split",[471]="Power Split",[472]="Wonder Room",[473]="Psyshock",[474]="Venoshock",[475]="Autotomize",[476]="Rage Powder",[477]="Telekinesis",[478]="Magic Room",[479]="Smack Down",[480]="Storm Throw",[481]="Flame Burst",[482]="Sludge Wave",[483]="Quiver Dance",[484]="Heavy Slam",[485]="Synchronoise",[486]="Electro Ball",[487]="Soak",[488]="Flame Charge",[489]="Coil",[490]="Low Sweep",[491]="Acid Spray",[492]="Foul Play",[493]="Simple Beam",[494]="Entrainment",[495]="After You",[496]="Round",[497]="Echoed Voice",[498]="Chip Away",[499]="Clear Smog",[500]="Stored Power",[501]="Quick Guard",[502]="Ally Switch",[503]="Scald",[504]="Shell Smash",[505]="Heal Pulse",[506]="Hex",[507]="Sky Drop",[508]="Shift Gear",[509]="Circle Throw",[510]="Incinerate",[511]="Quash",[512]="Acrobatics",[513]="Reflect Type",[514]="Retaliate",[515]="Final Gambit",[516]="Bestow",[517]="Inferno",[518]="Water Pledge",[519]="Fire Pledge",[520]="Grass Pledge",[521]="Volt Switch",[522]="Struggle Bug",[523]="Bulldoze",[524]="Frost Breath",[525]="Dragon Tail",[526]="Work Up",[527]="Electroweb",[528]="Wild Charge",[529]="Drill Run",[530]="Dual Chop",[531]="Heart Stamp",[532]="Horn Leech",[533]="Sacred Sword",[534]="Razor Shell",[535]="Heat Crash",[536]="Leaf Tornado",[537]="Steamroller",[538]="Cotton Guard",[539]="Night Daze",[540]="Psystrike",[541]="Tail Slap",[542]="Hurricane",[543]="Head Charge",[544]="Gear Grind",[545]="Searing Shot",[546]="Techno Blast",[547]="Relic Song",[548]="Secret Sword",[549]="Glaciate",[550]="Bolt Strike",[551]="Blue Flare",[552]="Fiery Dance",[553]="Freeze Shock",[554]="Ice Burn",[555]="Snarl",[556]="Icicle Crash",[557]="V-create",[558]="Fusion Flare",[559]="Fusion Bolt",[560]="Flying Press",[561]="Mat Block",[562]="Belch",[563]="Rototiller",[564]="Sticky Web",[565]="Fell Stinger",[566]="Phantom Force",[567]="Trick-or-Treat",[568]="Noble Roar",[569]="Ion Deluge",[570]="Parabolic Charge",[571]="Forest’s Curse",[572]="Petal Blizzard",[573]="Freeze-Dry",[574]="Disarming Voice",[575]="Parting Shot",[576]="Topsy-Turvy",[577]="Draining Kiss",[578]="Crafty Shield",[579]="Flower Shield",[580]="Grassy Terrain",[581]="Misty Terrain",[582]="Electrify",[583]="Play Rough",[584]="Fairy Wind",[585]="Moonblast",[586]="Boomburst",[587]="Fairy Lock",[588]="King’s Shield",[589]="Play Nice",[590]="Confide",[591]="Diamond Storm",[592]="Steam Eruption",[593]="Hyperspace Hole",[594]="Water Shuriken",[595]="Mystical Fire",[596]="Spiky Shield",[597]="Aromatic Mist",[598]="Eerie Impulse",[599]="Venom Drench",[600]="Powder",[601]="Geomancy",[602]="Magnetic Flux",[603]="Happy Hour",[604]="Electric Terrain",[605]="Dazzling Gleam",[606]="Celebrate",[607]="Hold Hands",[608]="Baby-Doll Eyes",[609]="Nuzzle",[610]="Hold Back",[611]="Infestation",[612]="Power-Up Punch",[613]="Oblivion Wing",[614]="Thousand Arrows",[615]="Thousand Waves",[616]="Land’s Wrath",[617]="Light Of Ruin",[618]="Origin Pulse",[619]="Precipice Blades",[620]="Dragon Ascent",[621]="Hyperspace Fury",[622]="Shore Up",[623]="First Impression",[624]="Baneful Bunker",[625]="Spirit Shackle",[626]="Darkest Lariat",[627]="Sparkling Aria",[628]="Ice Hammer",[629]="Floral Healing",[630]="High Horsepower",[631]="Strength Sap",[632]="Solar Blade",[633]="Leafage",[634]="Spotlight",[635]="Toxic Thread",[636]="Laser Focus",[637]="Gear Up",[638]="Throat Chop",[639]="Pollen Puff",[640]="Anchor Shot",[641]="Psychic Terrain",[642]="Lunge",[643]="Fire Lash",[644]="Power Trip",[645]="Burn Up",[646]="Speed Swap",[647]="Smart Strike",[648]="Purify",[649]="Revelation Dance",[650]="Core Enforcer",[651]="Trop Kick",[652]="Instruct",[653]="Beak Blast",[654]="Clanging Scales",[655]="Dragon Hammer",[656]="Brutal Swing",[657]="Aurora Veil",[658]="Shell Trap",[659]="Fleur Cannon",[660]="Psychic Fangs",[661]="Stomping Tantrum",[662]="Shadow Bone",[663]="Accelerock",[664]="Liquidation",[665]="Prismatic Laser",[666]="Spectral Thief",[667]="Sunsteel Strike",[668]="Moongeist Beam",[669]="Tearful Look",[670]="Zing Zap",[671]="Nature’s Madness",[672]="Multi-Attack",[673]="Mind Blown",[674]="Plasma Fists",[675]="Photon Geyser",[676]="Zippy Zap",[677]="Splishy Splash",[678]="Floaty Fall",[679]="Pika Papow",[680]="Bouncy Bubble",[681]="Buzzy Buzz",[682]="Sizzly Slide",[683]="Glitzy Glow",[684]="Baddy Bad",[685]="Sappy Seed",[686]="Freezy Frost",[687]="Sparkly Swirl",[688]="Veevee Volley",[689]="Double Iron Bash",[690]="Dynamax Cannon",[691]="Snipe Shot",[692]="Jaw Lock",[693]="Stuff Cheeks",[694]="No Retreat",[695]="Tar Shot",[696]="Magic Powder",[697]="Dragon Darts",[698]="Teatime",[699]="Octolock",[700]="Bolt Beak",[701]="Fishious Rend",[702]="Court Change",[703]="Clangorous Soul",[704]="Body Press",[705]="Decorate",[706]="Drum Beating",[707]="Snap Trap",[708]="Pyro Ball",[709]="Behemoth Blade",[710]="Behemoth Bash",[711]="Aura Wheel",[712]="Breaking Swipe",[713]="Branch Poke",[714]="Overdrive",[715]="Apple Acid",[716]="Grav Apple",[717]="Spirit Break",[718]="Strange Steam",[719]="Life Dew",[720]="Obstruct",[721]="False Surrender",[722]="Meteor Assault",[723]="Eternabeam",[724]="Steel Beam",[725]="Expanding Force",[726]="Steel Roller",[727]="Scale Shot",[728]="Meteor Beam",[729]="Shell Side Arm",[730]="Misty Explosion",[731]="Grassy Glide",[732]="Rising Voltage",[733]="Terrain Pulse",[734]="Skitter Smack",[735]="Burning Jealousy",[736]="Lash Out",[737]="Poltergeist",[738]="Corrosive Gas",[739]="Coaching",[740]="Flip Turn",[741]="Triple Axel",[742]="Dual Wingbeat",[743]="Scorching Sands",[744]="Jungle Healing",[745]="Wicked Blow",[746]="Surging Strikes",[747]="Thunder Cage",[748]="Dragon Energy",[749]="Freezing Glare",[750]="Fiery Wrath",[751]="Thunderous Kick",[752]="Glacial Lance",[753]="Astral Barrage",[754]="Eerie Spell",[755]="Dire Claw",[756]="Psyshield Bash",[757]="Power Shift",[758]="Stone Axe",[759]="Springtide Storm",[760]="Mystical Power",[761]="Raging Fury",[762]="Wave Crash",[763]="Chloroblast",[764]="Mountain Gale",[765]="Victory Dance",[766]="Headlong Rush",[767]="Barb Barrage",[768]="Esper Wing",[769]="Bitter Malice",[770]="Shelter",[771]="Triple Arrows",[772]="Infernal Parade",[773]="Ceaseless Edge",[774]="Bleakwind Storm",[775]="Wildbolt Storm",[776]="Sandsear Storm",[777]="Lunar Blessing",[778]="Take Heart",[779]="Tera Blast",[780]="Silk Trap",[781]="Axe Kick",[782]="Last Respects",[783]="Lumina Crash",[784]="Order Up",[785]="Jet Punch",[786]="Spicy Extract",[787]="Spin Out",[788]="Population Bomb",[789]="Ice Spinner",[790]="Glaive Rush",[791]="Revival Blessing",[792]="Salt Cure",[793]="Triple Dive",[794]="Mortal Spin",[795]="Doodle",[796]="Fillet Away",[797]="Kowtow Cleave",[798]="Flower Trick",[799]="Torch Song",[800]="Aqua Step",[801]="Raging Bull",[802]="Make It Rain",[803]="Ruination",[804]="Collision Course",[805]="Electro Drift",[806]="Shed Tail",[807]="Chilly Reception",[808]="Tidy Up",[809]="Snowscape",[810]="Pounce",[811]="Trailblaze",[812]="Chilling Water",[813]="Hyper Drill",[814]="Twin Beam",[815]="Rage Fist",[816]="Armor Cannon",[817]="Bitter Blade",[818]="Double Shock",[819]="Gigaton Hammer",[820]="Comeuppance",[821]="Aqua Cutter",[822]="Blazing Torque",[823]="Wicked Torque",[824]="Noxious Torque",[825]="Combat Torque",[826]="Magical Torque",[827]="Psyblade",[828]="Hydro Steam",[829]="Blood Moon",[830]="Matcha Gotcha",[831]="Syrup Bomb",[832]="Ivy Cudgel",[833]="Electro Shot",[834]="Tera Starstorm",[835]="Fickle Beam",[836]="Burning Bulwark",[837]="Thunderclap",[838]="Mighty Cleave",[839]="Tachyon Cutter",[840]="Hard Press",[841]="Dragon Cheer",[842]="Alluring Voice",[843]="Temper Flare",[844]="Supercell Slam",[845]="Psychic Noise",[846]="Upper Hand",[847]="Malignant Chain",[848]="Breakneck Blitz",[849]="All Out Pummeling",[850]="Supersonic Skystrike",[851]="Acid Downpour",[852]="Tectonic Rage",[853]="Continental Crush",[854]="Savage Spin Out",[855]="Never Ending Nightmare",[856]="Corkscrew Crash",[857]="Inferno Overdrive",[858]="Hydro Vortex",[859]="Bloom Doom",[860]="Gigavolt Havoc",[861]="Shattered Psyche",[862]="Subzero Slammer",[863]="Devastating Drake",[864]="Black Hole Eclipse",[865]="Twinkle Tackle",[866]="Catastropika",[867]="10,000,000 Volt Thunderbolt",[868]="Stoked Sparksurfer",[869]="Extreme Evoboost",[870]="Pulverizing Pancake",[871]="Genesis Supernova",[872]="Sinister Arrow Raid",[873]="Malicious Moonsault",[874]="Oceanic Operetta",[875]="Splintered Stormshards",[876]="Let’s Snuggle Forever",[877]="Clangorous Soulblaze",[878]="Guardian of Alola",[879]="Searing Sunraze Smash",[880]="Menacing Moonraze Maelstrom",[881]="Light That Burns The Sky",[882]="Soul Stealing 7 Star Strike",[883]="Max Guard",[884]="Max Strike",[885]="Max Knuckle",[886]="Max Airstream",[887]="Max Ooze",[888]="Max Quake",[889]="Max Rockfall",[890]="Max Flutterby",[891]="Max Phantasm",[892]="Max Steelspike",[893]="Max Flare",[894]="Max Geyser",[895]="Max Overgrowth",[896]="Max Lightning",[897]="Max Mindstorm",[898]="Max Hailstorm",[899]="Max Wyrmwind",[900]="Max Darkness",[901]="Max Starfall",[902]="G-Max Vine Lash",[903]="G-Max Wildfire",[904]="G-Max Canonade",[905]="G-Max Befuddle",[906]="G-Max Volt Crash",[907]="G-Max Gold Rush",[908]="G-Max Chi Strike",[909]="G-Max Terror",[910]="G-Max Foam Burst",[911]="G-Max Resonance",[912]="G-Max Cuddle",[913]="G-Max Replenish",[914]="G-Max Malodor",[915]="G-Max Meltdown",[916]="G-Max Drum Solo",[917]="G-Max Fireball",[918]="G-Max Hydrosnipe",[919]="G-Max Wind Rage",[920]="G-Max Gravitas",[921]="G-Max Stonesurge",[922]="G-Max Volcalith",[923]="G-Max Tartness",[924]="G-Max Sweetness",[925]="G-Max Sandblast",[926]="G-Max Stun Shock",[927]="G-Max Centiferno",[928]="G-Max Smite",[929]="G-Max Snooze",[930]="G-Max Finale",[931]="G-Max Steelsurge",[932]="G-Max Depletion",[933]="G-Max One Blow",[934]="G-Max Rapid Flow"}
local MOVE_PP = {[0]=0,[1]=35,[2]=25,[3]=10,[4]=15,[5]=20,[6]=20,[7]=15,[8]=15,[9]=15,[10]=35,[11]=30,[12]=8,[13]=10,[14]=30,[15]=30,[16]=35,[17]=35,[18]=20,[19]=15,[20]=20,[21]=8,[22]=15,[23]=20,[24]=30,[25]=8,[26]=25,[27]=15,[28]=5,[29]=15,[30]=25,[31]=20,[32]=5,[33]=35,[34]=15,[35]=20,[36]=20,[37]=20,[38]=8,[39]=1,[40]=35,[41]=20,[42]=15,[43]=30,[44]=25,[45]=40,[46]=20,[47]=8,[48]=20,[49]=20,[50]=20,[51]=30,[52]=25,[53]=15,[54]=3,[55]=25,[56]=8,[57]=15,[58]=10,[59]=5,[60]=20,[61]=20,[62]=20,[63]=8,[64]=35,[65]=20,[66]=8,[67]=20,[68]=20,[69]=20,[70]=15,[71]=25,[72]=15,[73]=10,[74]=5,[75]=25,[76]=15,[77]=35,[78]=30,[79]=15,[80]=20,[81]=40,[82]=10,[83]=15,[84]=30,[85]=15,[86]=20,[87]=10,[88]=15,[89]=10,[90]=5,[91]=10,[92]=10,[93]=25,[94]=10,[95]=20,[96]=1,[97]=1,[98]=30,[99]=20,[100]=20,[101]=15,[102]=10,[103]=5,[104]=6,[105]=10,[106]=5,[107]=3,[108]=5,[109]=10,[110]=5,[111]=40,[112]=5,[113]=2,[114]=30,[115]=2,[116]=5,[117]=10,[118]=40,[119]=20,[120]=5,[121]=10,[122]=15,[123]=20,[124]=20,[125]=20,[126]=5,[127]=15,[128]=10,[129]=20,[130]=10,[131]=15,[132]=15,[133]=3,[134]=15,[135]=25,[136]=20,[137]=30,[138]=15,[139]=40,[140]=20,[141]=15,[142]=10,[143]=5,[144]=10,[145]=30,[146]=10,[147]=15,[148]=5,[149]=15,[150]=40,[151]=1,[152]=10,[153]=5,[154]=15,[155]=10,[156]=3,[157]=10,[158]=15,[159]=3,[160]=30,[161]=10,[162]=10,[163]=20,[164]=2,[165]=1,[166]=1,[167]=10,[168]=10,[169]=10,[170]=5,[171]=15,[172]=25,[173]=15,[174]=3,[175]=15,[176]=30,[177]=5,[178]=40,[179]=15,[180]=10,[181]=25,[182]=10,[183]=15,[184]=10,[185]=20,[186]=10,[187]=1,[188]=10,[189]=10,[190]=10,[191]=20,[192]=5,[193]=5,[194]=5,[195]=5,[196]=15,[197]=10,[198]=10,[199]=5,[200]=15,[201]=1,[202]=15,[203]=10,[204]=3,[205]=5,[206]=40,[207]=15,[208]=10,[209]=20,[210]=20,[211]=25,[212]=5,[213]=15,[214]=20,[215]=1,[216]=20,[217]=15,[218]=20,[219]=5,[220]=20,[221]=5,[222]=30,[223]=5,[224]=10,[225]=20,[226]=40,[227]=3,[228]=15,[229]=20,[230]=2,[231]=15,[232]=35,[233]=10,[234]=5,[235]=5,[236]=5,[237]=15,[238]=5,[239]=15,[240]=1,[241]=1,[242]=15,[243]=20,[244]=10,[245]=8,[246]=10,[247]=15,[248]=15,[249]=15,[250]=15,[251]=10,[252]=5,[253]=10,[254]=10,[255]=10,[256]=10,[257]=10,[258]=1,[259]=15,[260]=15,[261]=15,[262]=10,[263]=20,[264]=20,[265]=10,[266]=20,[267]=20,[268]=1,[269]=15,[270]=20,[271]=10,[272]=10,[273]=10,[274]=35,[275]=1,[276]=5,[277]=15,[278]=15,[279]=10,[280]=15,[281]=10,[282]=20,[283]=5,[284]=5,[285]=2,[286]=10,[287]=10,[288]=5,[289]=10,[290]=20,[291]=10,[292]=20,[293]=20,[294]=20,[295]=5,[296]=5,[297]=3,[298]=20,[299]=10,[300]=15,[301]=20,[302]=15,[303]=10,[304]=10,[305]=15,[306]=10,[307]=5,[308]=5,[309]=10,[310]=15,[311]=10,[312]=1,[313]=3,[314]=25,[315]=5,[316]=40,[317]=10,[318]=5,[319]=3,[320]=15,[321]=6,[322]=6,[323]=5,[324]=15,[325]=20,[326]=30,[327]=15,[328]=15,[329]=5,[330]=10,[331]=30,[332]=20,[333]=30,[334]=3,[335]=5,[336]=3,[337]=15,[338]=5,[339]=3,[340]=5,[341]=15,[342]=5,[343]=25,[344]=5,[345]=15,[346]=15,[347]=3,[348]=15,[349]=1,[350]=8,[351]=15,[352]=20,[353]=5,[354]=5,[355]=8,[356]=5,[357]=40,[358]=10,[359]=10,[360]=8,[361]=10,[362]=10,[363]=1,[364]=10,[365]=5,[366]=30,[367]=30,[368]=10,[369]=20,[370]=5,[371]=10,[372]=10,[373]=15,[374]=10,[375]=10,[376]=1,[377]=15,[378]=5,[379]=10,[380]=10,[381]=30,[382]=20,[383]=20,[384]=3,[385]=3,[386]=5,[387]=5,[388]=10,[389]=5,[390]=20,[391]=10,[392]=20,[393]=10,[394]=8,[395]=10,[396]=10,[397]=1,[398]=20,[399]=15,[400]=15,[401]=10,[402]=15,[403]=20,[404]=15,[405]=10,[406]=10,[407]=10,[408]=20,[409]=5,[410]=30,[411]=5,[412]=10,[413]=15,[414]=10,[415]=10,[416]=5,[417]=1,[418]=10,[419]=10,[420]=10,[421]=15,[422]=15,[423]=15,[424]=15,[425]=10,[426]=10,[427]=20,[428]=15,[429]=10,[430]=10,[431]=20,[432]=1,[433]=1,[434]=5,[435]=15,[436]=15,[437]=5,[438]=10,[439]=8,[440]=20,[441]=8,[442]=15,[443]=20,[444]=5,[445]=3,[446]=20,[447]=20,[448]=20,[449]=10,[450]=20,[451]=10,[452]=15,[453]=5,[454]=15,[455]=10,[456]=10,[457]=5,[458]=10,[459]=5,[460]=5,[461]=10,[462]=5,[463]=5,[464]=5,[465]=5,[466]=5,[467]=5,[468]=15,[469]=10,[470]=10,[471]=10,[472]=10,[473]=10,[474]=10,[475]=10,[476]=20,[477]=15,[478]=10,[479]=15,[480]=10,[481]=15,[482]=10,[483]=3,[484]=10,[485]=10,[486]=10,[487]=20,[488]=20,[489]=5,[490]=20,[491]=20,[492]=15,[493]=15,[494]=15,[495]=15,[496]=15,[497]=15,[498]=20,[499]=15,[500]=10,[501]=15,[502]=15,[503]=15,[504]=3,[505]=10,[506]=10,[507]=10,[508]=5,[509]=10,[510]=15,[511]=15,[512]=15,[513]=15,[514]=10,[515]=5,[516]=15,[517]=5,[518]=10,[519]=10,[520]=10,[521]=20,[522]=20,[523]=30,[524]=10,[525]=10,[526]=15,[527]=15,[528]=20,[529]=10,[530]=15,[531]=25,[532]=10,[533]=15,[534]=10,[535]=10,[536]=10,[537]=20,[538]=3,[539]=10,[540]=10,[541]=10,[542]=8,[543]=15,[544]=15,[545]=5,[546]=5,[547]=10,[548]=10,[549]=10,[550]=5,[551]=5,[552]=10,[553]=5,[554]=5,[555]=15,[556]=10,[557]=5,[558]=5,[559]=5,[560]=10,[561]=15,[562]=10,[563]=10,[564]=20,[565]=20,[566]=10,[567]=20,[568]=20,[569]=25,[570]=20,[571]=20,[572]=15,[573]=20,[574]=15,[575]=15,[576]=20,[577]=10,[578]=10,[579]=10,[580]=10,[581]=10,[582]=20,[583]=10,[584]=30,[585]=15,[586]=10,[587]=10,[588]=10,[589]=20,[590]=20,[591]=5,[592]=5,[593]=5,[594]=20,[595]=20,[596]=10,[597]=20,[598]=15,[599]=20,[600]=20,[601]=3,[602]=20,[603]=30,[604]=10,[605]=10,[606]=40,[607]=40,[608]=20,[609]=20,[610]=40,[611]=20,[612]=20,[613]=10,[614]=10,[615]=10,[616]=10,[617]=5,[618]=10,[619]=10,[620]=5,[621]=5,[622]=10,[623]=10,[624]=10,[625]=10,[626]=10,[627]=10,[628]=10,[629]=10,[630]=10,[631]=10,[632]=10,[633]=40,[634]=15,[635]=20,[636]=30,[637]=20,[638]=15,[639]=15,[640]=20,[641]=10,[642]=15,[643]=15,[644]=10,[645]=5,[646]=10,[647]=10,[648]=20,[649]=15,[650]=10,[651]=15,[652]=15,[653]=15,[654]=5,[655]=15,[656]=20,[657]=20,[658]=5,[659]=5,[660]=15,[661]=10,[662]=10,[663]=20,[664]=10,[665]=10,[666]=10,[667]=5,[668]=5,[669]=20,[670]=10,[671]=10,[672]=10,[673]=5,[674]=15,[675]=5,[676]=15,[677]=15,[678]=15,[679]=20,[680]=15,[681]=15,[682]=15,[683]=15,[684]=15,[685]=15,[686]=15,[687]=15,[688]=20,[689]=5,[690]=5,[691]=15,[692]=10,[693]=10,[694]=5,[695]=15,[696]=20,[697]=10,[698]=10,[699]=15,[700]=10,[701]=10,[702]=10,[703]=5,[704]=10,[705]=15,[706]=10,[707]=15,[708]=5,[709]=5,[710]=5,[711]=10,[712]=15,[713]=40,[714]=10,[715]=10,[716]=10,[717]=15,[718]=10,[719]=10,[720]=10,[721]=10,[722]=5,[723]=5,[724]=5,[725]=10,[726]=5,[727]=20,[728]=10,[729]=10,[730]=5,[731]=20,[732]=20,[733]=10,[734]=10,[735]=5,[736]=5,[737]=5,[738]=40,[739]=10,[740]=20,[741]=15,[742]=10,[743]=10,[744]=10,[745]=5,[746]=5,[747]=15,[748]=5,[749]=10,[750]=10,[751]=10,[752]=5,[753]=5,[754]=5,[755]=15,[756]=10,[757]=10,[758]=15,[759]=5,[760]=10,[761]=10,[762]=10,[763]=5,[764]=5,[765]=20,[766]=5,[767]=15,[768]=10,[769]=15,[770]=10,[771]=15,[772]=15,[773]=15,[774]=5,[775]=5,[776]=5,[777]=10,[778]=10,[779]=10,[780]=10,[781]=10,[782]=10,[783]=10,[784]=10,[785]=15,[786]=15,[787]=5,[788]=10,[789]=15,[790]=5,[791]=1,[792]=15,[793]=10,[794]=15,[795]=10,[796]=10,[797]=10,[798]=10,[799]=10,[800]=10,[801]=10,[802]=5,[803]=10,[804]=5,[805]=5,[806]=10,[807]=10,[808]=10,[809]=10,[810]=20,[811]=20,[812]=20,[813]=5,[814]=10,[815]=10,[816]=5,[817]=10,[818]=5,[819]=5,[820]=10,[821]=15,[822]=10,[823]=10,[824]=10,[825]=10,[826]=10,[827]=15,[828]=15,[829]=5,[830]=15,[831]=10,[832]=10,[833]=10,[834]=5,[835]=5,[836]=10,[837]=5,[838]=5,[839]=10,[840]=10,[841]=15,[842]=10,[843]=10,[844]=15,[845]=10,[846]=15,[847]=5,[848]=1,[849]=1,[850]=1,[851]=1,[852]=1,[853]=1,[854]=1,[855]=1,[856]=1,[857]=1,[858]=1,[859]=1,[860]=1,[861]=1,[862]=1,[863]=1,[864]=1,[865]=1,[866]=1,[867]=1,[868]=1,[869]=1,[870]=1,[871]=1,[872]=1,[873]=1,[874]=1,[875]=1,[876]=1,[877]=1,[878]=1,[879]=1,[880]=1,[881]=1,[882]=1,[883]=10,[884]=10,[885]=10,[886]=10,[887]=10,[888]=10,[889]=10,[890]=10,[891]=10,[892]=10,[893]=10,[894]=10,[895]=10,[896]=10,[897]=10,[898]=10,[899]=10,[900]=10,[901]=10,[902]=10,[903]=10,[904]=10,[905]=10,[906]=10,[907]=10,[908]=10,[909]=10,[910]=10,[911]=10,[912]=10,[913]=10,[914]=10,[915]=10,[916]=10,[917]=10,[918]=10,[919]=10,[920]=10,[921]=10,[922]=10,[923]=10,[924]=10,[925]=10,[926]=10,[927]=10,[928]=10,[929]=10,[930]=10,[931]=10,[932]=10,[933]=10,[934]=10}
local ITEM_NAMES = {[0]="????????",[1]="Poké Ball",[2]="Great Ball",[3]="Ultra Ball",[4]="Master Ball",[5]="Premier Ball",[6]="Heal Ball",[7]="Net Ball",[8]="Nest Ball",[9]="Dive Ball",[10]="Dusk Ball",[11]="Timer Ball",[12]="Quick Ball",[13]="Repeat Ball",[14]="Luxury Ball",[15]="Level Ball",[16]="Lure Ball",[17]="Moon Ball",[18]="Friend Ball",[19]="Love Ball",[20]="Fast Ball",[21]="Heavy Ball",[22]="Dream Ball",[23]="Safari Ball",[24]="Sport Ball",[25]="Park Ball",[26]="Beast Ball",[27]="Cherish Ball",[28]="Potion",[29]="Super Potion",[30]="Hyper Potion",[31]="Max Potion",[32]="Full Restore",[33]="Revive",[34]="Max Revive",[35]="Fresh Water",[36]="Soda Pop",[37]="Lemonade",[38]="Moomoo Milk",[39]="Energy Powder",[40]="Energy Root",[41]="Heal Powder",[42]="Revival Herb",[43]="Antidote",[44]="Paralyze Heal",[45]="Burn Heal",[46]="Ice Heal",[47]="Awakening",[48]="Full Heal",[49]="Ether",[50]="Max Ether",[51]="Elixir",[52]="Max Elixir",[53]="Berry Juice",[54]="Sacred Ash",[55]="Sweet Heart",[56]="Max Honey",[57]="Pewter Crunchies",[58]="Rage Candy Bar",[59]="Lava Cookie",[60]="Old Gateau",[61]="Casteliacone",[62]="Lumiose Galette",[63]="Shalour Sable",[64]="Big Malasada",[65]="HP Up",[66]="Protein",[67]="Iron",[68]="Calcium",[69]="Zinc",[70]="Carbos",[71]="PP Up",[72]="PP Max",[73]="Health Feather",[74]="Muscle Feather",[75]="Resist Feather",[76]="Genius Feather",[77]="Clever Feather",[78]="Swift Feather",[79]="Ability Capsule",[80]="Ability Patch",[81]="Lonely Mint",[82]="Adamant Mint",[83]="Naughty Mint",[84]="Brave Mint",[85]="Bold Mint",[86]="Impish Mint",[87]="Lax Mint",[88]="Relaxed Mint",[89]="Modest Mint",[90]="Mild Mint",[91]="Rash Mint",[92]="Quiet Mint",[93]="Calm Mint",[94]="Gentle Mint",[95]="Careful Mint",[96]="Sassy Mint",[97]="Timid Mint",[98]="Hasty Mint",[99]="Jolly Mint",[100]="Naive Mint",[101]="Serious Mint",[102]="Rare Candy",[103]="Exp. Candy XS",[104]="Exp. Candy S",[105]="Exp. Candy M",[106]="Exp. Candy L",[107]="Exp. Candy XL",[108]="Dynamax Candy",[109]="Blue Flute",[110]="Yellow Flute",[111]="Red Flute",[112]="Black Flute",[113]="White Flute",[114]="Repel",[115]="Super Repel",[116]="Max Repel",[117]="Lure",[118]="Super Lure",[119]="Max Lure",[120]="Escape Rope",[121]="X Attack",[122]="X Defense",[123]="X Sp. Atk",[124]="X Sp. Def",[125]="X Speed",[126]="X Accuracy",[127]="Dire Hit",[128]="Guard Spec.",[129]="Poké Doll",[130]="Fluffy Tail",[131]="Poké Toy",[132]="Max Mushrooms",[133]="Bottle Cap",[134]="Gold Bottle Cap",[135]="Nugget",[136]="Big Nugget",[137]="Tiny Mushroom",[138]="Big Mushroom",[139]="Balm Mushroom",[140]="Pearl",[141]="Big Pearl",[142]="Pearl String",[143]="Stardust",[144]="Star Piece",[145]="Comet Shard",[146]="Berserk Gene",[147]="RageCandyBar",[148]="Red Shard",[149]="Blue Shard",[150]="Yellow Shard",[151]="Green Shard",[152]="Heart Scale",[153]="Honey",[154]="Rare Bone",[155]="Odd Keystone",[156]="Pretty Feather",[157]="Relic Copper",[158]="Relic Silver",[159]="Relic Gold",[160]="Relic Vase",[161]="Relic Band",[162]="Relic Statue",[163]="Relic Crown",[164]="Strange Souvenir",[165]="Helix Fossil",[166]="Dome Fossil",[167]="Old Amber",[168]="Root Fossil",[169]="Claw Fossil",[170]="Armor Fossil",[171]="Skull Fossil",[172]="Cover Fossil",[173]="Plume Fossil",[174]="Jaw Fossil",[175]="Sail Fossil",[176]="Fossilized Bird",[177]="Fossilized Fish",[178]="Fossilized Drake",[179]="Fossilized Dino",[180]="Growth Mulch",[181]="Damp Mulch",[182]="Stable Mulch",[183]="Gooey Mulch",[184]="Rich Mulch",[185]="Surprise Mulch",[186]="Boost Mulch",[187]="Amaze Mulch",[188]="Red Apricorn",[189]="Blue Apricorn",[190]="Yellow Apricorn",[191]="Green Apricorn",[192]="Pink Apricorn",[193]="White Apricorn",[194]="Black Apricorn",[195]="Wishing Piece",[196]="Galarica Twig",[197]="Armorite Ore",[198]="Dynite Ore",[199]="Orange Mail",[200]="Harbor Mail",[201]="Glitter Mail",[202]="Mech Mail",[203]="Wood Mail",[204]="Wave Mail",[205]="Bead Mail",[206]="Shadow Mail",[207]="Tropic Mail",[208]="Dream Mail",[209]="Fab Mail",[210]="Retro Mail",[211]="Fire Stone",[212]="Water Stone",[213]="Thunder Stone",[214]="Leaf Stone",[215]="Ice Stone",[216]="Sun Stone",[217]="Moon Stone",[218]="Shiny Stone",[219]="Dusk Stone",[220]="Dawn Stone",[221]="Sweet Apple",[222]="Tart Apple",[223]="Cracked Pot",[224]="Chipped Pot",[225]="Galarica Cuff",[226]="Galarica Wreath",[227]="Dragon Scale",[228]="Upgrade",[229]="Protector",[230]="Electirizer",[231]="Magmarizer",[232]="Dubious Disc",[233]="Reaper Cloth",[234]="Prism Scale",[235]="Whipped Dream",[236]="Sachet",[237]="Oval Stone",[238]="Strawberry Sweet",[239]="Love Sweet",[240]="Berry Sweet",[241]="Clover Sweet",[242]="Flower Sweet",[243]="Star Sweet",[244]="Ribbon Sweet",[245]="Everstone",[246]="Red Nectar",[247]="Yellow Nectar",[248]="Pink Nectar",[249]="Purple Nectar",[250]="Flame Plate",[251]="Splash Plate",[252]="Zap Plate",[253]="Meadow Plate",[254]="Icicle Plate",[255]="Fist Plate",[256]="Toxic Plate",[257]="Earth Plate",[258]="Sky Plate",[259]="Mind Plate",[260]="Insect Plate",[261]="Stone Plate",[262]="Spooky Plate",[263]="Draco Plate",[264]="Dread Plate",[265]="Iron Plate",[266]="Pixie Plate",[267]="Douse Drive",[268]="Shock Drive",[269]="Burn Drive",[270]="Chill Drive",[271]="Fire Memory",[272]="Water Memory",[273]="Electric Memory",[274]="Grass Memory",[275]="Ice Memory",[276]="Fighting Memory",[277]="Poison Memory",[278]="Ground Memory",[279]="Flying Memory",[280]="Psychic Memory",[281]="Bug Memory",[282]="Rock Memory",[283]="Ghost Memory",[284]="Dragon Memory",[285]="Dark Memory",[286]="Steel Memory",[287]="Fairy Memory",[288]="Rusted Sword",[289]="Rusted Shield",[290]="Red Orb",[291]="Blue Orb",[292]="Venusaurite",[293]="Charizardite X",[294]="Charizardite Y",[295]="Blastoisinite",[296]="Beedrillite",[297]="Pidgeotite",[298]="Alakazite",[299]="Slowbronite",[300]="Gengarite",[301]="Kangaskhanite",[302]="Pinsirite",[303]="Gyaradosite",[304]="Aerodactylite",[305]="Mewtwonite X",[306]="Mewtwonite Y",[307]="Ampharosite",[308]="Steelixite",[309]="Scizorite",[310]="Heracronite",[311]="Houndoominite",[312]="Tyranitarite",[313]="Sceptilite",[314]="Blazikenite",[315]="Swampertite",[316]="Gardevoirite",[317]="Sablenite",[318]="Mawilite",[319]="Aggronite",[320]="Medichamite",[321]="Manectite",[322]="Sharpedonite",[323]="Cameruptite",[324]="Altarianite",[325]="Banettite",[326]="Absolite",[327]="Glalitite",[328]="Salamencite",[329]="Metagrossite",[330]="Latiasite",[331]="Latiosite",[332]="Lopunnite",[333]="Garchompite",[334]="Lucarionite",[335]="Abomasite",[336]="Galladite",[337]="Audinite",[338]="Diancite",[339]="Normal Gem",[340]="Fire Gem",[341]="Water Gem",[342]="Electric Gem",[343]="Grass Gem",[344]="Ice Gem",[345]="Fighting Gem",[346]="Poison Gem",[347]="Ground Gem",[348]="Flying Gem",[349]="Psychic Gem",[350]="Bug Gem",[351]="Rock Gem",[352]="Ghost Gem",[353]="Dragon Gem",[354]="Dark Gem",[355]="Steel Gem",[356]="Fairy Gem",[357]="Normalium Z",[358]="Firium Z",[359]="Waterium Z",[360]="Electrium Z",[361]="Grassium Z",[362]="Icium Z",[363]="Fightinium Z",[364]="Poisonium Z",[365]="Groundium Z",[366]="Flyinium Z",[367]="Psychium Z",[368]="Buginium Z",[369]="Rockium Z",[370]="Ghostium Z",[371]="Dragonium Z",[372]="Darkinium Z",[373]="Steelium Z",[374]="Fairium Z",[375]="Pikanium Z",[376]="Eevium Z",[377]="Snorlium Z",[378]="Mewnium Z",[379]="Decidium Z",[380]="Incinium Z",[381]="Primarium Z",[382]="Lycanium Z",[383]="Mimikium Z",[384]="Kommonium Z",[385]="Tapunium Z",[386]="Solganium Z",[387]="Lunalium Z",[388]="Marshadium Z",[389]="Aloraichium Z",[390]="Pikashunium Z",[391]="Ultranecrozium Z",[392]="Light Ball",[393]="Leek",[394]="Thick Club",[395]="Lucky Punch",[396]="Metal Powder",[397]="Quick Powder",[398]="Deep Sea Scale",[399]="Deep Sea Tooth",[400]="Soul Dew",[401]="Adamant Orb",[402]="Lustrous Orb",[403]="Griseous Orb",[404]="Sea Incense",[405]="Lax Incense",[406]="Odd Incense",[407]="Rock Incense",[408]="Full Incense",[409]="Wave Incense",[410]="Rose Incense",[411]="Luck Incense",[412]="Pure Incense",[413]="Red Scarf",[414]="Blue Scarf",[415]="Pink Scarf",[416]="Green Scarf",[417]="Yellow Scarf",[418]="Macho Brace",[419]="Power Weight",[420]="Power Bracer",[421]="Power Belt",[422]="Power Lens",[423]="Power Band",[424]="Power Anklet",[425]="Silk Scarf",[426]="Charcoal",[427]="Mystic Water",[428]="Magnet",[429]="Miracle Seed",[430]="Never-Melt Ice",[431]="Black Belt",[432]="Poison Barb",[433]="Soft Sand",[434]="Sharp Beak",[435]="Twisted Spoon",[436]="Silver Powder",[437]="Hard Stone",[438]="Spell Tag",[439]="Dragon Fang",[440]="Black Glasses",[441]="Metal Coat",[442]="Choice Band",[443]="Choice Specs",[444]="Choice Scarf",[445]="Flame Orb",[446]="Toxic Orb",[447]="Damp Rock",[448]="Heat Rock",[449]="Smooth Rock",[450]="Icy Rock",[451]="Electric Seed",[452]="Psychic Seed",[453]="Misty Seed",[454]="Grassy Seed",[455]="Absorb Bulb",[456]="Cell Battery",[457]="Luminous Moss",[458]="Snowball",[459]="Bright Powder",[460]="White Herb",[461]="Exp. Share",[462]="Quick Claw",[463]="Soothe Bell",[464]="Mental Herb",[465]="King’s Rock",[466]="Amulet Coin",[467]="Cleanse Tag",[468]="Smoke Ball",[469]="Focus Band",[470]="Lucky Egg",[471]="Scope Lens",[472]="Leftovers",[473]="Shell Bell",[474]="Wide Lens",[475]="Muscle Band",[476]="Wise Glasses",[477]="Expert Belt",[478]="Light Clay",[479]="Life Orb",[480]="Power Herb",[481]="Focus Sash",[482]="Zoom Lens",[483]="Metronome",[484]="Iron Ball",[485]="Lagging Tail",[486]="Destiny Knot",[487]="Black Sludge",[488]="Grip Claw",[489]="Sticky Barb",[490]="Shed Shell",[491]="Big Root",[492]="Razor Claw",[493]="Razor Fang",[494]="Eviolite",[495]="Float Stone",[496]="Rocky Helmet",[497]="Air Balloon",[498]="Red Card",[499]="Ring Target",[500]="Binding Band",[501]="Eject Button",[502]="Weakness Policy",[503]="Assault Vest",[504]="Safety Goggles",[505]="Adrenaline Orb",[506]="Terrain Extender",[507]="Protective Pads",[508]="Throat Spray",[509]="Eject Pack",[510]="Heavy-Duty Boots",[511]="Blunder Policy",[512]="Room Service",[513]="Utility Umbrella",[514]="Cheri Berry",[515]="Chesto Berry",[516]="Pecha Berry",[517]="Rawst Berry",[518]="Aspear Berry",[519]="Leppa Berry",[520]="Oran Berry",[521]="Persim Berry",[522]="Lum Berry",[523]="Sitrus Berry",[524]="Figy Berry",[525]="Wiki Berry",[526]="Mago Berry",[527]="Aguav Berry",[528]="Iapapa Berry",[529]="Razz Berry",[530]="Bluk Berry",[531]="Nanab Berry",[532]="Wepear Berry",[533]="Pinap Berry",[534]="Pomeg Berry",[535]="Kelpsy Berry",[536]="Qualot Berry",[537]="Hondew Berry",[538]="Grepa Berry",[539]="Tamato Berry",[540]="Cornn Berry",[541]="Magost Berry",[542]="Rabuta Berry",[543]="Nomel Berry",[544]="Spelon Berry",[545]="Pamtre Berry",[546]="Watmel Berry",[547]="Durin Berry",[548]="Belue Berry",[549]="Chilan Berry",[550]="Occa Berry",[551]="Passho Berry",[552]="Wacan Berry",[553]="Rindo Berry",[554]="Yache Berry",[555]="Chople Berry",[556]="Kebia Berry",[557]="Shuca Berry",[558]="Coba Berry",[559]="Payapa Berry",[560]="Tanga Berry",[561]="Charti Berry",[562]="Kasib Berry",[563]="Haban Berry",[564]="Colbur Berry",[565]="Babiri Berry",[566]="Roseli Berry",[567]="Liechi Berry",[568]="Ganlon Berry",[569]="Salac Berry",[570]="Petaya Berry",[571]="Apicot Berry",[572]="Lansat Berry",[573]="Starf Berry",[574]="Enigma Berry",[575]="Micle Berry",[576]="Custap Berry",[577]="Jaboca Berry",[578]="Rowap Berry",[579]="Kee Berry",[580]="Maranga Berry",[581]="Enigma Berry",[582]="TM01",[583]="TM02",[584]="TM03",[585]="TM04",[586]="TM05",[587]="TM06",[588]="TM07",[589]="TM08",[590]="TM09",[591]="TM10",[592]="TM11",[593]="TM12",[594]="TM13",[595]="TM14",[596]="TM15",[597]="TM16",[598]="TM17",[599]="TM18",[600]="TM19",[601]="TM20",[602]="TM21",[603]="TM22",[604]="TM23",[605]="TM24",[606]="TM25",[607]="TM26",[608]="TM27",[609]="TM28",[610]="TM29",[611]="TM30",[612]="TM31",[613]="TM32",[614]="TM33",[615]="TM34",[616]="TM35",[617]="TM36",[618]="TM37",[619]="TM38",[620]="TM39",[621]="TM40",[622]="TM41",[623]="TM42",[624]="TM43",[625]="TM44",[626]="TM45",[627]="TM46",[628]="TM47",[629]="TM48",[630]="TM49",[631]="TM50",[632]="TM51",[633]="TM52",[634]="TM53",[635]="TM54",[636]="TM55",[637]="TM56",[638]="TM57",[639]="TM58",[640]="TM59",[641]="TM60",[642]="TM61",[643]="TM62",[644]="TM63",[645]="TM64",[646]="TM65",[647]="TM66",[648]="TM67",[649]="TM68",[650]="TM69",[651]="TM70",[652]="TM71",[653]="TM72",[654]="TM73",[655]="TM74",[656]="TM75",[657]="TM76",[658]="TM77",[659]="TM78",[660]="TM79",[661]="TM80",[662]="TM81",[663]="TM82",[664]="TM83",[665]="TM84",[666]="TM85",[667]="TM86",[668]="TM87",[669]="TM88",[670]="TM89",[671]="TM90",[672]="TM91",[673]="TM92",[674]="TM93",[675]="TM94",[676]="TM95",[677]="TM96",[678]="TM97",[679]="TM98",[680]="TM99",[681]="TM100",[682]="HM01",[683]="HM02",[684]="HM03",[685]="HM04",[686]="HM05",[687]="HM06",[688]="HM07",[689]="HM08",[690]="Oval Charm",[691]="Shiny Charm",[692]="Catching Charm",[693]="Exp. Charm",[694]="Rotom Catalog",[695]="Gracidea",[696]="Reveal Glass",[697]="DNA Splicers",[698]="Zygarde Cube",[699]="Prison Bottle",[700]="N-Solarizer",[701]="N-Lunarizer",[702]="Reins of Unity",[703]="Mega Ring",[704]="Z-Power Ring",[705]="Dynamax Band",[706]="Bicycle",[707]="Mach Bike",[708]="Acro Bike",[709]="Old Rod",[710]="Good Rod",[711]="Super Rod",[712]="Dowsing Machine",[713]="Town Map",[714]="Vs. Seeker",[715]="TM Case",[716]="Berry Pouch",[717]="ウエ Box Link",[718]="Coin Case",[719]="Powder Jar",[720]="Wailmer Pail",[721]="Poké Radar",[722]="オカキクケ Case",[723]="Soot Sack",[724]="Poké Flute",[725]="Fame Checker",[726]="Teachy TV",[727]="S.S. Ticket",[728]="Eon Ticket",[729]="Mystic Ticket",[730]="Aurora Ticket",[731]="Old Sea Map",[732]="Letter",[733]="Devon Parts",[734]="Go-Goggles",[735]="Devon Scope",[736]="Basement Key",[737]="Scanner",[738]="Storage Key",[739]="Key to Room 1",[740]="Key to Room 2",[741]="Key to Room 4",[742]="Key to Room 6",[743]="Meteorite",[744]="Magma Emblem",[745]="Contest Pass",[746]="Parcel",[747]="Secret Key",[748]="Bike Voucher",[749]="Gold Teeth",[750]="Card Key",[751]="Lift Key",[752]="Silph Scope",[753]="Tri-Pass",[754]="Rainbow Pass",[755]="Tea",[756]="Ruby",[757]="Sapphire",[758]="Ability Shield",[759]="Clear Amulet",[760]="Punching Glove",[761]="Covert Cloak",[762]="Loaded Dice",[763]="Auspicious Armor",[764]="Booster Energy",[765]="Big Bamboo Shoot",[766]="Gimmighoul Coin",[767]="Leader’s Crest",[768]="Malicious Armor",[769]="Mirror Herb",[770]="Scroll of Darkness",[771]="Scroll of Waters",[772]="Tera Orb",[773]="Tiny Bamboo Shoot",[774]="Bug Tera Shard",[775]="Dark Tera Shard",[776]="Dragon Tera Shard",[777]="Electric Tera Shard",[778]="Fairy Tera Shard",[779]="Fighting Tera Shard",[780]="Fire Tera Shard",[781]="Flying Tera Shard",[782]="Ghost Tera Shard",[783]="Grass Tera Shard",[784]="Ground Tera Shard",[785]="Ice Tera Shard",[786]="Normal Tera Shard",[787]="Poison Tera Shard",[788]="Psychic Tera Shard",[789]="Rock Tera Shard",[790]="Steel Tera Shard",[791]="Water Tera Shard",[792]="Adamant Crystal",[793]="Griseous Core",[794]="Lustrous Globe",[795]="Black Augurite",[796]="Linking Cord",[797]="Peat Block",[798]="Berserk Gene",[799]="Fairy Feather",[800]="Syrupy Apple",[801]="Unremarkable Teacup",[802]="Masterpiece Teacup",[803]="Cornerstone Mask",[804]="Wellspring Mask",[805]="Hearthflame Mask",[806]="Health Mochi",[807]="Muscle Mochi",[808]="Resist Mochi",[809]="Genius Mochi",[810]="Clever Mochi",[811]="Swift Mochi",[812]="Fresh Start Mochi",[813]="Glimmering Charm",[814]="Metal Alloy",[815]="Stellar Tera Shard",[816]="Jubilife Muffin",[817]="Remedy",[818]="Fine Remedy",[819]="Superb Remedy",[820]="Aux Evasion",[821]="Aux Guard",[822]="Aux Power",[823]="Aux Powerguard",[824]="Choice Dumpling",[825]="Swap Snack",[826]="Twice-Spiced Radish",[827]="Pokéshi Doll",[828]="Strange Ball"}
local ABILITY_NAMES = {[0]="-------",[1]="Stench",[2]="Drizzle",[3]="Speed Boost",[4]="Battle Armor",[5]="Sturdy",[6]="Damp",[7]="Limber",[8]="Sand Veil",[9]="Static",[10]="Volt Absorb",[11]="Water Absorb",[12]="Oblivious",[13]="Cloud Nine",[14]="Compound Eyes",[15]="Insomnia",[16]="Color Change",[17]="Immunity",[18]="Flash Fire",[19]="Shield Dust",[20]="Own Tempo",[21]="Suction Cups",[22]="Intimidate",[23]="Shadow Tag",[24]="Rough Skin",[25]="Wonder Guard",[26]="Levitate",[27]="Effect Spore",[28]="Synchronize",[29]="Clear Body",[30]="Natural Cure",[31]="Lightning Rod",[32]="Serene Grace",[33]="Swift Swim",[34]="Chlorophyll",[35]="Illuminate",[36]="Trace",[37]="Huge Power",[38]="Poison Point",[39]="Inner Focus",[40]="Magma Armor",[41]="Water Veil",[42]="Magnet Pull",[43]="Soundproof",[44]="Rain Dish",[45]="Sand Stream",[46]="Pressure",[47]="Thick Fat",[48]="Early Bird",[49]="Flame Body",[50]="Run Away",[51]="Keen Eye",[52]="Hyper Cutter",[53]="Pickup",[54]="Truant",[55]="Hustle",[56]="Cute Charm",[57]="Plus",[58]="Minus",[59]="Forecast",[60]="Sticky Hold",[61]="Shed Skin",[62]="Guts",[63]="Marvel Scale",[64]="Liquid Ooze",[65]="Overgrow",[66]="Blaze",[67]="Torrent",[68]="Swarm",[69]="Rock Head",[70]="Drought",[71]="Arena Trap",[72]="Vital Spirit",[73]="White Smoke",[74]="Pure Power",[75]="Shell Armor",[76]="Air Lock",[77]="Tangled Feet",[78]="Motor Drive",[79]="Rivalry",[80]="Steadfast",[81]="Snow Cloak",[82]="Gluttony",[83]="Anger Point",[84]="Unburden",[85]="Heatproof",[86]="Simple",[87]="Dry Skin",[88]="Download",[89]="Iron Fist",[90]="Poison Heal",[91]="Adaptability",[92]="Skill Link",[93]="Hydration",[94]="Solar Power",[95]="Quick Feet",[96]="Normalize",[97]="Sniper",[98]="Magic Guard",[99]="No Guard",[100]="Stall",[101]="Technician",[102]="Leaf Guard",[103]="Klutz",[104]="Mold Breaker",[105]="Super Luck",[106]="Aftermath",[107]="Anticipation",[108]="Forewarn",[109]="Unaware",[110]="Tinted Lens",[111]="Filter",[112]="Slow Start",[113]="Scrappy",[114]="Storm Drain",[115]="Ice Body",[116]="Solid Rock",[117]="Snow Warning",[118]="Honey Gather",[119]="Frisk",[120]="Reckless",[121]="Multitype",[122]="Flower Gift",[123]="Bad Dreams",[124]="Pickpocket",[125]="Sheer Force",[126]="Contrary",[127]="Unnerve",[128]="Defiant",[129]="Defeatist",[130]="Cursed Body",[131]="Healer",[132]="Friend Guard",[133]="Weak Armor",[134]="Heavy Metal",[135]="Light Metal",[136]="Multiscale",[137]="Toxic Boost",[138]="Flare Boost",[139]="Harvest",[140]="Telepathy",[141]="Moody",[142]="Overcoat",[143]="Poison Touch",[144]="Regenerator",[145]="Big Pecks",[146]="Sand Rush",[147]="Wonder Skin",[148]="Analytic",[149]="Illusion",[150]="Imposter",[151]="Infiltrator",[152]="Mummy",[153]="Moxie",[154]="Justified",[155]="Rattled",[156]="Magic Bounce",[157]="Sap Sipper",[158]="Prankster",[159]="Sand Force",[160]="Iron Barbs",[161]="Zen Mode",[162]="Victory Star",[163]="Turboblaze",[164]="Teravolt",[165]="Aroma Veil",[166]="Flower Veil",[167]="Cheek Pouch",[168]="Protean",[169]="Fur Coat",[170]="Magician",[171]="Bulletproof",[172]="Competitive",[173]="Strong Jaw",[174]="Refrigerate",[175]="Sweet Veil",[176]="Stance Change",[177]="Gale Wings",[178]="Mega Launcher",[179]="Grass Pelt",[180]="Symbiosis",[181]="Tough Claws",[182]="Pixilate",[183]="Gooey",[184]="Aerilate",[185]="Parental Bond",[186]="Dark Aura",[187]="Fairy Aura",[188]="Aura Break",[189]="Primordial Sea",[190]="Desolate Land",[191]="Delta Stream",[192]="Stamina",[193]="Wimp Out",[194]="Emergency Exit",[195]="Water Compaction",[196]="Merciless",[197]="Shields Down",[198]="Stakeout",[199]="Water Bubble",[200]="Steelworker",[201]="Berserk",[202]="Slush Rush",[203]="Long Reach",[204]="Liquid Voice",[205]="Triage",[206]="Galvanize",[207]="Surge Surfer",[208]="Schooling",[209]="Disguise",[210]="Battle Bond",[211]="Power Construct",[212]="Corrosion",[213]="Comatose",[214]="Queenly Majesty",[215]="Innards Out",[216]="Dancer",[217]="Battery",[218]="Fluffy",[219]="Dazzling",[220]="Soul-Heart",[221]="Tangling Hair",[222]="Receiver",[223]="Power Of Alchemy",[224]="Beast Boost",[225]="RKS System",[226]="Electric Surge",[227]="Psychic Surge",[228]="Misty Surge",[229]="Grassy Surge",[230]="Full Metal Body",[231]="Shadow Shield",[232]="Prism Armor",[233]="Neuroforce",[234]="Intrepid Sword",[235]="Dauntless Shield",[236]="Libero",[237]="Ball Fetch",[238]="Cotton Down",[239]="Propeller Tail",[240]="Mirror Armor",[241]="Gulp Missile",[242]="Stalwart",[243]="Steam Engine",[244]="Punk Rock",[245]="Sand Spit",[246]="Ice Scales",[247]="Ripen",[248]="Ice Face",[249]="Power Spot",[250]="Mimicry",[251]="Screen Cleaner",[252]="Steely Spirit",[253]="Perish Body",[254]="Wandering Spirit",[255]="Gorilla Tactics",[256]="Neutralizing Gas",[257]="Pastel Veil",[258]="Hunger Switch",[259]="Quick Draw",[260]="Unseen Fist",[261]="Curious Medicine",[262]="Transistor",[263]="Dragon’s Maw",[264]="Chilling Neigh",[265]="Grim Neigh",[266]="As One",[267]="As One",[268]="Lingering Aroma",[269]="Seed Sower",[270]="Thermal Exchange",[271]="Anger Shell",[272]="Purifying Salt",[273]="Well-Baked Body",[274]="Wind Rider",[275]="Guard Dog",[276]="Rocky Payload",[277]="Wind Power",[278]="Zero to Hero",[279]="Commander",[280]="Electromorphosis",[281]="Protosynthesis",[282]="Quark Drive",[283]="Good as Gold",[284]="Vessel of Ruin",[285]="Sword of Ruin",[286]="Tablets of Ruin",[287]="Beads of Ruin",[288]="Orichalcum Pulse",[289]="Hadron Engine",[290]="Opportunist",[291]="Cud Chew",[292]="Sharpness",[293]="Supreme Overlord",[294]="Costar",[295]="Toxic Debris",[296]="Armor Tail",[297]="Earth Eater",[298]="Mycelium Might",[299]="Hospitality",[300]="Mind’s Eye",[301]="Embody Aspect",[302]="Embody Aspect",[303]="Embody Aspect",[304]="Embody Aspect",[305]="Toxic Chain",[306]="Supersweet Syrup",[307]="Tera Shift",[308]="Tera Shell",[309]="Teraform Zero",[310]="Poison Puppeteer"}
-- END GENERATED

local PARTY_MON_SIZE = 100
local BOX_MON_SIZE = 80
local TOTAL_BOXES = 14
local IN_BOX = 30

-- struct Pokemon offsets
local OFF_PERSONALITY, OFF_OTID, OFF_NICKNAME = 0, 4, 8
local OFF_CHECKSUM, OFF_SECURE = 28, 32
local OFF_STATUS, OFF_LEVEL, OFF_HP, OFF_MAXHP = 80, 84, 86, 88

-- SpeciesInfo field offsets
local SI_GROWTH_RATE, SI_ABILITIES = 21, 24

local NATURES = {
    "Hardy","Lonely","Brave","Adamant","Naughty","Bold","Docile","Relaxed","Impish","Lax",
    "Timid","Hasty","Serious","Jolly","Naive","Modest","Mild","Quiet","Bashful","Rash",
    "Calm","Gentle","Sassy","Careful","Quirky",
}

-- position of substruct type t (0-3) for personality % 24, as EK.lua's table
local substructSelector = {
    [0]={0,1,2,3},[1]={0,1,3,2},[2]={0,2,1,3},[3]={0,3,1,2},[4]={0,2,3,1},[5]={0,3,2,1},
    [6]={1,0,2,3},[7]={1,0,3,2},[8]={2,0,1,3},[9]={3,0,1,2},[10]={2,0,3,1},[11]={3,0,2,1},
    [12]={1,2,0,3},[13]={1,3,0,2},[14]={2,1,0,3},[15]={3,1,0,2},[16]={2,3,0,1},[17]={3,2,0,1},
    [18]={1,2,3,0},[19]={1,3,2,0},[20]={2,1,3,0},[21]={3,1,2,0},[22]={2,3,1,0},[23]={3,2,1,0},
}

local buffer = nil
local function out(s)
    if buffer then buffer:print(s .. "\n") end
    console:log(s)
end

-- ==========================================================================
-- reading / writing a Pokemon
-- ==========================================================================

local function partyCount() return emu:read8(ADDR.gPlayerPartyCount) end
local function partyAddr(slot) return ADDR.gPlayerParty + (slot - 1) * PARTY_MON_SIZE end

local function validSlot(slot)
    if type(slot) ~= "number" or slot < 1 or slot > partyCount() then
        console:log("Invalid Slot, slot out of range")
        return false
    end
    return true
end

-- decrypted secure data: sub[t][w] = word w (0-2) of substruct type t (0-3)
local function decrypt(addr)
    local pers = emu:read32(addr + OFF_PERSONALITY)
    local key = pers ~ emu:read32(addr + OFF_OTID)
    local sel = substructSelector[pers % 24]
    local sub = {}
    for t = 0, 3 do
        sub[t] = {}
        for w = 0, 2 do
            sub[t][w] = (emu:read32(addr + OFF_SECURE + sel[t + 1] * 12 + w * 4) ~ key) & 0xFFFFFFFF
        end
    end
    return sub, key, sel
end

-- write decrypted data back (re-encrypt + fix checksum)
local function encrypt(addr, sub, key, sel)
    local sum = 0
    for t = 0, 3 do
        for w = 0, 2 do
            local v = sub[t][w] & 0xFFFFFFFF
            sum = sum + (v & 0xFFFF) + (v >> 16)
            emu:write32(addr + OFF_SECURE + sel[t + 1] * 12 + w * 4, (v ~ key) & 0xFFFFFFFF)
        end
    end
    emu:write16(addr + OFF_CHECKSUM, sum & 0xFFFF)
end

local function decodeText(addr, len)
    local s = {}
    for i = 0, len - 1 do
        local b = emu:read8(addr + i)
        if b == 0xFF then break end
        if b >= 0xBB and b <= 0xD4 then s[#s+1] = string.char(65 + b - 0xBB)
        elseif b >= 0xD5 and b <= 0xEE then s[#s+1] = string.char(97 + b - 0xD5)
        elseif b >= 0xA1 and b <= 0xAA then s[#s+1] = string.char(48 + b - 0xA1)
        else
            local sp = { [0x00]=" ", [0xAB]="!", [0xAC]="?", [0xAD]=".", [0xAE]="-", [0xB4]="'",
                         [0xB5]="M", [0xB6]="F", [0xB8]=",", [0xBA]="/", [0xF0]=":", [0x1B]="e" }
            s[#s+1] = sp[b] or ""
        end
    end
    return table.concat(s)
end

local function speciesField(species, off) return ADDR.gSpeciesInfoBase + species * SPECIES_INFO_STRIDE + off end
local function speciesName(species)
    if species == 0 then return "None" end
    return decodeText(speciesField(species, SPECIES_NAME_OFFSET), 12)
end

-- Experience curves (same formulas as the game's tables)
local function expRequiredForRate(rate, n)
    if n <= 1 then return 0 end
    local n3 = n * n * n
    if rate == 0 then return n3                                            -- medium fast
    elseif rate == 1 then                                                  -- erratic
        if n <= 50 then return (n3 * (100 - n)) // 50
        elseif n <= 68 then return (n3 * (150 - n)) // 100
        elseif n <= 98 then return (n3 * ((1911 - 10 * n) // 3)) // 500
        else return (n3 * (160 - n)) // 100 end
    elseif rate == 2 then                                                  -- fluctuating
        if n <= 15 then return (n3 * ((n + 1) // 3 + 24)) // 50
        elseif n <= 36 then return (n3 * (n + 14)) // 50
        else return (n3 * (n // 2 + 32)) // 50 end
    elseif rate == 3 then return (6 * n3) // 5 - 15 * n * n + 100 * n - 140 -- medium slow
    elseif rate == 4 then return (4 * n3) // 5                             -- fast
    elseif rate == 5 then return (5 * n3) // 4                             -- slow
    end
    return n3
end

function expRequired(species, level)
    return expRequiredForRate(emu:read8(speciesField(species, SI_GROWTH_RATE)), level)
end

function calcLevel(exp, species)
    local level = 1
    while level < 100 and expRequired(species, level + 1) <= exp do level = level + 1 end
    return level
end

local function readMon(addr, isParty)
    local sub = decrypt(addr)
    local m = {}
    m.personality = emu:read32(addr + OFF_PERSONALITY)
    m.nickname = decodeText(addr + OFF_NICKNAME, 10):match("^%s*(.-)%s*$")
    m.species = sub[0][0] & 0x7FF
    m.heldItem = (sub[0][0] >> 16) & 0x3FF
    m.experience = sub[0][1] & 0x1FFFFF
    m.ppBonuses = sub[0][2] & 0xFF
    m.moves = { sub[1][0] & 0x7FF, (sub[1][0] >> 16) & 0x7FF, sub[1][1] & 0x7FF, (sub[1][1] >> 16) & 0x7FF }
    m.evs = { sub[2][0] & 0xFF, (sub[2][0] >> 8) & 0xFF, (sub[2][0] >> 16) & 0xFF,
              (sub[2][1]) & 0xFF, (sub[2][1] >> 8) & 0xFF, (sub[2][0] >> 24) & 0xFF }  -- HP Atk Def SpA SpD Spe
    local iv = sub[3][1]
    m.ivs = { iv & 31, (iv >> 5) & 31, (iv >> 10) & 31, (iv >> 20) & 31, (iv >> 25) & 31, (iv >> 15) & 31 }
    m.isEgg = (iv >> 30) & 1
    m.abilityNum = (sub[3][2] >> 29) & 3
    m.nature = m.personality % 25
    if m.species ~= 0 then
        m.ability = emu:read16(speciesField(m.species, SI_ABILITIES + 2 * m.abilityNum))
        if m.ability == 0 then m.ability = emu:read16(speciesField(m.species, SI_ABILITIES)) end
    end
    if isParty then
        m.status = emu:read32(addr + OFF_STATUS)
        m.level = emu:read8(addr + OFF_LEVEL)
        m.hp = emu:read16(addr + OFF_HP)
        m.maxHP = emu:read16(addr + OFF_MAXHP)
    elseif m.species ~= 0 then
        m.level = calcLevel(m.experience, m.species)
    end
    return m
end

local function monText(m)
    local name = speciesName(m.species)
    local head = (m.nickname ~= "" and m.nickname:upper() ~= name:upper()) and (m.nickname .. " (" .. name .. ")") or name
    if m.heldItem ~= 0 then head = head .. " @ " .. (ITEM_NAMES[m.heldItem] or ("Item #" .. m.heldItem)) end
    local l = { head }
    l[#l+1] = "Ability: " .. (ABILITY_NAMES[m.ability or 0] or ("Ability #" .. tostring(m.ability)))
    l[#l+1] = "Level: " .. m.level
    local e = m.evs
    if e[1] + e[2] + e[3] + e[4] + e[5] + e[6] > 0 then
        l[#l+1] = string.format("EVs: %d HP / %d Atk / %d Def / %d SpA / %d SpD / %d Spe", e[1], e[2], e[3], e[4], e[5], e[6])
    end
    l[#l+1] = NATURES[m.nature + 1] .. " Nature"
    local v = m.ivs
    l[#l+1] = string.format("IVs: %d HP / %d Atk / %d Def / %d SpA / %d SpD / %d Spe", v[1], v[2], v[3], v[4], v[5], v[6])
    for i = 1, 4 do
        if m.moves[i] ~= 0 then l[#l+1] = "- " .. (MOVE_NAMES[m.moves[i]] or ("Move #" .. m.moves[i])) end
    end
    return table.concat(l, "\n")
end

local function writeFile(text)
    local ok, f = pcall(io.open, "bofa_export.txt", "w")
    if ok and f then f:write(text) f:close() return true end
    return false
end

-- ==========================================================================
-- EK commands
-- ==========================================================================

function statusSlot(slot, status)
    if validSlot(slot) then emu:write32(partyAddr(slot) + OFF_STATUS, status) end
end

function sleep(slot) statusSlot(slot, 1) end
function poison(slot) statusSlot(slot, 8) end
function burn(slot) statusSlot(slot, 16) end
function freeze(slot) statusSlot(slot, 32) end
function paralyze(slot) statusSlot(slot, 64) end
function toxic(slot) statusSlot(slot, 128) end
function cure(slot) statusSlot(slot, 0) end

function bedtime()
    for i = partyCount(), 1, -1 do statusSlot(i, 1) end
end

function sethp(slot, hp)
    if not validSlot(slot) then return end
    local a = partyAddr(slot)
    hp = math.max(0, math.min(hp, emu:read16(a + OFF_MAXHP)))
    emu:write16(a + OFF_HP, hp)
    out(string.format("Slot %d HP set to %d/%d", slot, hp, emu:read16(a + OFF_MAXHP)))
end

-- damage(slot, amount): take HP off
function damage(slot, amount)
    if not validSlot(slot) then return end
    sethp(slot, emu:read16(partyAddr(slot) + OFF_HP) - amount)
end

function edge(slot, xpToLevelUp)
    xpToLevelUp = xpToLevelUp or 1
    if not validSlot(slot) then return end
    local a = partyAddr(slot)
    local level = emu:read8(a + OFF_LEVEL)
    if level >= 100 then console:log("Invalid Level") return end
    local sub, key, sel = decrypt(a)
    local species = sub[0][0] & 0x7FF
    local target = expRequired(species, level + 1) - xpToLevelUp
    if target < expRequired(species, level) then target = expRequired(species, level) end
    sub[0][1] = (sub[0][1] & ~0x1FFFFF) | (target & 0x1FFFFF)
    encrypt(a, sub, key, sel)
    out(string.format("Slot %d (%s Lv%d) is now %d EXP from Lv%d", slot, speciesName(species), level,
        expRequired(species, level + 1) - target, level + 1))
end

function edgeparty()
    for i = partyCount(), 1, -1 do edge(i) end
end

function heal(slot)
    if not validSlot(slot) then return end
    local a = partyAddr(slot)
    emu:write16(a + OFF_HP, emu:read16(a + OFF_MAXHP))
    emu:write32(a + OFF_STATUS, 0)
    local sub, key, sel = decrypt(a)
    local bonuses = sub[0][2] & 0xFF
    local moves = { sub[1][0] & 0x7FF, (sub[1][0] >> 16) & 0x7FF, sub[1][1] & 0x7FF, (sub[1][1] >> 16) & 0x7FF }
    local ppWord = sub[1][2]
    for i = 0, 3 do
        local mv = moves[i + 1]
        if mv ~= 0 and MOVE_PP[mv] then
            local base = MOVE_PP[mv]
            local maxPP = base + (base * ((bonuses >> (2 * i)) & 3)) // 5
            local keep = (ppWord >> (8 * i)) & 0x80        -- hyper-trained bit
            ppWord = (ppWord & ~(0xFF << (8 * i))) | ((keep | (maxPP & 0x7F)) << (8 * i))
        end
    end
    sub[1][2] = ppWord & 0xFFFFFFFF
    encrypt(a, sub, key, sel)
    out(string.format("Healed slot %d (HP, PP, status)", slot))
end

function nursejoy()
    for i = partyCount(), 1, -1 do heal(i) end
end
healall = nursejoy

-- ivs(slot): show one party Pokemon's IVs / nature / ability
function ivs(slot)
    if not validSlot(slot) then return end
    local m = readMon(partyAddr(slot), true)
    local v = m.ivs
    out(string.format("Slot %d %s: IVs %d HP / %d Atk / %d Def / %d SpA / %d SpD / %d Spe | %s | %s",
        slot, speciesName(m.species), v[1], v[2], v[3], v[4], v[5], v[6], NATURES[m.nature + 1],
        ABILITY_NAMES[m.ability or 0] or "?"))
end

function printparty()
    for slot = 1, partyCount() do
        local m = readMon(partyAddr(slot), true)
        out(string.format("Slot %d: %s Lv%d  HP %d/%d  status=0x%X", slot, speciesName(m.species), m.level, m.hp, m.maxHP, m.status))
    end
end

function exportparty()
    if buffer then buffer:clear() end
    local chunks = {}
    for slot = 1, partyCount() do
        local m = readMon(partyAddr(slot), true)
        if m.species ~= 0 and m.isEgg == 0 then chunks[#chunks+1] = monText(m) end
    end
    local text = table.concat(chunks, "\n\n")
    out(text)
    writeFile(text)
    return text
end

function exportmon(slot)
    if not validSlot(slot) then return end
    local text = monText(readMon(partyAddr(slot), true))
    out(text)
    writeFile(text)
end

function exportall()
    local chunks = { exportparty() }
    local storage = emu:read32(ADDR.gPokemonStoragePtr)
    if storage ~= 0 then
        for i = 0, TOTAL_BOXES * IN_BOX - 1 do
            local a = storage + 4 + i * BOX_MON_SIZE
            if emu:read32(a + OFF_PERSONALITY) ~= 0 then
                local m = readMon(a, false)
                if m.species ~= 0 and m.isEgg == 0 then
                    local t = monText(m)
                    out("\n" .. t)
                    chunks[#chunks+1] = t
                end
            end
        end
    end
    writeFile(table.concat(chunks, "\n\n"))
end

local B_WEATHER = { rain = 4, sun = 128, sand = 32, hail = 1024, none = 0 }
function setweather(name)
    local v = B_WEATHER[string.lower(name)]
    if v == nil then console:log("Unknown weather (rain/sun/sand/hail/none)") return end
    emu:write16(ADDR.gBattleWeather, v)
    out("Battle weather set to " .. name .. " (only during a battle)")
end

function help()
    if buffer then buffer:clear() end
    out("Available commands:")
    out(" sleep(slot) - Pre-sleeps chosen slot in party")
    out(" poison(slot) - Poisons chosen slot in party")
    out(" paralyze(slot) - Paralyzes chosen slot in party")
    out(" burn(slot) - Burns chosen slot in party")
    out(" freeze(slot) - Freezes chosen slot in party")
    out(" toxic(slot) / cure(slot) - Badly poisons / clears status")
    out(" bedtime() - Pre-sleeps entire party")
    out(" sethp(slot,HP) - sets hp of slot to specified hp")
    out(" damage(slot,amount) - takes that much HP off slot")
    out(" ivs(slot) - shows IVs, nature and ability of slot")
    out(" exportparty() - exports showdown calc version of party (also bofa_export.txt)")
    out(" exportall() - exports showdown calc version of party + all PC boxes")
    out(" edge(slot) - edges slot (1 EXP from next level)")
    out(" edge(slot, amt) - edges slot to specific amount")
    out(" edgeparty() - edges entire party")
    out(" heal(slot) - heals all HP, PP and status for that party slot")
    out(" nursejoy() - heals all HP, PP and status for the entire party")
    out(" printparty() - one-line summary of each party slot")
end

if emu then
    buffer = console:createBuffer("Exports")
    if buffer then buffer:setSize(200, 1000) end
    help()
end
