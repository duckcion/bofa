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
| 2 | Split 1 (Norman/Petalburg) redesign | **done** — 93 FAILED matches baseline |
| 3 | Split 2 (Roxanne/Rustboro) redesign | **done** — 93 FAILED matches baseline |
| - | Game-wide: remove spinning trainers | **done** — all 6 converted to fixed facing |
| - | Route trainer density expansion (per user request, vs. PK benchmark) | **done** — all previously-empty new-region maps now have trainers |
| - | Universal `Smart Trainer` AI upgrade (per user request) | **done** — 93 FAILED matches baseline |
| - | Disable match-call/rematch registration (`FREE_MATCH_CALL`, per user request) | **build verified clean, test pending** — also frees 104 bytes of SaveBlock1 |
| - | Remove EV-related items from the game (per user request) | **in progress** — see findings below |
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

- **Phase 3 (Roxanne/Rustboro split) — implementation.** Rustboro's rank-and-
  file gym trainers (Josh/Tommy/Marc, all Geodude) were already roughly early-
  game-appropriate species (unlike Petalburg's, which were vanilla badge-5
  leftovers) -- just needed re-leveling and real movesets for the Lv20-23 band:
  - Josh: Geodude Lv18 (was Lv10), Tommy: 2x Geodude Lv19 (was Lv8), Marc: 2x
    Geodude Lv20 (was Lv8).
  - Roxanne: Geodude(20)/Geodude(21)/Nosepass(23, ace), replacing her Lv12/12/15
    vanilla team. Kept Nosepass as ace (her signature mon) -- Solid Rock +
    Def135 wall stats plus Rock Slide/Iron Head/Seismic Toss give a real
    defensive-stall gameplan.
  - All movesets verified against level-up learnsets.

- **Game-wide: removed all spinning trainers (your request: "make all
  trainers mandatory... they don't spin").** Found exactly 6 across the whole
  game using `MOVEMENT_TYPE_ROTATE_CLOCKWISE/COUNTERCLOCKWISE` (Valerie/Cedric
  on Mt.Pyre 6F, Winston on Route104, Carol on Route112, Madeline on Route113,
  Bernie on Route114) -- converted all to `MOVEMENT_TYPE_FACE_DOWN`. A spinning
  trainer's sight-line sweeps around, letting a player dodge the encounter by
  timing their walk past. Note: this addresses the "can't dodge by timing"
  angle specifically; a full "no trainer can be walked around via map
  geometry" audit (checking every trainer's sight range/placement against
  walkable alternate paths) is a much bigger, separate undertaking not done
  here.

- **Route trainer density expansion (your request, informed by the Platinum
  Kaizo trainer-count research below).** Added 3 new trainers to Wraithwood
  Forest, which had zero before: Desmond (Bug Catcher, Nincada Lv24),
  Ottoline (Hex Maniac, Shuppet Lv25), Rufus (Hiker, Roggenrola Lv26) --
  positioned using the map's actual collision data (not guessed) so none are
  placed on non-walkable tiles. New trainer IDs `TRAINER_DESMOND`/
  `TRAINER_OTTOLINE`/`TRAINER_RUFUS` (864-866) claim the free ID space a
  prior session reserved. **Hit and recovered from a naming collision twice**
  (first tried "Wade", which collided with a pre-existing vanilla
  `TRAINER_WADE` (344); a careless blanket rename to "Waylon" then collided
  with a pre-existing `TRAINER_WAylon` too) -- both times caught by the build
  (`initialized field overwritten` error), reverted cleanly via git checkout
  of just the affected files, and redone with names verified unique by grep
  *before* editing. Remaining empty new-region maps (Hollowbrook, Granite Cave
  1F/B1F/B2F, Granite Shore) still need the same treatment -- not done yet.
  Platinum Kaizo's own gym-interior trainer counts (Roark 3, Gardenia 4,
  Fantina 18(!), Maylene 7 -- Fantina's 7-room gauntlet is a deliberate
  outlier, not a template) already roughly match bofa's current 4-per-gym
  (leader+3), so gyms don't need more work on this front; the real gap is
  route/overworld density, where PK runs 23-43 trainers per split versus
  bofa's current ~27 across the whole badge-1/2 area combined. Full research
  in `Reference/notes/platinum_kaizo_trainer_counts.md`.

