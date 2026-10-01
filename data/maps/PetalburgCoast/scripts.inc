# 1 "data/maps/PetalburgCoast/scripts.pory"

# 2 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_MapScripts::
# 3 "data/maps/PetalburgCoast/scripts.pory"
	.byte 0
# 4 "data/maps/PetalburgCoast/scripts.pory"

# 5 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_EventScript_EverstoneBall::
# 6 "data/maps/PetalburgCoast/scripts.pory"
	goto Common_EventScript_FindItem
# 7 "data/maps/PetalburgCoast/scripts.pory"
	end
# 8 "data/maps/PetalburgCoast/scripts.pory"

# 9 "data/maps/PetalburgCoast/scripts.pory"
@ Split 1: after the Everstone, point the player at Viridian Forest (optional prep).
# 10 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_EventScript_HintFisherman::
# 11 "data/maps/PetalburgCoast/scripts.pory"
	lock
# 12 "data/maps/PetalburgCoast/scripts.pory"
	faceplayer
# 13 "data/maps/PetalburgCoast/scripts.pory"
	goto_if_set FLAG_ITEM_PETALBURG_COAST_EVERSTONE, PetalburgCoast_EventScript_HintViridian
# 14 "data/maps/PetalburgCoast/scripts.pory"
	msgbox PetalburgCoast_Text_HintBeforeStone, MSGBOX_DEFAULT
# 15 "data/maps/PetalburgCoast/scripts.pory"
	release
# 16 "data/maps/PetalburgCoast/scripts.pory"
	end
# 17 "data/maps/PetalburgCoast/scripts.pory"

# 18 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_EventScript_HintViridian::
# 19 "data/maps/PetalburgCoast/scripts.pory"
	msgbox PetalburgCoast_Text_HintViridian, MSGBOX_DEFAULT
# 20 "data/maps/PetalburgCoast/scripts.pory"
	release
# 21 "data/maps/PetalburgCoast/scripts.pory"
	end
# 22 "data/maps/PetalburgCoast/scripts.pory"

# 23 "data/maps/PetalburgCoast/scripts.pory"
@ One-time beach encounter.
# 24 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_EventScript_Corphish::
# 25 "data/maps/PetalburgCoast/scripts.pory"
	lock
# 26 "data/maps/PetalburgCoast/scripts.pory"
	faceplayer
# 27 "data/maps/PetalburgCoast/scripts.pory"
	setwildbattle SPECIES_CORPHISH, 11
# 28 "data/maps/PetalburgCoast/scripts.pory"
	waitse
# 29 "data/maps/PetalburgCoast/scripts.pory"
	playmoncry SPECIES_CORPHISH, CRY_MODE_ENCOUNTER
# 30 "data/maps/PetalburgCoast/scripts.pory"
	delay 40
# 31 "data/maps/PetalburgCoast/scripts.pory"
	waitmoncry
# 32 "data/maps/PetalburgCoast/scripts.pory"
	setflag FLAG_SYS_CTRL_OBJ_DELETE
# 33 "data/maps/PetalburgCoast/scripts.pory"
	dowildbattle
# 34 "data/maps/PetalburgCoast/scripts.pory"
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
# 35 "data/maps/PetalburgCoast/scripts.pory"
	removeobject VAR_LAST_TALKED
# 36 "data/maps/PetalburgCoast/scripts.pory"
	setflag FLAG_PETALBURG_COAST_CORPHISH
# 37 "data/maps/PetalburgCoast/scripts.pory"
	release
# 38 "data/maps/PetalburgCoast/scripts.pory"
	end
# 39 "data/maps/PetalburgCoast/scripts.pory"

# 40 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_Text_HintBeforeStone:
# 41 "data/maps/PetalburgCoast/scripts.pory"
	.string "Something shiny washed up north of\n"
# 42 "data/maps/PetalburgCoast/scripts.pory"
	.string "here. Could be what you're after.$"
# 43 "data/maps/PetalburgCoast/scripts.pory"

# 44 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_Text_HintViridian:
# 45 "data/maps/PetalburgCoast/scripts.pory"
	.string "I heard you're planning to challenge\n"
# 46 "data/maps/PetalburgCoast/scripts.pory"
	.string "NORMAN!\p"
# 47 "data/maps/PetalburgCoast/scripts.pory"
	.string "You might want to explore VIRIDIAN\n"
# 48 "data/maps/PetalburgCoast/scripts.pory"
	.string "FOREST first, off ROUTE 103 north\l"
# 49 "data/maps/PetalburgCoast/scripts.pory"
	.string "of OLDALE TOWN.\p"
# 50 "data/maps/PetalburgCoast/scripts.pory"
	.string "There's something there that could\n"
# 51 "data/maps/PetalburgCoast/scripts.pory"
	.string "help you in his GYM.$"
