"""Split 1 restructure (2026-10-01): play order, statuses and notes for the docs.

Read by build_pk_format.py. Since the restructure is now built into the ROM, every
trainer comes from trainers.party via `from_id`; this file only sets the play order,
mandatory/optional status and the notes shown on each trainer card.
The rival (Brendan/May) keeps its in-game names; it is not Spark.
"""

PLAN_TAG = "In ROM, not playtested yet"

_RIVAL_NOTE = ("Rival - fights once, on the tile in front of the gym door, after the Everstone quest. "
               "Same approved six-Pokemon Gen 6 team in every variant (Brendan/May x player's starter).")

SPLIT1_TRAINERS = [
    # 1-4 Route 102 (existing teams, all mandatory)
    {"order": 1, "from_id": "TRAINER_CALVIN_1", "loc": "Route102", "status": "REQUIRED"},
    {"order": 2, "from_id": "TRAINER_RICK", "loc": "Route102", "status": "REQUIRED",
     "note": "Now placed on the map at (21,10)."},
    {"order": 3, "from_id": "TRAINER_TIANA", "loc": "Route102", "status": "REQUIRED"},
    {"order": 4, "from_id": "TRAINER_ALLEN", "loc": "Route102", "status": "REQUIRED"},
    # 5-8 Route 104 (south of Petalburg Woods). Norman checks all four before giving the quest.
    {"order": 5, "from_id": "TRAINER_DARIAN", "loc": "Route104", "status": "REQUIRED",
     "note": "Fisherman slot. Moved from Split 2."},
    {"order": 6, "from_id": "TRAINER_BILLY", "loc": "Route104", "status": "REQUIRED",
     "note": "Regular trainer slot. Moved from Split 2."},
    {"order": 7, "from_id": "TRAINER_GRUNT_ROUTE_104_MAGMA", "loc": "Route104", "status": "REQUIRED",
     "note": "First Team Magma battle. New."},
    {"order": 8, "from_id": "TRAINER_GRUNT_ROUTE_104_AQUA", "loc": "Route104", "status": "REQUIRED",
     "note": "First Team Aqua battle. New."},
    # 9-11 Trainer Grove (gated until Norman gives the quest)
    {"order": 9, "from_id": "TRAINER_TRAINERGROVET1", "loc": "TrainerGrove", "status": "REQUIRED",
     "note": "Hiker Rocco. Nidoran-M intentionally has three moves (no Focus Energy)."},
    {"order": 10, "from_id": "TRAINER_TRAINERGROVET3", "loc": "TrainerGrove", "status": "REQUIRED",
     "note": "Camper Grant."},
    {"order": 11, "from_id": "TRAINER_TRAINERGROVET2", "loc": "TrainerGrove", "status": "REQUIRED",
     "note": "Picnicker Maisie, Gen 5 specialist."},
    # 12 Rival at the gym entrance (6 variants)
    *[{"order": 12, "from_id": f"TRAINER_{r}_PETALBURG_{s}", "loc": "PetalburgCity_GymEntrance",
       "status": "REQUIRED", "note": f"{_RIVAL_NOTE} This one: player chose {s.title()}."}
      for r in ("BRENDAN", "MAY") for s in ("TREECKO", "TORCHIC", "MUDKIP")],
    # 13-16 Petalburg Gym (unchanged)
    {"order": 13, "from_id": "TRAINER_RANDALL", "loc": "PetalburgCity_Gym", "status": "REQUIRED",
     "note": "Gym Youngster. Rooms unlock in order Randall -> Alexia -> Jody; Norman checks all three."},
    {"order": 14, "from_id": "TRAINER_ALEXIA", "loc": "PetalburgCity_Gym", "status": "REQUIRED",
     "note": "Gym defensive trainer. Solosis has even IVs so its Hidden Power is Fighting-type."},
    {"order": 15, "from_id": "TRAINER_JODY", "loc": "PetalburgCity_Gym", "status": "REQUIRED",
     "note": "Gym Ace Trainer. Girafarig intentionally has three moves."},
    {"order": 16, "from_id": "TRAINER_NORMAN_1", "loc": "PetalburgCity_Gym", "status": "REQUIRED"},
    # Optional: Viridian Forest
    {"order": 11.1, "from_id": "TRAINER_VIRIDIAN_FOREST_1", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": "Bug Catcher. Team finalized."},
    {"order": 11.2, "from_id": "TRAINER_VIRIDIAN_FOREST_3", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": "Gen 1 specialist. Beedrill intentionally has only Leech Life + Poison Sting."},
]

# Route 103 trainers leave Split 1 (east side is across water). Shown on the Unassigned sheet.
ROUTE_103_IDS = ["TRAINER_AMY_AND_LIV_1", "TRAINER_ANDREW", "TRAINER_DAISY", "TRAINER_ISABELLE",
                 "TRAINER_MARCOS", "TRAINER_MIGUEL_1", "TRAINER_PETE", "TRAINER_RHETT"]
REMOVED_RIVAL_PREFIXES = ["TRAINER_BRENDAN_ROUTE_103", "TRAINER_MAY_ROUTE_103"]

PROGRESSION = [
    # step, area, what happens, trainers, gate / event, status
    (1, "Littleroot Town - Birch's lab", "Pokédex, 5 Poké Balls and 90 Rare Candies right after the starter",
     "-", "Route 103 rival battle removed (FLAG_HIDE_ROUTE_103_RIVAL set)", PLAN_TAG),
    (2, "Littleroot Town - Starter Room", "Free gift: choose Chikorita, Cyndaquil or Totodile (Lv5)",
     "-", "Needs the Pokédex first; one pick only (FLAG_RECEIVED_STARTER_ROOM_JOHTO_MON)", PLAN_TAG),
    (3, "Oldale Town", "Hub", "-", "-", "In ROM"),
    (4, "Route 102", "Travel Oldale -> Petalburg", "4 (Calvin, Rick, Tiana, Allen)", "No rival battle", PLAN_TAG),
    (5, "Petalburg City (first visit)", "NPC in front of the gym door: Norman is on Route 104 looking for an Everstone",
     "-", "Blocker hidden once VAR_SPLIT1_STATE >= 2", PLAN_TAG),
    (6, "Route 104 (south)", "Beat 4 trainers, then talk to Norman at (12,58)",
     "4 (Fisherman Darian, Youngster Billy, Magma Grunt, Aqua Grunt)",
     "Norman checks all four, then sets VAR_SPLIT1_STATE = 1 and leaves", PLAN_TAG),
    (7, "Trainer Grove", "Path to the Everstone", "3 (Hiker Rocco, Camper Grant, Picnicker Maisie)",
     "Entrance turns the player back while VAR_SPLIT1_STATE = 0", PLAN_TAG),
    (8, "Petalburg Coast (your \"Shoal Cave\")", "Everstone item ball, one-time Corphish Lv11, "
     "fisherman points to Viridian Forest once you have the stone", "None (Ronnie removed)",
     "Returning to Petalburg with the stone sets VAR_SPLIT1_STATE = 2", PLAN_TAG),
    (9, "Viridian Forest (optional)", "2 trainers, new wild table (Kanto bugs, Pidgey, rare Pikachu), "
     "HM06 Rock Smash from the Black Belt at the end", "2 optional", "Not required for the gym", PLAN_TAG),
    (10, "Petalburg City (return)", "Blocker gone; the rival battles on the tile in front of the gym door",
     "Rival - once", "Win sets VAR_SPLIT1_STATE = 3; a loss lets the rival challenge again", PLAN_TAG),
    (11, "Petalburg Gym", "Hand Norman the Everstone, beat Jody, then Norman -> Badge 1",
     "Randall, Alexia, Jody (all required)", "FLAG_GAVE_NORMAN_EVERSTONE, FLAG_BADGE01_GET", "In ROM"),
]

GIFTS = {
    "remove_trade_species_at": ("ViridianForest", ["Chikorita", "Cyndaquil", "Totodile"]),
    "johto_gift_note": "Free gift - choose one (not a trade). Littleroot Town Starter Room, after the Pokédex.",
}

# Gift / trade / one-time Pokemon, checked against the map scripts and src/data/trade.h.
# (split, location, how you get it, what you give up, species, level, nature, ability, IVs, held item, moves, notes)
GIFT_POKEMON = [
    ("Split 1", "Route 101", "Starter from Prof. Birch's bag (pick one)", "Nothing", "Treecko / Torchic / Mudkip", 5,
     "Random", "Treecko: Overgrow / Torchic: Blaze / Mudkip: Huge Power", "Random", "-", "Level-up moves", "Choose one; your rival takes another"),
    ("Split 1", "Littleroot Town (Starter Room)", "Gift after you get the Pokedex (pick one)", "Nothing", "Chikorita / Cyndaquil / Totodile", 5,
     "Random", "Chikorita: Overgrow / Cyndaquil: Blaze / Totodile: Torrent", "Random", "-", "Level-up moves", "Free gift, choose one. Evolves at Lv16"),
    ("Split 1", "Petalburg City", "Trade with Dustin (rock trader)", "Any Pokemon", "Geodude or Rhyhorn (50/50)", 5,
     "Relaxed", "Geodude: Rock Head / Rhyhorn: Lightning Rod", "31 all", "-", "Level-up moves", "Random which one you get; once only. Nicknamed, OT DUSTIN"),
    ("Split 1", "Petalburg Coast", "One-time wild battle (Corphish in the open)", "Nothing (catch it)", "Corphish", 11,
     "Random", "Adaptability / Hyper Cutter", "Random", "-", "Level-up moves", "Battle disappears once beaten or caught"),
    ("Split 2", "Rustboro City (house)", "Trade with Kobe", "Ralts", "Seedot", "Same as the Ralts you give",
     "Relaxed", "Chlorophyll", "5/4/5/4/4/4", "Chesto Berry", "Level-up moves", "Nickname DOTS, OT KOBE"),
    ("Split 2", "Rustboro City (Devon Corp 2F)", "Revived fossil from the scientist", "Root Fossil or Claw Fossil", "Lileep or Anorith", 20,
     "Random", "Lileep: Solid Rock / Anorith: Swift Swim or Battle Armor", "Random", "-", "Level-up moves", "Which one depends on the fossil you bring"),
    ("Split 3", "Granite Shore", "Gift from the NPC on the shore", "Nothing", "Dwebble", 20,
     "Adamant", "Sturdy", "31 all", "-", "Rock Blast, Bug Bite, Pin Missile, Protect", "Fixed moveset"),
    ("Split 4", "Verdanturf Town", "Trade with Caspian (Skorupi trader)", "Any Pokemon", "Skorupi", 20,
     "Jolly", "Battle Armor", "31 all", "-", "Poison Jab, Leech Life, Bite, Slash", "Once only. Nickname SKORUPI, OT CASPIAN"),
    ("Split 6", "Fortree City (house)", "Trade", "Volbeat", "Plusle", "Same as the Volbeat you give",
     "Hasty", "Plus", "4/4/4/5/5/4", "Wood Mail", "Level-up moves", "Vanilla trade, not redesigned yet"),
    ("Split 6", "Route 119 (Weather Institute)", "Gift after clearing the Weather Institute", "Nothing", "Castform", 25,
     "Random", "Forecast", "Random", "-", "Level-up moves", "Vanilla gift, not redesigned yet"),
    ("Split 7", "Mossdeep City (Steven's house)", "Gift (Poke Ball on the table)", "Nothing", "Beldum", 5,
     "Random", "Clear Body", "Random", "-", "Level-up moves", "Vanilla gift, not redesigned yet"),
    ("Postgame", "Littleroot Town (Birch's lab)", "Choice after the National Dex", "Nothing", "Chikorita / Cyndaquil / Totodile", 5,
     "Random", "Chikorita: Overgrow / Cyndaquil: Blaze / Totodile: Torrent", "Random", "-", "Level-up moves", "Vanilla postgame gift"),
]

# NPC gifts and rewards (the doc generator only sees item balls, hidden items and marts).
NEW_ITEMS = [
    # item, method, map, split, note
    ("Good Rod", "NPC gift (Fisherman)", "OldaleTown", "Split 1", "Shares its flag with the Route 118 gift."),
    ("HM Rock Smash", "NPC gift (end of forest)", "ViridianForest", "Split 1", "Shares its flag with the Mauville gift."),
    ("TM Bullet Seed x2", "NPC gift (boy)", "Route104", "Split 1", "North end of Route 104 South"),
    ("Mystic Water", "NPC gift (Fisherman Darian, after battle)", "Route104", "Split 1", "Route 104 South"),
    ("Great Ball x5", "NPC gift (Norman, with the Everstone quest)", "Route104", "Split 1", "Route 104 South"),
    ("Miracle Seed", "NPC gift (Picnicker Maisie, after battle)", "TrainerGrove", "Split 1", ""),
    ("Silk Scarf", "NPC gift (Gentleman)", "PetalburgCity", "Split 1", "Explains type-boosting held items"),
    ("Focus Sash", "NPC gift (Wally runs up on first entering Petalburg)", "PetalburgCity", "Split 1",
     "The only Focus Sash in Splits 1-2"),
    ("TM Protect x1", "Gym reward (Norman)", "PetalburgCity_Gym", "Split 1", "Only TM given as a single copy"),
    ("Charcoal", "NPC gift (camper boy)", "PetalburgWoods", "Split 2", ""),
    ("Black Belt", "NPC gift (Cooltrainer Cole, after the double battle)", "Route116", "Split 2", "Cave-side end of Route 116"),
    ("Shiny Stone OR Moon Stone", "NPC choice (one only)", "LostCave", "Split 2", "Deepest point of Lost Cave"),
    ("Occa + Passho + Rindo Berry", "NPC gift (hiker, all three at once)", "RustboroCity_House1", "Split 2", "House next to the Gym"),
    ("HM01 Cut", "NPC gift (the Cutter, in his house)", "RustboroCity_CuttersHouse", "Split 2", "Clears the small trees in Petalburg Woods and Route 104 (needs Badge 1)"),
    ("TM68 Wing Attack x2", "NPC gift (Trainer School student)", "RustboroCity_PokemonSchool", "Split 2", "New BOFA TM"),
    ("TM Rock Tomb x2", "Gym reward (Roxanne)", "RustboroCity_Gym", "Split 2", ""),
    ("Town Map", "NPC gift (Mom, after you get the Pokedex)", "LittlerootTown_BrendansHouse_1F", "Split 1", "Your house"),
    ("Old Gateau", "NPC gift (Mom, after you get the Pokedex)", "LittlerootTown_BrendansHouse_1F", "Split 1", "Your house"),
    ("RageCandyBar", "Dropped by the Aqua grunt as he flees", "RusturfTunnel", "Split 3", "Converted Shoal Shell"),
    ("Lum Berry x5", "NPC gift (old woman)", "Hollowbrook", "Split 3", ""),
    ("HM05 Flash", "NPC gift (hiker)", "GraniteCave_1F", "Split 3", ""),
    ("TM Brick Break x2", "Gym reward (Brawly + Phoebe tag battle)", "DewfordTown_Gym", "Split 3", ""),
]
REMOVED_ITEMS = []

# Still open, or places the build differs from the original plan.
FLAGS = [
    ("Trainer AI", "All trainers use the Generation 4 (Platinum Kaizo) AI: Basic + Evaluate Attack + Expert, scored exactly as in Reference/Platnimu AI/gen4_trainer_ai.md (src/battle_ai_gen4.c). Risky is added for evil-team trainers carrying Explosion or setup moves (currently the Rusturf grunt and Wraithwood grunt 4). After a KO, the AI sends in the Pokemon whose best move does the most damage to your current Pokemon (ties go to the earlier party slot)."),
    ("Not playtested", "Everything builds, but the Split 1 scripts and teams have not been played through yet."),
    ("Trainer-only abilities", "16 approved abilities are not on the species (e.g. Slaking Slow Start, Smeargle/Spinda "
     "Own Tempo, Vulpix Drought, Wingull Drizzle). An engine change now applies a trainer's listed ability anyway; "
     "wild and player Pokemon keep their normal abilities."),
    ("Weather setters", "The Magma grunt's Vulpix (Drought + Heat Rock) and Aqua grunt's Wingull (Drizzle + Damp Rock) "
     "set weather. Earlier weather removal only covered species, learnsets and TMs, so these are allowed."),
    ("Level cap 16", "No level cap exists in the ROM (B_LEVEL_CAP_TYPE = LEVEL_CAP_NONE). Not implemented."),
    ("Viridian Forest", "Now exactly two trainers. Bug Catcher Doug was removed from the map (his trainer data is "
     "unused)."),
    ("Everstone location", "Your plan says Shoal Cave; you confirmed Petalburg Coast."),
    ("Route 104 extras", "Lady Cindy (16,42) and an unscripted girl (21,50) are also on the south part of Route 104."),
]

# ---------------------------------------------------------------- Split 2 (progression only; teams come later)
SPLIT2_PROGRESSION = [
    (1, "Route 104 (south)", "Hikers blocking Petalburg Woods leave once the player has Badge 1",
     "-", "Route 104 sets FLAG_PETALBURG_WOODS_UNLOCKED on entry when FLAG_BADGE01_GET is set", PLAN_TAG),
    (2, "Petalburg Woods", "4 mandatory battles in path order; Team Aqua's worldview introduced",
     "Bug Catcher James, Aqua Grunt 1 (idealist), Grunt 2 (pragmatist), Grunt 3 (ends justify the means)",
     "Sight lines verified unavoidable; Devon researcher ambush removed; Lyle removed", PLAN_TAG),
    (3, "Route 104 (north)", "Path to Rustboro", "Existing trainers (unchanged)", "-", "In ROM"),
    (4, "Rustboro City", "NPC blocks the gym door (27,19): Roxanne went to Route 116", "-",
     "Blocker steps aside to (28,20) once FLAG_ROUTE116_ROXANNE_FOUND is set", PLAN_TAG),
    (5, "Route 116", "Four double battles, each against two trainers, on unavoidable trigger lines",
     "Clark & Ethan (x=9), Diana & Chloe (x=21), Preston & Tasha (x=31), Cole & Maya (x=37); all Lv20",
     "Old Route 116 trainers removed", PLAN_TAG),
    (5.5, "Route 116 (east end)", "Roxanne and Sidney talking; Roxanne heads back, Sidney battles",
     "Sidney (Lv22, Lunatone Lv24)", "Beating Sidney (line x=43) sets FLAG_ROUTE116_ROXANNE_FOUND; Rusturf Tunnel stays optional",
     PLAN_TAG),
    (6, "Rustboro Gym", "Gym trainers + Roxanne -> Badge 2", "Existing (unchanged)", "FLAG_BADGE02_GET", "In ROM"),
]

# Split 3. Level cap Lv35.
SPLIT3_LEVEL_CAP = "Lv35"
SPLIT3_PROGRESSION = [
    (1, "Rusturf Tunnel", "Opening Aqua battle; grunt escapes through the doorway, Briney collects Peeko",
     "1 Team Aqua Grunt (Lv26: Trubbish, Simipour, Umbreon, Emolga, Fraxure)", "Doorway at (4,0) opens once the grunt is beaten", "In ROM"),
    (2, "Wraithwood Forest", "Major Team Aqua section and mid-split boss; wild Ghost types start here",
     "4 Aqua Grunts (Lv27; #2 rain, #3 trap/control, #4 screens) + Archie (Lv28, Sharpedo Lv29)",
     "Archie blocks the north exit (stepping on it starts the battle); entering sets FLAG_CLEARED_WRAITHWOOD_ROUTE",
     "In ROM"),
    (3, "Hollowbrook", "Safe town/breather: NPCs, lore, healing, items, trades/gifts, exploration",
     "None", "Cave door (9,1) leads into Granite Cave B2F", "In ROM"),
    (4, "Granite Cave", "Two distinctive battles; no Team Aqua",
     "Fossil Expert Ansel (B2F, singles, Lv29), Cave Researcher Dalton (B1F, doubles, Lv29)", "B2F ladder from Hollowbrook; exit 1F to Route 106",
     "In ROM"),
    (5, "Route 106 (path into Dewford)", "Final overworld checkpoint before Dewford",
     "Cooltrainer Milo (Lv30: Grovyle, Wailmer, Kadabra, Fletchinder; Mime Jr. Lv36)", "-", "In ROM (swimmers in the water remain for Surf)"),
    (6, "Dewford Town", "Safe hub; Mr. Briney waits at the dock: after the Route 106 trainer, talk to him to unlock the ferry to Route 104 (both ways); Slateport opens after Brawly", "None", "-", "In ROM"),
    (7, "Dewford Gym", "Gym gauntlet; four mechanically distinct trainers", "Laura, Brenden, Cristian, Jocelyn; then Player + Rival vs Brawly + Phoebe tag battle (3 each; cap Lv35)",
     "FLAG_BADGE03_GET", "In ROM"),
]

# Trainers whose map objects were removed; their data is unused, so the docs skip them.
REMOVED_FROM_MAP = ["TRAINER_LYLE", "TRAINER_IVAN", "TRAINER_JOEY", "TRAINER_JOSE", "TRAINER_JANICE",
                    "TRAINER_KAREN_1", "TRAINER_JERRY_1", "TRAINER_SARAH", "TRAINER_DAWSON", "TRAINER_JOHNSON",
                    "TRAINER_DEVAN", "TRAINER_VIRIDIAN_FOREST_2", "TRAINER_TRAINERGROVET4", "TRAINER_PETALBURGCOAST_1",
                    "TRAINER_DAISY", "TRAINER_AMY_AND_LIV_1", "TRAINER_ANDREW", "TRAINER_MIGUEL_1", "TRAINER_RHETT",
                    "TRAINER_MARCOS", "TRAINER_NOB_1", "TRAINER_CYNDY_1", "TRAINER_HECTOR", "TRAINER_MARLENE", "TRAINER_MIKE_2"]

# Play order for the Split 2 trainer sheet (trainer ID -> position); others follow by location.
SPLIT2_ORDER = {
    "TRAINER_JAMES_1": 1, "TRAINER_GRUNT_PETALBURG_WOODS": 2, "TRAINER_GRUNT_PETALBURG_WOODS_2": 3,
    "TRAINER_GRUNT_PETALBURG_WOODS_3": 4, "TRAINER_WINSTON_1": 5, "TRAINER_ROUTE104_AROMA_LADY": 6,
    "TRAINER_GINA_AND_MIA_1": 7, "TRAINER_HALEY_1": 8,
    "TRAINER_CLARK": 9, "TRAINER_ROUTE116_ETHAN": 10, "TRAINER_ROUTE116_DIANA": 11, "TRAINER_ROUTE116_CHLOE": 12,
    "TRAINER_ROUTE116_PRESTON": 13, "TRAINER_ROUTE116_TASHA": 14, "TRAINER_ROUTE116_COLE": 15,
    "TRAINER_ROUTE116_MAYA": 16, "TRAINER_SIDNEY_ROUTE116": 17,
    "TRAINER_JOSH": 18, "TRAINER_TOMMY": 19, "TRAINER_MARC": 20, "TRAINER_ROXANNE_1": 21,
}
SPLIT2_NOTES = {
    "TRAINER_JAMES_1": "Wildlife Expert. Mandatory (sight line can't be avoided).",
    "TRAINER_GRUNT_PETALBURG_WOODS": "Aqua Grunt #1 - the idealist. Mandatory.",
    "TRAINER_GRUNT_PETALBURG_WOODS_2": "Aqua Grunt #2 - the pragmatist. Mandatory. Trubbish leads for Toxic Spikes.",
    "TRAINER_GRUNT_PETALBURG_WOODS_3": "Aqua Grunt #3 - the hardliner. Mandatory.",
    "TRAINER_WINSTON_1": "Mandatory (by the woods exit). Persian intentionally knows only Pay Day.",
    "TRAINER_ROUTE104_AROMA_LADY": "Mandatory (by the woods exit). Cherubi leads; Petilil's Hidden Power is Fire (IVs).",
    "TRAINER_GINA_AND_MIA_1": "Double battle, mandatory. Plusle + Blitzle lead (Discharge into Motor Drive).",
    "TRAINER_HALEY_1": "Strongest Route 104 North trainer, mandatory. Munchlax last (Belly Drum + Gluttony + Salac).",
    "TRAINER_CLARK": "Route 116 double #1 (Sand) with Camper Ethan - trigger line x=9. Mandatory.",
    "TRAINER_ROUTE116_ETHAN": "Route 116 double #1 (Sand) with Hiker Clark.",
    "TRAINER_ROUTE116_DIANA": "Route 116 double #2 (Hail) with Skier Chloe - trigger line x=21. Mandatory.",
    "TRAINER_ROUTE116_CHLOE": "Route 116 double #2 (Hail). Skier is a new class; uses the Lass sprite (no Skier art).",
    "TRAINER_ROUTE116_PRESTON": "Route 116 double #3 (Trick Room) with Hex Maniac Tasha - trigger line x=31. Mandatory.",
    "TRAINER_ROUTE116_TASHA": "Route 116 double #3 (Trick Room). Parasect Fake Out + Spore support.",
    "TRAINER_ROUTE116_COLE": "Route 116 double #4 (Balanced) with Ace Trainer Maya - trigger line x=37. Mandatory.",
    "TRAINER_ROUTE116_MAYA": "Route 116 double #4 (Balanced).",
    "TRAINER_SIDNEY_ROUTE116": "Route 116 boss after the Roxanne/Sidney scene (x=43). Beating him opens Rustboro Gym.",
    "TRAINER_JOSH": "Rustboro Gym trainer #1.",
    "TRAINER_TOMMY": "Rustboro Gym trainer #2. Sudowoodo intentionally has two moves.",
    "TRAINER_MARC": "Rustboro Gym trainer #3 (sand). Crustle leads and knows only Sandstorm.",
    "TRAINER_ROXANNE_1": "Gym Leader. Barbaracle Lv26 is the ace.",
}

# Visit order used to sort the Item / TM location tabs (map name prefixes; first match wins).
CHRONO_MAPS = {
    "Split 1": ["LittlerootTown", "StarterRoom", "Route101", "OldaleTown", "Route103", "Route102", "PetalburgCity_Mart",
                "PetalburgCity", "Route104", "TrainerGrove", "PetalburgCoast", "ViridianForest", "PetalburgCity_Gym"],
    "Split 2": ["PetalburgWoods", "LostCave", "Route104", "RustboroCity_Gym", "RustboroCity", "Route116", "Route115"],
    "Split 3": ["RusturfTunnel", "WraithwoodForest", "Hollowbrook", "GraniteCave_B2F", "GraniteCave_B1F", "GraniteCave_1F",
                "GraniteCave_StevensRoom", "Route106", "DewfordTown", "GraniteShore", "DewfordTown_Gym"],
}

# Wild-encounter maps whose split the generic split model can't infer (new/custom maps)
ENCOUNTER_SPLIT = {"MAP_TRAINER_GROVE": "Split 1", "MAP_PETALBURG_COAST": "Split 1", "MAP_LOST_CAVE": "Split 2",
                   "MAP_RUSTURF_TUNNEL": "Split 2", "MAP_WRAITHWOOD_FOREST": "Split 3", "MAP_HOLLOWBROOK": "Split 3",
                   "MAP_GRANITE_CAVE_B2F": "Split 3", "MAP_GRANITE_CAVE_B1F": "Split 3", "MAP_GRANITE_CAVE_1F": "Split 3",
                   "MAP_GRANITE_CAVE_STEVENS_ROOM": "Split 3", "MAP_DEWFORD_TOWN": "Split 3", "MAP_ROUTE106": "Split 3"}


# ---------------------------------------------------------------- fight order, Splits 1-3
# (trainer ID, map folder, status, battle format, note). Format: Single / Double / Tag (2 trainers) / Tag with partner.
# The docs build the Split 1-3 trainer tabs from this list, in this order. Only trainers placed on the maps belong here.
_RIVAL_NOTE = "Rival battle in front of the gym. Team depends on your starter; May's teams mirror Brendan's."
FIGHT_ORDER = {
    "Split 1": [
        ("TRAINER_CALVIN_1", "Route102", "REQUIRED", "Single", None),
        ("TRAINER_RICK", "Route102", "REQUIRED", "Single", None),
        ("TRAINER_TIANA", "Route102", "REQUIRED", "Single", None),
        ("TRAINER_ALLEN", "Route102", "REQUIRED", "Single", None),
        ("TRAINER_DARIAN", "Route104", "REQUIRED", "Single", "Route 104 South"),
        ("TRAINER_BILLY", "Route104", "REQUIRED", "Single", "Route 104 South"),
        ("TRAINER_GRUNT_ROUTE_104_MAGMA", "Route104", "REQUIRED", "Single", "Route 104 South"),
        ("TRAINER_GRUNT_ROUTE_104_AQUA", "Route104", "REQUIRED", "Single", "Route 104 South"),
        ("TRAINER_TRAINERGROVET1", "TrainerGrove", "REQUIRED", "Single", None),
        ("TRAINER_TRAINERGROVET3", "TrainerGrove", "REQUIRED", "Single", None),
        ("TRAINER_TRAINERGROVET2", "TrainerGrove", "REQUIRED", "Single", None),
        ("TRAINER_VIRIDIAN_FOREST_1", "ViridianForest", "Opt-in", "Single", "Optional"),
        ("TRAINER_VIRIDIAN_FOREST_3", "ViridianForest", "Opt-in", "Single", "Optional"),
        ("TRAINER_BRENDAN_PETALBURG_TREECKO", "PetalburgCity", "REQUIRED", "Single", _RIVAL_NOTE + " (Treecko)"),
        ("TRAINER_BRENDAN_PETALBURG_TORCHIC", "PetalburgCity", "REQUIRED", "Single", _RIVAL_NOTE + " (Torchic)"),
        ("TRAINER_BRENDAN_PETALBURG_MUDKIP", "PetalburgCity", "REQUIRED", "Single", _RIVAL_NOTE + " (Mudkip)"),
        ("TRAINER_RANDALL", "PetalburgCity_Gym", "REQUIRED", "Single", None),
        ("TRAINER_ALEXIA", "PetalburgCity_Gym", "REQUIRED", "Single", None),
        ("TRAINER_JODY", "PetalburgCity_Gym", "REQUIRED", "Single", None),
        ("TRAINER_NORMAN_1", "PetalburgCity_Gym", "REQUIRED", "Single", "Gym Leader - Badge 1"),
    ],
    "Split 2": [
        ("TRAINER_JAMES_1", "PetalburgWoods", "REQUIRED", "Single", None),
        ("TRAINER_GRUNT_PETALBURG_WOODS", "PetalburgWoods", "REQUIRED", "Single", "Aqua Grunt 1 (idealist)"),
        ("TRAINER_GRUNT_PETALBURG_WOODS_2", "PetalburgWoods", "REQUIRED", "Single", "Aqua Grunt 2 (pragmatist)"),
        ("TRAINER_GRUNT_PETALBURG_WOODS_3", "PetalburgWoods", "REQUIRED", "Single", "Aqua Grunt 3 (hardliner)"),
        ("TRAINER_WINSTON_1", "Route104", "REQUIRED", "Single", "Route 104 North"),
        ("TRAINER_ROUTE104_AROMA_LADY", "Route104", "REQUIRED", "Single", "Route 104 North"),
        ("TRAINER_GINA_AND_MIA_1", "Route104", "REQUIRED", "Double", "Route 104 North"),
        ("TRAINER_HALEY_1", "Route104", "REQUIRED", "Single", "Route 104 North"),
        ("TRAINER_CLARK", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Camper Ethan"),
        ("TRAINER_ROUTE116_ETHAN", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Hiker Clark"),
        ("TRAINER_ROUTE116_DIANA", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Skier Chloe"),
        ("TRAINER_ROUTE116_CHLOE", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Picnicker Diana"),
        ("TRAINER_ROUTE116_PRESTON", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Hex Maniac Tasha"),
        ("TRAINER_ROUTE116_TASHA", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Psychic Preston"),
        ("TRAINER_ROUTE116_COLE", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Cooltrainer Maya"),
        ("TRAINER_ROUTE116_MAYA", "Route116", "REQUIRED", "Tag (2 trainers)", "Double battle with Cooltrainer Cole"),
        ("TRAINER_SIDNEY_ROUTE116", "Route116", "REQUIRED", "Single", "Beating Sidney opens Rustboro Gym"),
        ("TRAINER_JOSH", "RustboroCity_Gym", "REQUIRED", "Single", None),
        ("TRAINER_TOMMY", "RustboroCity_Gym", "REQUIRED", "Single", None),
        ("TRAINER_MARC", "RustboroCity_Gym", "REQUIRED", "Single", None),
        ("TRAINER_ROXANNE_1", "RustboroCity_Gym", "REQUIRED", "Single", "Gym Leader - Badge 2"),
    ],
    "Split 3": [
        ("TRAINER_GRUNT_RUSTURF_TUNNEL", "RusturfTunnel", "REQUIRED", "Single", None),
        ("TRAINER_WRAITHWOOD_GRUNT_1", "WraithwoodForest", "Forces on sight", "Single", "Aqua Grunt 1"),
        ("TRAINER_WRAITHWOOD_GRUNT_2", "WraithwoodForest", "Forces on sight", "Single", "Aqua Grunt 2 (rain team)"),
        ("TRAINER_WRAITHWOOD_GRUNT_3", "WraithwoodForest", "Forces on sight", "Single", "Aqua Grunt 3 (trap / control)"),
        ("TRAINER_WRAITHWOOD_GRUNT_4", "WraithwoodForest", "Forces on sight", "Single", "Aqua Grunt 4 (screens)"),
        ("TRAINER_ARCHIE_WRAITHWOOD", "WraithwoodForest", "REQUIRED", "Single", "Blocks the exit to Hollowbrook"),
        ("TRAINER_ANSEL", "GraniteCave_B2F", "Forces on sight", "Single", "Fossil Expert"),
        ("TRAINER_DALTON", "GraniteCave_B1F", "Forces on sight", "Double", "Cave Researcher"),
        ("TRAINER_ROUTE106_1", "Route106", "Forces on sight", "Single", "Last overworld trainer before Dewford"),
        ("TRAINER_LAURA", "DewfordTown_Gym", "REQUIRED", "Single", "Gym Trainer 1"),
        ("TRAINER_BRENDEN", "DewfordTown_Gym", "REQUIRED", "Single", "Gym Trainer 2"),
        ("TRAINER_CRISTIAN", "DewfordTown_Gym", "REQUIRED", "Single", "Gym Trainer 3"),
        ("TRAINER_JOCELYN", "DewfordTown_Gym", "REQUIRED", "Single", "Gym Trainer 4"),
        ("TRAINER_BRAWLY_1", "DewfordTown_Gym", "REQUIRED", "Tag with partner",
         "You + Rival vs Brawly + Phoebe; you pick 3. Rival: Greninja, Chesnaught, Delphox (Lv36). Badge 3"),
        ("TRAINER_PHOEBE_DEWFORD", "DewfordTown_Gym", "REQUIRED", "Tag with partner", "Brawly's tag partner"),
    ],
}
# Trainers still in the data but not on any map any more (scripts left over); the docs skip them.
REMOVED_FROM_MAP += ["TRAINER_DESMOND", "TRAINER_OTTOLINE", "TRAINER_RUFUS", "TRAINER_MORTIMER", "TRAINER_PRUDENCE",
                     "TRAINER_PERCY", "TRAINER_TAKAO", "TRAINER_LILITH", "TRAINER_ELLIOT_1", "TRAINER_CINDY_1"]
