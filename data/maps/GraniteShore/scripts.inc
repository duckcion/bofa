# 6 "data/maps/GraniteShore/scripts.pory"

# 7 "data/maps/GraniteShore/scripts.pory"
GraniteShore_MapScripts::
# 8 "data/maps/GraniteShore/scripts.pory"
	.byte 0
# 9 "data/maps/GraniteShore/scripts.pory"

# 10 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndy::
# 11 "data/maps/GraniteShore/scripts.pory"
	lock
# 12 "data/maps/GraniteShore/scripts.pory"
	faceplayer
# 13 "data/maps/GraniteShore/scripts.pory"
	goto_if_set FLAG_RECEIVED_RANCHER_ANDY_GIFT, GraniteShore_EventScript_RancherAndyAlreadyGiven
# 14 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyIntro, MSGBOX_DEFAULT
# 15 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyOffer, MSGBOX_YESNO
# 16 "data/maps/GraniteShore/scripts.pory"
	goto_if_eq VAR_RESULT, NO, GraniteShore_EventScript_RancherAndyDecline
# 17 "data/maps/GraniteShore/scripts.pory"
	getpartysize
# 18 "data/maps/GraniteShore/scripts.pory"
	copyvar VAR_0x8008, VAR_RESULT
# 19 "data/maps/GraniteShore/scripts.pory"
	givemon SPECIES_DWEBBLE, 20, nature=NATURE_ADAMANT, abilityNum=0, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31, move1=MOVE_ROCK_BLAST, move2=MOVE_BUG_BITE, move3=MOVE_PIN_MISSILE, move4=MOVE_PROTECT
# 20 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyDwebble, MSGBOX_DEFAULT
# 21 "data/maps/GraniteShore/scripts.pory"
	goto_if_ne VAR_RESULT, MON_GIVEN_TO_PARTY, GraniteShore_EventScript_RancherAndySkipLocation
# 22 "data/maps/GraniteShore/scripts.pory"
	setmonmetlocation VAR_0x8008, MAPSEC_RANCHER_ANDY
# 23 "data/maps/GraniteShore/scripts.pory"

# 24 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndySkipLocation::
# 25 "data/maps/GraniteShore/scripts.pory"
	setflag FLAG_RECEIVED_RANCHER_ANDY_GIFT
# 26 "data/maps/GraniteShore/scripts.pory"
	release
# 27 "data/maps/GraniteShore/scripts.pory"
	end
# 28 "data/maps/GraniteShore/scripts.pory"

# 29 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyDecline::
# 30 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyDecline, MSGBOX_DEFAULT
# 31 "data/maps/GraniteShore/scripts.pory"
	release
# 32 "data/maps/GraniteShore/scripts.pory"
	end
# 33 "data/maps/GraniteShore/scripts.pory"

# 34 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyAlreadyGiven::
# 35 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyAlreadyGiven, MSGBOX_DEFAULT
# 36 "data/maps/GraniteShore/scripts.pory"
	release
# 37 "data/maps/GraniteShore/scripts.pory"
	end
# 38 "data/maps/GraniteShore/scripts.pory"

# 39 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyIntro:
# 40 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Oh, hey there!\p"
# 41 "data/maps/GraniteShore/scripts.pory"
	.string "I used to run a little farm inland,\n"
# 42 "data/maps/GraniteShore/scripts.pory"
	.string "but I've packed it all up and moved\l"
# 43 "data/maps/GraniteShore/scripts.pory"
	.string "out here to the coast.$"
# 44 "data/maps/GraniteShore/scripts.pory"

# 45 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyOffer:
# 46 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Found something in the\n"
# 47 "data/maps/GraniteShore/scripts.pory"
	.string "rocks down by the tide line.\p"
# 48 "data/maps/GraniteShore/scripts.pory"
	.string "Little fella hauls its own boulder\n"
# 49 "data/maps/GraniteShore/scripts.pory"
	.string "around. Won't be shifted for anything.\p"
# 50 "data/maps/GraniteShore/scripts.pory"
	.string "Reckon it'd rather travel than sit\n"
# 51 "data/maps/GraniteShore/scripts.pory"
	.string "on my beach. Want it?$"
# 52 "data/maps/GraniteShore/scripts.pory"

# 53 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyDwebble:
# 54 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Ha! Good.\p"
# 55 "data/maps/GraniteShore/scripts.pory"
	.string "That shell of its takes a hit better\n"
# 56 "data/maps/GraniteShore/scripts.pory"
	.string "than any beast I ever raised.$"
# 57 "data/maps/GraniteShore/scripts.pory"

# 58 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyDecline:
# 59 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Suit yourself.\n"
# 60 "data/maps/GraniteShore/scripts.pory"
	.string "I'll be right here if you change\l"
# 61 "data/maps/GraniteShore/scripts.pory"
	.string "your mind.$"
# 62 "data/maps/GraniteShore/scripts.pory"

# 63 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyAlreadyGiven:
# 64 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Loving the retired\n"
# 65 "data/maps/GraniteShore/scripts.pory"
	.string "life out here on the coast.\p"
# 66 "data/maps/GraniteShore/scripts.pory"
	.string "You take good care of the one\n"
# 67 "data/maps/GraniteShore/scripts.pory"
	.string "I gave you, now!$"
# 68 "data/maps/GraniteShore/scripts.pory"

# 69 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_Deacon::
# 70 "data/maps/GraniteShore/scripts.pory"
	trainerbattle_single TRAINER_GRANITESHORE_1, GraniteShore_Text_DeaconIntro, GraniteShore_Text_DeaconDefeated
# 71 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_DeaconPostBattle, MSGBOX_AUTOCLOSE
# 72 "data/maps/GraniteShore/scripts.pory"
	end
# 73 "data/maps/GraniteShore/scripts.pory"

# 74 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_Priya::
# 75 "data/maps/GraniteShore/scripts.pory"
	trainerbattle_single TRAINER_GRANITESHORE_2, GraniteShore_Text_PriyaIntro, GraniteShore_Text_PriyaDefeated
# 76 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_PriyaPostBattle, MSGBOX_AUTOCLOSE
# 77 "data/maps/GraniteShore/scripts.pory"
	end
