"""Render one of the audit Markdown reports as a self-contained HTML page.

Usage:
    python3 tools/docgen/md_to_html.py <input.md> [output.html]

Produces a single file with a table of contents and one collapsible <details>
block per `##` section. No external assets, no CDN, so it opens offline.
Handles the subset of Markdown these reports actually use: headings, tables,
ordered and unordered lists, blockquotes, horizontal rules, inline code, bold,
italic.
"""

import datetime
import html
import os
import re
import sys

STYLE = """<style>
:root{--bg:#fff;--fg:#1a1a1a;--mut:#5b6472;--line:#d8dde5;--acc:#1f3864;--card:#f6f8fb;--warn:#fff4e5}
@media (prefers-color-scheme:dark){:root:not([data-theme=light]){--bg:#14171c;--fg:#e7eaf0;--mut:#9aa4b2;--line:#2b313a;--acc:#7da7e8;--card:#1b1f26;--warn:#2e2618}}
:root[data-theme=dark]{--bg:#14171c;--fg:#e7eaf0;--mut:#9aa4b2;--line:#2b313a;--acc:#7da7e8;--card:#1b1f26;--warn:#2e2618}
*{box-sizing:border-box}
body{background:var(--bg);color:var(--fg);font:15px/1.6 -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif;padding-block:28px;padding-left:18px;padding-right:18px;max-width:1080px;margin:0 auto}
h1{font-size:1.7rem;margin:0 0 .2em;color:var(--acc)}
h3{font-size:1.02rem;margin:1.5em 0 .4em;color:var(--acc)}
p{margin:.6em 0}ul,ol{margin:.5em 0 .8em 1.3em}li{margin:.25em 0}
code{background:var(--card);border:1px solid var(--line);border-radius:4px;padding:.05em .35em;font-size:.86em;word-break:break-word}
blockquote{margin:.9em 0;padding:.7em .9em;background:var(--warn);border-left:4px solid #e8a33d;border-radius:0 6px 6px 0}
hr{border:0;border-top:1px solid var(--line);margin:1.4em 0}
.toc{background:var(--card);border:1px solid var(--line);border-radius:10px;padding:.9em 1.1em;margin:1.2em 0}
.toc h2{font-size:.78rem;text-transform:uppercase;letter-spacing:.09em;color:var(--mut);margin:0 0 .5em}
.toc ol{margin:0;padding-left:1.3em}.toc a{color:var(--acc);text-decoration:none}
.toc a:hover{text-decoration:underline}
details{border:1px solid var(--line);border-radius:10px;margin:.7em 0;background:var(--card);overflow:hidden}
summary{cursor:pointer;padding:.75em 1em;font-weight:600;color:var(--acc);list-style:none}
summary::-webkit-details-marker{display:none}
summary::before{content:"\\25B8";display:inline-block;margin-right:.6em;transition:transform .15s;color:var(--mut)}
details[open]>summary::before{transform:rotate(90deg)}
.inner{padding:0 1em 1em;border-top:1px solid var(--line)}
.tw{overflow-x:auto;margin:.8em 0}
table{border-collapse:collapse;width:100%;font-size:.87rem;min-width:420px}
th,td{border:1px solid var(--line);padding:.4em .6em;text-align:left;vertical-align:top}
th{background:var(--acc);color:#fff;position:sticky;top:0}
tbody tr:nth-child(even){background:rgba(127,127,127,.06)}
.bar{display:flex;gap:.5em;flex-wrap:wrap;margin:1em 0}
button{font:inherit;padding:.4em .8em;border:1px solid var(--line);border-radius:7px;background:var(--bg);color:var(--fg);cursor:pointer}
button:hover{border-color:var(--acc)}
footer{margin-top:2em;padding-top:1em;border-top:1px solid var(--line);color:var(--mut);font-size:.82rem}
</style>"""


def inline(s):
    s = html.escape(s)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    s = re.sub(r"\*\*([^*]+)\*\*", r"<strong>\1</strong>", s)
    s = re.sub(r"(?<!\*)\*([^*]+)\*(?!\*)", r"<em>\1</em>", s)
    return s


def is_table_sep(line):
    stripped = line.replace("|", "").strip()
    return stripped and set(stripped) <= set("-: ")


ORDERED = re.compile(r"^\d+\. ")


