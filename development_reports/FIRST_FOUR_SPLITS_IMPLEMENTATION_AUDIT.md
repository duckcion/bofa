# First Four Splits — Implementation Audit

**Generated:** 2026-09-30, from project files. Every finding cites its source.

**Status vocabulary**

| Label | Meaning |
|---|---|
| IMPLEMENTED AND VERIFIED | Confirmed in files *and* checked by an automated test or build-output inspection |
| IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED | Data/scripts are correct and compile; never run in an emulator |
| PARTIALLY IMPLEMENTED | Exists but incomplete against its own apparent intent |
| PLANNED ONLY | Appears in design docs; no code |
| NOT IMPLEMENTED | Confirmed absent |
| UNKNOWN | Cannot be determined from files |

> **Blanket caveat.** Nothing in this project has been played. The build succeeds
> and `make check` reports 92 failures (matching the pre-existing baseline), but a
> passing build is not evidence the game is playable. Almost everything below is
> therefore *IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED* at best.

---

## HAVE ALL TRAINERS THROUGH THE FOURTH BADGE BEEN IMPLEMENTED?

**No.**

871 trainer IDs have party data and no trainer anywhere is left on a placeholder
stub team. But "has data" is not "designed for this hack". Splitting by whether the
team carries the project's own design signature — explicit abilities, natures, held
items and 31 IVs:

### Custom, fully-specified teams (splits 1–4)

| Area | Trainers | Source |
|---|---|---|
| Petalburg Gym | RANDALL, ALEXIA, JODY, NORMAN | `trainers.party` |
| Rustboro Gym | JOSH, TOMMY, MARC, ROXANNE | `trainers.party` |
| Hollowbrook | MORTIMER, PRUDENCE | `trainers.party` |
| Wraithwood Forest | DESMOND, OTTOLINE, RUFUS | `trainers.party` |
| Granite Cave 1F/B1F/B2F | PERCY, DALTON, ANSEL | `trainers.party` |
| Granite Shore | DEACON, PRIYA | `trainers.party` |
| Petalburg Coast | RONNIE | fixed 2026-09-30 |
| Route 105 | TESS | fixed 2026-09-30 |
| Route 106 | MILO | fixed 2026-09-30 |

**21 trainers.**

### Still vanilla in splits 1–4

Everything else. Specifically:

- **Dewford Gym (badge 3), all 7 trainers including BRAWLY** — `data/maps/DewfordTown_Gym/scripts.inc`,
  `TRAINER_BRAWLY_1`. Rank-and-file are single Lv13 Pokémon; Brawly has no
  abilities/natures/IVs and one held item. **PARTIALLY IMPLEMENTED** (exists, not redesigned).
- **Mauville Gym (badge 4), all 6 trainers including WATTSON** — `TRAINER_WATTSON_1`.
  Same picture. **PARTIALLY IMPLEMENTED.**
- **All route trainers** on Routes 101–110, 116, 117, Rusturf Tunnel, Petalburg Woods:
  vanilla teams, no items/abilities/natures/IVs specified. AI flags *were* upgraded
  game-wide, so they fight better with vanilla teams.
- **Trainer Grove (Bum1–Bum4)** — four identical Wailmer Lv34 + Horsea Lv34 teams
  with mismatched classes. **PARTIALLY IMPLEMENTED** (placed, clearly unfinished).
- **All rival battles** (Brendan/May ×3 starter variants on Routes 103/104/110,
  Rustboro, and Wally in Mauville): vanilla teams. **PARTIALLY IMPLEMENTED.**
- **Team Aqua grunts** in Petalburg Woods and Rusturf Tunnel: single Poochyena
  Lv9/Lv11, vanilla. **PARTIALLY IMPLEMENTED.**

### Mandatory vs optional — now derived (corrects an earlier draft)

An earlier draft of this audit said this could not be determined. It can. Each
trainer object event carries `trainer_type` and `trainer_sight_or_berry_tree_id`,
and `src/trainer_see.c` uses the latter as the approach distance, so:

