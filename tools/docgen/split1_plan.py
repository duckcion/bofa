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
    {"order": 20, "from_id": "TRAINER_VIRIDIAN_FOREST_1", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": "Bug Catcher. Team finalized."},
    {"order": 21, "from_id": "TRAINER_VIRIDIAN_FOREST_3", "loc": "ViridianForest_Optional", "status": "Opt-in",
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

NEW_ITEMS = [
    # item, method, map, split, note
    ("Everstone", "Item Ball", "PetalburgCoast", "Split 1",
     "Replaces the Oval Stone; Norman's quest item."),
    ("HM Rock Smash", "Gift (end of forest)", "ViridianForest", "Split 1",
     "Optional; shares its flag with the Mauville City gift, so it is only given once."),
]
REMOVED_ITEMS = [("Oval Stone", "PetalburgCoast")]

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
    (5, "Route 116 (east end)", "Meet Roxanne by the Rusturf Tunnel entrance; she heads back to her gym",
     "Existing trainers (unchanged)", "Trigger line x=43, y=9-13 sets FLAG_ROUTE116_ROXANNE_FOUND; "
     "Rusturf Tunnel stays optional", PLAN_TAG),
    (6, "Rustboro Gym", "Gym trainers + Roxanne -> Badge 2", "Existing (unchanged)", "FLAG_BADGE02_GET", "In ROM"),
]

# Trainers whose map objects were removed; their data is unused, so the docs skip them.
REMOVED_FROM_MAP = ["TRAINER_LYLE", "TRAINER_VIRIDIAN_FOREST_2", "TRAINER_TRAINERGROVET4", "TRAINER_PETALBURGCOAST_1"]
