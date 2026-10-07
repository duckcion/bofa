"""Extract game data from the bofa project's authoritative source files.

Every function here reads real project files. Nothing is hardcoded game data.
Where a value cannot be determined from the files, the extractor records None /
"UNVERIFIED" rather than guessing.

`baseline_*` variants read the same file as it existed at the upstream commit
before this project's own work began, so the workbook can show original vs
modified values.
"""

import json
import os
import re
import subprocess
import glob
from functools import lru_cache

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))

# Last upstream (RHH pokeemerald-expansion) commit before this project's own
# commits begin. Verified via: git log --author=duckcion --reverse, then walking
# back to the last commit authored by an upstream contributor.
BASELINE_COMMIT = "9a0b5756c"

SPECIES_FILES = [f"src/data/pokemon/species_info/gen_{g}_families.h" for g in range(1, 10)]
MOVES_FILE = "src/data/moves_info.h"
# Only this learnset table is compiled; see the #elif chain in src/pokemon.c
# gated on P_LVL_UP_LEARNSETS (= GEN_LATEST).
LEARNSET_FILE = "src/data/pokemon/level_up_learnsets/gen_9.h"
TRAINERS_FILE = "src/data/trainers.party"
ITEMS_FILE = "src/data/items.h"
ENCOUNTERS_FILE = "src/data/wild_encounters.json"
TRADES_FILE = "src/data/trade.h"


def repo_path(rel):
    return os.path.join(REPO, rel)


def read_current(rel):
    p = repo_path(rel)
    if not os.path.exists(p):
        return None
    with open(p, "r", encoding="utf-8", errors="replace") as f:
        return f.read()


@lru_cache(maxsize=None)
def read_baseline(rel, commit=BASELINE_COMMIT):
    """File contents at the upstream baseline, or None if absent there."""
    try:
        out = subprocess.run(
            ["git", "show", f"{commit}:{rel}"],
            cwd=REPO, capture_output=True, check=True,
        )
        return out.stdout.decode("utf-8", errors="replace")
    except subprocess.CalledProcessError:
        return None


# ---------------------------------------------------------------- species ----

def _iter_species_blocks(text):
    """Yield (SPECIES_CONST, block_text) for each struct literal entry."""
    for m in re.finditer(r"\[(SPECIES_[A-Z0-9_]+)\]\s*=\s*\n\s*\{", text):
        start = m.end()
        depth, i = 1, start
        while depth > 0 and i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
            i += 1
        yield m.group(1), text[start:i]


GEN_NUM = {f"GEN_{i}": i for i in range(1, 10)}
GEN_NUM["GEN_LATEST"] = 99


def _gen_value(token):
    token = token.strip()
    if token in GEN_NUM:
        return GEN_NUM[token]
    if token.isdigit():
        return int(token)
    return None


@lru_cache(maxsize=None)
def _config_macro(macro, revision):
    """Resolve a P_* config macro to a gen number, for the given revision.

    revision=None means the working tree.
    """
    for rel in ("include/config/pokemon.h", "include/config/battle.h",
                "include/config/item.h", "include/config/general.h"):
        text = read_current(rel) if revision is None else read_baseline(rel, revision)
        if not text:
            continue
        m = re.search(r"#define\s+" + re.escape(macro) + r"\s+([A-Za-z0-9_]+)", text)
        if m:
            return _gen_value(m.group(1))
    return None


@lru_cache(maxsize=None)
def _local_macros(revision):
    """`#define NAME <expr>` table from the species_info files.

    Regional forms reuse shared constants, e.g.
    `.baseSpeed = RAICHU_SPEED`, so a bare identifier has to be resolved
    before the stat can be reported.
    """
    table = {}
    for rel in SPECIES_FILES:
        text = read_current(rel) if revision is None else read_baseline(rel, revision)
        if not text:
            continue
        for m in re.finditer(r"^#define\s+([A-Za-z_][A-Za-z0-9_]*)\s+([^\n/]+)", text, re.M):
            table.setdefault(m.group(1), m.group(2).strip())
    return table