- sight range >= 1 → the NPC challenges on sight. **303 trainers game-wide.**
- sight range 0, or `TRAINER_TYPE_NONE` → must be talked to. **36 trainers.**
- Gym leaders all have range 0 but gate a badge, so they are labelled
  `REQUIRED - awards Badge N` (derived from which gym sets which badge flag).

This is now a real column in `BOFA_Documentation.xlsx` on every split sheet.

Joining object events to trainer IDs needs the script label as an intermediary,
and for leaders and story battles the `trainerbattle` call sits behind `goto`/`call`
chains, so the generator follows those edges transitively.

### Still cannot be determined without play testing

- **Whether a forcing sight line can be physically walked around.** This needs
  per-map collision pathfinding. It was done for Trainer Grove, where the answer is
  no — see the Critical finding in the Design Gaps report. It has **not** been done
  for any other map.
- Rival and story battles (Brendan/May, Wally) report UNKNOWN: they fire from coord
  events and story scripts rather than a trainer object event, so there is no sight
  range to read.
- Whether Route 103's east connection to Route 110 is walkable pre-badge-1 or
  blocked by terrain/ledges.

---

## Feature-by-feature

### Maps and connections — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
All splits 1–4 maps exist with `map.json`. The custom region is wired
Rustboro → `HollowbrookShack_MiningTunnel` → Hollowbrook → WraithwoodForest →
GraniteCave_B2F → B1F → 1F → Route106 → Dewford, with GraniteShore → DewfordTown.
Viridian Forest ↔ Route 103 two-way. TrainerGrove joins PetalburgCity ↔ PetalburgCoast.
`LostCave` warps from Petalburg Woods.

### Story scripts and progression flags — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
- Badge flags: each of the 8 gyms sets exactly one distinct flag. **Fixed 2026-09-30** —
  previously Lavaridge and Mauville both set `FLAG_BADGE04_GET` and nothing set
  `FLAG_BADGE05_GET`.
- Briney shortcut closed via `FLAG_CLEARED_WRAITHWOOD_ROUTE`
  (`data/maps/Route104_MrBrineysHouse/scripts.inc`, set in `WraithwoodForest`).

### Trainer AI — IMPLEMENTED AND VERIFIED (data-level)
All 871 trainers carry `AI: Smart Trainer` in `trainers.party`. Verified by parsing
the file: no trainer lacks an AI line. Effect in battle is not gameplay-verified.

### Level-cap restrictions — NOT IMPLEMENTED
`B_LEVEL_CAP_TYPE = LEVEL_CAP_NONE` (`include/config/caps.h`). `src/caps.c` contains
a working `GetCurrentLevelCap()` and `sLevelCapFlagMap` path, unused. The boss-entry
level limit from design notes: **PLANNED ONLY.**

### Convenience NPC and starting money — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
`LittlerootTown_EventScript_SuppliesMan` (in `scripts.pory`, generated into
`scripts.inc`), object event at (26,15), gated by `FLAG_RECEIVED_STARTER_SUPPLIES`.
Gives 4×900 items. `SetMoney(&gSaveBlock1Ptr->money, MAX_MONEY)` in `src/new_game.c`.
Untested in play: whether all four `giveitem` calls succeed back-to-back was not observed.

### ₽1 Rare Candies — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
`ITEM_RARE_CANDY` `.price = 1` in `src/data/items.h`; added to 12 mart inventories
(Oldale, Petalburg, Rustboro each have two tiers — both updated). Specialty shops
deliberately excluded.

### Berry harvesting — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
`GetBerryCountByBerryTreeId()` in `src/berry.c` returns a constant 255.

### Gift Dwebble — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
Rancher Andy, `GraniteShore` (Split 3). Lv20, Adamant, `abilityNum=0` → Sturdy,
31 IVs. Species changes applied: Dwebble evolves at 26 (was 34); Crustle stats set
to 80/105/125/65/80/45 (BST 500, was 485); Rock Blast@15, Rock Slide@24, Bug Bite
moved 5→19, Shell Smash moved to Crustle@32.

