"""Generate the bofa documentation workbook (.xlsx) from project source files.

Usage:
    python3 tools/docgen/generate_workbook.py [-o OUT.xlsx]

Everything is read from the project's own files via tools/docgen/extract.py.
Nothing about the game is hardcoded here. Values that cannot be determined from
the files are written as UNVERIFIED rather than guessed.

Manual content is preserved across regeneration: any rows you add to the
"Balance Issues" or "Change Log" sheets are read back out of the existing
workbook and re-emitted, so regenerating never destroys your notes.
"""

import argparse
import datetime
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import extract as E  # noqa: E402

from openpyxl import Workbook, load_workbook  # noqa: E402
from openpyxl.styles import Font, PatternFill, Alignment, Border, Side  # noqa: E402
from openpyxl.utils import get_column_letter  # noqa: E402

OUT_DEFAULT = os.path.join(E.REPO, "development_reports", "BOFA_Documentation.xlsx")

# ---------------------------------------------------------------- styling ----

HDR_FILL = PatternFill("solid", fgColor="1F3864")
HDR_FONT = Font(bold=True, color="FFFFFF", size=11)
TITLE_FONT = Font(bold=True, size=14, color="1F3864")
CHANGED_FILL = PatternFill("solid", fgColor="FFF2CC")   # project-modified row
ADDED_FILL = PatternFill("solid", fgColor="E2EFDA")     # new content
WARN_FILL = PatternFill("solid", fgColor="FCE4D6")      # needs attention
BAD_FILL = PatternFill("solid", fgColor="F8CBAD")       # problem
LINK_FONT = Font(color="0563C1", underline="single")
THIN = Side(style="thin", color="BFBFBF")
BORDER = Border(left=THIN, right=THIN, top=THIN, bottom=THIN)

TAB_COLORS = {
    "Dashboard": "1F3864",
    "Progression": "2E75B6",
    "Boss Battles": "C00000",
    "Balance Issues": "ED7D31",
    "Change Log": "7F7F7F",
    "Pokemon Stats": "548235",
    "Ability Changes": "548235",
    "Level-Up Learnsets": "548235",
    "Evolution Changes": "548235",
    "Move Changes": "548235",
    "Wild Encounters": "7030A0",
    "Pokemon Availability": "7030A0",
    "Item Locations": "BF8F00",
    "TM & HM Locations": "BF8F00",
    "Gifts & Trades": "7030A0",
}
SPLIT_TAB_COLOR = "4472C4"

STATUS_IMPLEMENTED = "IMPLEMENTED"
STATUS_UNVERIFIED = "UNVERIFIED"
STATUS_NOT_IMPL = "NOT IMPLEMENTED"
STATUS_PLANNED = "PLANNED"


def pretty_const(value, prefix):
    """SPECIES_MR_MIME -> 'Mr Mime'; keeps unknown input readable."""
    if value is None:
        return ""
    s = str(value)
    if prefix and s.startswith(prefix):
        s = s[len(prefix):]
    return s.replace("_", " ").title()


def fmt_types(types):
    return " / ".join(pretty_const(t, "TYPE_") for t in types) if types else ""


def fmt_abilities(abils):
    shown = [pretty_const(a, "ABILITY_") for a in abils if a and a != "ABILITY_NONE"]
    return " / ".join(shown)


def fmt_evolutions(evos):
    if not evos:
        return "None"
    parts = []
    for evo in evos:
        if len(evo) >= 3:
            method, param, target = evo[0], evo[1], evo[2]
            method_s = pretty_const(method, "EVO_")
            target_s = pretty_const(target, "SPECIES_")
            if method in ("EVO_LEVEL",) and str(param).isdigit():
                parts.append(f"Lv {param} -> {target_s}")
            elif str(param).startswith("ITEM_"):
                parts.append(f"{method_s} ({pretty_const(param, 'ITEM_')}) -> {target_s}")
            elif str(param) == "0":
                parts.append(f"{method_s} -> {target_s}")
            else:
                parts.append(f"{method_s} {param} -> {target_s}")
    return "; ".join(parts)


def write_sheet(ws, title, headers, rows, widths=None, tab_color=None,
                freeze="A3", note=None, row_fills=None):
    ws.sheet_properties.tabColor = tab_color or "808080"
    ws["A1"] = title
    ws["A1"].font = TITLE_FONT
    if note:
        ws["A2"] = note
        ws["A2"].font = Font(italic=True, size=9, color="595959")
        header_row = 3
    else:
        header_row = 2
    for c, h in enumerate(headers, start=1):
        cell = ws.cell(row=header_row, column=c, value=h)
        cell.fill = HDR_FILL
        cell.font = HDR_FONT
        cell.alignment = Alignment(vertical="center", wrap_text=True)
        cell.border = BORDER
    for r, row in enumerate(rows, start=header_row + 1):
        fill = row_fills.get(r - header_row - 1) if row_fills else None
        for c, val in enumerate(row, start=1):
            cell = ws.cell(row=r, column=c, value=val)
            cell.border = BORDER
            cell.alignment = Alignment(vertical="top", wrap_text=False)
            if fill:
                cell.fill = fill
    if rows:
        ws.auto_filter.ref = (
            f"A{header_row}:{get_column_letter(len(headers))}{header_row + len(rows)}"
        )
    ws.freeze_panes = ws[f"A{header_row + 1}"]
    if widths:
        for i, w in enumerate(widths, start=1):
            ws.column_dimensions[get_column_letter(i)].width = w
    else:
        for i in range(1, len(headers) + 1):
            ws.column_dimensions[get_column_letter(i)].width = 18


# ------------------------------------------------------- split assignment ----

def build_split_model():
    """Derive gym -> badge from scripts; return split definitions.

    The badge each gym awards is read from `setflag FLAG_BADGE0N_GET` in that
    gym's own script, so a renumbered gym order is picked up automatically
    rather than assumed.
    """
    badge_sources = E.badge_flag_sources()
    gym_by_badge = {}
    for flag, maps in badge_sources.items():
        n = int(re.search(r"BADGE0(\d)", flag).group(1))
        gym_by_badge.setdefault(n, []).extend(maps)
    return badge_sources, gym_by_badge


