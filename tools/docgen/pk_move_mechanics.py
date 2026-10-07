"""Apply Platinum Kaizo's move mechanic changes (Move Changes sheet) to src/data/moves_info.h.

BP/accuracy/PP were applied earlier; this pass applies effects, effect chances, targets,
categories, priority and descriptions. Moves PK *replaced* with a different move
(e.g. Bind -> Mystical Fire) are not touched here. Idempotent: safe to re-run.
"""
import re, sys, textwrap

ROOT = __file__.replace("\\", "/").rsplit("/tools/", 1)[0] + "/"
PATH = ROOT + "src/data/moves_info.h"

PARA, BRN, FRZ, PSN, TOX, CONF, FLINCH, TRI, WRAP = ("MOVE_EFFECT_PARALYSIS", "MOVE_EFFECT_BURN",
    "MOVE_EFFECT_FREEZE_OR_FROSTBITE", "MOVE_EFFECT_POISON", "MOVE_EFFECT_TOXIC", "MOVE_EFFECT_CONFUSION",
    "MOVE_EFFECT_FLINCH", "MOVE_EFFECT_TRI_ATTACK", "MOVE_EFFECT_WRAP")

# move: dict(set={field: value}, drop=[fields], add=[(moveEffect, chance)] or [] to clear, desc=[lines])
S = {
 "DOUBLE_SLAP":  dict(set={"effect": "EFFECT_HIT", "strikeCount": "2"}, desc=["Slaps the foe", "twice in a row."]),
 "VISE_GRIP":    dict(set={"type": "TYPE_WATER"}, add=[(WRAP, 0)], desc=["Grips the foe and", "traps it for", "several turns."]),
 "SLAM":         dict(add=[(PARA, 30)], desc=["Slams the foe.", "May cause", "paralysis."]),
 "THRASH":       dict(set={"effect": "EFFECT_HIT", "target": "MOVE_TARGET_SELECTED", "recoil": "33"}, add=[(PARA, 20)],
                      desc=["A reckless attack", "that may paralyze.", "User takes recoil."]),
 "AURORA_BEAM":  dict(set={"accuracy": "0"}, add=[], desc=["A rainbow beam", "that never misses."]),
 "HYPER_BEAM":   dict(set={"recoil": "50"}, add=[], desc=["A huge beam. The", "user takes 1/2 of", "the damage dealt."]),
 "DRILL_PECK":   dict(set={"criticalHitStage": "1"}, desc=["A drilling peck.", "High critical-", "hit ratio."]),
 "SUBMISSION":   dict(set={"recoil": None}, add=[(WRAP, 0)], desc=["Grapples the foe", "and traps it for", "several turns."]),
 "PETAL_DANCE":  dict(set={"effect": "EFFECT_HIT", "target": "MOVE_TARGET_BOTH"}, add=[(CONF, 20)],
                      desc=["Hits both foes", "with petals. May", "confuse them."]),
 "DIG":          dict(set={"effect": "EFFECT_HIT", "argument": None}, desc=["Strikes the foe", "from underground."]),
 "RAGE":         dict(set={"effect": "EFFECT_PURSUIT", "type": "TYPE_FIGHTING"}, add=[],
                      desc=["Hits a foe that is", "switching out for", "double damage."]),
 "BIDE":         dict(set={"effect": "EFFECT_METAL_BURST", "type": "TYPE_DARK", "power": "1", "accuracy": "100",
                           "target": "MOVE_TARGET_DEPENDS", "priority": "-1", "category": "DAMAGE_CATEGORY_PHYSICAL",
                           "argument": None}, add=[], desc=["Returns 1.5x the", "damage of the", "foe's last hit."]),
 "CONSTRICT":    dict(set={"effect": "EFFECT_HIT", "category": "DAMAGE_CATEGORY_SPECIAL", "type": "TYPE_ICE",
                           "makesContact": None}, add=[], desc=["An icy blast", "with no added", "effect."]),
 "KINESIS":      dict(set={"effect": "EFFECT_ACCURACY_DOWN_2", "target": "MOVE_TARGET_BOTH"},
                      desc=["Bends a spoon to", "sharply lower", "foes' accuracy."]),
 "POISON_GAS":   dict(set={"effect": "EFFECT_TOXIC", "target": "MOVE_TARGET_FOES_AND_ALLY"},
                      desc=["Sprays toxic gas", "that badly", "poisons."]),
 "SKY_ATTACK":   dict(set={"effect": "EFFECT_HIT", "argument": None, "criticalHitStage": None, "recoil": "33"},
                      add=[(PARA, 20)], desc=["A reckless dive", "that may paralyze.", "User takes recoil."]),
 "THIEF":        dict(add=[], desc=["A quick strike", "with no added", "effect."]),
 "AEROBLAST":    dict(set={"criticalHitStage": None}, add=[(TRI, 30)],
                      desc=["A vortex of air.", "May paralyze,", "burn or freeze."]),
 "OUTRAGE":      dict(set={"effect": "EFFECT_HIT", "target": "MOVE_TARGET_SELECTED", "recoil": "50"}, add=[],
                      desc=["A rampaging hit.", "The user takes 1/2", "of damage dealt."]),
 "ROLLOUT":      dict(set={"effect": "EFFECT_HIT"}, desc=["A quick rolling", "attack that", "strikes first."]),
 "FURY_CUTTER":  dict(set={"effect": "EFFECT_TRIPLE_KICK", "strikeCount": "3"},
                      desc=["Slashes up to 3", "times, harder", "with each hit."]),
 "RETURN":       dict(set={"effect": "EFFECT_HIT"}, desc=["A full-power", "attack with no", "added effect."]),
 "FRUSTRATION":  dict(set={"effect": "EFFECT_HIT"}, desc=["A full-power", "attack with no", "added effect."]),
 "MEGAHORN":     dict(set={"criticalHitStage": "1"}, desc=["A brutal ramming", "charge. High", "critical-hit ratio."]),
 "TWISTER":      dict(set={"damagesAirborneDoubleDamage": None}, add=[(WRAP, 0)],
                      desc=["Whips up a vortex", "that traps the", "foe for turns."]),
 "STOCKPILE":    dict(set={"effect": "EFFECT_COSMIC_POWER"}, desc=["Raises the user's", "Defense and", "Sp. Def."]),
 "SWALLOW":      dict(set={"effect": "EFFECT_HIT", "category": "DAMAGE_CATEGORY_PHYSICAL", "type": "TYPE_POISON",
                           "power": "90", "accuracy": "100", "target": "MOVE_TARGET_SELECTED", "ignoresProtect": "TRUE",
                           "healingMove": None, "snatchAffected": None, "mirrorMoveBanned": None}, add=[(WRAP, 0)],
                      desc=["Traps the foe for", "turns. Goes", "through Protect."]),
 "MEMENTO":      dict(set={"effect": "EFFECT_EXPLOSION", "category": "DAMAGE_CATEGORY_SPECIAL", "power": "255",
                           "accuracy": "100", "target": "MOVE_TARGET_SELECTED", "ignoresSubstitute": None}, add=[],
                      desc=["The user faints", "to deal massive", "damage."]),
 "SUPERPOWER":   dict(set={"recoil": "50"}, add=[], desc=["A huge attack.", "The user takes 1/2", "of damage dealt."]),
 "REVENGE":      dict(set={"effect": "EFFECT_PAYBACK", "priority": "0"},
                      desc=["Power doubles if", "the user moves", "after the foe."]),
 "YAWN":         dict(set={"effect": "EFFECT_SLEEP"}, desc=["A huge yawn that", "puts the foe", "to sleep."]),
 "ERUPTION":     dict(set={"effect": "EFFECT_HIT", "recoil": "33"}, add=[(BRN, 20)],
                      desc=["Erupts at foes.", "May burn. User", "takes recoil."]),
 "DIVE":         dict(set={"effect": "EFFECT_HIT", "argument": None}, desc=["Strikes the foe", "from the water."]),
 "ICE_BALL":     dict(set={"effect": "EFFECT_BLIZZARD"}, desc=["Hurls a ball of", "ice. Never misses", "in hail."]),
 "NEEDLE_ARM":   dict(add=[], desc=["Attacks with", "thorny arms. No", "added effect."]),
 "BLAST_BURN":   dict(set={"recoil": "33"}, add=[(BRN, 30)], desc=["A huge explosion.", "May burn. User", "takes recoil."]),
 "HYDRO_CANNON": dict(set={"recoil": "50"}, add=[], desc=["A huge blast. The", "user takes 1/2 of", "the damage dealt."]),
 "OVERHEAT":     dict(set={"recoil": "33"}, add=[(BRN, 10)], desc=["An intense blast.", "May burn. User", "takes recoil."]),
 "SKY_UPPERCUT": dict(set={"damagesAirborne": "TRUE", "damagesAirborneDoubleDamage": "TRUE"},
                      desc=["An uppercut that", "hits flying foes", "for 2x damage."]),
 "SHEER_COLD":   dict(set={"effect": "EFFECT_BLIZZARD", "power": "70", "accuracy": "50"}, add=[(FRZ, 100)],
                      desc=["Freezes the foe.", "Never misses in", "hail."]),
 "DRAGON_CLAW":  dict(set={"criticalHitStage": "1"}, desc=["Slashes with sharp", "claws. High crit-", "ical-hit ratio."]),
 "FRENZY_PLANT": dict(set={"recoil": "33"}, add=[(PARA, 20)], desc=["Hits with roots.", "May paralyze.", "User takes recoil."]),
 "BOUNCE":       dict(set={"effect": "EFFECT_HIT", "argument": None}, add=[(PARA, 30)],
                      desc=["A bouncing attack", "that may", "paralyze."]),
 "PSYCHO_BOOST": dict(add=[], desc=["A full-power", "psychic attack.", "No added effect."]),
 "CLOSE_COMBAT": dict(set={"recoil": "25"}, add=[], desc=["An all-out attack.", "The user takes 1/4", "of damage dealt."]),
 "WRING_OUT":    dict(set={"effect": "EFFECT_BRINE"}, desc=["Power doubles if", "the foe is at half", "HP or less."]),
 "SUCKER_PUNCH": dict(set={"effect": "EFFECT_HIT"}, desc=["A sneaky attack", "that always", "strikes first."]),
 "HEART_SWAP":   dict(set={"effect": "EFFECT_ABSORB", "argument": "50", "category": "DAMAGE_CATEGORY_SPECIAL",
                           "type": "TYPE_WATER", "power": "95", "accuracy": "0", "target": "MOVE_TARGET_SELECTED",
                           "ignoresProtect": None}, desc=["Drains the foe.", "Restores half the", "damage dealt."]),
 "X_SCISSOR":    dict(set={"criticalHitStage": "1"}, desc=["Slashes in an X.", "High critical-", "hit ratio."]),
 "DRAGON_PULSE": dict(set={"accuracy": "0"}, desc=["A shock wave from", "the mouth that", "never misses."]),
 "BRAVE_BIRD":   dict(set={"recoil": "25"}, desc=["A reckless dive.", "The user takes 1/4", "of damage dealt."]),
 "GIGA_IMPACT":  dict(set={"recoil": "50"}, add=[], desc=["A huge charge. User takes", "1/2 of the damage dealt."]),
 "AVALANCHE":    dict(set={"effect": "EFFECT_HIT", "priority": "0", "target": "MOVE_TARGET_BOTH"},
                      add=[(FRZ, 20), (FLINCH, 20)], desc=["Hits both foes.", "May freeze or", "flinch."]),
 "SHADOW_CLAW":  dict(set={"effect": "EFFECT_MULTI_HIT", "criticalHitStage": None},
                      desc=["Slashes the foe", "2 to 5 times."]),
 "DRACO_METEOR": dict(set={"recoil": "50"}, add=[], desc=["Comets fall. The", "user takes 1/2 of", "the damage dealt."]),
 "LEAF_STORM":   dict(add=[], desc=["A storm of leaves", "with no added", "effect."]),
 "POWER_WHIP":   dict(set={"criticalHitStage": "1"}, desc=["Lashes with vines.", "High critical-", "hit ratio."]),
 "ROCK_WRECKER": dict(set={"effect": "EFFECT_TRIPLE_KICK", "strikeCount": "3"}, add=[],
                      desc=["Hurls rocks up to", "3 times, harder", "with each hit."]),
 "CAPTIVATE":    dict(set={"effect": "EFFECT_SPECIAL_ATTACK_DOWN_2"}, desc=["Charms the foe to", "sharply lower its", "Sp. Atk."]),
 "ATTACK_ORDER": dict(set={"criticalHitStage": None, "target": "MOVE_TARGET_BOTH"}, add=[(PSN, 30)],
                      desc=["Underlings attack", "both foes. May", "poison."]),
 "LUNAR_DANCE":  dict(set={"effect": "EFFECT_RESTORE_HP", "target": "MOVE_TARGET_USER"},
                      desc=["Restores up to", "half of the user's", "max HP."]),
 "CRUSH_GRIP":   dict(set={"effect": "EFFECT_HIT"}, add=[(WRAP, 0)], desc=["Crushes the foe", "and traps it for", "several turns."]),
 "VOLT_TACKLE":  dict(add=[(PARA, 30)]),
 "CHATTER":      dict(set={"target": "MOVE_TARGET_FOES_AND_ALLY"}, add=[(CONF, 50)]),
 "GRAVITY":      dict(desc=["Gravity lasts the", "whole battle.", "Nothing can fly."]),
 "TRICK_ROOM":   dict(desc=["Slower Pokemon", "move first for", "the whole battle."]),
}
# plain effect-chance / target changes from the sheet (effect itself unchanged)
CHANCE = {"TWINEEDLE": 30, "BLIZZARD": 20, "CONFUSION": 30, "SMOG": 50, "BUBBLE": 20, "DIZZY_PUNCH": 50,
          "HYPER_FANG": 30, "TRI_ATTACK": 30, "BLAZE_KICK": 20, "POISON_FANG": 40,
          "DRAGON_RUSH": 30, "ROCK_CLIMB": 30, "CROSS_POISON": 20, "CHARGE_BEAM": 100, "SEED_FLARE": 50}
