GraniteCave_B2F_MapScripts::
	.byte 0

GraniteCave_B2F_EventScript_WraithwoodBlocker::
	lock
	faceplayer
	msgbox GraniteCave_B2F_Text_WraithwoodBlocker, MSGBOX_DEFAULT
	release
	end

GraniteCave_B2F_Text_WraithwoodBlocker:
	.string "PSYCHIC: Huh? You feel that too?\p"
	.string "Something's not right further in…\n"
	.string "I can't shake this awful feeling.\p"
	.string "It's too scary down there for me.\n"
	.string "I'm not going any deeper right now.$"

