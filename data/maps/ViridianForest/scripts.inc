ViridianForest_MapScripts::
	.byte 0

ViridianForest_EventScript_DwebbleGift::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_DWEBBLE_GIFT, ViridianForest_EventScript_DwebbleGiftAlreadyGiven
	msgbox ViridianForest_Text_DwebbleGiftIntro, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_DwebbleGiftOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianForest_EventScript_DwebbleGiftDecline
	getpartysize
	copyvar VAR_0x8008, VAR_RESULT
	givemon SPECIES_DWEBBLE, 10, nature=NATURE_ADAMANT, abilityNum=0, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31, move1=MOVE_ROCK_BLAST, move2=MOVE_STRUGGLE_BUG, move3=MOVE_TACKLE, move4=MOVE_STRING_SHOT
	goto_if_ne VAR_RESULT, MON_GIVEN_TO_PARTY, ViridianForest_EventScript_DwebbleGiftSkipLocation
	setmonmetlocation VAR_0x8008, MAPSEC_VIRIDIAN_FOREST

ViridianForest_EventScript_DwebbleGiftSkipLocation::
	msgbox ViridianForest_Text_DwebbleGiftGiven, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_DWEBBLE_GIFT
	release
	end

ViridianForest_EventScript_DwebbleGiftDecline::
	msgbox ViridianForest_Text_DwebbleGiftDecline, MSGBOX_DEFAULT
	release
	end

ViridianForest_EventScript_DwebbleGiftAlreadyGiven::
	msgbox ViridianForest_Text_DwebbleGiftAlreadyGiven, MSGBOX_DEFAULT
	release
	end

ViridianForest_Text_DwebbleGiftIntro:
	.string "You made it in here? Most folks turn\n"
	.string "back at the treeline.\p"
	.string "I collect the stones these little ones\n"
	.string "carry. Found one that won't part with\l"
	.string "its rock for anything.$"

ViridianForest_Text_DwebbleGiftOffer:
	.string "Stubborn thing. It'd do better with\n"
	.string "someone headed somewhere.\p"
	.string "Want it?$"

ViridianForest_Text_DwebbleGiftGiven:
	.string "That shell won't crack easily. Lean\n"
	.string "on it when a fight turns bad.$"

ViridianForest_Text_DwebbleGiftDecline:
	.string "Fair enough. It isn't going anywhere.$"

ViridianForest_Text_DwebbleGiftAlreadyGiven:
	.string "Still hauling that rock around, is it?\n"
	.string "Good. Means it trusts you.$"
