# Design Proposal — Splits 1–4 (Badges 1–4)

Status: **DRAFT — awaiting approval**. Nothing in this document has been implemented.
Sources: `docs/audit_notes.md` (repo audit), `Reference/notes/platinum_kaizo_summary.md`,
`Reference/notes/emerald_kaizo_summary.md` (both DOCUMENTED/INFERRED-tagged).

## 1. What's already implemented (from `docs/audit_notes.md`)

- Physical world already reroutes the player Petalburg → (Wraithwood/Granite
  Cave/Granite Shore detour, or a possibly-still-open direct road) → Rustboro →
  Dewford, in that order — but the `FLAG_BADGE0N_GET` flags on those gyms are
  untouched vanilla (Norman=05, Roxanne=01, Brawly=02). No 4th gym is connected yet.
- All 4 leader teams (Norman, Roxanne, Brawly, Wattson) are 100% untouched vanilla
  stats/levels/movesets. AI engine work (commits `9de32ead7`, `81e65420e`) upgraded
  the *engine*, not any trainer's actual data.
- `AI_FLAG_BASIC_TRAINER` (bad-move avoidance + KO-seeking + full viability scoring)
  is already bofa's/PK's baseline "expert" tier by another name. A hand-patch added
  `SMART_MON_CHOICES` to the compiled header only, **not** the `.party` source — this
  will silently vanish repo-wide the first time anyone edits `trainers.party` and
  rebuilds, because `COMPETITIVE_PARTY_SYNTAX` regenerates the header from source.
- TM consumability needs **zero engine work** — `I_REUSABLE_TMS FALSE` is already
  active Gen3-style behavior. Only data changes (award qty 2) are needed.
- SaveBlock1 has **no spare capacity** — bag pockets were just trimmed to exact
  current counts and Mystery Gift disabled to stop an overflow.
- Mega Evolution is always-on engine-wide (no config gate). First-pass Mega Stones
  were placed on Routes 110–112/Slateport — all beyond badge-4 in any gym order.
- Evolution-method and stat/ability changes so far are narrow spot-fixes (~29 and a
  few hundred lines respectively across the whole Gen1-6 dex), not a comprehensive
  pass.
- 93 pre-existing test failures are the current baseline (`test_failures_baseline_2026-09-29.txt`);
  some are AI-flag tests, not yet bisected against the AI-tier commits.
- Two uncommitted files (`teachable_learnsets.h`, `trainers.inc`) are safe regenerated
  build artifacts, not hand-edits in progress.
- Gift-NPC scaffolding already exists (Rancher Andy in Wraithwood, a Dewford gift
  NPC) — 2 of the requested "~2 gift mons" already have a physical NPC placeholder,
  just no confirmed species/trade partner yet.
- `HollowbrookShack_MiningTunnel` already exists as a map — a natural, already-built
  hook for a Team Magma mining/excavation subplot without inventing a new area.

## 2. Reference findings — documented vs. inferred

**Platinum Kaizo** (Sinnoh, not directly portable map-wise, but the primary stat/AI/
evolution reference per your instruction):
- Stat philosophy is targeted *redistribution toward a role* (e.g. Blastoise loses
  Atk/Speed, gains +25 SpAtk; cocoon stages get defensive niches), not uniform power
  creep. **DOCUMENTED.**
- Evolution fixes are case-by-case (Bulbasaur line sped up, Squirtle/Charmander left
  alone), not a blanket cut. **DOCUMENTED.** Trade-evolution conversion patterns are
  **INFERRED** only (not line-verified across all ~700 rows).
- Move changes buff weak early moves (elemental punches 75→95 BP uniformly; several
  Normal-type moves retyped/renamed to fit a specific mon). **DOCUMENTED.**
- No "AI tier list" tab exists — the real taxonomy is a 12-flag header
  (Basic/Eval Att/Expert/Status/Risky/Damage Prio/Baton Pass/Tag AI/Check
  HP/Weather/Harassment/Left Side AI) on each gym's sheet; per-trainer assignments
  weren't extractable this pass (blank/merged cells). **DOCUMENTED taxonomy, gap on
  assignments.**
