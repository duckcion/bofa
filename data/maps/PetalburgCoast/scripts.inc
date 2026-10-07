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
PetalburgCoast_Text_HintBeforeStone:
# 24 "data/maps/PetalburgCoast/scripts.pory"
	.string "Something shiny washed up north of\n"
# 25 "data/maps/PetalburgCoast/scripts.pory"
	.string "here. Could be what you're after.$"
# 26 "data/maps/PetalburgCoast/scripts.pory"

# 27 "data/maps/PetalburgCoast/scripts.pory"
PetalburgCoast_Text_HintViridian:
# 28 "data/maps/PetalburgCoast/scripts.pory"
	.string "I heard you're planning to challenge\n"
# 29 "data/maps/PetalburgCoast/scripts.pory"
	.string "NORMAN!\p"
# 30 "data/maps/PetalburgCoast/scripts.pory"
	.string "You might want to explore VIRIDIAN\n"
# 31 "data/maps/PetalburgCoast/scripts.pory"
	.string "FOREST first, off ROUTE 103 north\l"
# 32 "data/maps/PetalburgCoast/scripts.pory"
	.string "of OLDALE TOWN.\p"
# 33 "data/maps/PetalburgCoast/scripts.pory"
	.string "There's something there that could\n"
# 34 "data/maps/PetalburgCoast/scripts.pory"
	.string "help you in his GYM.$"
