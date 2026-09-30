GraniteCave_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveHole_CheckFallDownHole
	map_script MAP_SCRIPT_ON_TRANSITION, CaveHole_FixCrackedGround
	map_script MAP_SCRIPT_ON_RESUME, GraniteCave_B1F_SetHoleWarp
	.byte 0

GraniteCave_B1F_SetHoleWarp:
	setstepcallback STEP_CB_CRACKED_FLOOR
	setholewarp MAP_GRANITE_CAVE_B2F
	end

GraniteCave_B1F_EventScript_Dalton::
	trainerbattle_single TRAINER_DALTON, GraniteCave_B1F_Text_DaltonIntro, GraniteCave_B1F_Text_DaltonDefeat
	msgbox GraniteCave_B1F_Text_DaltonPostBattle, MSGBOX_AUTOCLOSE
	end

GraniteCave_B1F_Text_DaltonIntro:
	.string "DALTON: Training in the dark builds\n"
	.string "focus! Or so I keep telling myself.$"

GraniteCave_B1F_Text_DaltonDefeat:
	.string "DALTON: Knocked flat in the dark…$"

GraniteCave_B1F_Text_DaltonPostBattle:
	.string "DALTON: Mind the holes in the floor\n"
	.string "up ahead.$"

