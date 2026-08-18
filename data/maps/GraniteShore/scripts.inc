GraniteShore_MapScripts::
	.byte 0

GraniteShore_EventScript_RancherAndy::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_RANCHER_ANDY_GIFT, GraniteShore_EventScript_RancherAndyAlreadyGiven
	msgbox GraniteShore_Text_RancherAndyIntro, MSGBOX_DEFAULT
	msgbox GraniteShore_Text_RancherAndyOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, GraniteShore_EventScript_RancherAndyDecline
	getpartysize
	copyvar VAR_0x8008, VAR_RESULT
	random 3
	switch VAR_RESULT
	case 0, GraniteShore_EventScript_RancherAndyTauros
	case 1, GraniteShore_EventScript_RancherAndyMiltank
	case 2, GraniteShore_EventScript_RancherAndyBouffalant
	end

GraniteShore_EventScript_RancherAndyTauros::
	givemon SPECIES_TAUROS, 20, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31
	msgbox GraniteShore_Text_RancherAndyTauros, MSGBOX_DEFAULT
	goto GraniteShore_EventScript_RancherAndyFinish

GraniteShore_EventScript_RancherAndyMiltank::
	givemon SPECIES_MILTANK, 20, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31
	msgbox GraniteShore_Text_RancherAndyMiltank, MSGBOX_DEFAULT
	goto GraniteShore_EventScript_RancherAndyFinish

GraniteShore_EventScript_RancherAndyBouffalant::
	givemon SPECIES_BOUFFALANT, 20, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31
	msgbox GraniteShore_Text_RancherAndyBouffalant, MSGBOX_DEFAULT
	goto GraniteShore_EventScript_RancherAndyFinish

GraniteShore_EventScript_RancherAndyFinish::
	goto_if_ne VAR_RESULT, MON_GIVEN_TO_PARTY, GraniteShore_EventScript_RancherAndySkipLocation
	setmonmetlocation VAR_0x8008, MAPSEC_RANCHER_ANDY

GraniteShore_EventScript_RancherAndySkipLocation::
	setflag FLAG_RECEIVED_RANCHER_ANDY_GIFT
	release
	end

GraniteShore_EventScript_RancherAndyDecline::
	msgbox GraniteShore_Text_RancherAndyDecline, MSGBOX_DEFAULT
	release
	end

GraniteShore_EventScript_RancherAndyAlreadyGiven::
	msgbox GraniteShore_Text_RancherAndyAlreadyGiven, MSGBOX_DEFAULT
	release
	end

GraniteShore_Text_RancherAndyIntro:
	.string "RANCHER ANDY: Oh, hey there!\p"
	.string "I used to run a little farm inland,\n"
	.string "but I've packed it all up and moved\l"
	.string "out here to the coast.$"

GraniteShore_Text_RancherAndyOffer:
	.string "RANCHER ANDY: Fresh sea air, quiet\n"
	.string "waves… I don't need a farm animal\l"
	.string "out here anymore.\p"
	.string "Want one? I'll let fate pick which\n"
	.string "one goes home with you!$"

GraniteShore_Text_RancherAndyTauros:
	.string "RANCHER ANDY: Ha! Figures.\n"
	.string "Take good care of that TAUROS!$"

GraniteShore_Text_RancherAndyMiltank:
	.string "RANCHER ANDY: Ha! Figures.\n"
	.string "Take good care of that MILTANK!$"

GraniteShore_Text_RancherAndyBouffalant:
	.string "RANCHER ANDY: Ha! Figures.\n"
	.string "Take good care of that BOUFFALANT!$"

GraniteShore_Text_RancherAndyDecline:
	.string "RANCHER ANDY: Suit yourself.\n"
	.string "I'll be right here if you change\l"
	.string "your mind.$"

GraniteShore_Text_RancherAndyAlreadyGiven:
	.string "RANCHER ANDY: Loving the retired\n"
	.string "life out here on the coast.\p"
	.string "You take good care of the one\n"
	.string "I gave you, now!$"