# Maps grouped by the split a player reaches them in. Splits 1-4 are the region
# this project has actually built out and were traced from map.json connections
# and warps. Later-split entries are the vanilla gym towns for the remaining
# badges. Any map not listed lands in "Unassigned" and is reported UNVERIFIED
# rather than guessed into a split.
SPLIT_MAPS = {
    1: ["LittlerootTown", "Route101", "OldaleTown", "Route103", "ViridianForest",
        "Route102", "PetalburgCity", "PetalburgCity_Gym", "PetalburgCoast"],
    2: ["Route104", "PetalburgWoods", "RustboroCity", "RustboroCity_Gym",
        "Route104_PrettyPetalFlowerShop", "Route116", "RusturfTunnel"],
    3: ["HollowbrookShack_MiningTunnel", "Hollowbrook", "WraithwoodForest",
        "GraniteCave_B2F", "GraniteCave_B1F", "GraniteCave_1F",
        "GraniteCave_StevensRoom", "GraniteShore", "Route106", "DewfordTown",
        "DewfordTown_Gym", "Route105", "Route107"],
    4: ["Route108", "Route109", "SlateportCity", "Route110", "MauvilleCity",
        "MauvilleCity_Gym", "Route117", "VerdanturfTown"],
    5: ["Route111", "Route112", "FieryPath", "Route113", "FallarborTown",
        "Route114", "MeteorFalls", "MtChimney", "JaggedPass",
        "LavaridgeTown", "LavaridgeTown_Gym_1F", "LavaridgeTown_Gym_B1F"],
    6: ["Route118", "Route119", "FortreeCity", "FortreeCity_Gym",
        "Route119_WeatherInstitute_1F", "Route119_WeatherInstitute_2F"],
    7: ["Route120", "Route121", "Route122", "MtPyre_1F", "MtPyre_2F",
        "MtPyre_3F", "MtPyre_4F", "MtPyre_5F", "MtPyre_6F", "LilycoveCity",
        "Route123", "Route124", "MossdeepCity", "MossdeepCity_Gym"],
    8: ["Route125", "Route126", "Route127", "Route128", "SootopolisCity",
        "SootopolisCity_Gym_1F", "SootopolisCity_Gym_B1F", "SeafloorCavern_Room9"],
}
E4_MAPS = ["EverGrandeCity_Hall1", "EverGrandeCity_Hall2", "EverGrandeCity_Hall3",
           "EverGrandeCity_Hall4", "EverGrandeCity_Hall5",
           "EverGrandeCity_SidneysRoom", "EverGrandeCity_PhoebesRoom",
           "EverGrandeCity_GlaciasRoom", "EverGrandeCity_DrakesRoom",
           "EverGrandeCity_ChampionsRoom", "VictoryRoad_1F", "VictoryRoad_B1F",
           "VictoryRoad_B2F", "Route128"]
POSTGAME_MAPS = ["BattleFrontier", "TrainerHill", "SkyPillar", "AlteringCave",
                 "ArtisanCave", "DesertUnderpass", "MarineCave", "TerraCave",
                 "SouthernIsland", "FarawayIsland", "BirthIsland", "NavelRock"]


def map_to_split():
    m = {}
    for split, maps in SPLIT_MAPS.items():
        for name in maps:
            m[name] = f"Split {split}"
    for name in E4_MAPS:
        m.setdefault(name, "Elite Four & Champion")
    for name in POSTGAME_MAPS:
        m.setdefault(name, "Postgame")
    return m


def classify_map(mapname, table):
    if mapname in table:
        return table[mapname]
    for prefix, label in (("BattleFrontier", "Postgame"), ("TrainerHill", "Postgame"),
                          ("EverGrandeCity", "Elite Four & Champion"),
                          ("VictoryRoad", "Elite Four & Champion"),
                          ("MtPyre", "Split 7"), ("SeafloorCavern", "Split 8"),
                          ("AquaHideout", "Split 7"), ("MagmaHideout", "Split 5"),
                          ("SafariZone", "Postgame"), ("MirageTower", "Split 5"),
                          ("SSTidal", "Postgame"), ("AbandonedShip", "Split 4"),
                          ("NewMauville", "Split 4"), ("ScorchedSlab", "Split 6"),
                          ("ShoalCave", "Split 7"), ("TrickHouse", "Split 4"),
                          ("Route110_TrickHouse", "Split 4"),
                          ("LilycoveCity", "Split 7"), ("SlateportCity", "Split 4"),
                          ("MauvilleCity", "Split 4"), ("RustboroCity", "Split 2"),
                          ("PetalburgCity", "Split 1"), ("DewfordTown", "Split 3"),
                          ("GraniteCave", "Split 3"), ("FortreeCity", "Split 6"),
                          ("MossdeepCity", "Split 7"), ("SootopolisCity", "Split 8"),
                          ("LavaridgeTown", "Split 5"), ("FallarborTown", "Split 5"),
                          ("VerdanturfTown", "Split 4"), ("OldaleTown", "Split 1"),
                          ("LittlerootTown", "Split 1")):
        if mapname.startswith(prefix):
            return label
    return "Unassigned"


# ------------------------------------------------------------------ sheets ----

def dexnum(entry):
    """Real National Dex integer, or '' when the species has no dex entry."""
    return E.national_dex_numbers().get(entry.get("natDexNum"), "")


def dexsort(entry, const):
    n = E.national_dex_numbers().get(entry.get("natDexNum"))
    return (n if isinstance(n, int) else 9999, const)


def sheet_pokemon_stats(wb, cur, base):
    headers = ["Nat Dex #", "Species", "Form/Const", "Orig Typing", "New Typing",
               "Orig HP", "New HP", "Orig Atk", "New Atk", "Orig Def", "New Def",
               "Orig SpA", "New SpA", "Orig SpD", "New SpD", "Orig Spe", "New Spe",
               "Orig BST", "New BST", "BST Change", "Changed?", "Status"]
    rows, fills = [], {}
    for const in sorted(cur, key=lambda k: dexsort(cur[k], k)):
        c = cur[const]
        b = base.get(const)
        dex = dexnum(c)
        cb, bb = E.bst(c), E.bst(b) if b else None
        changed = bool(b) and any(c.get(s) != b.get(s) for s in E.STAT_KEYS)
        type_changed = bool(b) and c["types"] != b["types"]
        status = STATUS_IMPLEMENTED if cb is not None else STATUS_UNVERIFIED
        row = [
            dex, c.get("name") or "", const,
            fmt_types(b["types"]) if b else "UNVERIFIED",
            fmt_types(c["types"]),
        ]
        for s in ["baseHP", "baseAttack", "baseDefense", "baseSpAttack", "baseSpDefense", "baseSpeed"]:
            row.append(b.get(s) if b else None)
            row.append(c.get(s))
        row += [bb, cb, (cb - bb) if (cb is not None and bb is not None) else None,
                "YES" if (changed or type_changed) else "", status]
        if status == STATUS_UNVERIFIED:
            fills[len(rows)] = WARN_FILL
        elif changed or type_changed:
            fills[len(rows)] = CHANGED_FILL
        rows.append(row)
    ws = wb.create_sheet("Pokemon Stats")
    write_sheet(ws, "Pokemon Stats - original (upstream) vs current", headers, rows,
                widths=[10, 16, 26, 16, 16] + [8] * 12 + [9, 9, 11, 10, 18],
                tab_color=TAB_COLORS["Pokemon Stats"], row_fills=fills,
                note=("All species in the game, not only modified ones. 'Orig' = values at upstream "
                      f"baseline commit {E.BASELINE_COMMIT}; 'New' = current working tree. "
                      "Highlighted rows differ from upstream. UNVERIFIED rows use macro-generated "
                      "species_info entries this extractor cannot resolve."))
    return sum(1 for r in rows if r[-2] == "YES")


