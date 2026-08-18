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


TrainerGrove_EventScript_TrainerFour::
# 27 "data/maps/TrainerGrove/scripts.pory"
	trainerbattle_single TRAINER_TRAINERGROVET4, TrainerGrove_Text_Trainer4Intro, TrainerGrove_Text_Trainer4Outro
# 28 "data/maps/TrainerGrove/scripts.pory"
	msgbox TrainerGrove_Text_Trainer4PostBattle, MSGBOX_DEFAULT
# 29 "data/maps/TrainerGrove/scripts.pory"
	release
	end


TrainerGrove_Text_Trainer1Intro::
# 33 "data/maps/TrainerGrove/scripts.pory"
	.string "Bring that ass here boy!$"

TrainerGrove_Text_Trainer1Outro::
# 37 "data/maps/TrainerGrove/scripts.pory"
	.string "Your gyatt was too much for me to handle.$"

TrainerGrove_Text_Trainer1PostBattle::
# 41 "data/maps/TrainerGrove/scripts.pory"
	.string "Damn, you're the goat.$"

TrainerGrove_Text_Trainer2Intro::
# 45 "data/maps/TrainerGrove/scripts.pory"
	.string "Uhh I think I'm in the wrong place.$"

TrainerGrove_Text_Trainer2Outro::
# 49 "data/maps/TrainerGrove/scripts.pory"
	.string "Damn, could you at least get me unstuck stepbro?$"

TrainerGrove_Text_Trainer2PostBattle::
# 53 "data/maps/TrainerGrove/scripts.pory"
	.string "Arrgghhhh! I'm STUCK!$"

TrainerGrove_Text_Trainer3Intro::
# 57 "data/maps/TrainerGrove/scripts.pory"
	.string "IMA BOUNCE ON YOU!$"

TrainerGrove_Text_Trainer3Outro::
# 61 "data/maps/TrainerGrove/scripts.pory"
	.string "IM BOUNCING, IM BOUNCING$"

TrainerGrove_Text_Trainer3PostBattle::
# 65 "data/maps/TrainerGrove/scripts.pory"
	.string "This champ sucks, I'ma play Nunu instead.$"

TrainerGrove_Text_Trainer4Intro::
# 69 "data/maps/TrainerGrove/scripts.pory"
	.string "Your gyat is gonna get rizzled.$"

TrainerGrove_Text_Trainer4Outro::
# 73 "data/maps/TrainerGrove/scripts.pory"
	.string "After this loss, I'm going to Ohio.$"

TrainerGrove_Text_Trainer4PostBattle::
# 77 "data/maps/TrainerGrove/scripts.pory"
	.string "OHIO OHIO OHIO.$"
