# First Four Splits — Current Walkthrough

**What this is:** a chronological account of what a player actually encounters from
a new game through the fourth badge, read out of the project files as they stand.

**Generated:** 2026-09-30. Sources: `data/maps/*/map.json` (connections, warps,
object events), `data/maps/*/scripts.inc` and `scripts.pory`, `src/data/trainers.party`,
`src/data/wild_encounters.json`, `src/data/trade.h`, `include/constants/flags.h`.

**Nothing here is gameplay-verified.** It is what the data says will happen, not
what has been observed happening in an emulator.

---

## Actual gym order (derived, not assumed)

Read from which gym script calls `setflag FLAG_BADGE0N_GET`:

| Badge | Gym | Leader | Leader's ace | Level band |
|---|---|---|---|---|
| 1 | Petalburg | Norman | Slakoth Lv16 | 14–16 |
| 2 | Rustboro | Roxanne | Nosepass Lv23 | 20–23 |
| 3 | Dewford | Brawly | Makuhita Lv19 | 16–19 |
| 4 | Mauville | Wattson | Manectric Lv24 | 20–24 |
| 5 | Lavaridge | Flannery | — | — |
| 6 | Fortree | Winona | — | — |
| 7 | Mossdeep | Tate & Liza | — | — |
| 8 | Sootopolis | Juan | — | — |

This is a **custom order**, not vanilla Emerald's. Norman has been pulled from
badge 5 to badge 1, and Roxanne/Brawly/Wattson each shifted accordingly.

> **The level curve is inverted at badge 3.** Badge 2 (Roxanne, Lv20–23) is
> *higher* than badge 3 (Brawly, Lv16–19). See the Design Gaps report.

**Level caps:** none are enforced. `B_LEVEL_CAP_TYPE = LEVEL_CAP_NONE` in
`include/config/caps.h`. The engine supports caps via `LEVEL_CAP_FLAG_LIST` and
`sLevelCapFlagMap` in `src/caps.c`, but that path is switched off. The
boss-entry level restriction discussed in design notes does not exist in code.

---

## SPLIT 1 — New Game → Badge 1 (Norman, Petalburg)

### Littleroot Town
Required. Starting town. Warps to Birch's Lab, both rival houses, and `MAP_STARTER_ROOM`.
Connects north to Route 101 only.

- **Starter choice** is the *Johto* trio: Chikorita / Cyndaquil / Totodile, Lv5
  (`LittlerootTown_ProfessorBirchsLab`). Default IVs — not perfect.
- **Supplies NPC** (added this session): gives 900 each of Rare Candy, Max Repel,
  Full Restore, Escape Rope, once, gated on `FLAG_RECEIVED_STARTER_SUPPLIES`.
- Starting money: 999,999.
- No wild encounters, no overworld items.

### Route 101
Required corridor to Oldale. Wild land: Poochyena / Wurmple / Zigzagoon, **Lv2–3**
(vanilla table). No trainers, no items.

### Oldale Town
Required. Mart (now stocks ₽1 Rare Candies), Poké Center. Connects north to
Route 103, west to Route 102.

### Route 103 — *optional, and badly over-levelled*
Reachable immediately from Oldale. **14 trainers.** The rival fight here is Lv5,
but the rest of the route is Lv14–15:

| Trainer | Class | Team |
|---|---|---|
| BRENDAN / MAY | Rival | one starter, Lv5 (3 variants by player choice) |
| AMY & LIV | Twins | Plusle Lv15, Minun Lv15 |
| ANDREW | Fisherman | Magikarp Lv5, Tentacool Lv10, Magikarp Lv15 |
| DAISY | Aroma Lady | Shroomish Lv14, Roselia Lv14 |
| ISABELLE | Swimmer F | Marill Lv15 |
| MARCOS | Guitarist | Voltorb Lv15 |
| MIGUEL | Pokéfan | Skitty Lv15 @ Oran Berry |
| PETE | Swimmer M | Tentacool Lv15 |
| RHETT | Black Belt | Makuhita Lv15 |

Wild: Poochyena / Wingull / Zigzagoon Lv2–4 land; water and fishing tables reach
Lv25–45 (Sharpedo, Wailmer) but need Surf/Rod.
Items: Guard Spec., PP Up.

