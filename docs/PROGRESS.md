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
| 1.1 | Renumber `FLAG_BADGE0N_GET` to match approved order | **done** — build clean, 93 FAILED matches baseline |
| 1.2 | Connect Mauville into traversal path | **already satisfied — see finding below, no work needed** |
| 1.3 | Resolve Route104 bypass per decision 2 | done (see Phase 0.3) |
| 2 | Split 1 (Norman/Petalburg) redesign | **implemented, build/tests running** |
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

- **Phase 1.1 (badge renumbering) — implementation notes.** Swapped which
  gym sets which `FLAG_BADGE0N_GET`: Norman 05→01, Roxanne 01→02, Brawly
  02→03, Wattson 03→04 (a clean 4-way rotation, no collisions). Before
  touching anything, audited every one of the ~30 repo-wide references to
  these 4 flags (`grep` across `data/` and `src/`) to separate gym-identity-
  specific checks (need renumbering) from generic badge-count/slot-index
  systems (HM field-move gating, traded-mon obedience caps, stat boosts,
  Trainer Card display, PokeNav/match-call arrays, debug cheats) — those are
  all `FLAG_BADGE01_GET + i`-style loops or hardcoded vanilla per-count
  thresholds (e.g. Cut=badge1, Rock Smash=badge3, Strength=badge4, Surf=
  badge5 — standard vanilla Hoenn HM/badge mapping) and correctly need **no
  changes**, since they'll now track "Nth gym beaten in real order" instead of
  a stale count.
  - Also updated 6 NPC-dialogue references outside the gyms themselves that
    were specifically about a named leader (Mom's Amulet Coin gift for
    beating Norman in `players_house.inc`; Rustboro's "have you challenged the
    gym" NPC and Scott's dialogue ×2 for Roxanne; Mauville's Game Corner
    bouncer for Wattson).
  - **Left unchanged, and confirmed already correct for the new order**:
    `GraniteCave_B2F`'s `WraithwoodBlocker` object event already gates on
    `FLAG_BADGE02_GET` — under the new numbering this correctly means "must
    have beaten Norman AND Roxanne" before passing through the cave toward
    Dewford, which is exactly the intended gate. It reads like this was
    already written anticipating this renumbering.
  - **Not touched (correctly out of scope)**: Lavaridge/Flannery's gym
    (vanilla badge 4) still also sets `FLAG_BADGE04_GET`. Harmless for now
    (unreachable this early, and Wattson will set it first in practice), but
    a future milestone extending past badge 4 will need to renumber her too.
- **Phase 1.2 finding**: Mauville is already reachable from Rustboro via the
  untouched vanilla path Rustboro→Route116→Verdanturf→Route117→Mauville
  (BFS-confirmed at depth 4, no script blockers found on Route116/Verdanturf).
  The original audit's "not reachable within 10 hops" was scoped to a
  different traversal and doesn't hold up — no new route construction is
  needed for splits 1-4's badge-4 connectivity.

