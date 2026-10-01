"""Planned Split 1 restructure (2026-10-01).

Read by build_bofa_docs.py. Everything here is a DESIGN PLAN for the docs: none of it is in
the ROM yet. Trainers marked `from_id` keep their existing team from trainers.party; `levels`
overrides only the levels. New trainers (no `from_id`) are fully specified here.
"Spark" is what the player community calls the rival (Brendan/May), so the rival keeps its
in-game names here.
"""

PLAN_TAG = "PLANNED (not in ROM yet)"

def mon(species, level, item=None, ability=None, nature=None, moves=()):
    return {"species": species, "level": level, "item": item, "ability": ability,
            "nature": nature, "moves": list(moves)}

# Rival ("Spark") at the Petalburg Gym entrance: Route 103 starter variants, rebalanced.
# Partner Pokemon follow the existing Rustboro rival teams for each variant.
_RIVAL_VARIANTS = [
    ("Treecko", "Wingull", "Wingull"),
    ("Mudkip", "Slugma", "Torkoal"),   # Brendan uses Slugma, May uses Torkoal (as in Rustboro)
    ("Torchic", "Lotad", "Lotad"),
]

def _rival_trainers(order):
    out = []
    for rival, idx in (("BRENDAN", 1), ("MAY", 2)):
        for starter, partner_b, partner_m in _RIVAL_VARIANTS:
            partner = partner_b if rival == "BRENDAN" else partner_m
            out.append({
                "order": order, "id": f"TRAINER_{rival}_PETALBURG_{starter.upper()} (new; replaces "
                                      f"TRAINER_{rival}_ROUTE_103_{starter.upper()})",
                "name": rival, "cls": "Rival", "loc": "PetalburgCity_GymEntrance",
                "status": "REQUIRED",
                "note": f"\"Spark\" - fights once on first approach to the gym after the Everstone quest. "
                        f"Variant: rival has {starter}. {PLAN_TAG}",
                "mons": [mon("Zigzagoon", 12), mon(partner, 12), mon(starter, 14, item="Oran Berry")],
            })
    return out

