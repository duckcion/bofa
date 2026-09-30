# First Four Splits — Design Gaps

**Generated:** 2026-09-30 from project files.

Two kinds of statement appear below and are kept separate:
- **[FILE]** — read from the project.
- **[REFERENCE]** — read from the supplied Platinum Kaizo workbook or Emerald Kaizo
  Mastersheet.
- **[RECOMMENDATION]** — my own suggestion. Not authoritative, not implemented.

---

## Per-area summary

Trainer counts are by script reference. "Mandatory" is **unknown for every area** —
whether a trainer's sight range forces the battle cannot be read from data files, so
the mandatory/optional split below is not filled in rather than guessed.

| Area | Split | Trainers | Custom teams | Wild table | OW items | Density verdict |
|---|---|---|---|---|---|---|
| Littleroot Town | 1 | 0 | — | none | 0 | fine (hub) |
| Route 101 | 1 | 0 | — | vanilla Lv2–3 | 0 | **thin** |
| Oldale Town | 1 | 0 | — | none | 0 | fine (hub) |
| Route 103 | 1 | 14 | 0 | vanilla | 2 | dense but mis-levelled |
| Viridian Forest | 1 | 0 | — | **none** | 0 | **empty except the trade** |
| Route 102 | 1 | 4 | 0 | vanilla Lv3–4 | 2 | fine |
| Petalburg City | 1 | 0 | — | water only | 4 | fine (hub) |
| Trainer Grove | 1 | 4 | 0 | **none** | 0 | **placeholder content** |
| Petalburg Coast | 1 | 1 | 1 | **none** | 1 | thin |
| Petalburg Gym | 1 | 4 | 4 | — | 0 | good |
| Route 104 | 2 | 13 | 0 | vanilla Lv3–5 | 9 | dense |
| Petalburg Woods | 2 | 3 | 0 | vanilla Lv5–6 | 10 | fine |
| Lost Cave | 2 | 0 | — | **none** | 0 | **completely empty** |
| Rustboro City | 2 | 6 (rivals) | 0 | none | 1 | fine (hub) |
| Route 116 | 2 | 10 | 0 | vanilla Lv6–8 | 8 | dense |
| Rusturf Tunnel | 2 | 2 | 0 | vanilla Lv5–8 | 3 | fine |
| Rustboro Gym | 2 | 4 | 4 | — | 0 | good |
| Mining Tunnel | 3 | 0 | — | **none** | 0 | **empty corridor** |
| Hollowbrook | 3 | 2 | 2 | **none** | **0** | thin |
| Wraithwood Forest | 3 | 3 | 3 | **none** | **0** | thin |
| Granite Cave B2F | 3 | 1 | 1 | vanilla Lv10–12 | 5 | **thin (1 trainer/floor)** |
| Granite Cave B1F | 3 | 1 | 1 | vanilla Lv9–11 | 1 | **thin** |
| Granite Cave 1F | 3 | 1 | 1 | vanilla Lv6–10 | 1 | **thin** |
| Granite Shore | 3 | 2 | 2 | **none** | **0** | thin |
| Route 105 | 3 | 8 | 1 | water/fish only | 3 | fine |
| Route 106 | 3 | 5 | 1 | water/fish only | 5 | fine |
| Route 107 | 3 | 6 | 0 | water/fish only | 0 | fine |
| Dewford Gym | 3 | 7 | **0** | — | 0 | **un-redesigned** |
| Route 108 | 4 | 6 | 0 | water/fish only | 2 | fine |
| Route 109 | 4 | 13 | 0 | water/fish only | 8 | dense, mis-levelled |
| Slateport City | 4 | 0 | — | water/fish only | 0 | fine (hub) |
| Route 110 | 4 | 20 | 0 | vanilla | 8 | dense, mis-levelled |
| Mauville City | 4 | 1 (Wally) | 0 | none | 2 | fine (hub) |
| Mauville Gym | 4 | 6 | **0** | — | 0 | **un-redesigned** |

**Totals for splits 1–4: ~146 trainer placements, of which 21 have custom teams (~14%).**

---

## The significant problems

### 1. The badge level curve is inverted [FILE] — HIGH

| Badge | Leader | Ace level | Intent per `docs/PROGRESS.md` |
|---|---|---|---|
| 1 | Norman | 16 | 16 ✓ |
| 2 | Roxanne | 23 | 23 ✓ |
| 3 | Brawly | **19** | 31 ✗ |
| 4 | Wattson | **24** | 40 ✗ |

Badge 3 is *lower* than badge 2. Worse, the player reaches Brawly only after
crossing Hollowbrook/Wraithwood/Granite Cave/Granite Shore (Lv24–27) and Routes
105–107 (Lv24–27) — so route trainers outlevel the gym leader by 6–8 levels.