TARGET = {"SING": "MOVE_TARGET_FOES_AND_ALLY", "SUPERSONIC": "MOVE_TARGET_BOTH", "SCREECH": "MOVE_TARGET_BOTH",
          "SMOKESCREEN": "MOVE_TARGET_FOES_AND_ALLY", "SMOG": "MOVE_TARGET_BOTH", "SPIDER_WEB": "MOVE_TARGET_BOTH",
          "ATTRACT": "MOVE_TARGET_BOTH", "METAL_SOUND": "MOVE_TARGET_BOTH", "ROAR_OF_TIME": "MOVE_TARGET_BOTH"}
SETS = {"WILL_O_WISP": {"accuracy": "85"}}

FIELD_ORDER_ANCHOR = "category"   # new single-line fields go right after .category


def span(lines, i):
    """index past the end of the field starting at line i (handles multi-line parenthesised values)"""
    depth, j = 0, i
    while True:
        depth += lines[j].count("(") + lines[j].count("{") - lines[j].count(")") - lines[j].count("}")
        j += 1
        if depth <= 0 and lines[j - 1].rstrip().endswith(","):
            return j


def find(lines, f):
    for i, l in enumerate(lines):
        if re.match(r"^ {8,12}\.%s = " % f, l): return i
    return None


def set_field(lines, f, v):
    i = find(lines, f)
    if v is None:
        if i is not None: del lines[i:span(lines, i)]
        return
    new = f"        .{f} = {v},"
    if i is not None:
        lines[i:span(lines, i)] = [new]
    else:
        a = find(lines, FIELD_ORDER_ANCHOR)
        lines.insert(a + 1 if a is not None else len(lines), new)


