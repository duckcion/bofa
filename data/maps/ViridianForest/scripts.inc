ViridianForest_MapScripts::
	.byte 0

@ Split 1: optional preparation area before Norman. Two trainers and HM06 Rock Smash.
@ The old Johto starter trade was removed (the starter is a gift in the Starter Room now).
ViridianForest_EventScript_Trainer1::
	trainerbattle_single TRAINER_VIRIDIAN_FOREST_1, ViridianForest_Text_Trainer1Intro, ViridianForest_Text_Trainer1Defeated
	msgbox ViridianForest_Text_Trainer1PostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Trainer3::
	trainerbattle_single TRAINER_VIRIDIAN_FOREST_3, ViridianForest_Text_Trainer3Intro, ViridianForest_Text_Trainer3Defeated
	msgbox ViridianForest_Text_Trainer3PostBattle, MSGBOX_AUTOCLOSE
	end

@ Shares FLAG_RECEIVED_HM_ROCK_SMASH with the Mauville City gift, so the HM is only given once.
ViridianForest_EventScript_RockSmashGiver::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_ROCK_SMASH, ViridianForest_EventScript_ExplainRockSmash
	msgbox ViridianForest_Text_MadeItThrough, MSGBOX_DEFAULT
	giveitem ITEM_HM_ROCK_SMASH
	setflag FLAG_RECEIVED_HM_ROCK_SMASH
	msgbox ViridianForest_Text_ExplainRockSmash, MSGBOX_DEFAULT
	release
	end

ViridianForest_EventScript_ExplainRockSmash::
	msgbox ViridianForest_Text_ExplainRockSmash, MSGBOX_DEFAULT
	release
	end

ViridianForest_Text_Trainer1Intro:
	.string "My bugs are getting stronger every\n"
	.string "day! Wanna see?$"

ViridianForest_Text_Trainer1Defeated:
	.string "They need a little more time…$"

ViridianForest_Text_Trainer1PostBattle:
	.string "Bug POKéMON grow up super fast.$"

ViridianForest_Text_Trainer3Intro:
	.string "I came all the way here to catch a\n"
	.string "PIKACHU. Look!$"

ViridianForest_Text_Trainer3Defeated:
	.string "Zapped…$"

ViridianForest_Text_Trainer3PostBattle:
	.string "PIKACHU live deep in this forest.$"

ViridianForest_Text_MadeItThrough:
	.string "You made it through the whole forest?\n"
	.string "You've earned this!$"

ViridianForest_Text_ExplainRockSmash:
	.string "That's ROCK SMASH. It's a FIGHTING-type\n"
	.string "move, and FIGHTING moves hit NORMAL\l"
	.string "POKéMON hard.\p"
	.string "Sounds like it would come in handy\n"
	.string "at the PETALBURG GYM, doesn't it?$"
