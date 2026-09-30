Hollowbrook_MapScripts::
	.byte 0

Hollowbrook_EventScript_Mortimer::
	trainerbattle_single TRAINER_MORTIMER, Hollowbrook_Text_MortimerIntro, Hollowbrook_Text_MortimerDefeat, Hollowbrook_EventScript_TryRegisterMortimerAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Hollowbrook_EventScript_RematchMortimer
	setvar VAR_0x8004, TRAINER_MORTIMER
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Hollowbrook_EventScript_TryRegisterMortimer
	msgbox Hollowbrook_Text_MortimerPostBattle, MSGBOX_DEFAULT
	release
	end

Hollowbrook_EventScript_RematchMortimer::
	trainerbattle_rematch TRAINER_MORTIMER, Hollowbrook_Text_MortimerRematchIntro, Hollowbrook_Text_MortimerDefeat
	msgbox Hollowbrook_Text_MortimerPostBattle, MSGBOX_DEFAULT
	release
	end

Hollowbrook_EventScript_TryRegisterMortimer::
	msgbox Hollowbrook_Text_MortimerRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_MORTIMER
	release
	end

Hollowbrook_EventScript_TryRegisterMortimerAfterBattle::
	msgbox Hollowbrook_Text_MortimerRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_MORTIMER
	release
	end

Hollowbrook_Text_MortimerIntro:
	.string "MORTIMER: This town's gone quiet\n"
	.string "since the funerals stopped. Too\l"
	.string "quiet, if you ask me.$"

Hollowbrook_Text_MortimerDefeat:
	.string "MORTIMER: Rest in peace, I suppose.$"

Hollowbrook_Text_MortimerPostBattle:
	.string "MORTIMER: The cemetery's just north\n"
	.string "of here, if you're curious.$"

Hollowbrook_Text_MortimerRegister:
	.string "MORTIMER: Let's keep in touch.$"

Hollowbrook_Text_MortimerRematchIntro:
	.string "MORTIMER: Care for another round?$"

Hollowbrook_EventScript_Prudence::
	trainerbattle_single TRAINER_PRUDENCE, Hollowbrook_Text_PrudenceIntro, Hollowbrook_Text_PrudenceDefeat, Hollowbrook_EventScript_TryRegisterPrudenceAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Hollowbrook_EventScript_RematchPrudence
	setvar VAR_0x8004, TRAINER_PRUDENCE
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Hollowbrook_EventScript_TryRegisterPrudence
	msgbox Hollowbrook_Text_PrudencePostBattle, MSGBOX_DEFAULT
	release
	end

Hollowbrook_EventScript_RematchPrudence::
	trainerbattle_rematch TRAINER_PRUDENCE, Hollowbrook_Text_PrudenceRematchIntro, Hollowbrook_Text_PrudenceDefeat
	msgbox Hollowbrook_Text_PrudencePostBattle, MSGBOX_DEFAULT
	release
	end

Hollowbrook_EventScript_TryRegisterPrudence::
	msgbox Hollowbrook_Text_PrudenceRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_PRUDENCE
	release
	end

Hollowbrook_EventScript_TryRegisterPrudenceAfterBattle::
	msgbox Hollowbrook_Text_PrudenceRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_PRUDENCE
	release
	end

Hollowbrook_Text_PrudenceIntro:
	.string "PRUDENCE: Careful of the webs\n"
	.string "around here. They're not all\l"
	.string "empty.$"

Hollowbrook_Text_PrudenceDefeat:
	.string "PRUDENCE: Tangled up in defeat!$"

Hollowbrook_Text_PrudencePostBattle:
	.string "PRUDENCE: There's a shack up north\n"
	.string "if you need to rest.$"

Hollowbrook_Text_PrudenceRegister:
	.string "PRUDENCE: Let's trade numbers.$"

Hollowbrook_Text_PrudenceRematchIntro:
	.string "PRUDENCE: Back for more webs?$"