def set_additional(lines, effects):
    i = find(lines, "additionalEffects")
    if i is not None:
        j = span(lines, i)
        # drop a wrapping #if/#endif that only guarded the additional effects
        if i > 0 and lines[i - 1].strip().startswith("#if") and j < len(lines) and lines[j].strip() == "#endif":
            del lines[j]; del lines[i - 1]; i -= 1; j -= 1
        del lines[i:j]
    else:
        i = len(lines)
    if not effects: return
    blk = ["        .additionalEffects = ADDITIONAL_EFFECTS("]
    for k, (eff, ch) in enumerate(effects):
        blk.append("        {" if k else "        {")
        blk.append(f"            .moveEffect = {eff},")
        if ch: blk.append(f"            .chance = {ch},")
        blk.append("        }" + ("," if k < len(effects) - 1 else ""))
    blk[-1] = blk[-1]
    blk.append("        ),")
    # match the file's usual `ADDITIONAL_EFFECTS({ ... })` style for a single effect
    if len(effects) == 1:
        body = [l for l in blk[2:-2]]
        blk = ["        .additionalEffects = ADDITIONAL_EFFECTS({"] + body + ["        }),"]
    lines[i:i] = blk


def set_desc(lines, desc):
    i = find(lines, "description")
    body = ["        .description = COMPOUND_STRING("]
    text = " ".join(desc).replace("- ", "-").replace("crit-ical", "critical")
    desc = textwrap.wrap(text, 28)
    assert len(desc) <= 2, desc
    for k, l in enumerate(desc):
        body.append(f'            "{l}' + ('\\n"' if k < len(desc) - 1 else '"),'))
    if i is not None: lines[i:span(lines, i)] = body
    else: lines[1:1] = body


