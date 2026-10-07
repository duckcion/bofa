"""Export BOFA's battle data for the damage calculator (tools/damage_calc).

Numbers come from the built ROM (pokeemerald.gba + symbols from pokeemerald.elf), so they match
the game exactly: species stats/types/abilities, move data and flags, items + hold effects,
ability names and the type chart. Trainer teams (Splits 1-3, in fight order) come from the
generated src/data/trainers.h. Writes tools/damage_calc/data.json and icons.png.

    python tools/damage_calc/export_data.py
"""
import json, os, re, struct, subprocess, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
OUT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(ROOT, "tools", "docgen"))
sys.path.insert(0, os.path.join(ROOT, "tools", "kaizo_lua"))
import split1_plan as PLAN            # noqa: E402
from gen_tables import nm, charmap    # noqa: E402

ROM_BASE = 0x08000000
rom = open(os.path.join(ROOT, "pokeemerald.gba"), "rb").read()
cm = charmap()


def rd(p):
    return open(os.path.join(ROOT, p), encoding="utf-8", errors="replace").read()


def r8(a): return rom[a - ROM_BASE]
def r16(a): return struct.unpack_from("<H", rom, a - ROM_BASE)[0]
def r32(a): return struct.unpack_from("<I", rom, a - ROM_BASE)[0]


def text(a, maxlen=40):
    out = []
    for i in range(maxlen):
        b = r8(a + i)
        if b == 0xFF:
            break
        out.append(cm.get(b, "?"))
    return "".join(out)


def defines(path, prefix):
    vals = {}
    found = re.findall(r"#define (%s\w+)\s+\(?([^\n/]+?)\)?\s*(?://.*)?$" % prefix, rd(path), re.M)
    # repeat so aliases to names defined further down (SPECIES_VIVILLON -> SPECIES_VIVILLON_ICY_SNOW) resolve
    for _ in range(3):
        for name, expr in found:
            if name in vals:
                continue
            try:
                vals[name] = int(eval(re.sub(r"\b(%s\w+)\b" % prefix, lambda x: str(vals.get(x.group(1), "None")), expr.strip())))
            except Exception:
                pass
    return vals


def enum_values(path, start_name):
    """values of a simple C enum (one name per line)"""
    src = rd(path)
    i = src.index(start_name)
    vals, n = {}, 0
    for line in src[i:].split("\n"):
        m = re.match(r"\s*(\w+)\s*(?:=\s*(\d+))?\s*,", line)
        if m:
            if m.group(2):
                n = int(m.group(2))
            vals[m.group(1)] = n
            n += 1
        elif "}" in line:
            break
    return vals


syms = nm()
for extra in ("gTypeEffectivenessTable",):
    pass
out_nm = subprocess.run(["wsl.exe", "-d", "Ubuntu", "-e", "bash", "-lc",
                         "cd '/mnt/" + ROOT[0].lower() + ROOT[2:].replace("\\", "/") + "' && arm-none-eabi-nm -S pokeemerald.elf"],
                        capture_output=True, text=True).stdout if os.name == "nt" else \
    subprocess.run(["arm-none-eabi-nm", "-S", "pokeemerald.elf"], cwd=ROOT, capture_output=True, text=True).stdout
for line in out_nm.splitlines():
    p = line.split()
    if len(p) == 4 and p[3] == "gTypeEffectivenessTable":
        syms["gTypeEffectivenessTable"] = (int(p[0], 16), int(p[1], 16))

# ------------------------------------------------------------------ constants
TYPES = ["None", "Normal", "Fighting", "Flying", "Poison", "Ground", "Rock", "Bug", "Ghost", "Steel", "???", "Fire",
         "Water", "Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark", "Fairy", "Stellar"]
SPECIES = defines("include/constants/species.h", "SPECIES_")
MOVES = defines("include/constants/moves.h", "MOVE_")
ITEMS = defines("include/constants/items.h", "ITEM_")
ABILS = defines("include/constants/abilities.h", "ABILITY_")
HOLD = enum_values("include/constants/hold_effects.h", "HOLD_EFFECT_NONE")
EFFECTS = enum_values("include/constants/battle_move_effects.h", "EFFECT_PLACEHOLDER")
NATDEX = enum_values("include/constants/pokedex.h", "NATIONAL_DEX_NONE")
TARGETS = defines("include/constants/battle.h", "MOVE_TARGET_")
inv = lambda d: {v: k for k, v in reversed(list(d.items()))}
EFFECT_NAME, HOLD_NAME, TARGET_NAME = inv(EFFECTS), inv(HOLD), inv(TARGETS)