### Viridian Forest — *optional side area, Split 1*
Entered by warp from Route 103 at (4,0)/(5,0). A dead end — its only exit is back
to Route 103.

- **Gen 2 starter trade** at (8,1), the far end of the forest: trade any Pokémon
  for your choice of Chikorita / Cyndaquil / Totodile, **31 IVs in all six stats**,
  once only.
- **No wild encounters at all.**
- Three object events have `script: NULL` — two Bug Catchers and one
  `BRENDAN_FIELD_MOVE` NPC. They are placed but inert.

### Route 102
Required westward route to Petalburg. 4 trainers, **Lv3–5** (Allen, Calvin, Rick,
Tiana). Wild land Lv3–4 including Ralts and Seedot. Items: Potion, TM Pluck.

### Petalburg City
Required. Gym, Mart, Poké Center, Wally's house. Connects west to Route 104, east
to Route 102, **south to `MAP_TRAINER_GROVE`** (a custom map).
Items: Max Revive, Ether, TM Fling, hidden Rare Candy.

### Trainer Grove → Petalburg Coast — *optional, unfinished*
Custom maps reached south out of Petalburg City.

**Trainer Grove** holds four trainers named **Bum1–Bum4**, each with an identical
team of **Wailmer Lv34 + Horsea Lv34**, and with mismatched classes (Salon Maiden,
Swimmer M, Team Magma, Team Aqua). Lv34 is above every boss in splits 1–4.

**Petalburg Coast** holds RONNIE (Youngster; Zigzagoon Lv13, Taillow Lv14) who
gates an **Oval Stone** item ball behind `goto_if_not_defeated`. Neither map has
wild encounters.

### Petalburg Gym — BADGE 1
| Trainer | Team |
|---|---|
| RANDALL | Taillow Lv14 @ Flame Orb (Guts), Zigzagoon Lv14 @ Bright Powder (Quick Feet) |
| ALEXIA | Jigglypuff Lv14 @ Focus Sash (Aftermath), Whismur Lv15 @ Sitrus Berry (Rattled) |
| JODY | Zangoose Lv16 @ Quick Claw (Hyper Cutter), Slakoth Lv16 @ Sitrus Berry (Truant) |
| **NORMAN** | Zigzagoon Lv14 @ Bright Powder, Whismur Lv15 @ Sitrus Berry, Zangoose Lv16 @ Quick Claw, **Slakoth Lv16 @ Custap Berry** |

All fully specified: explicit abilities, natures, held items, 31 IVs, Smart Trainer AI.

---

## SPLIT 2 — Badge 1 → Badge 2 (Roxanne, Rustboro)

### Route 104
Required. **13 trainers, Lv5–15** — mostly Lv5–9 filler plus the Lv13–15 rival.
Wild land Lv3–5 (Marill, Poochyena, Taillow, Wingull, Wurmple).
9 items including Bright Powder and a hidden Heart Scale.
Warps to Petalburg Woods, Mr. Briney's house, the Flower Shop.

> Mr. Briney's boat to Dewford is gated behind `FLAG_CLEARED_WRAITHWOOD_ROUTE`,
> which is set on first entering Wraithwood Forest — closing what was a shortcut
> past Rustboro entirely.

### Petalburg Woods
Required-ish. 3 trainers: a **Team Aqua Grunt** (Poochyena Lv9), James (2× Nincada
Lv6), Lyle (4× Wurmple Lv3). Wild land Lv5–6.
**10 items**, including a hidden **Sceptilite** (Mega Stone) and TM Silver Wind.
Warps to `MAP_LOST_CAVE` (custom, currently empty: no trainers, no items, no wild table).

### Rustboro City
Required. Gym, Mart, Devon Corp, Poké School. Rival battle (Lv13–15).
Connects north to Route 115, south to Route 104, east to Route 116.
Warps additionally to **`MAP_HOLLOWBROOK_SHACK_MINING_TUNNEL`** — the entrance to
the custom region.

- **Evolution stone NPC** at (18,34) (added this session): choice of one Fire /
  Water / Leaf Stone, once only.