def sheet_ability_changes(wb, cur, base):
    headers = ["Nat Dex #", "Species", "Const", "Orig Ability 1", "Orig Ability 2",
               "Orig Hidden", "New Ability 1", "New Ability 2", "New Hidden",
               "Hidden Changed?", "Changed?", "Notes", "Status"]
    rows, fills = [], {}

    def slot(lst, i):
        return pretty_const(lst[i], "ABILITY_") if lst and len(lst) > i else ""

    for const in sorted(cur, key=lambda k: dexsort(cur[k], k)):
        c, b = cur[const], base.get(const)
        ca, ba = c["abilities"], (b["abilities"] if b else [])
        changed = bool(b) and ca != ba
        hidden_changed = bool(b) and slot(ca, 2) != slot(ba, 2)
        note = ""
        if changed:
            diffs = []
            for i, label in enumerate(("Ability 1", "Ability 2", "Hidden")):
                if slot(ca, i) != slot(ba, i):
                    diffs.append(f"{label}: {slot(ba, i) or '-'} -> {slot(ca, i) or '-'}")
            note = "; ".join(diffs)
        rows.append([
            dexnum(c), c.get("name") or "", const,
            slot(ba, 0), slot(ba, 1), slot(ba, 2),
            slot(ca, 0), slot(ca, 1), slot(ca, 2),
            "YES" if hidden_changed else "", "YES" if changed else "", note,
            STATUS_IMPLEMENTED if ca else STATUS_UNVERIFIED,
        ])
        if changed:
            fills[len(rows) - 1] = CHANGED_FILL
    ws = wb.create_sheet("Ability Changes")
    write_sheet(ws, "Ability Changes", headers, rows,
                widths=[10, 16, 26, 16, 16, 16, 16, 16, 16, 14, 10, 46, 16],
                tab_color=TAB_COLORS["Ability Changes"], row_fills=fills,
                note="Every species listed; 'Changed?' marks divergence from the upstream baseline. "
                     "Notes are generated from the diff, not hand-written.")
    return sum(1 for r in rows if r[-3] == "YES")


def sheet_learnsets(wb, cur, base, lcur, lbase):
    headers = ["Species", "Const", "Learnset Symbol", "Level", "Move",
               "New Move?", "Evolves At", "Missed If Evolved Early?", "Status"]
    rows, fills = [], {}
    for const in sorted(cur, key=lambda k: dexsort(cur[k], k)):
        c = cur[const]
        sym = c.get("learnsetSymbol")
        if not sym or sym not in lcur:
            rows.append([c.get("name") or "", const, sym or "", None, "", "", "", "",
                         STATUS_UNVERIFIED])
            fills[len(rows) - 1] = WARN_FILL
            continue
        evo_level = None
        for evo in c.get("evolutions") or []:
            if len(evo) >= 2 and evo[0] == "EVO_LEVEL" and str(evo[1]).isdigit():
                evo_level = int(evo[1])
                break
        baseline_moves = set(lbase.get(sym, []))
        for lv, mv in lcur[sym]:
            is_new = (lv, mv) not in baseline_moves
            missed = "YES" if (evo_level is not None and lv > evo_level) else ""
            rows.append([c.get("name") or "", const, sym, lv,
                         pretty_const(mv, "MOVE_"), "YES" if is_new else "",
                         evo_level, missed, STATUS_IMPLEMENTED])
            if is_new:
                fills[len(rows) - 1] = ADDED_FILL
            elif missed:
                fills[len(rows) - 1] = WARN_FILL
    ws = wb.create_sheet("Level-Up Learnsets")
    write_sheet(ws, "Level-Up Learnsets (active table only)", headers, rows,
                widths=[16, 26, 30, 8, 20, 11, 11, 24, 16],
                tab_color=TAB_COLORS["Level-Up Learnsets"], row_fills=fills,
                note=(f"Source: {E.LEARNSET_FILE} only. That is the sole table compiled in "
                      "(src/pokemon.c selects it via P_LVL_UP_LEARNSETS = GEN_LATEST); the gen_1-gen_8 "
                      "copies of these same symbols are not built. 'New Move?' = absent from the "
                      "upstream baseline. 'Missed If Evolved Early?' flags moves taught above the "
                      "species' own level-up evolution threshold."))
    return len(rows)


def sheet_evolutions(wb, cur, base):
    headers = ["Species", "Const", "Orig Evolution", "New Evolution", "Changed?",
               "Delayed-Evolution Note", "Status"]
    rows, fills = [], {}
    for const in sorted(cur, key=lambda k: dexsort(cur[k], k)):
        c, b = cur[const], base.get(const)
        cf = fmt_evolutions(c.get("evolutions"))
        bf = fmt_evolutions(b.get("evolutions")) if b else "UNVERIFIED"
        changed = bool(b) and c.get("evolutions") != b.get("evolutions")
        note = ""
        if changed:
            note = "Evolution requirement changed by this project."
        rows.append([c.get("name") or "", const, bf, cf, "YES" if changed else "", note,
                     STATUS_IMPLEMENTED])
        if changed:
            fills[len(rows) - 1] = CHANGED_FILL
    ws = wb.create_sheet("Evolution Changes")
    write_sheet(ws, "Evolution Changes", headers, rows,
                widths=[16, 26, 40, 40, 10, 40, 16],
                tab_color=TAB_COLORS["Evolution Changes"], row_fills=fills,
                note="Method, level and item are read from each species' .evolutions field.")
    return sum(1 for r in rows if r[4] == "YES")


