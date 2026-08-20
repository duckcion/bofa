PetalburgCoast_MapScripts::
	.byte 0

PetalburgCoast_EventScript_Ronnie::
	trainerbattle_single TRAINER_PETALBURGCOAST_1, PetalburgCoast_Text_RonnieIntro, PetalburgCoast_Text_RonnieDefeated
	msgbox PetalburgCoast_Text_RonniePostBattle, MSGBOX_AUTOCLOSE
	end

PetalburgCoast_EventScript_OvalStoneBall::
	goto_if_not_defeated TRAINER_PETALBURGCOAST_1, PetalburgCoast_EventScript_OvalStoneGuarded
	goto Common_EventScript_FindItem
	end

PetalburgCoast_EventScript_OvalStoneGuarded::
	msgbox PetalburgCoast_Text_OvalStoneGuarded, MSGBOX_DEFAULT
	end

PetalburgCoast_Text_OvalStoneGuarded:
	.string "The OVAL STONE is behind that kid.\n"
	.string "Guess you'll have to battle him.$"
