TrainerGrove_MapScripts::
# 2 "data/maps/TrainerGrove/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, TrainerGrove_OnTransition
	.byte 0


TrainerGrove_OnTransition::
	end


TrainerGrove_EventScript_TrainerOne::
# 10 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 11 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET1, TrainerGrove_Text_Trainer1Intro, TrainerGrove_Text_Trainer1Outro
# 12 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end


TrainerGrove_EventScript_TrainerTwo::
# 17 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 18 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET2, TrainerGrove_Text_Trainer2Intro, TrainerGrove_Text_Trainer2Outro
# 19 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end


TrainerGrove_EventScript_TrainerThree::
# 24 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 25 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET3, TrainerGrove_Text_Trainer3Intro, TrainerGrove_Text_Trainer3Outro
# 26 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end


TrainerGrove_EventScript_TrainerFour::
# 31 "data/maps/TrainerGrove/scripts.pory"
	lockall
# 32 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET4, TrainerGrove_Text_Trainer4Intro, TrainerGrove_Text_Trainer4Outro
# 33 "data/maps/TrainerGrove/scripts.pory"
	releaseall
	end

