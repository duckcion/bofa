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
	trainerbattle_double TRAINER_DALTON, GraniteCave_B1F_Text_DaltonIntro, GraniteCave_B1F_Text_DaltonDefeat, GraniteCave_B1F_Text_DaltonNeedTwoMons
	msgbox GraniteCave_B1F_Text_DaltonPostBattle, MSGBOX_AUTOCLOSE
	end

GraniteCave_B1F_Text_DaltonIntro:
	.string "I study how cave POKéMON work\n"
	.string "together. Let me show you a pair\l"
	.string "that covers every gap!$"

GraniteCave_B1F_Text_DaltonDefeat:
	.string "My data never predicted you…$"

GraniteCave_B1F_Text_DaltonPostBattle:
	.string "Two POKéMON who cover each other\n"
	.string "beat two strong ones. Usually.$"

GraniteCave_B1F_Text_DaltonNeedTwoMons:
	.string "My research needs a double battle.\n"
	.string "Come back with two POKéMON.$"
