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
	msgbox TrainerGrove_Text_Trainer2PostBattle, MSGBOX_DEFAULT
# 15 "data/maps/TrainerGrove/scripts.pory"
	release
	end


TrainerGrove_EventScript_TrainerThree::
# 20 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET3, TrainerGrove_Text_Trainer3Intro, TrainerGrove_Text_Trainer3Outro
# 21 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer3PostBattle, MSGBOX_DEFAULT
# 22 "data/maps/TrainerGrove/scripts.pory"
	release
	end


TrainerGrove_EventScript_PathClosed::
# 66 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 67 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_PathClosed, MSGBOX_DEFAULT
# 68 "data/maps/TrainerGrove/scripts.pory"
	closemessage
# 69 "data/maps/TrainerGrove/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, TrainerGrove_Movement_StepBack
# 70 "data/maps/TrainerGrove/scripts.pory"
	waitmovement 0
# 71 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end


# 75 "data/maps/TrainerGrove/scripts.pory"
TrainerGrove_Movement_StepBack:
# 76 "data/maps/TrainerGrove/scripts.pory"
	walk_up
	step_end

TrainerGrove_Text_Trainer1Intro::
# 26 "data/maps/TrainerGrove/scripts.pory"
	.string "Bring that ass here boy!$"

TrainerGrove_Text_Trainer1Outro::
# 30 "data/maps/TrainerGrove/scripts.pory"
	.string "Your gyatt was too much for me to handle.$"

TrainerGrove_Text_Trainer1PostBattle::
# 34 "data/maps/TrainerGrove/scripts.pory"
	.string "Damn, you're the goat.$"

TrainerGrove_Text_Trainer2Intro::
# 38 "data/maps/TrainerGrove/scripts.pory"
	.string "Uhh I think I'm in the wrong place.$"

TrainerGrove_Text_Trainer2Outro::
# 42 "data/maps/TrainerGrove/scripts.pory"
	.string "Damn, could you at least get me unstuck stepbro?$"

TrainerGrove_Text_Trainer2PostBattle::
# 46 "data/maps/TrainerGrove/scripts.pory"
	.string "Arrgghhhh! I'm STUCK!$"

TrainerGrove_Text_Trainer3Intro::
# 50 "data/maps/TrainerGrove/scripts.pory"
	.string "I came out here to camp, but a battle sounds even better!$"

TrainerGrove_Text_Trainer3Outro::
# 55 "data/maps/TrainerGrove/scripts.pory"
	.string "Aww, my campfire went out…$"

TrainerGrove_Text_Trainer3PostBattle::
# 59 "data/maps/TrainerGrove/scripts.pory"
	.string "The coast is just past here. Watch out for the tide!$"

TrainerGrove_Text_PathClosed::
# 79 "data/maps/TrainerGrove/scripts.pory"
	.string "The grove is full of TRAINERS.\nBetter find DAD on ROUTE 104 first.$"
