# Documentation generator

Generates `development_reports/BOFA_Documentation.xlsx` directly from this
project's own source files.

## Running it

```sh
python3 tools/docgen/generate_workbook.py
```

Optional: `-o path/to/output.xlsx`.

Requires `openpyxl` (already present in this environment).

Run it after any change to game data and commit the regenerated workbook
alongside the change. It takes a few seconds.

## What it reads

| Sheet | Source of truth |
|---|---|
| Pokemon Stats, Ability Changes, Evolution Changes | `src/data/pokemon/species_info/gen_*_families.h` |
| Level-Up Learnsets | `src/data/pokemon/level_up_learnsets/gen_9.h` |
| Move Changes | `src/data/moves_info.h` |
| Split 1-8 / Elite Four / Postgame / Unassigned, Boss Battles | `src/data/trainers.party` + `data/maps/*/scripts.inc` for locations |
| Progression | `setflag FLAG_BADGE0N_GET` scan + `include/config/caps.h` |
| Wild Encounters, Pokemon Availability | `src/data/wild_encounters.json` |
| Item Locations, TM & HM Locations | `data/maps/*/map.json` object_events + bg_events, and parsed `pokemart` lists |
| Gifts & Trades | `givemon` calls in map scripts + `src/data/trade.h` |

## "Original" vs "New" columns

`BASELINE_COMMIT` in `extract.py` is the last upstream (RHH pokeemerald-expansion)
commit before this project's own history begins. Every "Orig" column is that
file as it existed at that commit, read via `git show`, and parsed with the same
parser as the current file. So "changed" always means *changed by this project*,
not "differs from retail Pokémon".

Two parsing details worth knowing, because getting them wrong silently produces
wrong numbers:

- Some stats are written as config ternaries, e.g.
  `.baseAttack = P_UPDATED_STATS >= GEN_7 ? 105 : 95`. The extractor evaluates
  the condition against the config value *from the same revision* rather than
  taking the first integer it sees.
- Some stats are shared constants, e.g. `.baseSpeed = RAICHU_SPEED`. Those are
  resolved through a `#define` table built from the species files.
- Only `level_up_learnsets/gen_9.h` is parsed, because that is the only table
  actually compiled — `src/pokemon.c` selects it through an `#elif` chain on
  `P_LVL_UP_LEARNSETS` (currently `GEN_LATEST`). The `gen_1`–`gen_8` files
  define the same symbol names but are not built.

## Status labels

- `IMPLEMENTED` — read from a source file.
- `UNVERIFIED` — the file could not give a definite answer. Notably every
  trainer's "Mandatory?" column, because whether a trainer's sight range forces
  the battle is not knowable from data files alone.
- `NOT IMPLEMENTED` — confirmed absent (e.g. no gym sets `FLAG_BADGE05_GET`).
- `PLANNED` — reserved; the generator does not read design documents, so it
  never emits this on its own.

Nothing in the workbook is copied from design docs or roadmaps. If a feature is
only planned, it does not appear as implemented.

## Manual content is preserved

`Balance Issues` and `Change Log` are read back out of the existing workbook
before regeneration, and any rows you added are re-emitted. On `Balance Issues`,
rows whose ID starts with `AUTO-` are regenerated from the project each run;
your own rows are kept as-is. Add your notes freely — regenerating will not
erase them.

## Adding a sheet

Add an extractor to `extract.py` (it must read a real file and return `None` /
`"UNVERIFIED"` when it cannot determine a value), then a `sheet_*` function in
`generate_workbook.py` and a `TAB_COLORS` entry. Keep the rule that nothing
about the game is hardcoded in either file.
