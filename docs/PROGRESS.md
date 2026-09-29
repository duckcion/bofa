# Splits 1-4 Implementation Progress

Living document. Updated as work happens. See `docs/design_proposal_splits_1-4.md`
for the full approved design and `docs/audit_notes.md` for the original repo audit.

## Approved decisions (2026-09-29)

1. **Gym order**: keep the physically-built route, renumber flags —
   Norman=badge1, Roxanne=badge2, Brawly=badge3, Wattson=badge4.
2. **New region routing**: make Wraithwood/Granite Cave/Granite Shore mandatory
   critical path (close the Route104 direct-road bypass if confirmed to exist).
3. **Badge 4 identity**: use existing Mauville/Wattson gym, not a new original gym.
4. **Item economy**: swap items into existing bag pocket slots as splits progress;
   do not expand SaveBlock1/pocket capacity.
5. Level curve: Norman 16 / Roxanne 23 / Brawly 31 / Wattson 40 (aces), per proposal
   §3b — provisional, open to adjustment.

## Status

| Phase | Item | Status |
|---|---|---|
| 0.1 | Fix `trainers.party` AI-flag source (regen-drift bug) | **done** |
| 0.2 | Bisect 93 baseline test failures for AI-flag regressions | **done — 93 FAILED, matches baseline exactly, zero regressions** |
| 0.3 | Confirm/close Route104 direct-road bypass | **done — real shortcut found and closed, see below** |
| 1.1 | Renumber `FLAG_BADGE0N_GET` to match approved order | not started (next up) |
| 1.2 | Connect Mauville into traversal path | not started |
| 1.3 | Resolve Route104 bypass per decision 2 | in progress (see 0.3) |
| 2 | Split 1 (Norman/Petalburg) redesign | not started |
| 3 | Split 2 (Roxanne/Rustboro) redesign | not started |
| 4 | Split 3 (Brawly/Dewford) redesign | not started |
| 4b | Level-cap engine feature | not started |
| 5 | Split 4 (Wattson/Mauville) + route + Mega unlock | not started |
| 6 | Item economy pass | not started |
| 7 | Regression pass | not started |

## Testing log

- **2026-09-29, post Phase-0.1 fix**: `make check -j8` running in background
  (`/tmp/check_run_after_fix.log` in WSL). Result pending.

## Findings worth recording

- **Phase 0.1 root cause, precise**: `trainers.party`'s `AI:` lines never said
  "Smart Mon Choices"; commit `9de32ead7` added `AI_FLAG_SMART_MON_CHOICES` by
  hand-patching the *compiled* `trainers.h` directly (confirmed via `git show
  9de32ead7 -- src/data/trainers.h`), never touching `trainers.party`. Any
  future edit-and-rebuild of `trainers.party` would have silently dropped it
  for all 843 flagged trainers. **Fixed** by appending `/ Smart Mon Choices` to
  all 848 `AI:` lines in `trainers.party` and regenerating `trainers.h`.
