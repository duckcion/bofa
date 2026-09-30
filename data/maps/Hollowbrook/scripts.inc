# 6 "data/maps/Hollowbrook/scripts.pory"

# 7 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_MapScripts::
# 8 "data/maps/Hollowbrook/scripts.pory"
	.byte 0
# 9 "data/maps/Hollowbrook/scripts.pory"

# 10 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_Mortimer::
# 11 "data/maps/Hollowbrook/scripts.pory"
	trainerbattle_single TRAINER_MORTIMER, Hollowbrook_Text_MortimerIntro, Hollowbrook_Text_MortimerDefeat, Hollowbrook_EventScript_TryRegisterMortimerAfterBattle
# 12 "data/maps/Hollowbrook/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 13 "data/maps/Hollowbrook/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, Hollowbrook_EventScript_RematchMortimer
# 14 "data/maps/Hollowbrook/scripts.pory"
	setvar VAR_0x8004, TRAINER_MORTIMER
# 15 "data/maps/Hollowbrook/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 16 "data/maps/Hollowbrook/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, Hollowbrook_EventScript_TryRegisterMortimer
# 17 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_MortimerPostBattle, MSGBOX_DEFAULT
# 18 "data/maps/Hollowbrook/scripts.pory"
	release
# 19 "data/maps/Hollowbrook/scripts.pory"
	end
# 20 "data/maps/Hollowbrook/scripts.pory"

# 21 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_RematchMortimer::
# 22 "data/maps/Hollowbrook/scripts.pory"
	trainerbattle_rematch TRAINER_MORTIMER, Hollowbrook_Text_MortimerRematchIntro, Hollowbrook_Text_MortimerDefeat
# 23 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_MortimerPostBattle, MSGBOX_DEFAULT
# 24 "data/maps/Hollowbrook/scripts.pory"
	release
# 25 "data/maps/Hollowbrook/scripts.pory"
	end
# 26 "data/maps/Hollowbrook/scripts.pory"

# 27 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_TryRegisterMortimer::
# 28 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_MortimerRegister, MSGBOX_DEFAULT
# 29 "data/maps/Hollowbrook/scripts.pory"
	register_matchcall TRAINER_MORTIMER
# 30 "data/maps/Hollowbrook/scripts.pory"
	release
# 31 "data/maps/Hollowbrook/scripts.pory"
	end
# 32 "data/maps/Hollowbrook/scripts.pory"

# 33 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_TryRegisterMortimerAfterBattle::
# 34 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_MortimerRegister, MSGBOX_DEFAULT
# 35 "data/maps/Hollowbrook/scripts.pory"
	register_matchcall TRAINER_MORTIMER
# 36 "data/maps/Hollowbrook/scripts.pory"
	release
# 37 "data/maps/Hollowbrook/scripts.pory"
	end
# 38 "data/maps/Hollowbrook/scripts.pory"

# 39 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_MortimerIntro:
# 40 "data/maps/Hollowbrook/scripts.pory"
	.string "MORTIMER: This town's gone quiet\n"
# 41 "data/maps/Hollowbrook/scripts.pory"
	.string "since the funerals stopped. Too\l"
# 42 "data/maps/Hollowbrook/scripts.pory"
	.string "quiet, if you ask me.$"
# 43 "data/maps/Hollowbrook/scripts.pory"

# 44 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_MortimerDefeat:
# 45 "data/maps/Hollowbrook/scripts.pory"
	.string "MORTIMER: Rest in peace, I suppose.$"
# 46 "data/maps/Hollowbrook/scripts.pory"

# 47 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_MortimerPostBattle:
# 48 "data/maps/Hollowbrook/scripts.pory"
	.string "MORTIMER: The cemetery's just north\n"
# 49 "data/maps/Hollowbrook/scripts.pory"
	.string "of here, if you're curious.$"
# 50 "data/maps/Hollowbrook/scripts.pory"

# 51 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_MortimerRegister:
# 52 "data/maps/Hollowbrook/scripts.pory"
	.string "MORTIMER: Let's keep in touch.$"
# 53 "data/maps/Hollowbrook/scripts.pory"

# 54 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_MortimerRematchIntro:
# 55 "data/maps/Hollowbrook/scripts.pory"
	.string "MORTIMER: Care for another round?$"