def render(block):
    out, i = [], 0
    while i < len(block):
        ln = block[i]
        if ln.startswith("|") and i + 1 < len(block) and is_table_sep(block[i + 1]):
            hdr = [c.strip() for c in ln.strip("|").split("|")]
            i += 2
            rows = []
            while i < len(block) and block[i].startswith("|"):
                rows.append([c.strip() for c in block[i].strip("|").split("|")])
                i += 1
            out.append('<div class="tw"><table><thead><tr>'
                       + "".join(f"<th>{inline(h)}</th>" for h in hdr)
                       + "</tr></thead><tbody>")
            for r in rows:
                out.append("<tr>" + "".join(f"<td>{inline(c)}</td>" for c in r) + "</tr>")
            out.append("</tbody></table></div>")
            continue
        if ln.startswith("### "):
            out.append(f"<h3>{inline(ln[4:])}</h3>")
            i += 1
            continue
        if ln.startswith(">"):
            buf = []
            while i < len(block) and block[i].startswith(">"):
                buf.append(block[i].lstrip(">").strip())
                i += 1
            out.append("<blockquote>" + inline(" ".join(buf)) + "</blockquote>")
            continue
        if ORDERED.match(ln.strip()):
            items = []
            while i < len(block) and ORDERED.match(block[i].strip()):
                items.append(ORDERED.sub("", block[i].strip()))
                i += 1
            out.append("<ol>" + "".join(f"<li>{inline(x)}</li>" for x in items) + "</ol>")
            continue
        if ln.strip().startswith("- "):
            items = []
            while i < len(block) and block[i].strip().startswith("- "):
                items.append(block[i].strip()[2:])
                i += 1
            out.append("<ul>" + "".join(f"<li>{inline(x)}</li>" for x in items) + "</ul>")
            continue
        if ln.strip() == "---":
            out.append("<hr>")
            i += 1
            continue
        if not ln.strip():
            i += 1
            continue
        para = []
        while (i < len(block) and block[i].strip()
               and not block[i].startswith(("#", "|", ">", "- "))
               and block[i].strip() != "---"
               and not ORDERED.match(block[i].strip())):
            para.append(block[i].strip())
            i += 1
        if para:
            out.append("<p>" + inline(" ".join(para)) + "</p>")
    return "\n".join(out)


def slug(t):
    return re.sub(r"[^a-z0-9]+", "-", t.lower()).strip("-")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    src = sys.argv[1]
    dst = sys.argv[2] if len(sys.argv) > 2 else os.path.splitext(src)[0] + ".html"
    md = open(src, encoding="utf-8").read()

    sections, pre, cur = [], [], None
    for ln in md.split("\n"):
        if ln.startswith("## "):
            if cur:
                sections.append(cur)
            cur = [ln[3:].strip(), []]
        elif cur is None:
            pre.append(ln)
        else:
            cur[1].append(ln)
    if cur:
        sections.append(cur)

    title = next((l[2:].strip() for l in pre if l.startswith("# ")), os.path.basename(src))
    intro = render([l for l in pre if not l.startswith("# ")])
    toc = "".join(f'<li><a href="#{slug(t)}">{inline(t)}</a></li>' for t, _ in sections)
    # open the first section, and any section whose title signals a severe finding
    body = []
    for idx, (t, blk) in enumerate(sections):
        important = idx == 0 or "critical" in t.lower() or "softlock" in t.lower()
        body.append(f'<details id="{slug(t)}"{" open" if important else ""}>'
                    f"<summary>{inline(t)}</summary>"
                    f'<div class="inner">{render(blk)}</div></details>')

    doc = (f"<title>{html.escape(title)}</title>\n{STYLE}\n"
           f"<h1>{html.escape(title)}</h1>\n{intro}\n"
           '<div class="bar">'
           "<button onclick=\"document.querySelectorAll('details').forEach(d=>d.open=true)\">Expand all</button>"
           "<button onclick=\"document.querySelectorAll('details').forEach(d=>d.open=false)\">Collapse all</button>"
           "</div>\n"
           f'<nav class="toc"><h2>Contents</h2><ol>{toc}</ol></nav>\n'
           f"{''.join(body)}\n"
           f"<footer>Generated from <code>{html.escape(os.path.basename(src))}</code> on "
           f"{datetime.date.today().isoformat()}. Nothing in this document is gameplay-verified."
           "</footer>\n")
    open(dst, "w", encoding="utf-8").write(doc)
    print(f"wrote {dst} ({len(doc)} bytes, {len(sections)} sections)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
