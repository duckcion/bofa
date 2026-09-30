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

GraniteCave_B2F_EventScript_Ansel::
	trainerbattle_single TRAINER_ANSEL, GraniteCave_B2F_Text_AnselIntro, GraniteCave_B2F_Text_AnselDefeat
	msgbox GraniteCave_B2F_Text_AnselPostBattle, MSGBOX_AUTOCLOSE
	end

GraniteCave_B2F_Text_AnselIntro:
	.string "ANSEL: You feel that weird chill too?\n"
	.string "Let's battle, it'll warm us up!$"

GraniteCave_B2F_Text_AnselDefeat:
	.string "ANSEL: Brrr… beaten AND cold.$"

GraniteCave_B2F_Text_AnselPostBattle:
	.string "ANSEL: WRAITHWOOD FOREST is just\n"
	.string "through there.$"