def _eval_expr(expr, revision, depth=0):
    expr = expr.strip().rstrip(",").strip()
    if depth > 6:
        return None
    if re.fullmatch(r"\d+", expr):
        return int(expr)
    tern = re.match(
        r"\(?\s*([A-Z_][A-Z0-9_]*)\s*>=\s*([A-Za-z0-9_]+)\s*\)?\s*\?\s*([^:]+):\s*(.+)$",
        expr,
    )
    if tern:
        macro, gen_tok, a, b = tern.groups()
        left = _config_macro(macro, revision)
        right = _gen_value(gen_tok)
        chosen = a if (left is None or right is None or left >= right) else b
        return _eval_expr(chosen, revision, depth + 1)
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", expr):
        sub = _local_macros(revision).get(expr)
        if sub is not None and sub != expr:
            return _eval_expr(sub, revision, depth + 1)
        return None
    m2 = re.search(r"\b(\d+)\b", expr)
    return int(m2.group(1)) if m2 else None


def _resolve_int(block, field, revision=None):
    """Read an int field, resolving config ternaries and named macros.

    Upstream writes some stats as `P_UPDATED_STATS >= GEN_7 ? 105 : 95` and
    some as a shared constant like `RAICHU_SPEED`. Taking the first integer in
    the line would read the wrong branch in the first case and nothing at all
    in the second.
    """
    m = re.search(r"\." + field + r"\s*=\s*([^,\n]+)", block)
    if not m:
        return None
    return _eval_expr(m.group(1), revision)


def _first_abilities(block):
    """First .abilities occurrence = the active branch.

    P_UPDATED_ABILITIES is GEN_LATEST in include/config/pokemon.h, so the
    leading #if branch compiles and any #else fallback does not.
    """
    m = re.search(r"\.abilities\s*=\s*\{([^}]*)\}", block)
    if not m:
        return []
    return [a.strip() for a in m.group(1).split(",") if a.strip()]


def _types(block):
    m = re.search(r"\.types\s*=\s*MON_TYPES\(([^)]*)\)", block)
    if not m:
        return []
    return [t.strip() for t in m.group(1).split(",") if t.strip()]


def _evolutions(block):
    m = re.search(r"\.evolutions\s*=\s*EVOLUTION\((.*?)\)\s*,\s*\n", block, re.S)
    if not m:
        return []
    inner = m.group(1)
    return [tuple(p.strip() for p in t.split(",")) for t in re.findall(r"\{([^}]*)\}", inner)]


def parse_species(text, revision=None):
    out = {}
    if not text:
        return out
    for const, block in _iter_species_blocks(text):
        name_m = re.search(r'\.speciesName\s*=\s*_\("([^"]+)"\)', block)
        dex_m = re.search(r"\.natDexNum\s*=\s*(NATIONAL_DEX_[A-Z0-9_]+)", block)
        ls_m = re.search(r"\.levelUpLearnset\s*=\s*(\w+)", block)
        out[const] = {
            "const": const,
            "name": name_m.group(1) if name_m else None,
            "natDexNum": dex_m.group(1) if dex_m else None,
            "types": _types(block),
            "abilities": _first_abilities(block),
            "evolutions": _evolutions(block),
            "learnsetSymbol": ls_m.group(1) if ls_m else None,
            "baseHP": _resolve_int(block, "baseHP", revision),
            "baseAttack": _resolve_int(block, "baseAttack", revision),
            "baseDefense": _resolve_int(block, "baseDefense", revision),
            "baseSpAttack": _resolve_int(block, "baseSpAttack", revision),
            "baseSpDefense": _resolve_int(block, "baseSpDefense", revision),
            "baseSpeed": _resolve_int(block, "baseSpeed", revision),
        }
    return out


def species_current():
    merged = {}
    for rel in SPECIES_FILES:
        merged.update(parse_species(read_current(rel)))
    return merged