def sheet_move_changes(wb, mcur, mbase):
    headers = ["Move", "Const", "Orig Power", "New Power", "Orig Acc", "New Acc",
               "Orig PP", "New PP", "Orig Type", "New Type", "Orig Category",
               "New Category", "Orig Effect", "New Effect", "Secondary Chance",
               "Secondary Effects", "Changed?", "Status"]
    rows, fills = [], {}
    for const in sorted(mcur):
        c, b = mcur[const], mbase.get(const)
        fields = ("power", "accuracy", "pp", "type", "category", "effect")
        changed = bool(b) and any(c.get(f) != b.get(f) for f in fields)
        rows.append([
            c.get("name") or "", const,
            b.get("power") if b else None, c.get("power"),
            b.get("accuracy") if b else None, c.get("accuracy"),
            b.get("pp") if b else None, c.get("pp"),
            pretty_const(b.get("type"), "TYPE_") if b else "UNVERIFIED",
            pretty_const(c.get("type"), "TYPE_"),
            pretty_const(b.get("category"), "DAMAGE_CATEGORY_") if b else "UNVERIFIED",
            pretty_const(c.get("category"), "DAMAGE_CATEGORY_"),
            pretty_const(b.get("effect"), "EFFECT_") if b else "UNVERIFIED",
            pretty_const(c.get("effect"), "EFFECT_"),
            c.get("secondaryChance") or "", c.get("secondaryEffect") or "",
            "YES" if changed else "", STATUS_IMPLEMENTED,
        ])
        if changed:
            fills[len(rows) - 1] = CHANGED_FILL
    ws = wb.create_sheet("Move Changes")
    write_sheet(ws, "Move Changes", headers, rows,
                widths=[20, 26, 10, 10, 9, 9, 8, 8, 14, 14, 16, 16, 24, 24, 16, 30, 10, 16],
                tab_color=TAB_COLORS["Move Changes"], row_fills=fills,
                note="All moves listed. Power/accuracy/PP resolve config ternaries "
                     "(e.g. 'P_UPDATED_MOVE_DATA >= GEN_6 ? x : y') against this project's config.")
    return sum(1 for r in rows if r[-2] == "YES")


def engagement_note(const, entry, mapnames, forcing, gym_badge_by_map):
    """Mandatory/optional verdict.

    Two independent things matter: whether the NPC forces the battle on sight
    (from its object event's sight range), and whether the battle gates
    progression (gym leaders award a badge). A leader has sight range 0 -- you
    walk up and talk to them -- but is still required.
    """
    for m in mapnames:
        if m in gym_badge_by_map and "leader" in (entry.get("class") or "").lower():
            return f"REQUIRED - awards Badge {gym_badge_by_map[m]}"
    return E.classify_engagement(forcing.get(const, []))


def trainer_rows(entry, mapnames, split, mandatory_note):
    """One row per Pokemon, with trainer-level columns repeated."""
    out = []
    mons = entry.get("mons") or []
    ai = entry.get("ai") or "UNVERIFIED"
    fmt = "Double" if (entry.get("double battle", "No").lower() == "yes") else "Single"
    items = entry.get("items") or ""
    loc = ", ".join(mapnames) if mapnames else "UNVERIFIED (no script reference found)"
    if not mons:
        return [[entry.get("name") or "", entry.get("class") or "", entry["const"], loc,
                 split, mandatory_note, fmt, ai, items, "", "NO PARTY DATA", "", "", "",
                 "", "", "", "", "", STATUS_NOT_IMPL]]
    for i, m in enumerate(mons):
        moves = (m.get("moves") or []) + [""] * 4
        out.append([
            entry.get("name") or "" if i == 0 else "",
            entry.get("class") or "" if i == 0 else "",
            entry["const"] if i == 0 else "",
            loc if i == 0 else "",
            split if i == 0 else "",
            mandatory_note if i == 0 else "",
            fmt if i == 0 else "",
            ai if i == 0 else "",
            items if i == 0 else "",
            i + 1,
            m.get("species") or "",
            m.get("level"),
            m.get("ability") or "(default slot 1)",
            m.get("nature") or "(Hardy default)",
            m.get("item") or "",
            moves[0], moves[1], moves[2], moves[3],
            STATUS_IMPLEMENTED,
        ])
    return out


TRAINER_HEADERS = ["Trainer", "Class", "Trainer ID", "Location", "Split",
                   "Mandatory?", "Format", "AI Flags", "Trainer Items", "Slot",
                   "Species", "Level", "Ability", "Nature", "Held Item",
                   "Move 1", "Move 2", "Move 3", "Move 4", "Status"]
TRAINER_WIDTHS = [16, 14, 26, 26, 12, 13, 9, 30, 20, 6, 16, 7, 18, 16, 16, 16, 16, 16, 16, 16]


FORCING = {}
GYM_BADGE_BY_MAP = {}


