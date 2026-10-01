"""Split 1 restructure (2026-10-01): play order, statuses and notes for the docs.

Read by build_pk_format.py. Since the restructure is now built into the ROM, every
trainer comes from trainers.party via `from_id`; this file only sets the play order,
mandatory/optional status and the notes shown on each trainer card.
"Spark" is what players call the rival (Brendan/May), so the rival keeps its in-game names.
"""

PLAN_TAG = "In ROM, not playtested yet"

_RIVAL_NOTE = ("\"Spark\" - fights once, on the tile in front of the gym door, after the Everstone quest. "
               "Team depends on the player's starter (suffix) and gender (Brendan/May).")

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
     "note": "Team kept, levels 34 -> 11. Name is still the placeholder \"Bum1\"."},
    {"order": 10, "from_id": "TRAINER_TRAINERGROVET2", "loc": "TrainerGrove", "status": "REQUIRED",
     "note": "Team kept, levels 34 -> 11/12. Name is still the placeholder \"Bum2\"."},
    {"order": 11, "from_id": "TRAINER_TRAINERGROVET3", "loc": "TrainerGrove", "status": "REQUIRED",
     "note": "Camper Grant replaces the Team Magma grunt. The Team Aqua grunt was removed from the map."},
    # 12 Rival at the gym entrance (6 variants)
    *[{"order": 12, "from_id": f"TRAINER_{r}_PETALBURG_{s}", "loc": "PetalburgCity_GymEntrance",
       "status": "REQUIRED", "note": f"{_RIVAL_NOTE} This one: player chose {s.title()}."}
      for r in ("BRENDAN", "MAY") for s in ("TREECKO", "TORCHIC", "MUDKIP")],
    # 13-16 Petalburg Gym (unchanged)
    {"order": 13, "from_id": "TRAINER_ALEXIA", "loc": "PetalburgCity_Gym", "status": "Opt-in",
     "note": "Gym trainer (sight range 0)."},
    {"order": 14, "from_id": "TRAINER_JODY", "loc": "PetalburgCity_Gym", "status": "REQUIRED",
     "note": "Norman will not battle until Jody is beaten."},
    {"order": 15, "from_id": "TRAINER_RANDALL", "loc": "PetalburgCity_Gym", "status": "Opt-in",
     "note": "Gym trainer (sight range 0)."},
    {"order": 16, "from_id": "TRAINER_NORMAN_1", "loc": "PetalburgCity_Gym", "status": "REQUIRED"},
    # Optional: Viridian Forest
    {"order": 20, "from_id": "TRAINER_VIRIDIAN_FOREST_1", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": "Optional area. New."},
    {"order": 21, "from_id": "TRAINER_VIRIDIAN_FOREST_2", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": "Optional area. New."},
    {"order": 22, "from_id": "TRAINER_VIRIDIAN_FOREST_3", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": "Optional area. New."},
    # Not in the planned list - kept so it is not silently dropped
    {"order": 30, "from_id": "TRAINER_PETALBURGCOAST_1", "loc": "PetalburgCoast_NotInYourList",
     "status": "Forces", "note": "NEEDS DECISION: not in your Split 1 list, but he guards the Everstone "
                                 "on Petalburg Coast, so he is effectively mandatory."},
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
    (7, "Trainer Grove", "Path to the Everstone", "3 (Bum1, Bum2, Camper Grant)",
     "Entrance turns the player back while VAR_SPLIT1_STATE = 0", PLAN_TAG),
    (8, "Petalburg Coast (your \"Shoal Cave\")", "Everstone item ball (guarded by Ronnie), one-time Corphish Lv11, "
     "fisherman points to Viridian Forest once you have the stone", "Ronnie (needs decision)",
     "Returning to Petalburg with the stone sets VAR_SPLIT1_STATE = 2", PLAN_TAG),
    (9, "Viridian Forest (optional)", "3 trainers, new wild table (Kanto bugs, Pidgey, rare Pikachu), "
     "HM06 Rock Smash from the Black Belt at the end", "3 optional", "Not required for the gym", PLAN_TAG),
    (10, "Petalburg City (return)", "Blocker gone; rival (\"Spark\") battles on the tile in front of the gym door",
     "Rival - once", "Win sets VAR_SPLIT1_STATE = 3; a loss lets the rival challenge again", PLAN_TAG),
    (11, "Petalburg Gym", "Hand Norman the Everstone, beat Jody, then Norman -> Badge 1",
     "Jody required; Alexia/Randall optional", "FLAG_GAVE_NORMAN_EVERSTONE, FLAG_BADGE01_GET", "In ROM"),
]

GIFTS = {
    "remove_trade_species_at": ("ViridianForest", ["Chikorita", "Cyndaquil", "Totodile"]),
    "johto_gift_note": "Free gift - choose one (not a trade). Littleroot Town Starter Room, after the Pokédex.",
}

NEW_ITEMS = [
    # item, method, map, split, note
    ("Everstone", "Item Ball", "PetalburgCoast", "Split 1",
     "Replaces the Oval Stone; Norman's quest item. Guarded by Ronnie."),
    ("HM Rock Smash", "Gift (end of forest)", "ViridianForest", "Split 1",
     "Optional; shares its flag with the Mauville City gift, so it is only given once."),
]
REMOVED_ITEMS = [("Oval Stone", "PetalburgCoast")]

# Still open, or places the build differs from the original plan.
FLAGS = [
    ("Not playtested", "Everything builds, but none of the Split 1 scripts have been played through yet."),
    ("Everstone location", "Your plan said Shoal Cave; you confirmed Petalburg Coast. The real Shoal Cave maps are "
     "unchanged."),
    ("Trainer Grove count", "Your written plan implied 4 trainers; your ordered list has 3. Built with 3: the Team "
     "Aqua grunt was removed from the map (its trainer data is unused)."),
    ("Trainer Grove placeholders", "The two kept regulars are still named \"Bum1\"/\"Bum2\" and still have their "
     "old intro/defeat lines. Their teams are identical Wailmer + Horsea."),
    ("Petalburg Coast trainer", "Youngster Ronnie is not in your list but guards the Everstone, so every player "
     "fights him. Decide whether to keep, move or remove him."),
    ("Route 104 extras", "Lady Cindy (16,42) and an unscripted girl (21,50) are also on the south part of Route 104. "
     "Norman only checks Darian, Billy and the two grunts."),
    ("Viridian Forest encounters", "It had no wild table; a new one was added (my pick: Caterpie, Weedle, Metapod, "
     "Kakuna, Pidgey, rare Pikachu, Lv9-11). The extra end-of-forest encounter is still TBD."),
    ("Petalburg Coast encounter", "The coast has no grass, so its encounter is a one-time Corphish Lv11 on the "
     "beach (my pick)."),
    ("Gym trainers", "Alexia and Randall have sight range 0 (optional). Jody is required by Norman."),
    ("Route 103", "Only its 8 trainers across the water move out of Split 1 (Unassigned sheet). The west side "
     "(Viridian Forest entrance) is still reachable."),
    ("Rival name", "No trainer is called Spark; the rival stays Brendan/May (\"Spark\")."),
]