def species_baseline():
    merged = {}
    for rel in SPECIES_FILES:
        merged.update(parse_species(read_baseline(rel), BASELINE_COMMIT))
    return merged


STAT_KEYS = ["baseHP", "baseAttack", "baseDefense", "baseSpAttack", "baseSpDefense", "baseSpeed"]


def bst(entry):
    vals = [entry.get(k) for k in STAT_KEYS]
    return sum(v for v in vals if isinstance(v, int)) if all(isinstance(v, int) for v in vals) else None


# ------------------------------------------------------------------ moves ----

MOVE_FIELDS = {
    "power": r"\.power\s*=\s*(-?\d+)",
    "accuracy": r"\.accuracy\s*=\s*(\d+)",
    "pp": r"\.pp\s*=\s*(\d+)",
    "type": r"\.type\s*=\s*(TYPE_[A-Z_]+)",
    "category": r"\.category\s*=\s*(DAMAGE_CATEGORY_[A-Z_]+)",
    "effect": r"\.effect\s*=\s*(EFFECT_[A-Z0-9_]+)",
    "target": r"\.target\s*=\s*(MOVE_TARGET_[A-Z_]+)",
    "priority": r"\.priority\s*=\s*(-?\d+)",
    "recoil": r"\.recoil\s*=\s*(\d+)",
    "crit": r"\.criticalHitStage\s*=\s*(\d+)",
}


def parse_moves(text, revision=None):
    out = {}
    if not text:
        return out
    for m in re.finditer(r"\[(MOVE_[A-Z0-9_]+)\]\s*=\s*\n\s*\{", text):
        start = m.end()
        depth, i = 1, start
        while depth > 0 and i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
            i += 1
        block = text[start:i]
        name_m = re.search(r'\.name\s*=\s*COMPOUND_STRING\("([^"]+)"\)', block)
        entry = {"const": m.group(1), "name": name_m.group(1) if name_m else None}
        for key, pat in MOVE_FIELDS.items():
            if key in ("power", "accuracy", "pp"):
                entry[key] = _resolve_int(block, key, revision)
            else:
                mm = re.search(pat, block)
                entry[key] = mm.group(1) if mm else None
        chances = re.findall(r"\.chance\s*=\s*(\d+)", block)
        entry["secondaryChance"] = ", ".join(chances) if chances else None
        effs = re.findall(r"\.moveEffect\s*=\s*(MOVE_EFFECT_[A-Z0-9_]+)", block)
        dm = re.search(r"\.description\s*=\s*COMPOUND_STRING\((.*?)\),", block, re.S)
        entry["description"] = " ".join(re.findall(r'"([^"]*)"', dm.group(1))).replace("\\n", " ").replace("  ", " ") if dm else None
        entry["secondaryEffect"] = ", ".join(effs) if effs else None
        out[m.group(1)] = entry
    return out


def moves_current():
    return parse_moves(read_current(MOVES_FILE))


def moves_baseline():
    return parse_moves(read_baseline(MOVES_FILE), BASELINE_COMMIT)


# --------------------------------------------------------------- learnsets ---

def parse_learnsets(text):
    """symbol -> [(level, MOVE_CONST), ...]"""
    out = {}
    if not text:
        return out
    for m in re.finditer(
        r"static const struct LevelUpMove (\w+)\[\] = \{(.*?)\n\};", text, re.S
    ):
        sym, body = m.group(1), m.group(2)
        moves = [(int(lv), mv) for lv, mv in re.findall(r"LEVEL_UP_MOVE\(\s*(\d+),\s*(MOVE_[A-Z0-9_]+)\)", body)]
        out[sym] = moves
    return out


def learnsets_current():
    return parse_learnsets(read_current(LEARNSET_FILE))


def learnsets_baseline():
    return parse_learnsets(read_baseline(LEARNSET_FILE))


# ---------------------------------------------------------------- trainers ---