# ------------------------------------------------------------------ species
sbase, ssize = syms["gSpeciesInfo"]
SSTRIDE, SNAME = 260, 44
species_src = {}
for path in sorted(os.listdir(os.path.join(ROOT, "src/data/pokemon/species_info"))):
    src = rd("src/data/pokemon/species_info/" + path)
    # forms built from macros (VIVILLON_MISC_INFO(...), SPEWPA_SPECIES_INFO(...)): expand the macro body
    macros = {mm.group(1): ([p.strip() for p in mm.group(2).split(",") if p.strip()], mm.group(3))
              for mm in re.finditer(r"^#define (\w+)\(([^)]*)\)((?:.*\\\n)*.*)", src, re.M)}
    entries = [(m.group(1), m.group(2)) for m in re.finditer(r"^    \[(SPECIES_\w+)\] =\n    \{\n(.*?)^    \},", src, re.S | re.M)]
    entries += [(m.group(1), m.group(2)) for m in re.finditer(r"^    \[(SPECIES_\w+)\]\s*=\s*(\w+\([^)]*\)),", src, re.M)]
    for sname, body in entries:
        for mac, args in re.findall(r"\b(\w+)\(([^()]*)\)", body):
            if mac in macros:
                params, mbody = macros[mac]
                # fill in the macro's arguments (gMonIcon_Vivillon ##form -> gMonIcon_VivillonPolar)
                for p, a in zip(params, [x.strip() for x in args.split(",")]):
                    mbody = re.sub(r"\b%s\b" % re.escape(p), a, mbody)
                body += re.sub(r"\s*##\s*", "", mbody)
        g = lambda pat: (re.search(pat, body).group(1) if re.search(pat, body) else None)
        species_src[sname] = {
            "natdex": g(r"\.natDexNum = (NATIONAL_DEX_\w+)"),
            "weight": g(r"\.weight = (\d+)"),
            "icon": g(r"\.iconSprite = (gMonIcon_\w+)"),
            "evolves": bool(re.search(r"\.evolutions = EVOLUTION\(", body)),
        }
icon_paths = dict(re.findall(r"const u8 (gMonIcon_\w+)\[\] = INCBIN_U8\(\s*\"(graphics/pokemon/[^\"]+?)/icon\.4bpp\"\)",
                             rd("src/data/graphics/pokemon.h")))

base_by_dex = {}
for name, sid in SPECIES.items():
    info = species_src.get(name)
    if info and info["natdex"] and NATDEX.get(info["natdex"]) is not None:
        base_by_dex.setdefault(NATDEX[info["natdex"]], name)


def display_name(const, rom_name):
    """forms get a Showdown-style suffix: SPECIES_ROTOM_WASH -> Rotom-Wash"""
    info = species_src[const]
    base = base_by_dex.get(NATDEX[info["natdex"]], const)
    if const == base or not const.startswith(base + "_"):
        return rom_name
    suffix = const[len(base) + 1:].replace("_", " ").title().replace(" ", "-")
    return rom_name + "-" + suffix


species = {}
for name, sid in SPECIES.items():
    if sid <= 0 or sid * SSTRIDE >= ssize or name in ("SPECIES_EGG",):
        continue
    info = species_src.get(name)
    if not info or not info["natdex"] or NATDEX.get(info["natdex"], 9999) > 721:
        continue
    if re.search(r"_(GMAX|TOTEM|ALOLA|GALAR|HISUI|PALDEA\w*|STARTER|COSPLAY|ROCK_STAR|BELLE|POP_STAR|PHD|LIBRE|\w*_CAP)$", name):
        continue
    a = sbase + sid * SSTRIDE
    stats = [r8(a + i) for i in range(6)]
    if sum(stats) == 0:
        continue
    abil = [r16(a + 24 + 2 * i) for i in range(3)]
    species[sid] = {
        "id": sid, "const": name, "name": display_name(name, text(a + SNAME, 12)), "dex": NATDEX[info["natdex"]],
        "bs": stats, "types": [TYPES[r8(a + 6)], TYPES[r8(a + 7)]],
        "abilities": abil, "weight": int(info["weight"] or 0), "evolves": info["evolves"], "icon": info["icon"],
    }

