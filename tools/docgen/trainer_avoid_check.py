"""Which Split 1-3 trainers can be walked around?

For each map in FIGHT_ORDER, a trainer is avoidable if every exit the player could reach before
(warps and map-edge connections) is still reachable when that trainer's tile and sight line are
treated as walls. Sight follows the trainer's facing (LOOK_AROUND etc. = all four directions)
and stops at the first blocked tile.

    python tools/docgen/trainer_avoid_check.py
"""
import json, os, re, struct, sys
from collections import deque

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.dirname(__file__))
import split1_plan as P  # noqa: E402

L = {l["id"]: l for l in json.load(open(os.path.join(ROOT, "data/layouts/layouts.json")))["layouts"] if "id" in l}


def attrs(sym, kind):
    n = sym.replace("gTileset_", "")
    for c in (n.lower(), re.sub(r"(?<!^)(?=[A-Z])", "_", n).lower()):
        p = os.path.join(ROOT, f"data/tilesets/{kind}/{c}/metatile_attributes.bin")
        if os.path.exists(p):
            return open(p, "rb").read()


FACING = {"UP": [(0, -1)], "DOWN": [(0, 1)], "LEFT": [(-1, 0)], "RIGHT": [(1, 0)]}


def facing_dirs(mt):
    for k, v in FACING.items():
        if mt.endswith("FACE_" + k) or mt.endswith("_" + k) and "FACE" in mt:
            return v
    return [(0, -1), (0, 1), (-1, 0), (1, 0)]  # look around / wander: assume all directions


def analyse(mapname, trainer_scripts):
    j = json.load(open(os.path.join(ROOT, f"data/maps/{mapname}/map.json")))
    lay = L[j["layout"]]
    W, H = lay["width"], lay["height"]
    b = open(os.path.join(ROOT, lay["blockdata_filepath"]), "rb").read()
    pa, sa = attrs(lay["primary_tileset"], "primary"), attrs(lay["secondary_tileset"], "secondary")
    t = lambda x, y: struct.unpack_from("<H", b, (y * W + x) * 2)[0]

    def beh(x, y):
        m = t(x, y) & 0x3FF
        return (struct.unpack_from("<H", pa, m * 2)[0] if m < 512 else struct.unpack_from("<H", sa, (m - 512) * 2)[0]) & 0xFF

    objs = {(o["x"], o["y"]) for o in j["object_events"] if o["graphics_id"] not in ("OBJ_EVENT_GFX_CUTTABLE_TREE", "OBJ_EVENT_GFX_BREAKABLE_ROCK")}
    ok = lambda x, y: 0 <= x < W and 0 <= y < H and not (t(x, y) >> 10) & 3 and not 0x10 <= beh(x, y) <= 0x15

    exits = set()
    for c in j.get("connections") or []:
        d = c["direction"]
        exits |= set({"up": [(x, 0) for x in range(W)], "down": [(x, H - 1) for x in range(W)],
                      "left": [(0, y) for y in range(H)], "right": [(W - 1, y) for y in range(H)]}.get(d, []))
    exits |= {(w["x"], w["y"]) for w in j["warp_events"]}
    # gym leaders and other talk-to bosses are goals too: the tiles next to them
    for o in j["object_events"]:
        if o["graphics_id"] in ("OBJ_EVENT_GFX_ROXANNE", "OBJ_EVENT_GFX_BRAWLY", "OBJ_EVENT_GFX_NORMAN", "OBJ_EVENT_GFX_ARCHIE") \
                or o["script"] == "ViridianForest_EventScript_RockSmashGiver":
            exits |= {(o["x"] + dx, o["y"] + dy) for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))}
    exits = {e for e in exits if ok(*e) and e not in objs}

    def reach(start, blocked):
        seen = {s for s in start if s not in blocked}
        q = deque(seen)
        while q:
            x, y = q.popleft()
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                n = (x + dx, y + dy)
                if n not in seen and ok(*n) and n not in objs and n not in blocked:
                    seen.add(n); q.append(n)
        return seen

    base = reach(exits, set())
    results = []
    for o in j["object_events"]:
        if o["script"] not in trainer_scripts or o["graphics_id"] in ("OBJ_EVENT_GFX_ROXANNE", "OBJ_EVENT_GFX_BRAWLY", "OBJ_EVENT_GFX_NORMAN"):
            continue  # gym leaders are the goal, not something to walk past
        sight = int(o["trainer_sight_or_berry_tree_id"]) if str(o["trainer_sight_or_berry_tree_id"]).isdigit() else 0
        tiles = {(o["x"], o["y"])}
        for dx, dy in facing_dirs(o["movement_type"]):
            for k in range(1, sight + 1):
                x, y = o["x"] + dx * k, o["y"] + dy * k
                if not ok(x, y) or (x, y) in objs:
                    break
                tiles.add((x, y))
        # avoidable if, with the sight line walled off, all reachable exits are still mutually reachable
        reachable_exits = [e for e in exits if e in base and e not in tiles]
        avoidable = True
        if reachable_exits:
            comp = reach([reachable_exits[0]], tiles)
            avoidable = all(e in comp for e in reachable_exits)
        results.append((o["script"], (o["x"], o["y"]), o["movement_type"].replace("MOVEMENT_TYPE_", ""), sight, avoidable))
    return results


def main():
    by_map = {}
    for split, fights in P.FIGHT_ORDER.items():
        for tid, mapname, status, fmt, note in fights:
            by_map.setdefault(mapname, []).append(tid)
    out = []
    for mapname, tids in by_map.items():
        scripts = open(os.path.join(ROOT, f"data/maps/{mapname}/scripts.inc"), encoding="utf-8", errors="replace").read()
        tscripts = {}
        blocks = re.split(r"^(\w+)::\s*$", scripts, flags=re.M)
        for lab, body in zip(blocks[1::2], blocks[2::2]):
            for tid in re.findall(r"trainerbattle\w*\s+(TRAINER_\w+)", body):
                if tid in tids:
                    tscripts.setdefault(lab, tid)
        for script, pos, mt, sight, avoidable in analyse(mapname, tscripts):
            out.append((mapname, tscripts[script], pos, mt, sight, avoidable))
    for r in out:
        print(("AVOIDABLE " if r[5] else "forced    "), r[0], r[1], r[2], r[3], "sight", r[4])
    return out


if __name__ == "__main__":
    main()