SPLIT1_TRAINERS = [
    # 1-4 Route 102 (existing teams, all mandatory)
    {"order": 1, "from_id": "TRAINER_CALVIN_1", "loc": "Route102", "status": "REQUIRED"},
    {"order": 2, "from_id": "TRAINER_RICK", "loc": "Route102", "status": "REQUIRED",
     "note": "ROM currently has no object event wired to this trainer."},
    {"order": 3, "from_id": "TRAINER_TIANA", "loc": "Route102", "status": "REQUIRED"},
    {"order": 4, "from_id": "TRAINER_ALLEN", "loc": "Route102", "status": "REQUIRED"},
    # 5-8 Route 104 (south of Petalburg Woods)
    {"order": 5, "from_id": "TRAINER_DARIAN", "loc": "Route104", "status": "REQUIRED",
     "note": "Moved from Split 2. Fisherman slot."},
    {"order": 6, "from_id": "TRAINER_BILLY", "loc": "Route104", "status": "REQUIRED",
     "note": "Moved from Split 2. Regular trainer slot."},
    {"order": 7, "id": "TRAINER_GRUNT_ROUTE_104_MAGMA (new)", "name": "GRUNT", "cls": "Team Magma",
     "loc": "Route104", "status": "REQUIRED",
     "note": f"First Team Magma battle. {PLAN_TAG}",
     "mons": [mon("Poochyena", 8), mon("Numel", 9, item="Oran Berry", nature="Adamant")]},
    {"order": 8, "id": "TRAINER_GRUNT_ROUTE_104_AQUA (new)", "name": "GRUNT", "cls": "Team Aqua",
     "loc": "Route104", "status": "REQUIRED",
     "note": f"First Team Aqua battle. {PLAN_TAG}",
     "mons": [mon("Poochyena", 8), mon("Carvanha", 9, item="Oran Berry", nature="Jolly")]},
    # 9-11 Trainer Grove (existing regulars rebalanced from Lv34; grunts replaced)
    {"order": 9, "from_id": "TRAINER_TRAINERGROVET1", "loc": "TrainerGrove", "status": "REQUIRED",
     "levels": [11, 11], "note": "Team kept, levels 34 -> 11. Name is still the placeholder \"Bum1\"."},
    {"order": 10, "from_id": "TRAINER_TRAINERGROVET2", "loc": "TrainerGrove", "status": "REQUIRED",
     "levels": [11, 12], "note": "Team kept, levels 34 -> 11/12. Name is still the placeholder \"Bum2\"."},
    {"order": 11, "id": "TRAINER_TRAINERGROVET3 (team replaced; was a Team Magma grunt)",
     "name": "(name TBD)", "cls": "Camper", "loc": "TrainerGrove", "status": "REQUIRED",
     "note": f"Replaces the Team Magma grunt. {PLAN_TAG}",
     "mons": [mon("Lotad", 11), mon("Nincada", 12, item="Oran Berry")]},
    # 12 Rival at the gym entrance (6 entries: Brendan/May x 3 starter variants)
    *_rival_trainers(12),
    # 13-16 Petalburg Gym (unchanged)
    {"order": 13, "from_id": "TRAINER_ALEXIA", "loc": "PetalburgCity_Gym", "status": "Opt-in",
     "note": "Gym trainer (sight range 0 in ROM)."},
    {"order": 14, "from_id": "TRAINER_JODY", "loc": "PetalburgCity_Gym", "status": "REQUIRED",
     "note": "Norman will not battle until Jody is beaten (NormanCheckTrials)."},
    {"order": 15, "from_id": "TRAINER_RANDALL", "loc": "PetalburgCity_Gym", "status": "Opt-in",
     "note": "Gym trainer (sight range 0 in ROM)."},
    {"order": 16, "from_id": "TRAINER_NORMAN_1", "loc": "PetalburgCity_Gym", "status": "REQUIRED"},
    # Optional: Viridian Forest
    {"order": 20, "id": "Viridian Forest trainer 1 (new; uses the Bug Catcher at (0,18))",
     "name": "(name TBD)", "cls": "Bug Catcher", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": f"Optional area. {PLAN_TAG}",
     "mons": [mon("Caterpie", 10), mon("Weedle", 10)]},
    {"order": 21, "id": "Viridian Forest trainer 2 (new; uses the Bug Catcher at (5,10))",
     "name": "(name TBD)", "cls": "Bug Catcher", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": f"Optional area. {PLAN_TAG}",
     "mons": [mon("Metapod", 10), mon("Kakuna", 10), mon("Wurmple", 11)]},
    {"order": 22, "id": "Viridian Forest trainer 3 (new object event)",
     "name": "(name TBD)", "cls": "Youngster", "loc": "ViridianForest_Optional", "status": "Opt-in",
     "note": f"Optional area. {PLAN_TAG}",
     "mons": [mon("Pidgey", 10), mon("Pikachu", 11, item="Oran Berry")]},
    # Not in the planned list - kept so it is not silently dropped
    {"order": 30, "from_id": "TRAINER_PETALBURGCOAST_1", "loc": "PetalburgCoast_NotInYourList",
     "status": "Forces", "note": "NEEDS DECISION: not in your Split 1 list, but he is in the ROM on "
                                 "Petalburg Coast (the Everstone area) and challenges on sight."},
]

# Route 103 trainers leave Split 1 (east side is across water). Shown on the Unassigned sheet.
ROUTE_103_IDS = ["TRAINER_AMY_AND_LIV_1", "TRAINER_ANDREW", "TRAINER_DAISY", "TRAINER_ISABELLE",
                 "TRAINER_MARCOS", "TRAINER_MIGUEL_1", "TRAINER_PETE", "TRAINER_RHETT"]
REMOVED_RIVAL_PREFIXES = ["TRAINER_BRENDAN_ROUTE_103", "TRAINER_MAY_ROUTE_103"]