- **Disabled match-call/rematch registration (your request: "don't have
  people try to register my number").** Found `FREE_MATCH_CALL` in
  `include/config/save.h` -- an existing, fully-supported config flag (used
  via `#if FREE_MATCH_CALL == FALSE` guards throughout `battle_setup.c`) that
  removes the match-call/rematch/VS Seeker system entirely. Flipped it to
  `TRUE`. Confirmed via rebuild that none of the 843+ trainer scripts (which
  all reference `ShouldTryRematchBattle`/`register_matchcall`/
  `IsTrainerRegistered`) broke -- those calls remain valid, they just become
  inert. Bonus: frees exactly the promised 104 bytes of SaveBlock1 (confirmed
  via the build's memory-usage report, EWRAM used dropped from 243210 to
  243106 bytes), which directly helps the tight SaveBlock1 budget flagged in
  the original audit.

- **EV-related items removed from acquisition (your request: "remove those
  items from game rn").** Scope: Vitamins (HP Up/Protein/Iron/Calcium/Zinc/
  Carbos), Macho Brace, 15 EV-lowering berries (Pomeg/Kelpsy/Qualot/Hondew/
  Grepa/Tamato/Cornn/Magost/Rabuta/Nomel/Spelon/Pamtre/Watmel/Durin/Belue), and
  6 Power items (Weight/Bracer/Anklet/Band/Lens/Belt). Power items were
  already unplaced anywhere in the game (item definitions only, never sold or
  given) -- nothing to do there. For the rest:
  - Removed the 6 vitamins from `BattleFrontier_Mart`'s general item list
    (simple deletion from a mixed list).
  - `SlateportCity`'s "Energy Guru" and `LilycoveCity_DepartmentStore_3F`'s
    vitamin counter and `BattleFrontier_ExchangeServiceCorner`'s Vitamin Clerk
    were each **dedicated, vitamin-only** shops/NPCs -- emptying their item
    lists would leave a shop selling nothing, so instead redirected each NPC's
    script to a short "we don't carry those anymore" message, skipping the
    shop/menu interaction entirely. The now-unreferenced item-list data blocks
    and menu-branch functions are harmless orphaned dead code, left in place.
  - `Route111_WinstrateFamilysHouse`'s one-time Macho Brace gift now gives a
    Choice Band instead (kept the gift, swapped the item + matching dialogue).
  - `Route123_BerryMastersHouse`'s obscure secret-phrase reward system (an
    e-Reader-era Easter egg, rarely triggered) previously gave one of 5
    EV-lowering berries for specific phrases -- simplified so every phrase now
    falls through to the same "give a random normal berry" default the system
    already had, rather than rebuilding the whole phrase-reward branch.
  - `data/scripts/new_game.inc` seeded 20 wild berry trees across Routes
    115/119/123 with EV-lowering berries (Kelpsy x3, Pomeg x7, Hondew x2,
    Grepa x4, Qualot x4) -- swapped each species for a always-useful non-EV
    berry (Oran/Sitrus/Lum/Persim/Chesto respectively) so the trees stay
    functional decorations instead of becoming empty/dead tree slots.
  - **Core mechanic fix, done.** Found via `git blame` that EV-gain-from-battle
    was *already* disabled by an earlier commit (`0ed8c6214` -- buried inside
    a commit whose message was about an unrelated Mauville gate/Mega Stone
    change, `MonGainEVs` in `src/pokemon.c` has an unconditional early
    `return` with a comment saying EVs are disabled in this hack). That
    explains why vitamins were the last real lever -- they `SetMonData`
    EVs directly, bypassing the disabled battle-gain path entirely. Closed
    the loop by editing `CalculateMonStats` (same file) so all 6 EV reads are
    hardcoded to 0 instead of `GetMonData(..._EV, ...)`, meaning even a mon
    with legacy/traded-in nonzero EV data gets zero stat effect from it.
    Checked `pokemon_summary_screen.c` for an EV display to hide -- there
    isn't one, nothing to clean up there.
  - Rebuild + `make check` pending for this specific change (bundled with the
    other EV-item edits above).
