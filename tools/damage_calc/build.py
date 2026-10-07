"""Build tools/damage_calc/bofa_calc.html from template.html + data.json + icons.png.

    python tools/damage_calc/export_data.py   # after a ROM build
    python tools/damage_calc/build.py
"""
import base64, os

HERE = os.path.dirname(os.path.abspath(__file__))
tpl = open(os.path.join(HERE, "template.html"), encoding="utf-8").read()
data = open(os.path.join(HERE, "data.json"), encoding="utf-8").read()
icons = base64.b64encode(open(os.path.join(HERE, "icons.png"), "rb").read()).decode()
assert "/*__DATA__*/null" in tpl and "__ICONS__" in tpl
out = tpl.replace("/*__DATA__*/null", data.replace("</", "<\\/")).replace("__ICONS__", "data:image/png;base64," + icons)
open(os.path.join(HERE, "bofa_calc.html"), "w", encoding="utf-8").write(out)
print("bofa_calc.html", round(len(out) / 1024), "KB")