- Item: Razor Claw. Two in-game trade NPCs sit in Rustboro houses (Seedot trade).

### Route 116 / Rusturf Tunnel — *optional*
Route 116: 10 trainers Lv8–9, wild Lv6–8, 8 items including TM Steel Wing.
Rusturf Tunnel: a **Team Aqua Grunt** (Poochyena Lv11) and Mike (2× Geodude +
Machop, Lv16). Wild Whismur Lv5–8.

### Rustboro Gym — BADGE 2
| Trainer | Team |
|---|---|
| JOSH | Geodude Lv18 @ Hard Stone, Geodude Lv18 @ Rocky Helmet, Nosepass Lv18 @ Sitrus Berry |
| TOMMY | same shape at Lv19 |
| MARC | same shape at Lv20 |
| **ROXANNE** | Geodude Lv20 @ Hard Stone, Geodude Lv21 @ Rocky Helmet, **Aron Lv22 @ Life Orb**, **Nosepass Lv23 @ Wide Lens** |

Fully specified (abilities, items, 31 IVs). Note the three rank-and-file trainers
run the *identical* 2×Geodude + Nosepass shape, differing only by level.

---

## SPLIT 3 — Badge 2 → Badge 3 (Brawly, Dewford)

The intended route runs **Rustboro → Mining Tunnel → Hollowbrook → Wraithwood
Forest → Granite Cave B2F → B1F → 1F → Route 106 → Dewford**, with Granite Shore
hanging off Dewford.

### Hollowbrook Shack / Mining Tunnel
Corridor from inside Rustboro. No trainers, items, or encounters.

### Hollowbrook
2 trainers, 3 Pokémon each, Lv25–26, fully specified:
- **MORTIMER** (Gentleman): Duskull @ Lum Berry, Shuppet @ Spell Tag, Roggenrola @ Sitrus Berry
- **PRUDENCE** (Lass): Spinarak @ Black Sludge, Nincada @ Silver Powder, Whismur @ Sitrus Berry

No wild encounters. No items.

### Wraithwood Forest
Sets `FLAG_CLEARED_WRAITHWOOD_ROUTE` on first entry. 3 trainers, 3 Pokémon each, Lv24–26:
- **DESMOND** (Bug Catcher): Nincada, Spinarak, Shuppet
- **OTTOLINE** (Hex Maniac): Shuppet, Duskull, Roggenrola
- **RUFUS** (Hiker): Roggenrola, Aron @ Metal Coat, Nosepass @ Wide Lens

**Gift: Skorupi Lv22**, Jolly, Battle Armor, 31 IVs, at (20,11). Evolves to Drapion at 32.
No wild encounters. No items.

### Granite Cave (B2F → B1F → 1F)
One trainer per floor, 3 Pokémon each:
- **ANSEL** (B2F): Aron Lv27 @ Focus Sash, Nosepass Lv26, Roggenrola Lv27
- **DALTON** (B1F): Makuhita Lv25 @ Black Belt, Aron Lv25 @ Life Orb, Geodude Lv25
- **PERCY** (1F): Aron Lv25 @ Metal Coat, Geodude Lv25 @ Hard Stone, Nosepass Lv25

Items: B2F has Repel, Rare Candy, 2× hidden Everstone and a hidden **Aerodactylite**
(Mega Stone). B1F a Poké Ball; 1F an Escape Rope.

**Wild encounters are vanilla and far below the trainers:** 1F Zubat/Geodude/Makuhita/Abra
**Lv6–10**, B1F Aron/Sableye/Zubat/Makuhita/Abra **Lv9–11**, B2F Aron/Sableye/Zubat/Abra
**Lv10–12** — against trainers at Lv25–27.

### Granite Shore
Connects up to Dewford Town. 2 trainers, 3 Pokémon each at Lv24:
- **DEACON**: Wingull @ Mystic Water, Tentacool @ Poison Barb, Zigzagoon @ Bright Powder
- **PRIYA**: Tentacool, Wingull, Jigglypuff @ Focus Sash

**Gift: Dwebble Lv20** (Rancher Andy), Adamant, Sturdy, 31 IVs. Evolves to Crustle at 26.
No wild encounters, no items.

