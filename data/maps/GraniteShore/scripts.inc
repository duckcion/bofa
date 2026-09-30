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
	random 3
# 20 "data/maps/GraniteShore/scripts.pory"
	switch VAR_RESULT
# 21 "data/maps/GraniteShore/scripts.pory"
	case 0, GraniteShore_EventScript_RancherAndyTauros
# 22 "data/maps/GraniteShore/scripts.pory"
	case 1, GraniteShore_EventScript_RancherAndyMiltank
# 23 "data/maps/GraniteShore/scripts.pory"
	case 2, GraniteShore_EventScript_RancherAndyBouffalant
# 24 "data/maps/GraniteShore/scripts.pory"
	end
# 25 "data/maps/GraniteShore/scripts.pory"

# 26 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyTauros::
# 27 "data/maps/GraniteShore/scripts.pory"
	givemon SPECIES_TAUROS, 20, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31
# 28 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyTauros, MSGBOX_DEFAULT
# 29 "data/maps/GraniteShore/scripts.pory"
	goto GraniteShore_EventScript_RancherAndyFinish
# 30 "data/maps/GraniteShore/scripts.pory"

# 31 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyMiltank::
# 32 "data/maps/GraniteShore/scripts.pory"
	givemon SPECIES_MILTANK, 20, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31
# 33 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyMiltank, MSGBOX_DEFAULT
# 34 "data/maps/GraniteShore/scripts.pory"
	goto GraniteShore_EventScript_RancherAndyFinish
# 35 "data/maps/GraniteShore/scripts.pory"

# 36 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyBouffalant::
# 37 "data/maps/GraniteShore/scripts.pory"
	givemon SPECIES_BOUFFALANT, 20, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31
# 38 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyBouffalant, MSGBOX_DEFAULT
# 39 "data/maps/GraniteShore/scripts.pory"
	goto GraniteShore_EventScript_RancherAndyFinish
# 40 "data/maps/GraniteShore/scripts.pory"

# 41 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyFinish::
# 42 "data/maps/GraniteShore/scripts.pory"
	goto_if_ne VAR_RESULT, MON_GIVEN_TO_PARTY, GraniteShore_EventScript_RancherAndySkipLocation
# 43 "data/maps/GraniteShore/scripts.pory"
	setmonmetlocation VAR_0x8008, MAPSEC_RANCHER_ANDY
# 44 "data/maps/GraniteShore/scripts.pory"

# 45 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndySkipLocation::
# 46 "data/maps/GraniteShore/scripts.pory"
	setflag FLAG_RECEIVED_RANCHER_ANDY_GIFT
# 47 "data/maps/GraniteShore/scripts.pory"
	release
# 48 "data/maps/GraniteShore/scripts.pory"
	end
# 49 "data/maps/GraniteShore/scripts.pory"

# 50 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyDecline::
# 51 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyDecline, MSGBOX_DEFAULT
# 52 "data/maps/GraniteShore/scripts.pory"
	release
# 53 "data/maps/GraniteShore/scripts.pory"
	end
# 54 "data/maps/GraniteShore/scripts.pory"

# 55 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_RancherAndyAlreadyGiven::
# 56 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_RancherAndyAlreadyGiven, MSGBOX_DEFAULT
# 57 "data/maps/GraniteShore/scripts.pory"
	release
# 58 "data/maps/GraniteShore/scripts.pory"
	end
# 59 "data/maps/GraniteShore/scripts.pory"

# 60 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyIntro:
# 61 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Oh, hey there!\p"
# 62 "data/maps/GraniteShore/scripts.pory"
	.string "I used to run a little farm inland,\n"
# 63 "data/maps/GraniteShore/scripts.pory"
	.string "but I've packed it all up and moved\l"
# 64 "data/maps/GraniteShore/scripts.pory"
	.string "out here to the coast.$"
# 65 "data/maps/GraniteShore/scripts.pory"

# 66 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyOffer:
# 67 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Fresh sea air, quiet\n"
# 68 "data/maps/GraniteShore/scripts.pory"
	.string "waves… I don't need a farm animal\l"
# 69 "data/maps/GraniteShore/scripts.pory"
	.string "out here anymore.\p"
# 70 "data/maps/GraniteShore/scripts.pory"
	.string "Want one? I'll let fate pick which\n"
# 71 "data/maps/GraniteShore/scripts.pory"
	.string "one goes home with you!$"
# 72 "data/maps/GraniteShore/scripts.pory"

# 73 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyTauros:
# 74 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Ha! Figures.\n"
# 75 "data/maps/GraniteShore/scripts.pory"
	.string "Take good care of that TAUROS!$"
# 76 "data/maps/GraniteShore/scripts.pory"

# 77 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyMiltank:
# 78 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Ha! Figures.\n"
# 79 "data/maps/GraniteShore/scripts.pory"
	.string "Take good care of that MILTANK!$"
# 80 "data/maps/GraniteShore/scripts.pory"

# 81 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyBouffalant:
# 82 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Ha! Figures.\n"
# 83 "data/maps/GraniteShore/scripts.pory"
	.string "Take good care of that BOUFFALANT!$"
# 84 "data/maps/GraniteShore/scripts.pory"

# 85 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyDecline:
# 86 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Suit yourself.\n"
# 87 "data/maps/GraniteShore/scripts.pory"
	.string "I'll be right here if you change\l"
# 88 "data/maps/GraniteShore/scripts.pory"
	.string "your mind.$"
# 89 "data/maps/GraniteShore/scripts.pory"

# 90 "data/maps/GraniteShore/scripts.pory"
GraniteShore_Text_RancherAndyAlreadyGiven:
# 91 "data/maps/GraniteShore/scripts.pory"
	.string "RANCHER ANDY: Loving the retired\n"
# 92 "data/maps/GraniteShore/scripts.pory"
	.string "life out here on the coast.\p"
# 93 "data/maps/GraniteShore/scripts.pory"
	.string "You take good care of the one\n"
# 94 "data/maps/GraniteShore/scripts.pory"
	.string "I gave you, now!$"
# 95 "data/maps/GraniteShore/scripts.pory"

# 96 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_Deacon::
# 97 "data/maps/GraniteShore/scripts.pory"
	trainerbattle_single TRAINER_GRANITESHORE_1, GraniteShore_Text_DeaconIntro, GraniteShore_Text_DeaconDefeated
# 98 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_DeaconPostBattle, MSGBOX_AUTOCLOSE
# 99 "data/maps/GraniteShore/scripts.pory"
	end
# 100 "data/maps/GraniteShore/scripts.pory"

# 101 "data/maps/GraniteShore/scripts.pory"
GraniteShore_EventScript_Priya::
# 102 "data/maps/GraniteShore/scripts.pory"
	trainerbattle_single TRAINER_GRANITESHORE_2, GraniteShore_Text_PriyaIntro, GraniteShore_Text_PriyaDefeated
# 103 "data/maps/GraniteShore/scripts.pory"
	msgbox GraniteShore_Text_PriyaPostBattle, MSGBOX_AUTOCLOSE
# 104 "data/maps/GraniteShore/scripts.pory"
	end
