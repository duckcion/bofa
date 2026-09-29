# Repo Audit Notes — 2026-09-29

Audit of branch `garett2` (29 commits ahead of upstream baseline `c0459aacc`), scoped to
what a Platinum-Kaizo/Emerald-Kaizo-style difficulty pass through badge 4 would need to
know before touching anything. This file is raw findings, not a proposal.

## 1. Actual overworld gym order (verified, not assumed)

Traced real map topology via `data/maps/*/map.json` `connections` **and** `warp_events`
(BFS from `MAP_LITTLEROOT_TOWN`), then cross-checked which gym script sets which
`FLAG_BADGEnn_GET`:

| Gym building | BFS depth from Littleroot | Leader | Badge flag it sets |
|---|---|---|---|
| `PetalburgCity_Gym` | 5 (reachable right after Oldale) | Norman | `FLAG_BADGE05_GET` |
| `RustboroCity_Gym` | 7 (via Route104's direct N connection) | Roxanne | `FLAG_BADGE01_GET` |
| `DewfordTown_Gym` | 9 (via Route104 boat → cave chain) | Brawly | `FLAG_BADGE02_GET` |
| `MauvilleCity_Gym` | not reached within depth 10 from this path | Wattson | `FLAG_BADGE03_GET` |

**This is the single biggest inconsistency in the repo right now.** Physically, Petalburg
is the *first* gym a walking player reaches, but its script still stamps the vanilla
`FLAG_BADGE05_GET` (Norman's original slot, gated in vanilla behind 4 other badges).
Rustboro/Dewford still stamp vanilla `BADGE01`/`BADGE02` despite being reached *after*
Petalburg here. Nothing downstream has been re-pointed at new flag numbers. Any code or
script elsewhere that checks "has N badges" (Vs. Seeker unlock, Repel/Exp Share unlocks,
trainer flag gates, HM gating) will read a badge *count* that no longer matches the badge
*order* the player actually experiences. This needs an explicit decision (renumber the
flags to match real order, or keep flag identity per-leader and stop treating "badge N"
as a proxy for progression count) before any splits are built on top of it.

No 4th gym has been integrated into the reordered path yet — Mauville is not reachable
within 10 hops of the traced route from Petalburg/Rustboro/Dewford. A 4th badge for the
"first four splits" scope does not currently exist as a placed, connected gym.

Also note: `Route104/map.json`'s first `connections` entry is a direct edge to
`MAP_RUSTBORO_CITY` (not gated through `PetalburgWoods`). `PetalburgWoods` is reached via
separate warp events on the same route. Blocking Petalburg Woods (commit `a37232794`)
does **not**, by itself, appear to close the direct road to Rustboro — worth confirming
in an emulator before assuming the new Wraithwood/Granite Cave/Granite Shore detour is
the only path to Rustboro. If the direct road still works, that new region content is
currently optional/bypassable rather than mandatory critical path.

## 2. Current early-gym trainer rosters

Checked `src/data/trainers.party` (source of truth, compiled by `tools/trainerproc` into
`src/data/trainers.h`) for the 4 leaders found above:

- **Roxanne** (`TRAINER_ROXANNE_1`): Geodude Lv12 / Geodude Lv12 / Nosepass Lv15 @Oran —
  100% vanilla Emerald moveset, levels, item.
- **Brawly** (`TRAINER_BRAWLY_1`): Machop Lv16 / Meditite Lv16 / Makuhita Lv19 @Sitrus —
  100% vanilla.
- **Norman** (`TRAINER_NORMAN_1`): Spinda Lv27 / Vigoroth Lv27 / Linoone Lv29 / Slaking
  Lv31 @Sitrus — 100% vanilla (this is Norman's *late-game* vanilla level range, badge 5
  in the original game — reinforces the flag-number mismatch above, since this team is
  clearly balanced for "5th gym," not "1st gym reached").
- **Wattson** (`TRAINER_WATTSON_1`): Voltorb Lv20 / Electrike Lv20 / Magneton Lv22 /
  Manectric Lv24 @Sitrus — 100% vanilla.

**None of the four leader teams have been touched yet.** All Kaizo-style team design
(movesets, held items, bench size, AI-aware sets, level curve matched to the real badge
order) for splits 1–4 is greenfield work, not a tuning pass on existing work. Rank-and-file
gym trainers (e.g. Dewford's Takao/Jocelyn/Laura/Brenden/Cristian/Lilith, Petalburg's
Randall/Alexia/Jody door-trigger trainers, Rustboro's Josh/Tommy/Marc, Mauville's
Kirk/Shawn/Ben/Vivian/Angelo) are likewise untouched vanilla.

## 3. AI tier config

`include/constants/battle_ai.h`:
```
AI_FLAG_BASIC_TRAINER  = CHECK_BAD_MOVE | TRY_TO_FAINT | CHECK_VIABILITY
AI_FLAG_SMART_TRAINER  = BASIC_TRAINER | OMNISCIENT | SMART_SWITCHING | SMART_MON_CHOICES
```
So the engine's own "Basic Trainer" preset **is** effectively PK/EK's baseline "expert"
tier already (bad-move avoidance + KO-seeking + full viability scoring) — the naming is
just an engine holdover, not a sign the AI is dumb.

Commit `9de32ead7` additionally hand-edited the **generated** `src/data/trainers.h` to
bolt `AI_FLAG_SMART_MON_CHOICES` onto trainers (confirmed on Roxanne's compiled entry:
`.aiFlags = AI_FLAG_BASIC_TRAINER | AI_FLAG_SMART_MON_CHOICES`), but the **source**
`trainers.party` still just says `AI: Basic Trainer` for her (and the other leaders) with
no `Smart Mon Choices` suffix.

**Regen-drift risk**: `include/config/general.h` has `COMPETITIVE_PARTY_SYNTAX TRUE`,
which activates the Makefile rule `%.h: %.party ; ... $(TRAINERPROC) ...`. Standard `make`
mtime rules mean the *first* time anyone edits `trainers.party` (to redesign any trainer
for the Kaizo pass — which is the whole point of this project) and rebuilds, `trainerproc`
regenerates the **entire** `trainers.h` from the `.party` source and **silently drops every
hand-added `SMART_MON_CHOICES` flag repo-wide**, since the source text doesn't request it.
Fix is cheap: `trainerproc` already supports multi-flag AI lines (confirmed precedent
elsewhere in the file: `AI: Check Bad Move / Try To Faint / Force Setup First Turn`), so
the fix is just editing the `.party` source lines to say
`AI: Basic Trainer / Smart Mon Choices` (or whatever the intended tier is) instead of
patching the generated header. This should happen before any further trainer-party edits.

Move-scoring bug fixes from `81e65420e` (Simple ability cap, Magic Guard/immunity/screen
penalty tuning, Belly Drum threshold, recovery-at-full-HP tuning, Haze/Psych Up stacking
cap, Mold Breaker/Suction Cups, Metal Burst) are in `src/battle_ai_main.c` /
`src/battle_ai_util.c` directly — these are engine-level, apply globally, no data-side
follow-up needed.

Known **pre-existing upstream** AI gaps (not introduced by the 29 commits, but relevant to
"improve trainer AI" scope): `battle_ai_main.c`/`battle_ai_util.c` have long-standing
`TODO`s for predicted-move tracking, opponent-switch prediction, and hazard-aware
switching logic. These are real known ceilings on how "smart" the AI can get without new
engine work, not data/config issues.

## 4. Species changes so far

`e89683532` ("buff underpowered Gen5-6/cross-gen mons") touched every
`src/data/pokemon/species_info/gen_{1,2,3,4,5,6}_families.h` file (194 to 966 changed
lines each) plus `src/data/moves_info.h` (1260 lines) and
`src/data/pokemon/level_up_learnsets/gen_9.h` (15,331 lines — this file is the shared
level-up-learnset table despite the "gen_9" name, not a Gen-9-only file). Scope is broad
but I did not verify individual species changes for balance quality — spot-checking
correctness of these edits against the PK/EK references is unstarted work.

`b0b2da473` (setup-move restriction) is a narrower, purely-subtractive pass: 73 lines
removed from `level_up_learnsets/gen_9.h`, stripping named setup moves (Swords Dance,
Dragon Dance, Quiver Dance, Calm Mind, Bulk Up, Coil, Nasty Plot, Agility, Rock Polish,
Autotomize, Shift Gear, Shell Smash, Belly Drum, Tail Glow, No Retreat, Victory Dance,
Geomancy) from legendaries/pseudo-legendaries/top-tier mons only — did not touch
stats/abilities.

## 5. Evolution method changes

`4444b4a1b` + `30d9ec091` together touched only `species_info/gen_{2,3,4,5,6}_families.h`
for a combined ~29 changed lines. This is a **narrow, spot-fix pass** (the second commit
was explicitly patching species the first commit's macro-based scan missed: Unown,
Mothim, Arceus), not a comprehensive Gen 1–6 evolution-accessibility audit. Given the
stated requirement ("make all evolutions reasonably accessible without trading/unusual
party requirements"), trade evolutions, held-item evolutions, and other non-level methods
across the full Gen 1–6 dex are still largely an open, unstarted task.

## 6. Item economy currently in code

- **TM consumability is already implemented and already active** —
  `include/config/item.h:25`: `#define I_REUSABLE_TMS FALSE`. Comment confirms Gen3-style
  single-use TMs is the current behavior; making a TM reusable is opt-in per-TM via an
  "importance" flag, not the default. So "make TMs consumable" needs **no engine work** —
  it's already the engine default. What's still needed is purely data/script-side:
  updating every TM pickup point (16 new TM item balls from `e56efa36d`, any gift/NPC TM
  hand-outs) to give quantity 2 instead of 1.
- **Bag pocket sizes are currently tight.** The `b6c4905de`/`af9c4528b`/`6e5337836`/
  `74168d2b2` chain was a live SaveBlock1-overflow firefight: TM/HM and Berry pockets were
  trimmed to their *exact current item counts* and Mystery Gift was disabled outright just
  to fit under the SaveBlock1 size ceiling. There is currently **no slack** in that budget —
  adding more berries/held items/TM slots for the progressive item-economy requirement will
  immediately re-trigger this same overflow unless pocket capacity or SaveBlock1 layout is
  revisited first. This is a hard technical constraint on the "stronger berries/held items
  progressively" and "unlimited Rare Candy/Max Repel/Full Restore/status berries" asks.

## 7. Mega Evolution status

No on/off config gate exists — Mega Evolution is a core always-on expansion battle
gimmick (per upstream README's documented feature list); `CanMegaEvolve()` is a plain
engine function (`include/battle_util.h:241`). The only Mega-related config is
`B_MEGA_EVO_TURN_ORDER` (post-mega Speed timing, Gen7 behavior), unrelated to
availability.

`0ed8c6214`'s "first-pass Mega Stone hidden items" placed stones across
`Route104/105/106/110/111/112` and `SlateportCity`. **Routes 110–112 and Slateport are
well beyond the badge-4 scope** (they're vanilla-later-game areas) — so Mega Stone
reachability-by-badge-4 is not currently satisfied by this first pass and needs explicit
re-scoping (either relocate stones earlier, or place a badge-4-appropriate subset) before
"player Mega Evolution around the fourth badge" can be delivered.

## 8. Testing infrastructure

`docs/tutorials/how_to_testing_system.md` documents the battle-engine test suite: `make
check -j` (all tests), `make check TESTS="Foo"` (filtered), `make pokeemerald-test.elf
TESTS="Foo"` (buildable-in-mGBA variant). Tests live under `test/battle/`. This suite
covers battle mechanics (move effects, abilities, AI behavior flags), not overworld
scripts/maps/story content — there is no automated coverage for the map/gym-order/trainer
rework itself.

`test_failures_baseline_2026-09-29.txt` (captured by the project's build hook at current
HEAD) records **93 pre-existing FAILED/ERROR test cases**. This is the correct baseline
for all future `make check` runs to diff against — a new run introducing failures beyond
these 93 is a regression; matching or improving on 93 is neutral-or-better. Notable: a
handful of the 93 are AI-behavior tests (`AI_FLAG_SMART_MON_CHOICES`,
`AI_FLAG_SMART_SWITCHING`, `AI_FLAG_SEQUENCE_SWITCHING`, `AI_FLAG_DOUBLE_ACE_POKEMON`).
Whether these predate the AI-tier commits or were introduced by them hasn't been
bisected — worth a targeted check before trusting the AI upgrade is regression-free,
since AI-flag test failures are exactly what an AI rework would produce if it broke
something.

## 9. Unfinished / in-progress work

Two uncommitted working-tree files, both **safe, regenerated build artifacts** rather
than in-progress hand edits — fine to commit as-is once verified building clean:

- `src/data/pokemon/teachable_learnsets.h`: +5389 lines. Commit `e89683532` had emptied
  this file's content (net -1923 lines in that commit) as an apparent side effect; the
  working tree now has it **regenerated/restored**, including teachable-move entries for
  the 16 new TMs (`MOVE_GRASS_KNOT`, `MOVE_SLEEP_TALK`, etc. now appear in e.g.
  Bulbasaur/Ivysaur's teachable list). This looks like the output of a codegen step run
  after the new-TM commits, not manual authoring.
- `data/text/trainers.inc`: 78-line diff, but it's pure poryscript-recompilation noise —
  adds `# <line> "trainers.pory"` source-map comments and makes labels global (`::`
  instead of `:`). No text content changed.

Untracked `last_check_run.log` (545 KB) is the build hook's captured `make` output; the
build hook itself already ignores both this and the baseline-failures file by name
pattern, so they won't interfere with future hook runs.

`TODO`/`FIXME`/`XXX` markers inside files touched by the 29 commits are all **pre-existing
upstream** AI-engine limitations (see §3), not new markers left behind by this project's
own work — the 29 commits didn't leave any of their own TODO breadcrumbs.
