TrainerGrove_MapScripts::
	.byte 0


TrainerGrove_EventScript_TrainerOne::
# 6 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET1, TrainerGrove_Text_Trainer1Intro, TrainerGrove_Text_Trainer1Outro
# 7 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer1PostBattle, MSGBOX_DEFAULT
# 8 "data/maps/TrainerGrove/scripts.pory"
	release
	end


TrainerGrove_EventScript_TrainerTwo::
# 13 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET2, TrainerGrove_Text_Trainer2Intro, TrainerGrove_Text_Trainer2Outro
# 14 "data/maps/TrainerGrove/scripts.pory"
	lock
# 15 "data/maps/TrainerGrove/scripts.pory"
	faceplayer
# 16 "data/maps/TrainerGrove/scripts.pory"
	goto_if_unset FLAG_RECEIVED_MIRACLE_SEED_GROVE, TrainerGrove_EventScript_TrainerTwo_2
TrainerGrove_EventScript_TrainerTwo_1:
# 24 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer2PostBattle, MSGBOX_DEFAULT
# 25 "data/maps/TrainerGrove/scripts.pory"
	release
	end

TrainerGrove_EventScript_TrainerTwo_2:
# 17 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_MaisieGift, MSGBOX_DEFAULT
# 18 "data/maps/TrainerGrove/scripts.pory"
	giveitem ITEM_MIRACLE_SEED
# 19 "data/maps/TrainerGrove/scripts.pory"
	compare VAR_RESULT, FALSE
	goto_if_eq TrainerGrove_EventScript_TrainerTwo_5
TrainerGrove_EventScript_TrainerTwo_4:
# 22 "data/maps/TrainerGrove/scripts.pory"
	setflag FLAG_RECEIVED_MIRACLE_SEED_GROVE
	goto TrainerGrove_EventScript_TrainerTwo_1

TrainerGrove_EventScript_TrainerTwo_5:
# 20 "data/maps/TrainerGrove/scripts.pory"
	goto Common_EventScript_ShowBagIsFull
	goto TrainerGrove_EventScript_TrainerTwo_4


TrainerGrove_EventScript_TrainerThree::
# 34 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET3, TrainerGrove_Text_Trainer3Intro, TrainerGrove_Text_Trainer3Outro
# 35 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer3PostBattle, MSGBOX_DEFAULT
# 36 "data/maps/TrainerGrove/scripts.pory"
	release
	end


TrainerGrove_EventScript_PathClosed::
# 80 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 81 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_PathClosed, MSGBOX_DEFAULT
# 82 "data/maps/TrainerGrove/scripts.pory"
	closemessage
# 83 "data/maps/TrainerGrove/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, TrainerGrove_Movement_StepBack
# 84 "data/maps/TrainerGrove/scripts.pory"
	waitmovement 0
# 85 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end


# 89 "data/maps/TrainerGrove/scripts.pory"
TrainerGrove_Movement_StepBack:
# 90 "data/maps/TrainerGrove/scripts.pory"
	walk_up
	step_end

TrainerGrove_Text_MaisieGift::
# 29 "data/maps/TrainerGrove/scripts.pory"
	.string "You're good with POKéMON!\pTake this MIRACLE SEED. It powers\nup GRASS-type moves.$"

TrainerGrove_Text_Trainer1Intro::
# 40 "data/maps/TrainerGrove/scripts.pory"
	.string "I've hiked every trail around PETALBURG.\nLet's see if you can keep up!$"

TrainerGrove_Text_Trainer1Outro::
# 44 "data/maps/TrainerGrove/scripts.pory"
	.string "Whew! You're tougher than a\nmountain climb.$"

TrainerGrove_Text_Trainer1PostBattle::
# 48 "data/maps/TrainerGrove/scripts.pory"
	.string "My POKéMON are tough as rocks, but\nyours hit even harder.$"

TrainerGrove_Text_Trainer2Intro::
# 52 "data/maps/TrainerGrove/scripts.pory"
	.string "I packed a picnic and my favorite\nPOKéMON from UNOVA. Want to battle?$"

TrainerGrove_Text_Trainer2Outro::
# 56 "data/maps/TrainerGrove/scripts.pory"
	.string "Oh no, my lunch is going to\nget cold!$"

TrainerGrove_Text_Trainer2PostBattle::
# 60 "data/maps/TrainerGrove/scripts.pory"
	.string "Every one of my POKéMON comes from\nthe UNOVA region. Aren't they cute?$"

TrainerGrove_Text_Trainer3Intro::
# 64 "data/maps/TrainerGrove/scripts.pory"
	.string "I came out here to camp, but a battle sounds even better!$"

TrainerGrove_Text_Trainer3Outro::
# 69 "data/maps/TrainerGrove/scripts.pory"
	.string "Aww, my campfire went out…$"

TrainerGrove_Text_Trainer3PostBattle::
# 73 "data/maps/TrainerGrove/scripts.pory"
	.string "The coast is just past here. Watch out for the tide!$"

TrainerGrove_Text_PathClosed::
# 93 "data/maps/TrainerGrove/scripts.pory"
	.string "The grove is full of TRAINERS.\nBetter find DAD on ROUTE 104 first.$"