TRAINER_HEADER_KEYS = (
    "name", "class", "pic", "gender", "music", "items", "double battle",
    "ai", "mugshot", "starting status",
)


def parse_trainers(text):
    """Parse trainers.party into {TRAINER_CONST: {...header..., 'mons': [...]}}"""
    out = {}
    if not text:
        return out
    text = text.replace("\r\n", "\n")
    for m in re.finditer(r"=== (TRAINER_\S+) ===\n(.*?)(?=\n=== |\Z)", text, re.S):
        const, body = m.group(1), m.group(2)
        chunks = [c for c in re.split(r"\n\s*\n", body.strip("\n")) if c.strip()]
        header_lines = chunks[0].split("\n") if chunks else []
        header = {}
        for line in header_lines:
            if ":" in line:
                k, v = line.split(":", 1)
                if k.strip().lower() in TRAINER_HEADER_KEYS:
                    header[k.strip().lower()] = v.strip()
        mons = []
        for chunk in chunks[1:]:
            lines = [l for l in chunk.split("\n") if l.strip()]
            if not lines:
                continue
            mon = {"species": None, "item": None, "level": None, "ability": None,
                   "nature": None, "ivs": None, "evs": None, "moves": [], "gender": None}
            head = lines[0].strip()
            if "@" in head:
                left, right = head.split("@", 1)
                mon["item"] = right.strip()
            else:
                left = head
            left = left.strip()
            gm = re.search(r"\((M|F)\)", left)
            if gm:
                mon["gender"] = gm.group(1)
                left = left.replace(gm.group(0), "").strip()
            pm = re.search(r"\(([^)]+)\)", left)
            if pm:
                mon["species"] = pm.group(1).strip()
            else:
                mon["species"] = left
            for line in lines[1:]:
                s = line.strip()
                if s.startswith("- "):
                    mon["moves"].append(s[2:].strip())
                    continue
                if ":" not in s:
                    continue
                k, v = s.split(":", 1)
                k, v = k.strip().lower(), v.strip()
                if k == "level":
                    mon["level"] = int(v) if v.isdigit() else v
                elif k in ("ability", "nature", "ivs", "evs"):
                    mon[k] = v
            mons.append(mon)
        entry = dict(header)
        entry["const"] = const
        entry["mons"] = mons
        out[const] = entry
    return out


def trainers_current():
    return parse_trainers(read_current(TRAINERS_FILE))


PLACEHOLDER_SPECIES = "Lillipup"


def is_placeholder_team(entry):
    """The project's reserved-but-undesigned stub: a single Lv1 Lillipup, 0 IVs."""
    mons = entry.get("mons") or []
    if len(mons) != 1:
        return False
    m = mons[0]
    return (
        str(m.get("species")) == PLACEHOLDER_SPECIES
        and m.get("level") == 1
        and not m.get("moves")
    )


def trainer_locations():
    """TRAINER_CONST -> sorted list of map names referencing it in a script."""
    loc = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "scripts.inc")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        for const in set(re.findall(r"\b(TRAINER_[A-Z0-9_]+)\b", text)):
            loc.setdefault(const, set()).add(mapname)
    return {k: sorted(v) for k, v in loc.items()}


# -------------------------------------------------------------- encounters ---

def wild_encounters():
    """Flat list of encounter rows from src/data/wild_encounters.json."""
    raw = read_current(ENCOUNTERS_FILE)
    if not raw:
        return []
    data = json.loads(raw)
    rows = []
    for group in data.get("wild_encounter_groups", []):
        group_label = group.get("label")
        for enc in group.get("encounters", []):
            mapname = enc.get("map")
            for field, payload in enc.items():
                if not isinstance(payload, dict) or "mons" not in payload:
                    continue
                rate = payload.get("encounter_rate")
                for slot, mon in enumerate(payload["mons"]):
                    rows.append({
                        "group": group_label,
                        "map": mapname,
                        "method": field,
                        "encounter_rate": rate,
                        "slot": slot,
                        "species": mon.get("species"),
                        "min_level": mon.get("min_level"),
                        "max_level": mon.get("max_level"),
                    })
    return rows