- Splits 1-4 (Roark/Gardenia/Fantina/Maylene) aces: **Lv16 / Lv28 / Lv38 / Lv47.**
  Gym 4 (47) runs meaningfully higher than "low 40s." Fantina's gym is a 7-room
  gauntlet, not a single fight — a bigger structural lift than a re-leveled leader.
  **DOCUMENTED.**
- Badges-1-4 non-wild content: 2 trades + 2 gifts. **DOCUMENTED.**
- TM consumability and "unlimited candy/berries" have **no PK precedent** — Gen4
  TMs are natively infinite-use, so this is a from-scratch Gen3 decision for bofa,
  not something to copy. **DOCUMENTED (absence confirmed).**

**EK Mastersheet** (Hoenn-native, directly portable):
- Keeps **vanilla Hoenn gym order** for badges 1-4: Roxanne→Brawly→Wattson→Flannery.
  **DOCUMENTED.**
- Aces: Lv15 / Lv19 / Lv29 / Lv42. All single fights, no gauntlets. **DOCUMENTED.**
- Held items near-universal even on route trainers; TMs/tutors used liberally to arm
  early trainers with off-level movesets; Explosion/Selfdestruct common on glass
  cannons even at Lv5-15; doubles seeded as early as Route 104/Mauville.
  **DOCUMENTED.**
- Encounter design: small curated pools (6-12 species) per area, freely mixing all
  generations, themed by area flavor rather than strict Hoenn canon.
  **DOCUMENTED/INFERRED pattern.**
- Team Aqua/Magma already active well before badge 4 (Petalburg Woods, Rusturf
  Tunnel, Route 106), with full held-item/moveset rigor, not "easy mode" cameos.
  **DOCUMENTED.**
- "Split" as a segment/checkpoint concept is a **Platinum Kaizo-only** convention —
  EK's document has no equivalent structure.

## 3. Proposed design — pending your approval (see gating questions at the end)

### 3a. Gym order (RECOMMENDED, gated — see Q1/Q3)

Adopt the order the map already physically implies, and fix the flags to match it,
rather than reverting built content:

| Split | Badge | Gym leader | Type | Existing map location |
|---|---|---|---|---|
| 1 | 1 | Norman | Normal | Petalburg City (already first-reachable) |
| 2 | 2 | Roxanne | Rock | Rustboro City |
| 3 | 3 | Brawly | Fighting | Dewford Town |
| 4 | 4 | Wattson | Electric | Mauville City (**not yet connected** — needs a route link from the Dewford/Rustboro cluster) |

This keeps all 4 gyms as **existing vanilla buildings/rosters to re-skin**, rather
than inventing a new original gym — lowest implementation risk for an initial-scope
milestone. Normal-type as gym 1 is a reasonable "simple, no early coverage trap"
introductory type, not a mismatch.

### 3b. Level curve (RECOMMENDED, calibrated to your stated "high-30s/low-40s" target)

| Split | Leader | Ace level | Team band | Reference anchor |
|---|---|---|---|---|
| 1 | Norman | 16 | 13–16 | PK Roark (16), EK Roxanne (15) |
| 2 | Roxanne | 23 | 20–23 | midpoint toward PK Gardenia (28) |
| 3 | Brawly | 31 | 28–31 | PK Gardenia (28) / EK Wattson (29) zone |
| 4 | Wattson | 40 | 37–40 | your stated "low 40s"; EK Flannery (42) |

Flagging honestly: PK's own badge-4 (Maylene, 47) runs higher than this. I've
anchored to *your* explicit "low 40s" target plus EK's Hoenn-native Flannery (42)
rather than PK's raw number, since you cited "high-30s/low-40s" as your intent. Say
the word and I'll push badge 3/4 closer to PK's actual 38/47 instead.

### 3c. Encounter philosophy (RECOMMENDED)