### Gift Skorupi — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
`WraithwoodForest` (20,11). Lv22, Jolly, Battle Armor, 31 IVs. Skorupi and Drapion
both have Battle Armor in slot 1; evolution moved 40→32. Ability persistence through
evolution confirmed by reading `ScriptGiveMonParameterized` in
`src/script_pokemon_util.c` (sets `MON_DATA_ABILITY_NUM`, which survives evolution).

### Gen 2 starter trade — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
`ViridianForest` (8,1). Three entries added to `src/data/trade.h`
(`INGAME_TRADE_CHIKORITA/CYNDAQUIL/TOTODILE`), each `.ivs = {31,…}`. Accepts any
species — the script deliberately omits the `goto_if_ne` species check the other
trades use. Gated by `FLAG_JOHTO_STARTER_TRADE_DONE`.

> **Design note, not a defect:** the player's own starter is already this same
> Johto trio, so this trade grants a second starter line rather than access to an
> otherwise-unobtainable family.

### Evolution Stone NPC — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
`RustboroCity` (18,34), object event appended last so no existing local IDs shifted.
Choice of Fire/Water/Leaf Stone, gated by `FLAG_RECEIVED_EVOLUTION_STONE`. The flag
is set only if the item actually fit (checks `VAR_0x8007`), so a full bag does not
burn the one pick.

### Pokémon evolution and stat changes — IMPLEMENTED (data-level, verified by diff)
Against upstream baseline `9a0b5756c`: **449 species with stat changes, 443 with
ability changes, 35 with evolution changes, 408 moves changed, 855 learnsets changed.**
Full detail in `BOFA_Documentation.xlsx`.

### Wild encounter tables — PARTIALLY IMPLEMENTED
`src/data/wild_encounters.json` is **vanilla** for splits 1–4. No route's table
appears customised for this hack's level curve. Critically, **every custom map has
no table at all**: ViridianForest, PetalburgCoast, TrainerGrove, LostCave,
Hollowbrook, WraithwoodForest, GraniteShore, HollowbrookShack_MiningTunnel.

### Overworld item placements — IMPLEMENTED BUT NOT GAMEPLAY-VERIFIED
359 placements repo-wide (240 visible balls, 119 hidden). 31 are TM/HM placements,
including 17 added by an earlier project commit. All four custom Split-3 maps
(Hollowbrook, Wraithwood, Granite Shore, Mining Tunnel) have **zero items**.

---

## Defects found and fixed during this audit

1. **Badge 5 unobtainable.** `LavaridgeTown_Gym_1F` set `FLAG_BADGE04_GET`, the same
   flag as Mauville; nothing set `FLAG_BADGE05_GET`. The obedience ladder in
   `src/battle_util.c` is positional, so the cap stalled at 50 until badge 6.
   Fixed — all three uses in that gym moved to badge 5.
2. **Three live trainers on stub teams.** RONNIE (gating the Oval Stone), TESS and
   MILO each fought with one Lv1 Lillipup with no moves. All three are wired to
   object events. Fixed with real levelled teams.

## Defects found and *not* fixed (documented for review)

3. **321 trainer IDs have party data but no map script references them** — 297 are
   `_2`–`_5` rematch variants, expected dead data since `FREE_MATCH_CALL = TRUE`
   disables rematches. The other 24 are worth individual review.
4. **Three `script: NULL` object events in Viridian Forest** — two Bug Catchers and
   a `BRENDAN_FIELD_MOVE` NPC, placed but inert.
5. **Brawly's and Wattson's gyms are un-redesigned**, breaking the level curve.
6. **Badge 1 is walled behind three unavoidable Lv34 battles.** Norman requires an
   Oval Stone; the stone is on Petalburg Coast; Petalburg Coast is only reachable
   through Trainer Grove; three of Trainer Grove's four Lv34 trainers have sight
   lines that cannot be dodged. Verified by BFS over the map's collision data.
   This is the highest-severity finding in the audit — see the Design Gaps report.
