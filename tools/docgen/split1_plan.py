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
    ("RageCandyBar", "Dropped by the Aqua grunt as he flees", "RusturfTunnel", "Split 3", "Converted Shoal Shell"),
    ("Lum Berry x5", "NPC gift (old woman)", "Hollowbrook", "Split 3", ""),
    ("HM05 Flash", "NPC gift (hiker)", "GraniteCave_1F", "Split 3", ""),
    ("TM Brick Break x2", "Gym reward (Brawly + Phoebe tag battle)", "DewfordTown_Gym", "Split 3", ""),
]
REMOVED_ITEMS = []

# Still open, or places the build differs from the original plan.
FLAGS = [
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
     "Grunts 1-4 (Lv27; #2 rain, #3 trap/control, #4 screens) + Grunt 5 (TBD) + Archie (Lv28, Sharpedo Lv29)",
     "Archie blocks the north exit (stepping on it starts the battle); entering sets FLAG_CLEARED_WRAITHWOOD_ROUTE",
     "In ROM except Grunt 5"),
    (3, "Hollowbrook", "Safe town/breather: NPCs, lore, healing, items, trades/gifts, exploration",
     "None", "Cave door (9,1) leads into Granite Cave B2F", "In ROM"),
    (4, "Granite Cave", "Two distinctive battles; no Team Aqua",
     "Fossil Expert Ansel (B2F, singles, Lv29), Cave Researcher Dalton (B1F, doubles, Lv29)", "B2F ladder from Hollowbrook; exit 1F to Route 106",
     "In ROM"),
    (5, "Route 106 (path into Dewford)", "Final overworld checkpoint before Dewford",
     "Cooltrainer Milo (Lv30: Grovyle, Wailmer, Kadabra, Fletchinder; Mime Jr. Lv36)", "-", "In ROM (swimmers in the water remain for Surf)"),
    (6, "Dewford Town", "Safe hub", "None", "-", "In ROM"),
    (7, "Dewford Gym", "Gym gauntlet; four mechanically distinct trainers", "Laura, Brenden, Cristian, Jocelyn; then Player + Rival vs Brawly + Phoebe tag battle (3 each; cap Lv35)",
     "FLAG_BADGE03_GET", "In ROM"),
]

# Trainers whose map objects were removed; their data is unused, so the docs skip them.
REMOVED_FROM_MAP = ["TRAINER_LYLE", "TRAINER_IVAN", "TRAINER_JOEY", "TRAINER_JOSE", "TRAINER_JANICE",
                    "TRAINER_KAREN_1", "TRAINER_JERRY_1", "TRAINER_SARAH", "TRAINER_DAWSON", "TRAINER_JOHNSON",
                    "TRAINER_DEVAN", "TRAINER_VIRIDIAN_FOREST_2", "TRAINER_TRAINERGROVET4", "TRAINER_PETALBURGCOAST_1"]

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
ENCOUNTER_SPLIT = {"MAP_TRAINER_GROVE": "Split 1", "MAP_PETALBURG_COAST": "Split 1", "MAP_LOST_CAVE": "Split 2"}