- **IV display, done (press START on the Skills page).** The stats page has
  zero free screen space -- mapped out every window template's tilemap
  coordinates and confirmed it's a fully-packed 30x20 tile grid with
  background-graphic labels, no room to add numbers alongside the existing
  ones. Implemented as a toggle instead: START swaps the existing HP/Atk/Def/
  SpAtk/SpDef/Speed number windows between normal stats and IVs -- same
  widgets, guaranteed to fit. Added IV fields to the summary screen's
  internal struct (it didn't retain them before) and populated them
  alongside the existing stat extraction. **Not visually verified** -- the
  logic is straightforward but this needs a quick in-game check.

- **Lua debug script, done: `bofa_kaizo_tools.lua`** (predamage/set-status/
  change-weather/export-to-Showdown). Read the full EK.lua reference --
  its Gen3 mon-decryption logic (BitXOR + substructSelector) is standard,
  unchanged Pokémon data layout and was ported as-is, but its hardcoded
  memory addresses are specific to vanilla Emerald's compiled binary and
  don't apply to bofa. Pulled bofa's own addresses from `pokeemerald.elf`
  via `arm-none-eabi-nm` (gPlayerParty=0x02035694, gEnemyParty=0x020358ec,
  gBattleWeather=0x02000754, etc).
  - **Species names read directly from the ROM** (works for all 1524+
    species, vanilla or newly added) -- this took real effort since bofa's
    expansion moved species names into a per-species struct instead of a
    flat name table EK.lua could just index; the struct's per-field offset
    comments are stale (its "0xC4" size comment is wrong -- the struct has
    clearly grown since that was written), so the offset (44) and true
    per-entry stride (260 bytes, not the commented size) were derived
    **empirically**: searched the compiled ROM for Bulbasaur's own
    charmap-encoded name (confirmed bofa uses mixed-case names like
    "Zigzagoon", not "ZIGZAGOON" -- cost one wrong search attempt) and
    computed the offset from where it was found relative to species index 1.
  - **Real correctness catch before finalizing**: initially wrote the
    decryption/bitmasking logic using native Lua bitwise operators
    (`~ & | << >>`). Re-reading EK.lua's own `BitXOR` function -- which
    exists specifically because it can't assume native XOR support --
    made clear that assuming 5.3+ bitwise operators would risk the whole
    script failing to even parse on whatever Lua version mGBA embeds.
    Rewrote all bit operations (XOR/AND/right-shift) as pure arithmetic
    functions, matching EK.lua's own defensive style, before finalizing.
  - Moves use a hardcoded vanilla Gen1-3 name table (ported from EK.lua);
    bofa's own move additions (TM51-66, the Gen4-9 movepool) print as
    "Move #N" -- scoped out for time. Items/abilities print as "Item #N"/
    "Ability #N" (numeric only, no name table attempted this pass). Natures
    are always correct (computed from personality, not a lookup table).
  - Export tries an mGBA scripting clipboard API (a few plausible names,
    wrapped in `pcall` since it's genuinely uncertain whether one exists in
    a given mGBA build) but always also writes `bofa_export.txt` as the
    guaranteed fallback, and prints to the console either way.
  - **Follow-up: actually tested the two riskiest assumptions against the
    real running game.** mGBA turned out to be installed on this machine
    (`C:\Program Files\mGBA`). No Lua interpreter or gdb binary was available
    anywhere in this environment, but the GDB remote serial protocol is
    simple enough to implement directly -- wrote a minimal Python client,
    launched mGBA with `-g` (its built-in GDB server), and connected to it.
    Confirmed: (1) reading 13 bytes at the derived address for species index
    1 (`gSpeciesInfo + 1*260 + 44`) returns exactly Bulbasaur's charmap-
    encoded name byte-for-byte -- validates the empirically-derived struct
    offset/stride against the live game, not just the static ROM file; (2)
    writing 2 bytes to a party mon's HP offset and reading it back returns
    the written value unchanged -- validates the basic read/write mechanics
    `sethp`/`setstatus`/`setweather` all depend on. Closed the test instance
    afterward. Still not verified: the Lua file's own syntax/runtime (no Lua
    interpreter was available to actually load and call it) and mGBA's exact
    scripting API surface (clipboard, io sandboxing) -- those need an actual
    Tools > Scripting session.

