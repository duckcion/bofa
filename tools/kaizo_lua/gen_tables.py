"""Regenerate the GENERATED block of bofa_kaizo_tools.lua from the built ROM.

Pulls RAM addresses from pokeemerald.elf (arm-none-eabi-nm, via WSL on Windows) and the
move/item/ability name tables + move max PP straight out of pokeemerald.gba, so the Lua
tool always matches the current build. Run after every build that changes C code or data:
    python tools/kaizo_lua/gen_tables.py
"""
import os, re, struct, subprocess, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
LUA = os.path.join(ROOT, "bofa_kaizo_tools.lua")
ROM_BASE = 0x08000000
SYMS = ["gPlayerParty", "gPlayerPartyCount", "gPokemonStoragePtr", "gBattleWeather",
        "gMovesInfo", "gItemsInfo", "gAbilitiesInfo", "gSpeciesInfo"]


def nm():
    cmd = "arm-none-eabi-nm -S pokeemerald.elf"
    if os.name == "nt":
        wsl_root = "/mnt/" + ROOT[0].lower() + ROOT[2:].replace("\\", "/")
        out = subprocess.run(["wsl.exe", "-d", "Ubuntu", "-e", "bash", "-lc", f"cd '{wsl_root}' && {cmd}"],
                             capture_output=True, text=True).stdout
    else:
        out = subprocess.run(cmd.split(), cwd=ROOT, capture_output=True, text=True).stdout
    syms = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and p[3] in SYMS:
            syms[p[3]] = (int(p[0], 16), int(p[1], 16))
    missing = [s for s in SYMS if s not in syms]
    if missing: sys.exit(f"symbols not found: {missing}")
    return syms


def charmap():
    table = {}
    for line in open(os.path.join(ROOT, "charmap.txt"), encoding="utf-8"):
        m = re.match(r"^'(.)'\s*=\s*([0-9A-Fa-f]{2})\s*$", line.split("@")[0].rstrip())
        if m and int(m.group(2), 16) not in table:
            table[int(m.group(2), 16)] = m.group(1)
    table[0x00] = " "
    return table


def main():
    syms = nm()
    rom = open(os.path.join(ROOT, "pokeemerald.gba"), "rb").read()
    cm = charmap()
    r8 = lambda a: rom[a - ROM_BASE]
    r32 = lambda a: struct.unpack_from("<I", rom, a - ROM_BASE)[0]

    def text(a, maxlen=40):
        out = []
        for i in range(maxlen):
            b = r8(a + i)
            if b == 0xFF: break
            out.append(cm.get(b, "?"))
        return "".join(out).replace("\\", "").replace('"', "'")

    # moves: name pointer at offset 0; find stride + pp offset from known vanilla PP values
    mbase, msize = syms["gMovesInfo"]
    known_pp = {1: 35, 2: 25, 3: 10, 4: 15, 5: 20, 6: 20, 7: 15, 10: 35, 33: 35}
    found = None
    for stride in range(12, 64, 4):
        if msize % stride: continue
        for off in range(8, stride):
            if all(r8(mbase + mid * stride + off) == pp for mid, pp in known_pp.items()):
                found = (stride, off); break
        if found: break
    if not found: sys.exit("could not locate MoveInfo pp field")
    mstride, ppoff = found
    moves = []
    for mid in range(msize // mstride):
        p = r32(mbase + mid * mstride)
        name = text(p) if ROM_BASE <= p < ROM_BASE + len(rom) else ""
        moves.append((mid, name, r8(mbase + mid * mstride + ppoff)))

    ibase, isize = syms["gItemsInfo"]
    istride = 80
    assert isize % istride == 0
    items = [(iid, text(ibase + iid * istride + 20, 20)) for iid in range(isize // istride)]

    abase, asize = syms["gAbilitiesInfo"]
    astride = next(s for s in range(20, 64, 4) if asize % s == 0 and text(abase + s, 17) == "Stench"
                   and text(abase + 2 * s, 17) == "Drizzle")
    abilities = [(aid, text(abase + aid * astride, 17)) for aid in range(asize // astride)]

    def lua_names(rows):
        return ",".join(f'[{i}]="{n}"' for i, n in rows if n)

    block = [
        "-- BEGIN GENERATED (tools/kaizo_lua/gen_tables.py) -- do not edit by hand",
        "local ADDR = {",
        f"    gPlayerParty       = 0x{syms['gPlayerParty'][0]:08x},",
        f"    gPlayerPartyCount  = 0x{syms['gPlayerPartyCount'][0]:08x},",
        f"    gPokemonStoragePtr = 0x{syms['gPokemonStoragePtr'][0]:08x},",
        f"    gBattleWeather     = 0x{syms['gBattleWeather'][0]:08x},",
        f"    gSpeciesInfoBase   = 0x{syms['gSpeciesInfo'][0]:08x},",
        "}",
        "",  # SpeciesInfo stride/name offset, filled in below
        "local MOVE_NAMES = {" + lua_names([(i, n) for i, n, _ in moves]) + "}",
        "local MOVE_PP = {" + ",".join(f"[{i}]={pp}" for i, n, pp in moves if n) + "}",
        "local ITEM_NAMES = {" + lua_names(items) + "}",
        "local ABILITY_NAMES = {" + lua_names(abilities) + "}",
        "-- END GENERATED",
    ]
    # SpeciesInfo stride: sizeof table / entry count; entry count from the Bulbasaur name search
    sbase, ssize = syms["gSpeciesInfo"]
    bulba = bytes(next(k for k, v in cm.items() if v == c) for c in "Bulbasaur") + b"\xff"
    sstride = None
    for s in range(100, 400, 4):
        if ssize % s: continue
        nm_off = rom.find(bulba, sbase + s - ROM_BASE, sbase + 2 * s - ROM_BASE)
        if nm_off < 0: continue
        off = nm_off + ROM_BASE - (sbase + s)
        ivy = bytes(next(k for k, v in cm.items() if v == c) for c in "Ivysaur") + b"\xff"
        if rom[sbase + 2 * s + off - ROM_BASE: sbase + 2 * s + off - ROM_BASE + len(ivy)] == ivy:
            sstride, name_off = s, off; break
    if not sstride: sys.exit("could not determine SpeciesInfo stride")
    block[8] = f"local SPECIES_INFO_STRIDE = {sstride}\nlocal SPECIES_NAME_OFFSET = {name_off}"

    src = open(LUA, encoding="utf-8").read()
    new = re.sub(r"-- BEGIN GENERATED.*?-- END GENERATED", lambda _: "\n".join(block), src, flags=re.S)
    assert new != src or "-- BEGIN GENERATED" in src, "markers missing"
    open(LUA, "w", encoding="utf-8", newline="\n").write(new)
    print(f"moves {len(moves)} (stride {mstride}, pp @{ppoff}), items {len(items)}, abilities {len(abilities)} "
          f"(stride {astride}), species stride {sstride} name @{name_off}")


if __name__ == "__main__":
    main()