# ------------------------------------------------------ overworld item data ---

def overworld_items():
    """Item balls (object_events) and hidden items (bg_events) across all maps."""
    rows = []
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "map.json")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                data = json.load(f)
        except (OSError, json.JSONDecodeError):
            continue
        for o in data.get("object_events") or []:
            if o.get("graphics_id") == "OBJ_EVENT_GFX_ITEM_BALL":
                rows.append({
                    "map": mapname, "kind": "Item Ball (visible)",
                    "item": o.get("trainer_sight_or_berry_tree_id"),
                    "x": o.get("x"), "y": o.get("y"), "flag": o.get("flag"),
                })
        for b in data.get("bg_events") or []:
            if b.get("type") == "hidden_item":
                rows.append({
                    "map": mapname, "kind": "Hidden",
                    "item": b.get("item"),
                    "x": b.get("x"), "y": b.get("y"), "flag": b.get("flag"),
                })
    return rows


def mart_inventories():
    """map -> list of ITEM_ constants sold, parsed from pokemart lists."""
    out = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "scripts.inc")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        for block in re.findall(r"\.align 2\n\w+:\n((?:\s*\.2byte ITEM_[A-Z0-9_]+\n)+)\s*pokemartlistend", text):
            items = re.findall(r"ITEM_[A-Z0-9_]+", block)
            out.setdefault(mapname, []).extend(items)
    return out


# ---------------------------------------------------------- gifts / trades ---

def script_gift_mons():
    """givemon calls found in map scripts: the project's gift Pokemon."""
    rows = []
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "scripts.inc")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        for line in text.split("\n"):
            s = line.strip()
            if not s.startswith("givemon "):
                continue
            args = s[len("givemon "):]
            parts = [p.strip() for p in args.split(",")]
            row = {"map": mapname, "species": parts[0] if parts else None,
                   "level": parts[1] if len(parts) > 1 else None, "raw": args}
            for p in parts[2:]:
                if "=" in p:
                    k, v = p.split("=", 1)
                    row[k.strip()] = v.strip()
            rows.append(row)
    return rows


def ingame_trades():
    """Entries from src/data/trade.h."""
    text = read_current(TRADES_FILE)
    rows = []
    if not text:
        return rows
    for m in re.finditer(r"\[(INGAME_TRADE_[A-Z0-9_]+)\]\s*=\s*\n\s*\{(.*?)\n\s*\}", text, re.S):
        const, block = m.group(1), m.group(2)
        def g(pat, cast=str):
            mm = re.search(pat, block)
            return cast(mm.group(1)) if mm else None
        ivs = re.search(r"\.ivs\s*=\s*\{([^}]*)\}", block)
        rows.append({
            "const": const,
            "nickname": g(r'\.nickname\s*=\s*_\("([^"]*)"\)'),
            "species": g(r"\.species\s*=\s*(SPECIES_[A-Z0-9_]+)"),
            "ivs": ", ".join(p.strip() for p in ivs.group(1).split(",")) if ivs else None,
            "abilityNum": g(r"\.abilityNum\s*=\s*(\d+)"),
            "heldItem": g(r"\.heldItem\s*=\s*(ITEM_[A-Z0-9_]+)"),
            "requestedSpecies": g(r"\.requestedSpecies\s*=\s*(SPECIES_[A-Z0-9_]+)"),
            "otName": g(r'\.otName\s*=\s*_\("([^"]*)"\)'),
            "level": g(r"\.level\s*=\s*(\d+)"),
            "moves": [m.replace("_", " ").title() for m in re.findall(r"MOVE_(\w+)", (re.search(r"\.moves\s*=\s*\{([^}]*)\}", block) or [None, ""])[1])],
            "personality": g(r"\.personality\s*=\s*(0x[0-9A-Fa-f]+|\d+)", lambda v: int(v, 0)),
        })
    return rows