Adopt EK's approach for all badge-1-4 routes and the new custom regions
(Wraithwood/Granite Shore/Hollowbrook): small curated pools (6-12 species) per area,
freely mixing Gen 1-6, themed by area flavor rather than strict Hoenn canon or flat
vanilla tables. This directly serves "preserve recognizable Hoenn while adding new
areas" — new areas get their own distinct flavor pool rather than reusing whatever's
nearby.

### 3d. Item economy (RECOMMENDED, gated — see Q4)

- TM consumability: give quantity 2 per TM pickup (already-active engine behavior,
  zero engine risk).
- Unlimited Rare Candy/Max Repel/Full Restore/status berries + progressively
  stronger held items: implement via mart availability and NPC/hidden-item
  placement **without increasing bag pocket slot counts** — swap which items occupy
  existing slots as the game progresses, rather than adding net-new slots. This
  avoids re-triggering the SaveBlock1 overflow that was just fixed. If this proves
  too cramped in practice, expanding SaveBlock1 becomes its own dedicated technical
  task (see risks below) rather than something to attempt casually mid-split.

### 3e. Story structure (RECOMMENDED)

- Seed Team Magma into `HollowbrookShack_MiningTunnel` as an active excavation site
  (ties their canonical "land/geology" motif directly to an already-built map, no
  new area needed) starting around split 2-3.
- Seed Team Aqua into the Granite Cave/Granite Shore corridor (coastal/cave —
  matches their canonical "sea" motif) around split 1-2, following EK's precedent of
  early, competent (not cameo) grunt encounters with held items and real movesets.
- Gift/trade plan: confirm Rancher Andy (Wraithwood) and the existing Dewford gift
  NPC as the 2 gift mons; add one new trade NPC (location TBD, likely Rustboro or
  Mauville) to complete "2 gifts + 1 trade."
- Mega Stones: relocate a badge-4-reachable subset from Routes 110-112/Slateport
  into the actual splits-1-4 path (candidates: Granite Cave, Wraithwood Forest, or a
  Mauville-area hidden item) so "player Mega Evolution around badge 4" is actually
  deliverable.

### 3f. Boss-entry level caps (new engine-adjacent feature, not in either reference)

Neither PK nor EK document this mechanic — Nuzlocke-community "level cap" enforcement
is a bofa-original addition. Proposed implementation: a per-required-trainer level-cap
constant table + a pre-battle script check that blocks entering the fight (with a
clear in-game message) if any party member exceeds that trainer's cap. Overleveling
is otherwise unrestricted, per your spec. This is scoped as its own roadmap phase
(4b below) since it's genuinely new engineering, not a data/tuning task.

## 4. Technical dependencies and risks

1. **Badge-flag/order mismatch** (see §3a) — must be resolved before any other split
   work, since anything checking "badge count" (HM gates, Repel/Exp Share unlocks,
   Vs. Seeker, HM access) currently reads a count that doesn't match play order.
2. **AI regen-drift bug** — must fix `trainers.party` source lines to say
   `AI: Basic Trainer / Smart Mon Choices` (or the intended tier) *before* any other
   trainer-party edits, or the existing AI upgrade silently regresses repo-wide on
   first rebuild.
3. **SaveBlock1 has zero headroom** — constrains every item-economy task; the
   swap-don't-add approach (§3d) is the mitigation, but needs validating against
   actual SaveBlock1 usage as splits are built.
4. **Route104 direct-road bypass** — needs confirming in an emulator whether the new
   Wraithwood/Granite Cave/Granite Shore region is currently skippable via a direct
   Petalburg→Rustboro road. If so, decide (gated — see Q2) whether to close it and
   make the new region mandatory critical path.
5. **Mauville is not yet connected** to the Petalburg/Rustboro/Dewford cluster — a
   new route link is required work, not just re-leveling.
6. **93-test baseline** needs a targeted bisect on the AI-flag-related failures
   before trusting the AI-tier commits are regression-free.