Splits 1–2 were redesigned; splits 3–4's gyms were not. This is the single biggest gap.

### 2. Custom areas have no wild encounters [FILE] — HIGH for a Nuzlocke hack

ViridianForest, PetalburgCoast, TrainerGrove, LostCave, Hollowbrook,
WraithwoodForest, GraniteShore and the Mining Tunnel all have **no entry in
`wild_encounters.json`**. In a Nuzlocke, an area with no encounter grants no catch —
so the entire custom region contributes zero party options.

[REFERENCE] Emerald Kaizo gives every route a curated 6–12 species pool, and
Platinum Kaizo likewise treats each area as an encounter slot. Both treat area
encounters as core, not optional.

### 3. Granite Cave's wild levels are ~15 below its trainers [FILE] — HIGH

Trainers Lv25–27; wild Aron/Sableye/Zubat/Abra at **Lv9–12** (vanilla tables). Any
Pokémon caught there is immediately unusable at that point in the game.

### 4. Trainer Grove is unfinished placeholder content on a Split-1 path [FILE] — HIGH

Four trainers, "Bum1"–"Bum4", identical Wailmer Lv34 + Horsea Lv34 teams, classes
that don't match the sprites or each other (Salon Maiden / Swimmer M / Team Magma /
Team Aqua). Lv34 exceeds every boss in splits 1–4. Reachable from Petalburg City in
Split 1, and it's the route to the Oval Stone.

### 5. Route 103 is over-levelled for Split 1 [FILE] — MEDIUM

14 trainers, most at Lv14–15, reachable straight out of Oldale with a Lv5–8 party.
The rival there is Lv5. Route 102 next door is Lv3–5.

### 6. Vanilla level leftovers inside redesigned areas [FILE] — MEDIUM

- Route 106: Elliot (Lv7–10) and Ned (Lv11) sit among Lv24–26 trainers.
- Route 109: six trainers at Lv12–13 among Lv24–27.
- Route 110: Jacob and Jasmine carry Lv6 Voltorbs among Lv14–20.

### 7. Mega Stones available before the intended unlock [FILE] — MEDIUM

Sceptilite (Petalburg Woods, Split 2), Aerodactylite (Granite Cave B2F, Split 3),
Swampertite (Route 110, Split 4) — all hidden items, all obtainable at or before
badge 4, while design notes place the Mega unlock at badge 4.

### 8. No level caps [FILE] — MEDIUM

`B_LEVEL_CAP_TYPE = LEVEL_CAP_NONE`. With 900 Rare Candies handed out in Littleroot
and unlimited money, nothing prevents arriving at badge 1 massively overlevelled.
[RECOMMENDATION] If the boss-entry restriction is still wanted, `sLevelCapFlagMap`
in `src/caps.c` with `LEVEL_CAP_FLAG_LIST` is the ready-made path.

### 9. Repetitive gym rosters [FILE] — LOW

Rustboro's Josh/Tommy/Marc all run the identical 2× Geodude + Nosepass shape,
differing only in level. Granite Cave's three floors are one trainer each, all
Rock/Steel.

### 10. Empty custom maps [FILE] — LOW

`LostCave` (warp from Petalburg Woods) has no trainers, items or encounters.
Mining Tunnel is a bare corridor. Viridian Forest has three inert `script: NULL`
object events.

---

## Progression and softlock risks

- **No softlock identified.** Briney's shortcut is gated behind
  `FLAG_CLEARED_WRAITHWOOD_ROUTE`, which is set on entering Wraithwood — reachable
  from inside Rustboro, so the gate cannot lock a player out of Dewford.
- [FILE] **Not verified:** whether Route 103's east connection to Route 110 is
  actually walkable before badge 1. If it is, a Split-1 player can wander into
  Split-4 terrain. Needs in-game checking.
- [FILE] **Not verified:** whether Trainer Grove's Lv34 trainers have forcing sight
  ranges. If they do, a Split-1 player could be hard-blocked from the Oval Stone.

---

## Density compared with the references

[REFERENCE] Platinum Kaizo's own split sheets (counted via the per-trainer
`AI Flags:` rows): Roark split 26 trainers total (23 outside the gym), Gardenia 47,
Fantina 42, Maylene 37. Gym interiors scale 3 → 4 → 18 → 7.

[FILE] This project's splits 1–4 run roughly 23 / 38 / 36 / 49 trainer placements.

[RECOMMENDATION] Raw counts are already in PK's range; the gap is *quality*, not
quantity — 86% of splits 1–4 trainers still carry vanilla teams with no items,
abilities, natures or IVs. PK specifies all of those on every trainer, including
Route 202 filler. Bringing Dewford and Mauville gyms up to the standard already set
at Petalburg and Rustboro would matter more than adding bodies.