PROGRESSION = [
    # step, area, what happens, trainers, gate / event, status
    (1, "Littleroot Town", "Receive the Pokédex immediately (no rival battle needed first)", "-",
     "Pokédex no longer tied to the Route 103 rival", PLAN_TAG),
    (2, "Littleroot Town - additional lab", "Free Gen 2 starter gift: choose Chikorita, Cyndaquil or Totodile (Lv5)",
     "-", "Gift, not a trade", "In ROM (givemon in Birch's lab script)"),
    (3, "Oldale Town", "Hub", "-", "-", "In ROM"),
    (4, "Route 102", "Travel Oldale -> Petalburg", "4 mandatory (Calvin, Rick, Tiana, Allen)",
     "No rival battle", PLAN_TAG + " (trainers exist; Rick has no object event)"),
    (5, "Petalburg City (first visit)", "NPC blocks the gym: Norman is on Route 104 looking for an Everstone",
     "-", "Gym blocked until the Everstone quest is done; no rival battle", PLAN_TAG),
    (6, "Route 104 (south)", "Beat 4 trainers, then meet Norman", "4 mandatory (Fisherman Darian, Youngster Billy, "
     "Magma Grunt, Aqua Grunt)", "Norman asks for an Everstone; sends you to Trainer Grove / Petalburg Coast",
     PLAN_TAG),
    (7, "Trainer Grove", "Path to the Everstone", "3 mandatory (2 existing regulars rebalanced + 1 new Camper)",
     "Must pass through", PLAN_TAG),
    (8, "Petalburg Coast (your \"Shoal Cave\")", "Get the Everstone; at least one encounter; more items TBD",
     "Ronnie (needs decision)", "Everstone replaces the Oval Stone item ball; NPC points you to Viridian Forest",
     PLAN_TAG),
    (9, "Viridian Forest (optional)", "Optional prep: 3 trainers, encounters, Rock Smash at the end",
     "3 optional", "Not required for the gym", PLAN_TAG),
    (10, "Petalburg City (return)", "Gym blocker gone; rival (\"Spark\") battle on first approach to the gym",
     "Rival - mandatory, once", "Battle never repeats after a win", PLAN_TAG),
    (11, "Petalburg Gym", "3 gym trainers + Norman -> Badge 1", "Jody required by Norman; Alexia/Randall optional",
     "FLAG_BADGE01_GET", "In ROM"),
]

GIFTS = {
    "remove_trade_species_at": ("ViridianForest", ["Chikorita", "Cyndaquil", "Totodile"]),
    "johto_gift_note": "Free gift - choose one (not a trade). Littleroot Town, additional lab.",
}

NEW_ITEMS = [
    # item, method, map, split, note
    ("Everstone", "Item Ball", "PetalburgCoast", "Split 1",
     PLAN_TAG + " - replaces the Oval Stone; needed for Norman's quest"),
    ("HM Rock Smash", "Gift (end of forest)", "ViridianForest", "Split 1",
     PLAN_TAG + " - optional; useful against Norman"),
]
REMOVED_ITEMS = [("Oval Stone", "PetalburgCoast")]

# Contradictions between the plan and the current ROM / workbook - surfaced, not assumed away.
FLAGS = [
    ("Everstone location", "Your plan says Shoal Cave; you confirmed that means Petalburg Coast (the only area past "
     "Trainer Grove). The real Shoal Cave maps are unchanged."),
    ("Norman's item", "In the ROM Norman asks for an Oval Stone and waits inside the gym. The plan moves him to "
     "Route 104 and changes the item to an Everstone."),
    ("Trainer Grove count", "Your written plan says to replace both grunts and keep the others (= 4 trainers). "
     "Your ordered list has 3 (2 existing + 1 new). The docs follow the list: the Team Aqua grunt is removed."),
    ("Trainer Grove placeholders", "The two kept regulars are still named \"Bum1\"/\"Bum2\" and had identical "
     "Wailmer + Horsea Lv34 teams. Levels are cut to 11-12; real names are still needed."),
    ("Petalburg Coast trainer", "Youngster Ronnie is in the ROM on Petalburg Coast (challenges on sight) but is not "
     "in your list. Kept on the Split 1 sheet under 'Not In Your List' until you decide."),
    ("Route 104 extras", "Lady Cindy (16,42) and an unscripted girl (21,50) also sit on the south part of Route 104. "
     "Only Darian and Billy are used; Cindy stays in Split 2. The ROM also has a rival event at (17,50) by "
     "Mr. Briney's house, which must not fire in Split 1."),
    ("Viridian Forest encounters", "The plan says to keep its existing encounters, but Viridian Forest has no wild "
     "encounter table in the ROM. The end-of-forest encounter is listed as TBD."),
    ("Rock Smash", "HM Rock Smash is still also given in Mauville City (House 1). Decide whether to keep both."),
    ("Gym trainers", "Alexia and Randall have sight range 0 (optional). Jody is effectively mandatory because "
     "Norman checks for her."),
    ("Route 103", "The Viridian Forest entrance and the old rival spot are on Route 103's west (land) side, so "
     "Route 103 itself is reachable; only its 8 trainers across the water move out of Split 1 "
     "(listed on the Unassigned sheet)."),
    ("Rival name", "No trainer is called Spark in the ROM; the docs show the rival as Brendan/May (\"Spark\")."),
]