def main():
    raw = open(PATH, encoding="utf-8", newline="").read()
    nl = "\r\n" if "\r\n" in raw else "\n"
    src = raw.replace("\r\n", "\n")
    todo = {}
    for mv, spec in S.items(): todo.setdefault(mv, {}).update(spec)
    for mv, ch in CHANCE.items(): todo.setdefault(mv, {})["chance"] = ch
    for mv, t in TARGET.items(): todo.setdefault(mv, {}).setdefault("set", {})["target"] = t
    for mv, d in SETS.items(): todo.setdefault(mv, {}).setdefault("set", {}).update(d)
    for mv, spec in todo.items():
        m = re.search(r"^    \[MOVE_%s\] =\n    \{\n(.*?)^    \},?\n" % mv, src, re.S | re.M)
        assert m, mv
        lines = m.group(1).rstrip("\n").split("\n")
        for f, v in spec.get("set", {}).items(): set_field(lines, f, v)
        if "add" in spec: set_additional(lines, spec["add"])
        if "chance" in spec:
            i = find(lines, "additionalEffects"); assert i is not None, mv
            seg = lines[i:span(lines, i)]
            # collapse any #if'd chance branches into one plain line
            out, skip, done = [], False, False
            for l in seg:
                s = l.strip()
                if s.startswith("#if") and not done: skip = True; continue
                if skip and s.startswith(("#elif", "#else")): continue
                if skip and s == "#endif": skip = False; continue
                if ".chance =" in s:
                    if not done: out.append(re.sub(r"\.chance = .*?,", f".chance = {spec['chance']},", l)); done = True
                    continue
                out.append(l)
            assert done, mv
            lines[i:span(lines, i)] = out
        if "desc" in spec: set_desc(lines, spec["desc"])
        src = src[:m.start(1)] + "\n".join(lines) + "\n" + src[m.end(1):]
    open(PATH, "w", encoding="utf-8", newline="").write(src.replace("\n", nl))
    print("updated", len(todo), "moves")


if __name__ == "__main__":
    main()