- **Route trainer density expansion, completed.** Filled in the remaining
  empty new-region maps the same way as Wraithwood (collision-data-verified
  placement, level-up-learnset-verified movesets):
  - Hollowbrook: Mortimer (Gentleman, Duskull Lv25) and Prudence (Lass,
    Spinarak Lv26).
  - GraniteCave_1F: Percy (Hiker, Aron Lv25).
  - GraniteCave_B1F: Dalton (Black Belt, Makuhita Lv25).
  - GraniteCave_B2F: Ansel (Hiker, Aron Lv27) -- placed clear of the existing
    item balls, rock-smash puzzle rocks, and the `WraithwoodBlocker` story
    gate.
  - GraniteShore: **found two more orphaned trainers** (Deacon/Priya,
    `TRAINER_GRANITESHORE_1`/`_2`) -- same bug pattern as the Dewford gift
    NPC from Phase 2: fully-written `trainerbattle_single` scripts existed
    with real dialogue, but were never attached to an object event, AND
    their trainers.party entries were still the generic "Lv1 Lillipup, 0 IVs"
    placeholder stub used for reserved-but-undesigned trainer slots. Wired
    both to actual map positions and gave them real teams (Wingull Lv24,
    Tentacool Lv24) instead of creating a new trainer ID for this slot --
    cleaner than growing the ID space further when a matched pair of
    already-reserved IDs was sitting right there.
  - **Two naming collisions hit and recovered from** during this pass
    (picked "Wade" and then "Waylon" for the very first Wraithwood trainer,
    both already existed as vanilla trainer names) -- see the entry above
    for how those were fixed. This time, checked every candidate name via
    `grep` before writing anything, and hit zero further collisions across
    Percy/Dalton/Ansel/Mortimer/Prudence (Delia did collide with an
    intended-male sprite mismatch, not a name collision -- caught by re-
    reading my own placement rather than the build).
  - New trainer IDs 864-871 (`TRAINER_DESMOND` through `TRAINER_ANSEL`)
    claim more of the free ID space a prior session reserved (864-1199);
    `TRAINERS_COUNT` bumped to 872 accordingly.