def trade_script_locations():
    """map -> set of INGAME_TRADE_ constants referenced by its scripts."""
    out = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "scripts.inc")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        found = set(re.findall(r"\b(INGAME_TRADE_[A-Z0-9_]+)\b", text))
        if found:
            out[mapname] = sorted(found)
    return out


# ---------------------------------------------------------------- progression -

def badge_flag_sources():
    """badge flag -> [maps whose scripts setflag it]. Derived, not assumed."""
    out = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "scripts.inc")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        for flag in re.findall(r"setflag (FLAG_BADGE0\d_GET)", text):
            out.setdefault(flag, set()).add(mapname)
    return {k: sorted(v) for k, v in out.items()}


def map_connections():
    """map -> {'connections': [...], 'warps': [...]} from map.json."""
    out = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "map.json")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                data = json.load(f)
        except (OSError, json.JSONDecodeError):
            continue
        conns = []
        raw_conns = data.get("connections")
        if isinstance(raw_conns, list):
            for c in raw_conns:
                conns.append({"direction": c.get("direction"), "map": c.get("map")})
        warps = []
        for w in data.get("warp_events") or []:
            warps.append({"dest": w.get("dest_map"), "x": w.get("x"), "y": w.get("y")})
        out[mapname] = {"id": data.get("id"), "connections": conns, "warps": warps}
    return out


# --------------------------------------------------------------------- items --

def parse_items():
    text = read_current(ITEMS_FILE)
    out = {}
    if not text:
        return out
    for m in re.finditer(r"\[(ITEM_[A-Z0-9_]+)\]\s*=\s*\n\s*\{", text):
        start = m.end()
        depth, i = 1, start
        while depth > 0 and i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
            i += 1
        block = text[start:i]
        name_m = re.search(r'\.name\s*=\s*_\("([^"]*)"\)', block)
        price_m = re.search(r"\.price\s*=\s*([^,\n]+)", block)
        pocket_m = re.search(r"\.pocket\s*=\s*(POCKET_[A-Z_]+)", block)
        sec_m = re.search(r"\.secondaryId\s*=\s*(MOVE_[A-Z0-9_]+)", block)
        out[m.group(1)] = {
            "const": m.group(1),
            "name": name_m.group(1) if name_m else None,
            "price": price_m.group(1).strip() if price_m else None,
            "pocket": pocket_m.group(1) if pocket_m else None,
            "teachesMove": sec_m.group(1) if sec_m else None,
        }
    return out


def config_value(rel, macro):
    """Read a #define's value from a config header, or None."""
    text = read_current(rel)
    if not text:
        return None
    m = re.search(r"#define\s+" + re.escape(macro) + r"\s+([^/\n]+)", text)
    return m.group(1).strip() if m else None


def grep_literal(rel, pattern):
    """Return list of matching lines from a source file (for verification)."""
    text = read_current(rel)
    if not text:
        return []
    return [l.strip() for l in text.split("\n") if re.search(pattern, l)]


# ------------------------------------------------------------ national dex ---

@lru_cache(maxsize=None)
def national_dex_numbers():
    """NATIONAL_DEX_X -> integer, by walking the enum in include/constants/pokedex.h.

    The enum uses implicit sequential values (NATIONAL_DEX_NONE = 0), so order
    of appearance is the number. Explicit `= N` assignments reset the counter.
    """
    text = read_current("include/constants/pokedex.h")
    if not text:
        return {}
    out, counter = {}, 0
    in_enum = False
    for line in text.split("\n"):
        s = line.strip()
        if not in_enum:
            if re.match(r"enum\b", s) and "NATIONAL_DEX" not in s:
                in_enum = True
            elif s.startswith("NATIONAL_DEX_"):
                in_enum = True
            else:
                continue
        if s.startswith("}"):
            if out:
                break
            in_enum = False
            continue
        m = re.match(r"(NATIONAL_DEX_[A-Z0-9_]+)\s*(?:=\s*(\d+))?\s*,?", s)
        if m:
            name, explicit = m.group(1), m.group(2)
            if explicit is not None:
                counter = int(explicit)
            out[name] = counter
            counter += 1
    return out