7. **Full Gen 1-6 dex balancing** (stats/abilities/movepool niches) is realistically
   a multi-milestone effort (only ~29-a-few-hundred lines touched so far across 900+
   species). For this badge-4 milestone, scope is: species actually
   encountered/battled/available by badge 4, with the data structures built so later
   splits can extend the same pass incrementally. Full living-dex balancing is a
   later-milestone goal, not a badge-4 blocker. Flagging this interpretation now —
   correct me if you intended full-dex balancing as part of badge-4 scope.
8. **Mega Evolution gating** — need to verify during implementation whether this
   expansion base requires a "Mega Bracelet"-equivalent item before
   `CanMegaEvolve()` succeeds, or purely a held Mega Stone; affects how "unlock at
   badge 4" is delivered (an item grant vs. just stone placement).

## 5. Prioritized roadmap (splits 1-4 only)

**Phase 0 — engineering fixes (blocking, do first)**
- 0.1 Fix `trainers.party` AI-flag source lines so `SMART_MON_CHOICES` survives
  regen. *Acceptance: rebuild `trainers.h`, confirm Roxanne's compiled entry still
  carries `AI_FLAG_SMART_MON_CHOICES` with no manual header edits.*
- 0.2 Bisect the 93 baseline test failures for AI-flag-specific regressions.
  *Acceptance: written verdict on whether AI-tier commits introduced any of the 93.*
- 0.3 Confirm in an emulator whether Route104's direct road bypasses the new region.
  *Acceptance: documented yes/no with a save-state or route trace.*

**Phase 1 — world/progression lock-in (blocking, depends on Q1-Q3 answers)**
- 1.1 Renumber `FLAG_BADGE0N_GET` to match the approved order.
- 1.2 Connect Mauville into the traversal path (if Wattson is badge 4).
- 1.3 Resolve the Route104 bypass per your decision (close it, or accept it as
  optional content).
- *Acceptance: BFS trace from Littleroot confirms badges are earned in the intended
  order and no vanilla progression flag (HMs, Repel, Exp Share, Vs Seeker) reads a
  mismatched count.*

**Phase 2 — split 1 (Norman/Petalburg)**
- Re-level and redesign Norman's team + Petalburg's required trainers to the Q-approved
  level band; assign held items/AI flags; confirm Rancher Andy's gift species.
  *Acceptance: `make check` shows no new failures vs. baseline; manual playtest of
  the gym clears at the target level band without needing to grind past it.*

**Phase 3 — split 2 (Roxanne/Rustboro)**
- Same pattern as Phase 2, plus: seed Team Aqua's first Granite Cave/Shore
  appearance; place a Mega Stone if this split is the chosen location.

**Phase 4 — split 3 (Brawly/Dewford)**
- Same pattern; confirm Dewford gift NPC's species; seed Team Magma's
  Hollowbrook mining-tunnel presence if this split is the chosen timing.

**Phase 4b — level-cap engine feature**
- Design + implement the pre-battle level-cap check (see §3f). *Acceptance: a test
  battle correctly refuses entry with an over-cap party member and allows entry once
  swapped out, verified both by an automated battle-engine test and a manual
  playtest.*

**Phase 5 — split 4 (Wattson/Mauville)**
- Build the new Petalburg/Dewford/Rustboro-cluster→Mauville route; re-level Wattson
  and required trainers; deliver Mega Evolution unlock (stone + any bracelet-item
  requirement); add the trade NPC to complete the gift/trade plan.

**Phase 6 — item economy pass**
- Implement TM qty-2 awards; unlimited Candy/Repel/Full Restore/status-berry
  availability and progressively-stronger held items across splits 1-4, within the
  no-new-pocket-slots constraint (§3d).

**Phase 7 — regression pass**
- Full `make check` vs. baseline; targeted manual playtest of all 4 gyms end-to-end
  at the intended level bands; update `docs/audit_notes.md`/progress doc with final
  state.

Each phase compiles via the existing build hook and runs `make check` before being
called done; results get compared against the 93-failure baseline, not treated as
"compiles = works."