- **Phase 2 (Petalburg/Norman split) — implementation.** Redesigned all 4
  required Petalburg Gym trainers for the approved Lv13-16 band (ace 16),
  replacing 100% vanilla late-game data:
  - **Randall**: Taillow Lv14 @ Sitrus Berry (Wing Attack/Tackle/Uproar/
    Whirlwind) — was Swellow Lv26. Swapped to the pre-evolution since Swellow
    doesn't naturally evolve until Lv22.
  - **Alexia**: Jigglypuff Lv14 @ Oran Berry (Sing/Double Slap/Water Pulse/
    Pound) — was Wigglytuff Lv26 (Wigglytuff needs a Moon Stone, not level,
    so pre-evolution is the only legal early option).
  - **Jody**: Zangoose Lv16 @ Quick Claw (False Swipe/Metal Claw/Fury Cutter/
    Cut) — was Lv26; Zangoose has no evolution so only the level/moveset/item
    needed changing.
  - **Norman (leader)**: Zigzagoon Lv14 -> Whismur Lv15 -> Slakoth Lv16 (ace),
    replacing his old Spinda/Vigoroth/Linoone/Slaking Lv27-31 vanilla team.
    Kept the Truant-ace callback to his vanilla Slaking identity via Slakoth
    (pre-evolution, still has Truant, evolves at 18 so it's legal at 16).
    AI upgraded to `Smart Trainer` (a single token that expands to
    `AI_FLAG_BASIC_TRAINER | AI_FLAG_OMNISCIENT | AI_FLAG_SMART_SWITCHING |
    AI_FLAG_SMART_MON_CHOICES` -- confirmed this composite macro exists and
    resolves correctly) rather than the baseline `Basic Trainer / Smart Mon
    Choices` the 3 rank-and-file trainers use, giving the boss fight
    meaningfully stronger AI per "bosses substantially more demanding."
  - All movesets verified against actual level-up learnsets (not guessed) so
    every move is legally learnable by that level; `trainerproc` and the full
    ROM build both accepted everything without a manual header hand-patch.
- **Gift-NPC finding**: the "Dewford gift NPC" the earlier audit flagged as
  existing is actually **dead/orphaned code** --
  `DewfordTown_EventScript_RanchGift` exists in `scripts.inc` but is not
  attached to any object event in `DewfordTown/map.json`, so it's currently
  unreachable in-game. It's also a near-exact duplicate of Rancher Andy's
  Granite Shore gift (same Tauros/Miltank/Bouffalant choice, different flag
  name), so wiring it up as-is would just offer the same reward twice rather
  than satisfying "~2 *different* gift mons." Deferring the real fix (pick a
  distinct species, wire up an actual NPC) to Phase 4 (split 3, Dewford) as
  already scheduled in the roadmap -- flagging now so it isn't mistaken for
  already-done work.

- **Universal AI upgrade (your request: "all trainers... highest AI tier...
  smart and predictable").** Applied `AI_FLAG_SMART_TRAINER` (the engine's own
  documented ceiling: `BASIC_TRAINER | OMNISCIENT | SMART_SWITCHING |
  SMART_MON_CHOICES` -- full knowledge of the player's moves/abilities/items,
  smarter switching, smarter mid-battle mon choices, plus the core bad-move-
  avoidance/KO-seeking/viability-scoring trio) to **every trainer in the
  game**, not just gym leaders:
  - Replaced all 848 existing `AI:` lines (whatever mix of `Basic Trainer`,
    `Check Bad Move`, `Try To Faint`, `Force Setup First Turn`, `Risky`,
    `Smart Mon Choices` they previously had) with a single `AI: Smart Trainer`.
  - Added a `Smart Trainer` line to 11 real trainers that had **no AI line at
    all** (Dianne, Jani, the 5 Lao ninja boys, Lung, Mariela, Alvaro, Everett)
    -- these were previously fighting with zero AI logic, an upstream data gap
    unrelated to bofa's own work. Left `TRAINER_NONE` and 4 explicitly-named
    `_PLACEHOLDER`/cameo trainers (Red, Leaf, Brendan, May) untouched since
    they're not real battles.
  - Deliberately did NOT stack additional flags beyond `Smart Trainer` (e.g.
    `Risky`, `Conservative`, `Prefer Strongest Move`, `Stall`) -- the header
    comment in `battle_ai.h` explicitly documents `Smart Trainer` as the
    complete "smart" ceiling and warns other flags are situational tuning that
    "could make the trainer worse/better depending on the flag," not a
    strictly-higher tier. Removing `Risky` specifically (a handful of trainers
    had it) directly serves "predictable" -- it made AI play more recklessly/
    accuracy-ignoring. `Force Setup First Turn` is superseded by `Smart
    Trainer`'s `CHECK_VIABILITY`, which the flag's own doc comment says
    "will instead do this when the AI determines it makes sense" -- a smarter,
    context-aware version of the same idea, not a downgrade.
  - Verified before applying: grepped every historical AI-line variant that
    ever existed in the file and confirmed none used a structural mechanic
    (Ace Pokemon, Double Ace Pokemon, Sequence Switching, etc.) that a blanket
    replace would have silently destroyed.
  - Note this changes the earlier "bosses get stronger AI than rank-and-file
    trainers" distinction from the original design proposal -- difficulty
    separation between ordinary trainers and gym leaders now needs to come
    from levels/movesets/items alone, not AI tier, since everyone's at the
    ceiling now. That's a direct consequence of this request, flagging it so
    it's a known tradeoff rather than a silent side effect.
  - Full rebuild + `make check` run to confirm no regressions before
    committing.

## Decisions needing your input

(none currently outstanding beyond what's already been asked)
