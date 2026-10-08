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
# 14 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET2, TrainerGrove_Text_Trainer2Intro, TrainerGrove_Text_Trainer2Outro, TrainerGrove_EventScript_MaisieReward
# 15 "data/maps/TrainerGrove/scripts.pory"
	lock
# 16 "data/maps/TrainerGrove/scripts.pory"
	faceplayer
# 17 "data/maps/TrainerGrove/scripts.pory"
	goto_if_unset FLAG_RECEIVED_MIRACLE_SEED_GROVE, TrainerGrove_EventScript_TrainerTwo_2
TrainerGrove_EventScript_TrainerTwo_1:
# 20 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer2PostBattle, MSGBOX_DEFAULT
# 21 "data/maps/TrainerGrove/scripts.pory"
	release
	end

TrainerGrove_EventScript_TrainerTwo_2:
# 18 "data/maps/TrainerGrove/scripts.pory"
	goto TrainerGrove_EventScript_MaisieReward
	goto TrainerGrove_EventScript_TrainerTwo_1


TrainerGrove_EventScript_MaisieReward::
# 26 "data/maps/TrainerGrove/scripts.pory"
	lock
# 27 "data/maps/TrainerGrove/scripts.pory"
	faceplayer
# 28 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_MaisieGift, MSGBOX_DEFAULT
# 29 "data/maps/TrainerGrove/scripts.pory"
	giveitem ITEM_MIRACLE_SEED
# 30 "data/maps/TrainerGrove/scripts.pory"
	compare VAR_RESULT, FALSE
	goto_if_eq TrainerGrove_EventScript_MaisieReward_2
TrainerGrove_EventScript_MaisieReward_1:
# 33 "data/maps/TrainerGrove/scripts.pory"
	setflag FLAG_RECEIVED_MIRACLE_SEED_GROVE
# 34 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer2PostBattle, MSGBOX_DEFAULT
# 35 "data/maps/TrainerGrove/scripts.pory"
	release
	end

TrainerGrove_EventScript_MaisieReward_2:
# 31 "data/maps/TrainerGrove/scripts.pory"
	goto Common_EventScript_ShowBagIsFull
	goto TrainerGrove_EventScript_MaisieReward_1


TrainerGrove_EventScript_TrainerThree::
# 44 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET3, TrainerGrove_Text_Trainer3Intro, TrainerGrove_Text_Trainer3Outro
# 45 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer3PostBattle, MSGBOX_DEFAULT
# 46 "data/maps/TrainerGrove/scripts.pory"
	release
	end


TrainerGrove_EventScript_PathClosed::
# 90 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 91 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_PathClosed, MSGBOX_DEFAULT
# 92 "data/maps/TrainerGrove/scripts.pory"
	closemessage
# 93 "data/maps/TrainerGrove/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, TrainerGrove_Movement_StepBack
# 94 "data/maps/TrainerGrove/scripts.pory"
	waitmovement 0
# 95 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end


# 99 "data/maps/TrainerGrove/scripts.pory"
TrainerGrove_Movement_StepBack:
# 100 "data/maps/TrainerGrove/scripts.pory"
	walk_up
	step_end

TrainerGrove_Text_MaisieGift::
# 39 "data/maps/TrainerGrove/scripts.pory"
	.string "You're good with POKéMON!\pTake this MIRACLE SEED. It powers\nup GRASS-type moves.$"

TrainerGrove_Text_Trainer1Intro::
# 50 "data/maps/TrainerGrove/scripts.pory"
	.string "I've hiked every trail around PETALBURG.\nLet's see if you can keep up!$"

TrainerGrove_Text_Trainer1Outro::
# 54 "data/maps/TrainerGrove/scripts.pory"
	.string "Whew! You're tougher than a\nmountain climb.$"

TrainerGrove_Text_Trainer1PostBattle::
# 58 "data/maps/TrainerGrove/scripts.pory"
	.string "My POKéMON are tough as rocks, but\nyours hit even harder.$"

TrainerGrove_Text_Trainer2Intro::
# 62 "data/maps/TrainerGrove/scripts.pory"
	.string "I packed a picnic and my favorite\nPOKéMON from UNOVA. Want to battle?$"

TrainerGrove_Text_Trainer2Outro::
# 66 "data/maps/TrainerGrove/scripts.pory"
	.string "Oh no, my lunch is going to\nget cold!$"

TrainerGrove_Text_Trainer2PostBattle::
# 70 "data/maps/TrainerGrove/scripts.pory"
	.string "Every one of my POKéMON comes from\nthe UNOVA region. Aren't they cute?$"

TrainerGrove_Text_Trainer3Intro::
# 74 "data/maps/TrainerGrove/scripts.pory"
	.string "I came out here to camp, but a battle sounds even better!$"

TrainerGrove_Text_Trainer3Outro::
# 79 "data/maps/TrainerGrove/scripts.pory"
	.string "Aww, my campfire went out…$"

TrainerGrove_Text_Trainer3PostBattle::
# 83 "data/maps/TrainerGrove/scripts.pory"
	.string "The coast is just past here. Watch out for the tide!$"

TrainerGrove_Text_PathClosed::
# 103 "data/maps/TrainerGrove/scripts.pory"
	.string "The grove is full of TRAINERS.\nBetter find DAD on ROUTE 104 first.$"