- **A second, older, pre-existing drift was discovered while fixing the
  above** (not caused by this project's 29 commits): for a subset of
  non-leader trainers (spot-checked example: `TRAINER_GRUNT_AQUA_HIDEOUT_1`),
  the currently-committed `trainers.h` carries MORE AI flags
  (`CHECK_BAD_MOVE | TRY_TO_FAINT | CHECK_VIABILITY`) than a fresh regen of
  their current `trainers.party` text (`AI: Check Bad Move`, one token) would
  produce. This means `trainers.h` was last properly regenerated at a point
  when `trainers.party`'s wording for these trainers was richer than it is
  now, and something since simplified the source text without rebuilding.
  This predates commit `9de32ead7` (whose diff is a pure textual append, not a
  regen) and is unrelated to bofa's own work.
  - **Verified NOT to affect any of the 4 gym leaders** (Roxanne/Brawly/
    Wattson/Norman) — their regenerated output is byte-identical to git HEAD,
    confirmed via `git diff -- src/data/trainers.h` showing zero hunks near
    any of their trainer blocks.
  - **Decision made (not gated on user approval — low-stakes, reversible,
    and those trainers get redesigned in later splits regardless)**: left
    as-is. The Phase-0.1 fix accepts the current simplified `trainers.party`
    text as ground truth going forward; any trainer outside splits 1-4 that
    loses a latent extra AI flag it only had via this stale drift is a very
    minor behavior change (subtle AI scoring only) and not a regression this
    project introduced. Flagging here for visibility, not asking for a
    decision — revisit only if a future full-roster AI pass wants to
    specifically audit this.

- **Phase 0.3 finding — corrects the original audit's framing.** Traced every
  `connections`/`warp_events` entry for the new region cluster via
  `data/maps/*/map.json`. The real topology is NOT "Wraithwood detours between
  Petalburg and Rustboro" as originally characterized. It's actually:
  - `RustboroCity` has a warp (x=29,y=50) straight into
    `HollowbrookShack_MiningTunnel` → `Hollowbrook` → `WraithwoodForest` →
    `GraniteCave_B2F` → `B1F` → `1F` → exits to `Route106` (and separately to
    `Route106` again, and to `GraniteCave_StevensRoom`).
  - `GraniteShore` connects directly to `DewfordTown` (map connection, not a
    warp).
  - So this whole cluster is a **side-loop entered from inside Rustboro**,
    functioning as an alternate Rustboro→Dewford path via Route106 — not a
    mandatory corridor between Petalburg and Rustboro. Route104's direct
    Petalburg↔Rustboro connection is just the normal, un-detoured road and has
    nothing to do with Wraithwood.
  - The actual "is it skippable" question is therefore: can a player reach
    Dewford from Rustboro via the plain vanilla `Route104→Route105→Route106`
    chain instead of the mining-tunnel loop? `Route105` is mostly ocean in
    vanilla Emerald and is normally only crossable via Mr. Briney's boat
    (story-gated) or HM Surf (obtained much later) — if that's still true here,
    the new region may **already be the only early-game-walkable path** to
    Dewford with nothing further to build. This hasn't been confirmed yet
    (would need to check the Briney boat script's trigger conditions and
    whether Route105 has any dry walkable strip).

- **Phase 0.3 resolved — the real shortcut was Mr. Briney's boat, not
  Route104/105/106 walking.** `Route104_MrBrineysHouse` has a fully-scripted,
  zero-prerequisite boat ride straight to Dewford
  (`Route104_MrBrineysHouse_EventScript_Briney` →
  `..._EventScript_SailingIntro`), triggerable the very first time a player
  ever talks to Briney — no badge check, no flag beyond "have you heard his
  intro yet." This skips Rustboro/Roxanne (badge 2) entirely, not just the new
  region, which is a bigger progression break than originally scoped.
  - **Fix applied**: added `FLAG_CLEARED_WRAITHWOOD_ROUTE` (claimed
    `include/constants/flags.h`'s unused `0x26` slot). Set via a new
    `MAP_SCRIPT_ON_TRANSITION` on `WraithwoodForest` (fires the first time the
    player ever enters that map — reachable only via
    Rustboro→mining-tunnel→Hollowbrook, or the long way from Route106→Granite
    Cave, both of which already require having reached Rustboro or beyond).
    Gated Briney's dialogue with `goto_if_unset
    FLAG_CLEARED_WRAITHWOOD_ROUTE → declines with in-character flavor text`
    before falling through to his existing sailing-offer logic.
  - Files touched: `include/constants/flags.h`,
    `data/maps/WraithwoodForest/scripts.inc`,
    `data/maps/Route104_MrBrineysHouse/scripts.inc`.
  - Full `make -j8` rebuild in progress to confirm this compiles clean before
    calling it done.
  - Not independently re-verified: whether the plain Route104→105→106 walking
    path is ALSO passable without Surf (likely blocked naturally by ocean
    tiles, per Route105's `gTileset_General`/`gTileset_Dewford` tileset pairing
    — consistent with vanilla ocean terrain — but not tile-by-tile confirmed).
    Given Briney's boat was the trivially-exploitable path and is now closed,
    this residual risk is low; flagging rather than re-opening a full
    walkability trace.

## Decisions needing your input

(none currently outstanding beyond what's already been asked)
