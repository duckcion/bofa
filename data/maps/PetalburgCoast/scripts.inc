PetalburgCoast_MapScripts::
	.byte 0

PetalburgCoast_EventScript_Ronnie::
	trainerbattle_single TRAINER_PETALBURGCOAST_1, PetalburgCoast_Text_RonnieIntro, PetalburgCoast_Text_RonnieDefeated
	msgbox PetalburgCoast_Text_RonniePostBattle, MSGBOX_AUTOCLOSE
	end

PetalburgCoast_EventScript_EverstoneBall::
	goto_if_not_defeated TRAINER_PETALBURGCOAST_1, PetalburgCoast_EventScript_EverstoneGuarded
	goto Common_EventScript_FindItem
	end

PetalburgCoast_EventScript_EverstoneGuarded::
	msgbox PetalburgCoast_Text_EverstoneGuarded, MSGBOX_DEFAULT
	end

PetalburgCoast_Text_EverstoneGuarded:
	.string "The EVERSTONE is behind that kid.\n"
	.string "Guess you'll have to battle him.$"

@ Split 1: after the Everstone, point the player at Viridian Forest (optional prep).
PetalburgCoast_EventScript_HintFisherman::
	lock
	faceplayer
	goto_if_set FLAG_ITEM_PETALBURG_COAST_EVERSTONE, PetalburgCoast_EventScript_HintViridian
	msgbox PetalburgCoast_Text_HintBeforeStone, MSGBOX_DEFAULT
	release
	end

PetalburgCoast_EventScript_HintViridian::
	msgbox PetalburgCoast_Text_HintViridian, MSGBOX_DEFAULT
	release
	end

@ One-time beach encounter.
PetalburgCoast_EventScript_Corphish::
	lock
	faceplayer
	setwildbattle SPECIES_CORPHISH, 11
	waitse
	playmoncry SPECIES_CORPHISH, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	removeobject VAR_LAST_TALKED
	setflag FLAG_PETALBURG_COAST_CORPHISH
	release
	end

PetalburgCoast_Text_HintBeforeStone:
	.string "Something shiny washed up north of\n"
	.string "here. Could be what you're after.$"

PetalburgCoast_Text_HintViridian:
	.string "I heard you're planning to challenge\n"
	.string "NORMAN!\p"
	.string "You might want to explore VIRIDIAN\n"
	.string "FOREST first, off ROUTE 103 north\l"
	.string "of OLDALE TOWN.\p"
	.string "There's something there that could\n"
	.string "help you in his GYM.$"