# ------------------------------------------------- trainer forcing behaviour ---

@lru_cache(maxsize=None)
def script_label_to_trainers():
    """Script label -> tuple of TRAINER_ constants that label starts a battle with.

    Object events reference a script *label*, not a trainer constant, so the two
    have to be joined through the script body to tell which physical NPC
    corresponds to which trainer entry.
    """
    out = {}
    bodies = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "scripts.inc")):
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        text = text.replace("\r\n", "\n")
        # split on top-level labels
        chunks = re.split(r"\n(?=[A-Za-z_][A-Za-z0-9_]*::)", text)
        for chunk in chunks:
            m = re.match(r"([A-Za-z_][A-Za-z0-9_]*)::", chunk)
            if not m:
                continue
            label = m.group(1)
            consts = tuple(dict.fromkeys(re.findall(r"trainerbattle\w*\s+(TRAINER_[A-Z0-9_]+)", chunk)))
            refs = tuple(dict.fromkeys(
                re.findall(r"(?:goto|call)(?:_if_\w+)?\s+(?:[A-Z_]+,\s*)?([A-Za-z_][A-Za-z0-9_]*)", chunk)))
            bodies[label] = (consts, refs)

    # Gym leaders and story battles sit behind goto/call chains rather than
    # calling trainerbattle in the object's own script, so follow those edges.
    def resolve(label, seen):
        if label in seen or label not in bodies:
            return ()
        seen.add(label)
        consts, refs = bodies[label]
        found = list(consts)
        for r in refs:
            found.extend(resolve(r, seen))
        return tuple(dict.fromkeys(found))

    for label in bodies:
        got = resolve(label, set())
        if got:
            out[label] = got
    return out


def trainer_forcing():
    """TRAINER_ const -> list of placement dicts describing how it engages.

    Derived from each object event's trainer_type and trainer_sight_or_berry_tree_id,
    which src/trainer_see.c uses as the approach distance. A range of 0 means the
    NPC never initiates, so the battle is opt-in; a range >= 1 means it challenges
    the player on sight along its facing direction.

    Whether the player can physically route around that line of sight is a
    collision question this does not attempt to answer.
    """
    label_map = script_label_to_trainers()
    out = {}
    for path in glob.glob(os.path.join(REPO, "data", "maps", "*", "map.json")):
        mapname = os.path.basename(os.path.dirname(path))
        try:
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                data = json.load(f)
        except (OSError, json.JSONDecodeError):
            continue
        for o in data.get("object_events") or []:
            ttype = o.get("trainer_type") or "TRAINER_TYPE_NONE"
            script = o.get("script") or ""
            consts = label_map.get(script, ())
            if not consts:
                continue
            raw = o.get("trainer_sight_or_berry_tree_id")
            try:
                rng = int(str(raw))
            except (TypeError, ValueError):
                rng = None
            for const in consts:
                out.setdefault(const, []).append({
                    "map": mapname,
                    "trainer_type": ttype,
                    "sight_range": rng,
                    "script": script,
                    "x": o.get("x"), "y": o.get("y"),
                })
    return out


def classify_engagement(placements):
    """Human-readable mandatory/optional verdict for a trainer's placements."""
    if not placements:
        return "UNKNOWN (no object event found)"
    verdicts = set()
    for p in placements:
        if p["trainer_type"] == "TRAINER_TYPE_NONE":
            verdicts.add("Opt-in (not a sight trainer)")
        elif p["sight_range"] in (0, None):
            verdicts.add("Opt-in (sight range 0)")
        else:
            verdicts.add(f"Forces on sight (range {p['sight_range']})")
    return "; ".join(sorted(verdicts))