# ------------------------------------------------------------------ abilities / items
abase, asize = syms["gAbilitiesInfo"]
ASTRIDE = 28
abilities = {i: text(abase + i * ASTRIDE, 17) for i in range(asize // ASTRIDE)}

ibase, isize = syms["gItemsInfo"]
ISTRIDE = 80
items = {}
for i in range(1, isize // ISTRIDE):
    a = ibase + i * ISTRIDE
    name = text(a + 20, 20)
    if not name or name == "????????":
        continue
    he = r8(a + 62)
    pocket = r8(a + 65)
    items[i] = {"name": name, "hold": HOLD_NAME.get(he, "HOLD_EFFECT_NONE").replace("HOLD_EFFECT_", ""),
                "param": r8(a + 63), "sid": r16(a + 4), "fling": r8(a + 68), "berry": pocket == 4}

# ------------------------------------------------------------------ moves
mbase, msize = syms["gMovesInfo"]
MSTRIDE = 52
CATS = ["Physical", "Special", "Status"]
gen6_count = MOVES.get("MOVES_COUNT_GEN6", 622)


def move_entry(mid):
    a = mbase + mid * MSTRIDE
    w10, w12, w16, w20 = r16(a + 10), r16(a + 12), r32(a + 20), r32(a + 24)
    prio = w16 & 0xF
    if prio >= 8:
        prio -= 16
    return {
        "id": mid, "name": text(r32(a)) if ROM_BASE <= r32(a) < ROM_BASE + len(rom) else "",
        "effect": EFFECT_NAME.get(r16(a + 8), "EFFECT_HIT").replace("EFFECT_", ""),
        "type": TYPES[w10 & 0x1F], "cat": CATS[(w10 >> 5) & 3], "bp": (w10 >> 7) & 0x1FF,
        "acc": w12 & 0x7F, "target": TARGET_NAME.get((w12 >> 7) & 0x1FF, "").replace("MOVE_TARGET_", ""),
        "pp": r8(a + 14), "prio": prio, "recoil": (w16 >> 4) & 0x7F, "hits": (w16 >> 11) & 0xF,
        "crit": (w16 >> 15) & 3, "alwaysCrit": (w16 >> 17) & 1, "secondary": (w16 >> 18) & 3,
        "contact": (w16 >> 20) & 1, "punch": (w16 >> 25) & 1, "bite": (w16 >> 26) & 1, "pulse": (w16 >> 27) & 1,
        "sound": (w16 >> 28) & 1, "ballistic": (w16 >> 29) & 1,
        "ignoreDefStages": (w20 >> 5) & 1, "airborne2x": (w20 >> 9) & 1,
    }


moves = {}
wanted = set(range(1, gen6_count)) | {MOVES["MOVE_WATER_BALL"]}

# ------------------------------------------------------------------ trainers (Splits 1-3, fight order)
tsrc = rd("src/data/trainers.h")
psrc = rd("src/data/battle_partners.h") if os.path.exists(os.path.join(ROOT, "src/data/battle_partners.h")) else ""


def reorder(v):
    return [v[0], v[1], v[2], v[4], v[5], v[3]]


def parse_trainer(src, tid):
    m = re.search(r"^    \[%s\] =\n    \{\n(.*?)^    \},\n" % tid, src, re.S | re.M)
    if not m:
        return None
    body = re.sub(r"^#line.*\n", "", m.group(1), flags=re.M)
    tname = re.search(r'\.trainerName = _\("([^"]*)"\)', body)
    tclass = re.search(r"\.trainerClass = (TRAINER_CLASS_\w+)", body)
    mons = []
    for mm in re.finditer(r"\{\n\s*\.species = (SPECIES_\w+),(.*?)\.moves = \{(.*?)\},\n\s*\},", body, re.S):
        mb = mm.group(2)
        g = lambda pat, d=None: (re.search(pat, mb).group(1) if re.search(pat, mb) else d)
        ivs = re.search(r"TRAINER_PARTY_IVS\(([^)]*)\)", mb)
        evs = re.search(r"TRAINER_PARTY_EVS\(([^)]*)\)", mb)
        mv = [x.strip().rstrip(",") for x in mm.group(3).split("\n") if x.strip().startswith("MOVE_")]
        mons.append({
            "species": SPECIES.get(mm.group(1), 0),
            "level": int(g(r"\.lvl = (\d+)", "1")),
            "item": ITEMS.get(g(r"\.heldItem = (ITEM_\w+)", "ITEM_NONE"), 0),
            "ability": ABILS.get(g(r"\.ability = (ABILITY_\w+)", "ABILITY_NONE"), 0),
            "nature": g(r"\.nature = NATURE_(\w+)", "HARDY").title(),
            # macros take (hp, atk, def, speed, spatk, spdef); export as HP/Atk/Def/SpA/SpD/Spe
            "ivs": reorder([int(x) for x in ivs.group(1).split(",")]) if ivs else [0] * 6,
            "evs": reorder([int(x) for x in evs.group(1).split(",")]) if evs else [0] * 6,
            "moves": [MOVES.get(x, 0) for x in mv],
        })
        for x in mv:
            if x in MOVES:
                wanted.add(MOVES[x])
    return {"id": tid, "name": tname.group(1) if tname else tid,
            "class": (tclass.group(1)[len("TRAINER_CLASS_"):].replace("_", " ").title() if tclass else ""),
            "double": ".doubleBattle = TRUE" in body, "mons": mons}


trainers = []
for split, fights in PLAN.FIGHT_ORDER.items():
    for entry in fights:
        tid, mapname, status, fmt, note = entry
        t = parse_trainer(tsrc, tid)
        if not t:
            continue
        t.update({"split": split, "map": mapname, "format": fmt, "note": note})
        trainers.append(t)

# tag / two-trainer fights: consecutive "Tag ..." entries on the same map are one battle
for k in range(len(trainers) - 1):
    t, u = trainers[k], trainers[k + 1]
    if "pair" not in t and t["format"].startswith("Tag") and u["format"].startswith("Tag") and t["map"] == u["map"]:
        t["pair"], u["pair"] = k + 1, k
# your partner's team in "Tag with partner" fights (the rival, Brendan or May depending on your character)
partners = []
for pid in ("PARTNER_RIVAL_BRENDAN", "PARTNER_RIVAL_MAY"):
    pt = parse_trainer(psrc, pid)
    if pt:
        partners.append(pt)
for t in trainers:
    if t["format"] == "Tag with partner":
        t["partners"] = list(range(len(partners)))

for mid in sorted(wanted):
    if mid * MSTRIDE < msize:
        e = move_entry(mid)
        if e["name"]:
            moves[mid] = e

# ------------------------------------------------------------------ type chart
tbase, tsize = syms["gTypeEffectivenessTable"]
n = len(TYPES)
esize = tsize // (n * n)
chart = {}
for atk in range(1, n):
    for dfn in range(1, n):
        a = tbase + (atk * n + dfn) * esize
        v = (r32(a) if esize == 4 else r16(a)) / 4096
        if v != 1.0:
            chart.setdefault(TYPES[atk], {})[TYPES[dfn]] = v

# ------------------------------------------------------------------ icons
try:
    from PIL import Image
    ids = sorted(species)
    missing_icons = []
    cols = 32
    sheet = Image.new("RGBA", (cols * 32, ((len(ids) + cols - 1) // cols) * 32), (0, 0, 0, 0))
    for k, sid in enumerate(ids):
        path = icon_paths.get(species[sid]["icon"] or "")
        if not path:  # forms without their own icon use the base form's
            base = base_by_dex.get(species[sid]["dex"])
            path = icon_paths.get((species_src.get(base) or {}).get("icon") or "")
        if not path:  # e.g. Arceus forms point at per-type icons that only exist with unique form icons enabled
            stem = re.sub(r"[A-Z][a-z]+$", "", species[sid]["icon"] or "") or ("gMonIcon_" + re.sub(r"[^A-Za-z]", "", species[sid]["name"].split("-")[0]))
            path = next((icon_paths[k] for k in sorted(icon_paths) if k.startswith(stem)), None)
        species[sid]["icon"] = k
        if not path or not os.path.exists(os.path.join(ROOT, path, "icon.png")):
            missing_icons.append(species[sid]["name"])
            continue
        im = Image.open(os.path.join(ROOT, path, "icon.png"))
        if im.mode == "P":
            im.info["transparency"] = 0
        im = im.convert("RGBA").crop((0, 0, 32, 32))
        sheet.paste(im, ((k % cols) * 32, (k // cols) * 32))
    sheet.save(os.path.join(OUT, "icons.png"), optimize=True)
    print("species without an icon:", missing_icons or "none")
except ImportError:
    print("Pillow missing: icons.png not written")

data = {
    "types": TYPES[1:20], "chart": chart,
    "species": list(species.values()), "moves": list(moves.values()),
    "items": {str(k): v for k, v in items.items()}, "abilities": {str(k): v for k, v in abilities.items() if v},
    "trainers": trainers, "partners": partners,
}
json.dump(data, open(os.path.join(OUT, "data.json"), "w", encoding="utf-8"), separators=(",", ":"))
print(f"species {len(species)}, moves {len(moves)}, items {len(items)}, abilities {len(abilities)}, trainers {len(trainers)}, "
      f"type entries {sum(len(v) for v in chart.values())}, type cell size {esize}")