# 56 "data/maps/Hollowbrook/scripts.pory"

# 57 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_Prudence::
# 58 "data/maps/Hollowbrook/scripts.pory"
	trainerbattle_single TRAINER_PRUDENCE, Hollowbrook_Text_PrudenceIntro, Hollowbrook_Text_PrudenceDefeat, Hollowbrook_EventScript_TryRegisterPrudenceAfterBattle
# 59 "data/maps/Hollowbrook/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 60 "data/maps/Hollowbrook/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, Hollowbrook_EventScript_RematchPrudence
# 61 "data/maps/Hollowbrook/scripts.pory"
	setvar VAR_0x8004, TRAINER_PRUDENCE
# 62 "data/maps/Hollowbrook/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 63 "data/maps/Hollowbrook/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, Hollowbrook_EventScript_TryRegisterPrudence
# 64 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_PrudencePostBattle, MSGBOX_DEFAULT
# 65 "data/maps/Hollowbrook/scripts.pory"
	release
# 66 "data/maps/Hollowbrook/scripts.pory"
	end
# 67 "data/maps/Hollowbrook/scripts.pory"

# 68 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_RematchPrudence::
# 69 "data/maps/Hollowbrook/scripts.pory"
	trainerbattle_rematch TRAINER_PRUDENCE, Hollowbrook_Text_PrudenceRematchIntro, Hollowbrook_Text_PrudenceDefeat
# 70 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_PrudencePostBattle, MSGBOX_DEFAULT
# 71 "data/maps/Hollowbrook/scripts.pory"
	release
# 72 "data/maps/Hollowbrook/scripts.pory"
	end
# 73 "data/maps/Hollowbrook/scripts.pory"

# 74 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_TryRegisterPrudence::
# 75 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_PrudenceRegister, MSGBOX_DEFAULT
# 76 "data/maps/Hollowbrook/scripts.pory"
	register_matchcall TRAINER_PRUDENCE
# 77 "data/maps/Hollowbrook/scripts.pory"
	release
# 78 "data/maps/Hollowbrook/scripts.pory"
	end
# 79 "data/maps/Hollowbrook/scripts.pory"

# 80 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_EventScript_TryRegisterPrudenceAfterBattle::
# 81 "data/maps/Hollowbrook/scripts.pory"
	msgbox Hollowbrook_Text_PrudenceRegister, MSGBOX_DEFAULT
# 82 "data/maps/Hollowbrook/scripts.pory"
	register_matchcall TRAINER_PRUDENCE
# 83 "data/maps/Hollowbrook/scripts.pory"
	release
# 84 "data/maps/Hollowbrook/scripts.pory"
	end
# 85 "data/maps/Hollowbrook/scripts.pory"

# 86 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_PrudenceIntro:
# 87 "data/maps/Hollowbrook/scripts.pory"
	.string "PRUDENCE: Careful of the webs\n"
# 88 "data/maps/Hollowbrook/scripts.pory"
	.string "around here. They're not all\l"
# 89 "data/maps/Hollowbrook/scripts.pory"
	.string "empty.$"
# 90 "data/maps/Hollowbrook/scripts.pory"

# 91 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_PrudenceDefeat:
# 92 "data/maps/Hollowbrook/scripts.pory"
	.string "PRUDENCE: Tangled up in defeat!$"
# 93 "data/maps/Hollowbrook/scripts.pory"

# 94 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_PrudencePostBattle:
# 95 "data/maps/Hollowbrook/scripts.pory"
	.string "PRUDENCE: There's a shack up north\n"
# 96 "data/maps/Hollowbrook/scripts.pory"
	.string "if you need to rest.$"
# 97 "data/maps/Hollowbrook/scripts.pory"

# 98 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_PrudenceRegister:
# 99 "data/maps/Hollowbrook/scripts.pory"
	.string "PRUDENCE: Let's trade numbers.$"
# 100 "data/maps/Hollowbrook/scripts.pory"

# 101 "data/maps/Hollowbrook/scripts.pory"
Hollowbrook_Text_PrudenceRematchIntro:
# 102 "data/maps/Hollowbrook/scripts.pory"
	.string "PRUDENCE: Back for more webs?$"