def sheet_trainers(wb, trainers, locations, split_label, consts):
    rows, fills = [], {}
    for const in consts:
        entry = trainers[const]
        maps = locations.get(const, [])
        mandatory = engagement_note(const, entry, maps, FORCING, GYM_BADGE_BY_MAP)
        placeholder = E.is_placeholder_team(entry)
        sub = trainer_rows(entry, maps, split_label, mandatory)
        for r in sub:
            if placeholder:
                fills[len(rows)] = WARN_FILL
                r[-1] = "UNVERIFIED (placeholder stub team)"
            elif r[-1] == STATUS_NOT_IMPL:
                fills[len(rows)] = BAD_FILL
            rows.append(r)
    ws = wb.create_sheet(split_label[:31])
    write_sheet(ws, f"{split_label} - Trainers", TRAINER_HEADERS, rows,
                widths=TRAINER_WIDTHS, tab_color=SPLIT_TAB_COLOR, row_fills=fills,
                note=("One row per Pokemon. Location is derived from which map scripts reference the "
                      "trainer ID. 'Mandatory?' is derived from the object event's trainer_type and "
                      "sight range (src/trainer_see.c uses that field as the approach distance): "
                      "range >= 1 means the NPC challenges on sight, range 0 means it must be talked "
                      "to. Gym leaders are labelled REQUIRED because they gate a badge even though "
                      "their sight range is 0. Whether a forcing sight line can be physically walked "
                      "around is a collision question this does not answer. "
                      "Orange = placeholder team. Red = trainer ID with no party data."))
    return len(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-o", "--out", default=OUT_DEFAULT)
    args = ap.parse_args()

    print("Extracting from project files...")
    cur = E.species_current()
    base = E.species_baseline()
    mcur = E.moves_current()
    mbase = E.moves_baseline()
    lcur = E.learnsets_current()
    lbase = E.learnsets_baseline()
    trainers = E.trainers_current()
    locations = E.trainer_locations()
    encounters = E.wild_encounters()
    ow_items = E.overworld_items()
    marts = E.mart_inventories()
    gifts = E.script_gift_mons()
    trades = E.ingame_trades()
    trade_locs = E.trade_script_locations()
    badge_sources, gym_by_badge = build_split_model()
    global FORCING, GYM_BADGE_BY_MAP
    FORCING = E.trainer_forcing()
    GYM_BADGE_BY_MAP = {m: n for n, maps in gym_by_badge.items() for m in maps}
    items = E.parse_items()

    # preserve manual sheets
    preserved = {"Balance Issues": [], "Change Log": []}
    if os.path.exists(args.out):
        try:
            old = load_workbook(args.out)
            for name in preserved:
                if name in old.sheetnames:
                    sh = old[name]
                    hdr_row = None
                    for r in range(1, 6):
                        if sh.cell(row=r, column=1).value in ("ID", "Date"):
                            hdr_row = r
                            break
                    if hdr_row:
                        for r in range(hdr_row + 1, sh.max_row + 1):
                            vals = [sh.cell(row=r, column=c).value
                                    for c in range(1, sh.max_column + 1)]
                            if any(v not in (None, "") for v in vals):
                                preserved[name].append(vals)
            print(f"  preserved {len(preserved['Balance Issues'])} balance rows, "
                  f"{len(preserved['Change Log'])} change-log rows")
        except Exception as exc:  # noqa: BLE001
            print(f"  WARNING: could not read existing workbook to preserve notes: {exc}")

    wb = Workbook()
    wb.remove(wb.active)

    dash = wb.create_sheet("Dashboard")

    stat_changed = sheet_pokemon_stats(wb, cur, base)
    abil_changed = sheet_ability_changes(wb, cur, base)
    ls_rows = sheet_learnsets(wb, cur, base, lcur, lbase)
    evo_changed = sheet_evolutions(wb, cur, base)
    move_changed = sheet_move_changes(wb, mcur, mbase)

    # ---- trainer split sheets
    split_table = map_to_split()
    by_split = {}
    for const, entry in trainers.items():
        if const == "TRAINER_NONE":
            continue
        maps = locations.get(const, [])
        labels = {classify_map(m, split_table) for m in maps} or {"Unassigned"}
        label = sorted(labels)[0] if len(labels) == 1 else sorted(labels)[0]
        by_split.setdefault(label, []).append(const)

    order = [f"Split {i}" for i in range(1, 9)] + ["Elite Four & Champion", "Postgame", "Unassigned"]
    trainer_counts = {}
    for label in order:
        consts = sorted(by_split.get(label, []))
        if not consts:
            continue
        trainer_counts[label] = len(consts)
        sheet_trainers(wb, trainers, locations, label, consts)

    # ---- Boss Battles
    boss_consts = []
    for const in trainers:
        cls = (trainers[const].get("class") or "").lower()
        if any(k in cls for k in ("leader", "elite four", "champion", "team leader")):
            boss_consts.append(const)
    boss_rows, boss_fills = [], {}
    for const in sorted(boss_consts):
        entry = trainers[const]
        maps = locations.get(const, [])
        label = sorted({classify_map(m, split_table) for m in maps} or {"Unassigned"})[0]
        for r in trainer_rows(entry, maps, label,
                              engagement_note(const, entry, maps, FORCING, GYM_BADGE_BY_MAP)):
            boss_rows.append(r)
    ws = wb.create_sheet("Boss Battles")
    write_sheet(ws, "Boss Battles - leaders, Elite Four, Champion", TRAINER_HEADERS,
                boss_rows, widths=TRAINER_WIDTHS, tab_color=TAB_COLORS["Boss Battles"],
                row_fills=boss_fills,
                note="Selected by trainer Class containing Leader / Elite Four / Champion. "
                     "Rival and evil-team boss battles are not distinguishable by class alone and "
                     "are listed in their split sheets instead.")

    # ---- Progression
    prog_headers = ["Order", "Milestone", "Map(s)", "Badge Flag Set", "Level Cap",
                    "Notes", "Status"]
    prog_rows, prog_fills = [], {}
    lvl_cap_type = E.config_value("include/config/caps.h", "B_LEVEL_CAP_TYPE")
    cap_status = (STATUS_NOT_IMPL if (lvl_cap_type or "").strip() == "LEVEL_CAP_NONE"
                  else STATUS_UNVERIFIED)
    for n in range(1, 9):
        maps = gym_by_badge.get(n, [])
        flag = f"FLAG_BADGE0{n}_GET"
        notes = ""
        status = STATUS_IMPLEMENTED
        if not maps:
            notes = ("NO GYM SETS THIS FLAG. The badge is unobtainable, yet the flag is still read "
                     "by the game (obedience caps in src/battle_util.c, badge-count arrays in "
                     "src/battle_setup.c and src/battle_script_commands.c, and the Trick House "
                     "puzzle gate in Route110_TrickHouseEntrance).")
            status = STATUS_NOT_IMPL
            prog_fills[len(prog_rows)] = BAD_FILL
        elif len(maps) > 1:
            notes = (f"CONFLICT: {len(maps)} gyms set the same badge flag ({', '.join(maps)}). "
                     "Beating the second one awards no new badge.")
            status = "IMPLEMENTED (CONFLICT)"
            prog_fills[len(prog_rows)] = BAD_FILL
        prog_rows.append([n, f"Badge {n}", ", ".join(maps) or "(none)", flag,
                          "Not enforced" if cap_status == STATUS_NOT_IMPL else "UNVERIFIED",
                          notes, status])
    prog_rows.append(["-", "Level cap system", "src/caps.c, include/config/caps.h", "-",
                      f"B_LEVEL_CAP_TYPE = {lvl_cap_type or 'UNVERIFIED'}",
                      "Engine supports level caps via LEVEL_CAP_FLAG_LIST / sLevelCapFlagMap, "
                      "but the config selects LEVEL_CAP_NONE, so no cap is enforced. The "
                      "boss-entry level restriction discussed in design notes is not implemented.",
                      cap_status])
    if cap_status == STATUS_NOT_IMPL:
        prog_fills[len(prog_rows) - 1] = WARN_FILL
    ws = wb.create_sheet("Progression")
    write_sheet(ws, "Gym Badge Progression & Level Caps", prog_headers, prog_rows,
                widths=[8, 20, 40, 24, 24, 80, 24], tab_color=TAB_COLORS["Progression"],
                row_fills=prog_fills,
                note="Badge order is derived by reading which gym script calls "
                     "setflag FLAG_BADGE0N_GET - not taken from any design document.")

    # ---- Wild Encounters
    enc_headers = ["Map", "Split", "Method", "Encounter Rate", "Slot", "Species",
                   "Min Level", "Max Level", "Time/Weather Restriction",
                   "Requirement", "Status"]
    enc_rows = []
    for e in encounters:
        mapname = (e["map"] or "").replace("MAP_", "")
        pretty_map = mapname.title().replace("_", "")
        enc_rows.append([
            e["map"], classify_map(pretty_map, split_table),
            e["method"].replace("_mons", "").replace("_", " ").title(),
            e["encounter_rate"], e["slot"], pretty_const(e["species"], "SPECIES_"),
            e["min_level"], e["max_level"],
            "None in data", "See method (Surf/Fish/Rock Smash need the HM)",
            STATUS_IMPLEMENTED,
        ])
    ws = wb.create_sheet("Wild Encounters")
    write_sheet(ws, "Wild Encounters", enc_headers, enc_rows,
                widths=[30, 12, 14, 14, 6, 18, 10, 10, 24, 40, 16],
                tab_color=TAB_COLORS["Wild Encounters"],
                note=f"Source: {E.ENCOUNTERS_FILE}. This file has no time-of-day or weather "
                     "fields, so those columns report 'None in data' rather than guessing.")

    # ---- Pokemon Availability
    avail = {}
    for e in encounters:
        sp = e["species"]
        if not sp:
            continue
        pretty_map = (e["map"] or "").replace("MAP_", "").title().replace("_", "")
        split = classify_map(pretty_map, split_table)
        rec = avail.setdefault(sp, {"locs": set(), "splits": set(), "minlv": None, "how": set()})
        if e["map"]:
            rec["locs"].add(e["map"])
        rec["splits"].add(split)
        rec["how"].add("Wild")
        if e["min_level"] is not None:
            rec["minlv"] = e["min_level"] if rec["minlv"] is None else min(rec["minlv"], e["min_level"])
    for g in gifts:
        sp = g.get("species")
        if not sp:
            continue
        rec = avail.setdefault(sp, {"locs": set(), "splits": set(), "minlv": None, "how": set()})
        rec["locs"].add(g["map"])
        rec["splits"].add(classify_map(g["map"], split_table))
        rec["how"].add("Gift")
    for t in trades:
        sp = t.get("species")
        if not sp:
            continue
        rec = avail.setdefault(sp, {"locs": set(), "splits": set(), "minlv": None, "how": set()})
        for m, consts in trade_locs.items():
            if t["const"] in consts:
                rec["locs"].add(m)
                rec["splits"].add(classify_map(m, split_table))
        rec["how"].add("Trade")

    def split_key(s):
        m = re.match(r"Split (\d)", s or "")
        return int(m.group(1)) if m else (90 if s == "Elite Four & Champion" else 95)

    av_headers = ["Species", "How Obtained", "Earliest Split", "All Splits",
                  "Locations", "Earliest Wild Level", "Evolves Into", "Evolution Method",
                  "Status"]
    av_rows = []
    for sp in sorted(avail):
        rec = avail[sp]
        const = sp
        c = cur.get(const)
        earliest = sorted(rec["splits"], key=split_key)[0] if rec["splits"] else "Unassigned"
        av_rows.append([
            pretty_const(sp, "SPECIES_"), ", ".join(sorted(rec["how"])), earliest,
            ", ".join(sorted(rec["splits"], key=split_key)),
            ", ".join(sorted(x for x in rec["locs"] if x))[:250],
            rec["minlv"],
            fmt_evolutions(c.get("evolutions")) if c else "UNVERIFIED",
            "see Evolution Changes sheet",
            STATUS_IMPLEMENTED,
        ])
    ws = wb.create_sheet("Pokemon Availability")
    write_sheet(ws, "Pokemon Availability", av_headers, av_rows,
                widths=[18, 16, 14, 24, 60, 16, 40, 24, 16],
                tab_color=TAB_COLORS["Pokemon Availability"],
                note="Built from wild encounter tables, script givemon gifts, and the in-game "
                     "trade table. Species obtainable only by evolving are not listed as separate "
                     "rows; see the Evolution Changes sheet.")

    # ---- Item / TM locations
    def item_split(mapname):
        return classify_map(mapname, split_table)

    it_headers = ["Item", "Item Const", "Method", "Map", "Split", "Coords", "Flag", "Status"]
    tm_headers = ["TM/HM", "Move Taught", "Item Const", "Method", "Map", "Split", "Coords",
                  "Flag", "Status"]
    it_rows, tm_rows = [], []
    for row in sorted(ow_items, key=lambda r: (r["map"], str(r["item"]))):
        itc = row["item"] or ""
        if not itc:
            continue
        info = items.get(itc, {})
        nice = info.get("name") or pretty_const(itc, "ITEM_")
        coords = f"({row['x']}, {row['y']})"
        if re.match(r"ITEM_(TM|HM)(\d|_)", itc):
            tm_rows.append([nice, pretty_const(info.get("teachesMove"), "MOVE_") or "UNVERIFIED",
                            itc, row["kind"], row["map"], item_split(row["map"]), coords,
                            row["flag"] or "", STATUS_IMPLEMENTED])
        else:
            it_rows.append([nice, itc, row["kind"], row["map"], item_split(row["map"]),
                            coords, row["flag"] or "", STATUS_IMPLEMENTED])
    for mapname, sold in sorted(marts.items()):
        for itc in sorted(set(sold)):
            info = items.get(itc, {})
            nice = info.get("name") or pretty_const(itc, "ITEM_")
            price = info.get("price") or "UNVERIFIED"
            method = f"Purchased (price field: {price})"
            if re.match(r"ITEM_(TM|HM)(\d|_)", itc):
                tm_rows.append([nice, pretty_const(info.get("teachesMove"), "MOVE_") or "UNVERIFIED",
                                itc, method, mapname, item_split(mapname), "-", "-",
                                STATUS_IMPLEMENTED])
            else:
                it_rows.append([nice, itc, method, mapname, item_split(mapname), "-", "-",
                                STATUS_IMPLEMENTED])
    ws = wb.create_sheet("Item Locations")
    write_sheet(ws, "Item Locations", it_headers, it_rows,
                widths=[22, 28, 30, 30, 12, 12, 40, 16],
                tab_color=TAB_COLORS["Item Locations"],
                note="Visible item balls come from map.json object_events with "
                     "OBJ_EVENT_GFX_ITEM_BALL; hidden items from bg_events of type hidden_item; "
                     "purchasable items from parsed pokemart lists.")
    ws = wb.create_sheet("TM & HM Locations")
    write_sheet(ws, "TM & HM Locations", tm_headers, tm_rows,
                widths=[14, 20, 30, 30, 30, 12, 12, 40, 16],
                tab_color=TAB_COLORS["TM & HM Locations"],
                note="Same sources as Item Locations, filtered to TM/HM items. The TM label and the "
                     "move it teaches both come from that item's own entry in src/data/items.h "
                     "(.name and .secondaryId), so they cannot drift apart.")

    # ---- Gifts & Trades
    gt_headers = ["Type", "Species", "Level", "Map", "Split", "Nature", "Ability Slot",
                  "IVs", "Held Item", "Moves / Notes", "Requested Species", "Status"]
    gt_rows = []
    for g in sorted(gifts, key=lambda r: (r["map"], str(r.get("species")))):
        mv = [v for k, v in g.items() if k.startswith("move")]
        gt_rows.append(["Gift (script)", pretty_const(g.get("species"), "SPECIES_"),
                        g.get("level"), g["map"], item_split(g["map"]),
                        pretty_const(g.get("nature"), "NATURE_"),
                        g.get("abilityNum", ""),
                        "31 all" if str(g.get("hpIv")) == "31" else (g.get("hpIv") or "default"),
                        "", ", ".join(pretty_const(m, "MOVE_") for m in mv), "-",
                        STATUS_IMPLEMENTED])
    for t in trades:
        locs = [m for m, consts in trade_locs.items() if t["const"] in consts]
        gt_rows.append(["In-game Trade", pretty_const(t.get("species"), "SPECIES_"), "-",
                        ", ".join(locs) or "UNVERIFIED (no script reference)",
                        item_split(locs[0]) if locs else "Unassigned",
                        "-", t.get("abilityNum") or "", t.get("ivs") or "",
                        pretty_const(t.get("heldItem"), "ITEM_"),
                        f"Nickname {t.get('nickname')}, OT {t.get('otName')}",
                        pretty_const(t.get("requestedSpecies"), "SPECIES_") or "-",
                        STATUS_IMPLEMENTED])
    ws = wb.create_sheet("Gifts & Trades")
    write_sheet(ws, "Gift Pokemon & In-Game Trades", gt_headers, gt_rows,
                widths=[16, 18, 8, 30, 12, 14, 12, 16, 16, 50, 20, 16],
                tab_color=TAB_COLORS["Gifts & Trades"],
                note="Gifts are every `givemon` call found in map scripts. Trades come from "
                     "src/data/trade.h cross-referenced with the scripts that reference each ID.")

    # ---- Balance Issues (auto-seeded findings + preserved manual rows)
    bi_headers = ["ID", "Issue", "Category", "Location / File", "Affected",
                  "Severity", "Proposed Solution", "Implementation Status", "Source"]
    auto_issues = []
    for n in range(1, 9):
        maps = gym_by_badge.get(n, [])
        if not maps:
            auto_issues.append([
                f"AUTO-BADGE{n}-MISSING",
                f"No gym awards Badge {n}; FLAG_BADGE0{n}_GET is never set",
                "Progression",
                "data/maps/*/scripts.inc",
                f"FLAG_BADGE0{n}_GET",
                "HIGH",
                "Assign this badge flag to the intended gym, or renumber the gyms so the "
                "badge sequence has no gap.",
                "OPEN (not auto-fixed)",
                "Derived from setflag scan",
            ])
        elif len(maps) > 1:
            auto_issues.append([
                f"AUTO-BADGE{n}-CONFLICT",
                f"{len(maps)} gyms all set FLAG_BADGE0{n}_GET: {', '.join(maps)}",
                "Progression",
                ", ".join(f"data/maps/{m}/scripts.inc" for m in maps),
                f"FLAG_BADGE0{n}_GET",
                "HIGH",
                "Give each gym a distinct badge flag so badge count, obedience caps and "
                "HM/Trick-House gating advance correctly.",
                "OPEN (not auto-fixed)",
                "Derived from setflag scan",
            ])
    for const, entry in sorted(trainers.items()):
        if const == "TRAINER_NONE":
            continue
        if not entry.get("mons"):
            auto_issues.append([
                f"AUTO-NOPARTY-{const}", "Trainer ID has no party data (compiles to an empty team)",
                "Trainer data", E.TRAINERS_FILE, const, "HIGH",
                "Add a `=== <ID> ===` block with a real team, or remove the trainer and its "
                "object event.", "OPEN (not auto-fixed)", "trainers.party parse",
            ])
        elif E.is_placeholder_team(entry):
            auto_issues.append([
                f"AUTO-STUB-{const}", "Trainer still uses the reserved placeholder stub team "
                "(Lv1 Lillipup, no moves)", "Trainer data", E.TRAINERS_FILE, const, "MEDIUM",
                "Design a real team, or leave reserved and ensure the object event stays unwired.",
                "OPEN (not auto-fixed)", "trainers.party parse",
            ])
    # unreachable trainers (no map script references them)
    unref = [k for k in sorted(trainers) if k != "TRAINER_NONE" and not locations.get(k)]
    rematch_like = [k for k in unref if re.search(r"_[2-5]$", k)]
    other_unref = [k for k in unref if k not in rematch_like]
    free_match_call = E.config_value("include/config/save.h", "FREE_MATCH_CALL")
    if unref:
        auto_issues.append([
            "AUTO-UNREACHABLE-TRAINERS",
            f"{len(unref)} trainer IDs have party data but are not referenced by any map script "
            f"({len(rematch_like)} look like _2.._5 rematch variants, {len(other_unref)} do not)",
            "Trainer data", "src/data/trainers.party vs data/maps/*/scripts.inc",
            "See the Unassigned sheet", "LOW",
            (f"FREE_MATCH_CALL is {free_match_call}, which disables match-call rematches, so the "
             "rematch variants are expected to be unreachable and are effectively dead data. The "
             "non-rematch entries are worth checking individually - some may be trainers that were "
             "meant to be placed but never got an object event."),
            "OPEN (not auto-fixed)", "trainer/script cross-reference",
        ])
    if (lvl_cap_type or "").strip() == "LEVEL_CAP_NONE":
        auto_issues.append([
            "AUTO-LEVELCAP", "No level cap is enforced (B_LEVEL_CAP_TYPE = LEVEL_CAP_NONE)",
            "Progression", "include/config/caps.h", "Whole game", "MEDIUM",
            "If boss-entry level limits are wanted, set B_LEVEL_CAP_TYPE to "
            "LEVEL_CAP_FLAG_LIST and populate sLevelCapFlagMap in src/caps.c.",
            "OPEN (not auto-fixed)", "config read",
        ])
    bi_rows = auto_issues + [r + [""] * (len(bi_headers) - len(r)) for r in preserved["Balance Issues"]
                             if r and not str(r[0] or "").startswith("AUTO-")]
    bi_fills = {i: (BAD_FILL if r[5] == "HIGH" else WARN_FILL) for i, r in enumerate(bi_rows)
                if len(r) > 5 and r[5] in ("HIGH", "MEDIUM")}
    ws = wb.create_sheet("Balance Issues")
    write_sheet(ws, "Balance Issues", bi_headers, bi_rows,
                widths=[26, 60, 16, 44, 26, 10, 60, 24, 24],
                tab_color=TAB_COLORS["Balance Issues"], row_fills=bi_fills,
                note="Rows with an AUTO- prefix are regenerated each run from the project files. "
                     "Rows you add yourself are preserved across regeneration. Nothing here is "
                     "auto-fixed; these are findings for review only.")

    # ---- Change Log
    cl_headers = ["Date", "Area", "Subject", "Previous Value", "New Value", "Reason",
                  "Implementation Status", "Source File"]
    today = datetime.date.today().isoformat()
    cl_rows = [r + [""] * (len(cl_headers) - len(r)) for r in preserved["Change Log"]]
    seen = {tuple(str(x) for x in r[:5]) for r in cl_rows}

    def add_change(area, subject, old, new, reason, src):
        key = (today, area, subject, str(old), str(new))
        if key in seen:
            return
        seen.add(key)
        cl_rows.append([today, area, subject, old, new, reason, STATUS_IMPLEMENTED, src])

    for const in sorted(cur):
        c, b = cur[const], base.get(const)
        if not b:
            continue
        for s in E.STAT_KEYS:
            if c.get(s) != b.get(s):
                add_change("Pokemon Stats", f"{c.get('name')} {s}", b.get(s), c.get(s),
                           "Differs from upstream baseline", "species_info/gen_*_families.h")
        if c["abilities"] != b["abilities"]:
            add_change("Abilities", f"{c.get('name')} abilities",
                       fmt_abilities(b["abilities"]), fmt_abilities(c["abilities"]),
                       "Differs from upstream baseline", "species_info/gen_*_families.h")
        if c.get("evolutions") != b.get("evolutions"):
            add_change("Evolutions", f"{c.get('name')} evolution",
                       fmt_evolutions(b.get("evolutions")), fmt_evolutions(c.get("evolutions")),
                       "Differs from upstream baseline", "species_info/gen_*_families.h")
    for const in sorted(mcur):
        c, b = mcur[const], mbase.get(const)
        if not b:
            continue
        for f in ("power", "accuracy", "pp", "type", "category"):
            if c.get(f) != b.get(f):
                add_change("Moves", f"{c.get('name')} {f}", b.get(f), c.get(f),
                           "Differs from upstream baseline", E.MOVES_FILE)

    ws = wb.create_sheet("Change Log")
    write_sheet(ws, "Change Log", cl_headers, cl_rows,
                widths=[12, 18, 34, 26, 26, 40, 22, 40],
                tab_color=TAB_COLORS["Change Log"],
                note="Auto-derived by diffing the working tree against upstream baseline "
                     f"{E.BASELINE_COMMIT}. Rows you add by hand are preserved on regeneration. "
                     "Dates reflect when the workbook was generated, not when each edit was made.")

    # ---- Dashboard
    dash.sheet_properties.tabColor = TAB_COLORS["Dashboard"]
    dash["A1"] = "bofa - Game Documentation"
    dash["A1"].font = Font(bold=True, size=18, color="1F3864")
    dash["A2"] = (f"Generated {datetime.datetime.now():%Y-%m-%d %H:%M} from project source files. "
                  f"Upstream baseline for all 'original' columns: {E.BASELINE_COMMIT}")
    dash["A2"].font = Font(italic=True, size=9, color="595959")
    dash["A4"] = "Contents"
    dash["A4"].font = Font(bold=True, size=12)
    r = 5
    for name in wb.sheetnames:
        if name == "Dashboard":
            continue
        cell = dash.cell(row=r, column=1, value=name)
        cell.hyperlink = f"#'{name}'!A1"
        cell.font = LINK_FONT
        r += 1
    stats_start = r + 1
    dash.cell(row=stats_start, column=1, value="Counts (from files)").font = Font(bold=True, size=12)
    summary = [
        ("Species entries parsed", len(cur)),
        ("Species with stat changes vs upstream", stat_changed),
        ("Species with ability changes vs upstream", abil_changed),
        ("Species with evolution changes vs upstream", evo_changed),
        ("Moves with data changes vs upstream", move_changed),
        ("Level-up learnset rows", ls_rows),
        ("Trainers with party data", sum(1 for k, v in trainers.items()
                                         if k != "TRAINER_NONE" and v.get("mons"))),
        ("Trainer IDs with NO party data", sum(1 for k, v in trainers.items()
                                               if k != "TRAINER_NONE" and not v.get("mons"))),
        ("Trainers on placeholder stub teams", sum(1 for k, v in trainers.items()
                                                   if k != "TRAINER_NONE" and E.is_placeholder_team(v))),
        ("Wild encounter rows", len(encounters)),
        ("Overworld item placements", len(ow_items)),
        ("Gift Pokemon (givemon calls)", len(gifts)),
        ("In-game trade entries", len(trades)),
        ("Auto-detected balance issues", len(auto_issues)),
    ]
    for i, (label, val) in enumerate(summary):
        dash.cell(row=stats_start + 1 + i, column=1, value=label)
        dash.cell(row=stats_start + 1 + i, column=2, value=val)
    tr_start = stats_start + len(summary) + 3
    dash.cell(row=tr_start, column=1, value="Trainers per split").font = Font(bold=True, size=12)
    for i, label in enumerate(order):
        if label in trainer_counts:
            dash.cell(row=tr_start + 1 + i, column=1, value=label)
            dash.cell(row=tr_start + 1 + i, column=2, value=trainer_counts[label])
    dash.column_dimensions["A"].width = 46
    dash.column_dimensions["B"].width = 16
    dash.freeze_panes = "A5"

    # move Dashboard first
    wb.move_sheet("Dashboard", offset=-wb.sheetnames.index("Dashboard"))

    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    wb.save(args.out)
    print(f"Wrote {args.out}")
    print(f"  sheets: {len(wb.sheetnames)}")
    for label, val in summary:
        print(f"  {label}: {val}")


if __name__ == "__main__":
    main()