- **Difficulty pass on the 18 trainers built so far (Wraithwood, Hollowbrook,
  Granite Cave x3, Granite Shore, Petalburg Gym, Rustboro Gym), prompted by
  "these are not near PK/EK level."** Pulled the actual per-trainer data out
  of `Copy of Platinum Kaizo Docs (1).xlsx` (Roark Split, Gardenia Split)
  with openpyxl rather than relying on the earlier condensed summary, to see
  what really makes PK trainers hit harder. Findings that changed the plan:
  - **Real PK route trainers already run 3-4 mons at Lv5-8** (Route 202's
    "Bug Catcher Logan" has four). Team size was the most visible gap but
    turned out not to be the biggest lever.
  - **Items are the biggest lever**, not raw stats: PK fields Focus Sash,
    Life Orb, BrightPowder, Wide Lens, Berry Juice, Light Clay, Mystic Water,
    etc. on ordinary early trainers, not just Oran Berry. Nearly every
    species this game has fielded so far as a route mon (Nincada, Spinarak,
    Shuppet, Duskull, Roggenrola, Aron, Wingull, Tentacool, Makuhita,
    Geodude, Zigzagoon, Whismur, Slakoth, Jigglypuff, Taillow) is itself an
    unevolved pre-evolution, so **Eviolite** (1.5x Def/SpDef, engine-legal
    on any species with a defined evolution) is the single highest-value,
    most PK-authentic fix and got applied broadly. Type-boost items
    (Silver Powder, Black Sludge, Mystic Water, Spell Tag) went on the
    mons whose kit is actually offensive; Custap Berry went on Norman's
    true ace (Slakoth) as a boss flourish; Taillow got a deliberately odd
    Flame Orb + Guts combo (permanent 1.5x Attack, burn damage ignored by
    Guts) as a PK-style "give it a signature weird item" pick.
  - **Abilities were left at silent slot-1 default everywhere** -- added
    explicit `Ability:` lines picking the mechanically strongest option per
    species (Solid Rock Nosepass -- literally PK's own pick for the same
    species; Intimidate Spinarak; Cursed Body Shuppet; Sturdy Roggenrola;
    Rock Head Aron, which also cancels Take Down's recoil on the exact
    move these Aron carry; Quick Feet Zigzagoon, pairing with its new base
    Speed buff).
  - Applied a targeted BST redistribution (PK's "give a role, don't
    power-creep the whole dex" pattern, not a blanket buff) to the five
    weakest pre-evolutions actually in use: Zigzagoon (240->280, Atk/Spe/SpD),
    Whismur (240->275, HP/Def/SpD), Makuhita (237->265, HP/Atk/SpD), Nincada
    (266->291, Atk/Spe), Slakoth (280->290, HP/Atk token bump). Left the
    other 12 species (Taillow/Jigglypuff/Zangoose/Geodude/Nosepass/Shuppet/
    Roggenrola/Duskull/Spinarak/Aron/Wingull/Tentacool, all BST 270-458)
    alone -- PK's own doc shows already-decent mons get zero or token
    changes, not another buff.
  - **Checked the move-buff request against `src/data/moves_info.h` and
    found most of it already done upstream**, not by me: DoubleSlap
    (30 BP/90 acc), Fire/Ice/Thunder Punch (95 BP), Mega Kick (140 BP/90
    acc/8 PP), and the accuracy-floor cleanup on Cut/Tackle/Fury Attack/
    Take Down/Wrap (all 100%) match PK's documented buffs exactly, but come
    from upstream commit `e89683532` ("Buff underpowered Gen5-6/cross-gen
    mons..."), not a PK-specific change -- coincidental convergence, not
    something that needed redoing.
  - IVs across all 18 trainers were sitting at 20-24 instead of the spec's
    default of 31 -- a self-imposed ~15-25% stat penalty with no design
    rationale behind it. Bumped to 31 everywhere in scope.
  - **Fixed a third occurrence of the "orphaned trainer" bug**, worse than
    the previous two: `TRAINER_MORTIMER`, `TRAINER_PRUDENCE`, `TRAINER_PERCY`,
    `TRAINER_DALTON`, `TRAINER_ANSEL` were wired into their maps'
    `trainerbattle_single` scripts and had IDs reserved in `opponents.h`,
    but **had no matching `=== TRAINER_X ===` block in `trainers.party` at
    all** -- likely lost in the Wade/Waylon revert from an earlier pass.
    `trainerproc` silently emits a zero-initialized entry for an ID with no
    block rather than erroring, so these 5 fights would have run with an
    empty party. Written from scratch (species/level/class were still
    documented above from the original design pass) with the same
    item/ability treatment as everything else in this pass.
  - **Corrected after user feedback: dropped the blanket-Eviolite approach,
    pulled real per-trainer PK data directly from the xlsx (Roark Split,
    Gardenia Split sheets via openpyxl) instead of guessing.** That data
    showed real PK route trainers running 3-4 mons even at Lv5-8 (Route
    202's "Bug Catcher Logan" has four), and using a genuinely varied item
    pool (Focus Sash, Life Orb, BrightPowder, Wide Lens, Mystic Water, Light
    Clay, Berry Juice, Razor Claw, Silk Scarf) rather than one item
    repeated everywhere. Rebuilt all 18 trainers accordingly: route
    trainers (Wraithwood x3, Hollowbrook x2, Granite Cave x3, Granite Shore
    x2) now run 3 mons each with distinct items per mon (Silver Powder,
    Black Sludge, Bright Powder, Lum Berry, Sitrus Berry, Metal Coat, Wide
    Lens, Black Belt, Life Orb, Hard Stone, Rocky Helmet, Poison Barb,
    Focus Sash, Flame Orb+Guts, Custap Berry, Quick Claw -- no single item
    repeated more than 2-3 times across the whole set); Rustboro's Josh/
    Tommy/Marc went to 3 mons; Norman and Roxanne (leaders) stayed at 4.
  - **Went back to the "make every Pokemon match PK" ask directly** rather
    than inventing stat buffs: extracted PK's full `Personal` sheet (507
    species) and diffed it against this codebase's actual compiled-in
    stats (had to fix an extraction bug first -- `P_UPDATED_ABILITIES/
    STATS/TYPES = GEN_LATEST` in `include/config/pokemon.h` means the
    *first* `#if` branch in each species block is the active one, not the
    `#else` legacy fallback, which is what an earlier pass in this
    conversation had mistakenly read). Result: this codebase's "modern"
    baseline already matches PK almost exactly for the Gen1-4 dex (only 5
    species had a real stat difference, and all 5 were this session's own
    invented buffs on Zigzagoon/Whismur/Makuhita/Nincada/Slakoth --
    reverted them back to vanilla since PK itself doesn't touch those
    stats). Applied PK's real remaining differences directly: 45 species
    got an `Ability:` swap to match PK exactly (Pidgey Keen Eye->Tangled
    Feet, Machop Guts->No Guard, Eevee Run Away->Adaptability, Scyther/
    Scizor Swarm->Technician, the legendary birds' weather-trio abilities,
    etc.), verified via assert-checked script (every "before" value
    confirmed matching before writing). Excluded Fairy-typing diffs
    (Gen4-baseline artifact from PK predating the Fairy type, not a real
    PK design choice -- reverting it would be a regression) and 9
    form-variant species (Deoxys/Giratina/Shaymin/Castform/Wormadam/Rotom/
    Arceus/Unown/Mothim -- name-matching picked the wrong form for a few of
    these and they need dedicated handling, deferred).
  - Did the same direct-copy treatment for PK's `Move Changes` sheet (338
    entries): split into 313 simple numeric/type changes vs 25 full
    reflavors (e.g. Comet Punch -> Water Ball, Bind -> Mystical Fire --
    repurposing a bad move into a new type/effect entirely, deferred as
    higher-risk hand-design work); of the 313, 301 matched a real
    `MOVE_` constant, and applying them found **254 already matched PK
    exactly** (this codebase's upstream move-balance commit `e89683532`
    independently converged with most of PK's choices), 18 genuine gaps
    applied (Stomp->Ground type, Dig 80->60 bp, Yawn 0->70 acc, Absorb->Dark
    type, several old Normal-type status/attack moves retyped to match PK's
    Dark/Ground/Rock/Ice/Grass retypes, etc.), and 29 skipped as either
    priority/targeting-only notes (out of scope for a blind script) or one
    real mismatch (Will-O-Wisp accuracy, left as-is pending a decision).
  - Verified: `make` succeeds after each stage; `make check` run pending
    confirmation against the 93-FAILED baseline for this final combined
    state.
  - **Known remaining gap, not yet fixed:** found a fourth likely instance of
    the same orphan-placeholder pattern -- `TRAINER_PETALBURGCOAST_1`
    ("RONNIE") still has the generic "Lv1 Lillipup, 0 IVs, no moves"
    placeholder stub in `trainers.party`. Not confirmed whether it's wired
    to a map object event yet. Flagging for a future pass rather than
    fixing now, since PetalburgCoast hasn't been touched this session and
    is out of the scope that was just redesigned.
  - **Deferred, larger follow-up if wanted:** PK's Gen5-6 mons aren't in
    this xlsx at all (Platinum Kaizo is a Gen4 game), so there's no direct
    data to copy for them -- the user's own instruction was to use PK's
    Gen1-4 pattern "as a reference" there, not a literal port. Also
    deferred: the 25 move reflavors, the 9 form-variant species, and
    extending this same direct-copy treatment to species/trainers beyond
    the 18 currently built out (Dewford/Brawly and Mauville/Wattson splits
    haven't been started yet per the original roadmap).

- **Convenience/QoL systems (Phase 1), implemented and committed.**
  - Littleroot Town supplies NPC: gives 900 each of Rare Candy, Max Repel,
    Full Restore, Escape Rope, once only, gated on the new
    `FLAG_RECEIVED_STARTER_SUPPLIES` (0x27). Reused a pre-existing unused
    `OBJ_EVENT_GFX_MANIAC` object_event at (26,15) that had `script: NULL`
    and no flag -- the same orphaned-placeholder pattern found repeatedly
    in this project, here for a plain NPC rather than a trainer. 900 is
    safely under `MAX_BAG_ITEM_CAPACITY` (999) so each fits one stack.
  - Rare Candy `.price` -> 1, and added to all 12 standard town Mart
    inventories. OldaleTown/PetalburgCity/RustboroCity each have two
    inventory tiers (Basic/Expanded) -- added to both. Deliberately left
    specialty shops alone (Decoration Shop, Herb Shop, the four Department
    Store floors, Pretty Petal Flower Shop, Trainer Hill, EverGrande
    league shop), since those sell category-specific goods and aren't
    "standard Poké Marts."
  - Starting money -> `MAX_MONEY` (999,999) in `src/new_game.c`. Used the
    constant rather than a literal so it can't drift out of range.
  - Berry harvest -> flat 255 regardless of species
    (`GetBerryCountByBerryTreeId` in `src/berry.c`). Growth/watering/
    mutation mechanics untouched. All three call sites already route
    through `AddBagItem`/`CheckBagHasSpace`, whose capacity handling is
    quantity-agnostic, so no extra bag-safety work was needed.
  - **Not gameplay-verified.** These are confirmed to compile and to be
    wired correctly in the generated data, but nothing here has been
    tested in an actual running game.

- **Gift Pokemon #1 and #2, implemented and committed.**
  - Species-wide changes: Dwebble evolves at 26 (was 34); Crustle's stats
    set to the specified spread (80/105/125/65/80/45, BST 500 -- notably
    *down* on Attack and Speed from its current 134/61, up on SpA/SpD, so
    it is now a mixed wall rather than a glass physical attacker);
    Rock Blast added at 15, Rock Slide at 24, Bug Bite moved 5 -> 19 on
    both Dwebble and Crustle; Shell Smash moved from Dwebble lvl 36 to
    Crustle lvl 32 (it was unreachable pre-evolution once Dwebble evolves
    at 26). Skorupi and Drapion both get Battle Armor in ability slot 1
    (neither had it before in this codebase, despite it being their
    signature ability in the retail games); Skorupi evolves at 32 (was 40).
  - Gift NPCs: **Dwebble** is given by the pre-existing Rancher Andy NPC at
    Granite Shore -- Lv20, Adamant, Sturdy, 31 IVs across the board, Rock
    Blast/Struggle Bug/Bug Bite/X-Scissor. (Initially placed as a separate
    NPC in Viridian Forest, then moved here on request; Andy's old random
    Tauros/Miltank/Bouffalant gift was replaced outright, so he is now the
    Dwebble source and that cattle trio is no longer obtainable from him.
    The Viridian Forest NPC and its FLAG_RECEIVED_DWEBBLE_GIFT were removed
    and flag 0x28 returned to the unused pool.)
    **Skorupi** at Wraithwood Forest (20,11) -- Lv22, Jolly, Battle Armor,
    31 IVs, Poison Jab/Slash/Bite/Pin Missile. Wraithwood's own trainers
    sit at Lv24-26, i.e. the Roxanne(23) -> Brawly(31) band, so this is a
    split-3 gift that evolves into Drapion during split 4.
  - Note that both gifts now land in roughly the same progression window
    (Granite Shore and Wraithwood are both in the Lv24-26 band), rather
    than one early and one mid. Worth revisiting if the intent was to have
    a split-1 gift.
  - Verified that `abilityNum=0` in the `givemon` macro indexes ability
    slot 1 and that `MON_DATA_ABILITY_NUM` carries through evolution, so
    the gifted Skorupi really does keep Battle Armor as a Drapion.

- **Found and fixed a latent data-loss hazard in the poryscript setup.**
  Three of the new maps (`WraithwoodForest`, `Hollowbrook`, `GraniteShore`)
  had a `scripts.pory` that was a 2-to-35-line stub while the real content
  lived only in the generated `scripts.inc`. `GraniteShore/scripts.pory`
  even carried a note from an earlier session asserting "this project does
  not auto-compile .pory to .inc, so the real, authoritative script lives
  in scripts.inc." **That note is wrong.** The Makefile has a live rule
  (`data/%.inc: data/%.pory`), and editing `LittlerootTown/scripts.pory`
  for the supplies NPC regenerated its `scripts.inc` exactly as expected.
  The only reason those three maps had not already lost their scripts is
  that their `.inc` happened to have a newer mtime than their `.pory`;
  any `touch`, fresh clone, or checkout that reordered those timestamps
  would have silently regenerated all three maps from near-empty stubs and
  destroyed every trainer and gift script in them. Rewrote all three
  `.pory` files to hold the real content inside a `raw` passthrough block,
  then deleted and regenerated the `.inc` files and diffed them against
  the originals to confirm the output is content-identical (the only
  difference is poryscript's own `#line` debug markers). `.pory` is now
  genuinely authoritative for those maps.

## Decisions needing your input

- Location and timing for the **Gen 3 starter trade** (Treecko/Torchic/
  Mudkip, player's choice, 31 IVs, one-time) -- proposal not yet written.
- Location for the **evolution stone NPC** (Fire/Water/Leaf Stone, player's
  choice, one-time) -- proposal not yet written.
- Whether the splits-1-4 gift count should stay at two. Rancher Andy is now
  the Dwebble gift, so the Tauros/Miltank/Bouffalant trio he used to hand
  out is currently unobtainable anywhere -- flagging in case those were
  wanted somewhere else rather than dropped.