### Routes 105 / 106 / 107 — *optional, water routes*
- **Route 105**: 8 trainers Lv25–26 (Ruin Maniacs with Sandshrew/Sandslash, Swimmers,
  Bird Keeper, and TESS with Tentacool Lv25 / Wingull Lv26). Water + fishing only.
  Items: Star Piece, hidden Heart Scale, hidden Big Pearl.
- **Route 106**: 5 trainers. Douglas (2× Tentacool Lv24), Kyla (Wailmer Lv26), MILO
  (Geodude Lv25 @ Hard Stone, Aron Lv25 @ Metal Coat) — plus **Elliot (Lv7–10) and
  Ned (Lv11)**, vanilla leftovers far under the rest. Items include TM Aurora Beam.
- **Route 107**: 6 trainers Lv24–27, all water.

### Dewford Gym — BADGE 3
| Trainer | Team |
|---|---|
| BRENDEN, TAKAO | Machop Lv13 |
| CRISTIAN | Makuhita Lv13 |
| JOCELYN, LAURA, LILITH | Meditite Lv13 |
| **BRAWLY** | Machop Lv16, Meditite Lv16, Makuhita Lv19 @ Sitrus Berry |

**This gym is still vanilla.** Single-Pokémon Lv13 rank-and-file; the leader has no
specified abilities, natures or IVs, and only one held item. The player arrives here
having fought Lv24–27 trainers.

---

## SPLIT 4 — Badge 3 → Badge 4 (Wattson, Mauville)

### Route 108 / 109
- **Route 108**: 6 trainers Lv24–26 (incl. Carolina with 2× Manectric + Swellow Lv24).
  Warps to the Abandoned Ship. Items: Star Piece, hidden Rare Candy.
- **Route 109**: 13 trainers, **Lv12–27 mixed** — Elijah (2× Skarmory Lv25) and
  Mel & Paul (Dustox/Beautifly Lv27) sit beside Chandler, Edmond, Hailey, Huey,
  Lola and Ricky at Lv12–13. 8 items.

### Slateport City
Required. No trainers. Water/fishing tables only. Mart stocks ₽1 Rare Candies.

### Route 110
Required north to Mauville. **20 trainers, Lv6–20** — the widest spread in the game:
Triathletes and Psychics at Lv14–16, Jacob and Jasmine carrying **Lv6 Voltorbs**,
and the rival at Lv18–20. Items include Scope Lens, Rare Candy, and a hidden
**Swampertite** (Mega Stone). Trick House entrance is here (its puzzles gate on badge count).

### Mauville City
Required. Wally rival battle (Ralts Lv16). Items: Ultra Ball, TM Sleep Talk.
Mart stocks ₽1 Rare Candies. Game Corner bouncer hides once badge 4 is earned.

### Mauville Gym — BADGE 4
| Trainer | Team |
|---|---|
| ANGELO | Illumise Lv17, Volbeat Lv17 |
| BEN | Zigzagoon Lv17, Gulpin Lv17 |
| KIRK | Electrike Lv17, Voltorb Lv17 |
| SHAWN | Voltorb Lv17, Magnemite Lv17 |
| VIVIAN | Meditite Lv17, Meditite Lv17 |
| **WATTSON** | Voltorb Lv20, Electrike Lv20, Magneton Lv22, Manectric Lv24 @ Sitrus Berry |

**Also still vanilla** — no abilities, natures or IVs specified; one held item on the ace.

---

## Accessible earlier than intended

- **Route 103's Lv14–15 trainers** are reachable before badge 1, with a Lv5–8 party.
- **Trainer Grove's Lv34 trainers** are reachable in Split 1 from Petalburg City.
- **Route 110 and Route 117** are reachable from Route 103's east connection
  (`Route103 → right → MAP_ROUTE110`), i.e. potentially before badge 1.
- **Three Mega Stones** are obtainable across splits 2–4 (Sceptilite in Petalburg
  Woods, Aerodactylite in Granite Cave, Swampertite on Route 110), earlier than the
  badge-4 Mega unlock discussed in design notes.
- **Altering Cave** warps directly off Route 103 (Split 1).
