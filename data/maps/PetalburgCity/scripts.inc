# 1 "data/maps/PetalburgCity/scripts.pory"

# 2 "data/maps/PetalburgCity/scripts.pory"
.set LOCALID_WALLY, 2
# 3 "data/maps/PetalburgCity/scripts.pory"
.set LOCALID_BOY, 3
# 4 "data/maps/PetalburgCity/scripts.pory"
.set LOCALID_WALLYS_DAD, 5
# 5 "data/maps/PetalburgCity/scripts.pory"
.set LOCALID_GYM_BOY, 8
# 6 "data/maps/PetalburgCity/scripts.pory"
.set LOCALID_SCOTT, 9
# 7 "data/maps/PetalburgCity/scripts.pory"
.set LOCALID_SPLIT1_RIVAL, 12
# 8 "data/maps/PetalburgCity/scripts.pory"

# 9 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_MapScripts::
# 10 "data/maps/PetalburgCity/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_OnTransition
# 11 "data/maps/PetalburgCity/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PetalburgCity_OnFrame
# 12 "data/maps/PetalburgCity/scripts.pory"
	.byte 0
# 13 "data/maps/PetalburgCity/scripts.pory"

# 14 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_OnTransition:
# 15 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_VISITED_PETALBURG_CITY
# 16 "data/maps/PetalburgCity/scripts.pory"
	call Common_EventScript_SetupRivalGfxId
# 17 "data/maps/PetalburgCity/scripts.pory"
	call_if_set FLAG_ITEM_PETALBURG_COAST_EVERSTONE, PetalburgCity_EventScript_Split1GotEverstone
# 18 "data/maps/PetalburgCity/scripts.pory"
	call_if_ge VAR_SPLIT1_STATE, 2, PetalburgCity_EventScript_Split1HideGymBlocker
# 19 "data/maps/PetalburgCity/scripts.pory"
	call_if_lt VAR_SPLIT1_STATE, 3, PetalburgCity_EventScript_Split1HideRival
# 20 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_PETALBURG_CITY_STATE, 0, PetalburgCity_EventScript_MoveGymBoyToWestEntrance
# 21 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_PETALBURG_CITY_STATE, 2, PetalburgCity_EventScript_DisableMapNameAndMusic
# 22 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_EventScript_DisableMapNameAndMusic
# 23 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_PETALBURG_GYM_STATE, 8, PetalburgCity_EventScript_SetGymDoorsUnlocked
# 24 "data/maps/PetalburgCity/scripts.pory"
	end
# 25 "data/maps/PetalburgCity/scripts.pory"

# 26 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_MoveGymBoyToWestEntrance::
# 27 "data/maps/PetalburgCity/scripts.pory"
	setobjectxyperm LOCALID_GYM_BOY, 5, 11
# 28 "data/maps/PetalburgCity/scripts.pory"
	return
# 29 "data/maps/PetalburgCity/scripts.pory"

# 30 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_DisableMapNameAndMusic::
# 31 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
# 32 "data/maps/PetalburgCity/scripts.pory"
	savebgm MUS_FOLLOW_ME
# 33 "data/maps/PetalburgCity/scripts.pory"
	return
# 34 "data/maps/PetalburgCity/scripts.pory"

# 35 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_SetGymDoorsUnlocked::
# 36 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_PETALBURG_GYM_STATE, 7
# 37 "data/maps/PetalburgCity/scripts.pory"
	return
# 38 "data/maps/PetalburgCity/scripts.pory"

# 39 "data/maps/PetalburgCity/scripts.pory"
@ ---------------------------------------------------------------- Split 1
# 40 "data/maps/PetalburgCity/scripts.pory"
@ VAR_SPLIT1_STATE: 0 start, 1 Norman asked for the Everstone (Route 104),
# 41 "data/maps/PetalburgCity/scripts.pory"
@ 2 has the Everstone, 3 beat the rival at the gym entrance.
# 42 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1GotEverstone::
# 43 "data/maps/PetalburgCity/scripts.pory"
	call_if_lt VAR_SPLIT1_STATE, 2, PetalburgCity_EventScript_Split1SetHasEverstone
# 44 "data/maps/PetalburgCity/scripts.pory"
	return
# 45 "data/maps/PetalburgCity/scripts.pory"

# 46 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1SetHasEverstone::
# 47 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_SPLIT1_STATE, 2
# 48 "data/maps/PetalburgCity/scripts.pory"
	return
# 49 "data/maps/PetalburgCity/scripts.pory"

# 50 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1HideGymBlocker::
# 51 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_PETALBURG_GYM_BLOCKER
# 52 "data/maps/PetalburgCity/scripts.pory"
	return
# 53 "data/maps/PetalburgCity/scripts.pory"

# 54 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1HideRival::
# 55 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_PETALBURG_CITY_SPLIT1_RIVAL
# 56 "data/maps/PetalburgCity/scripts.pory"
	return
# 57 "data/maps/PetalburgCity/scripts.pory"

# 58 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_GymBlocker::
# 59 "data/maps/PetalburgCity/scripts.pory"
	lock
# 60 "data/maps/PetalburgCity/scripts.pory"
	faceplayer
# 61 "data/maps/PetalburgCity/scripts.pory"
	goto_if_ge VAR_SPLIT1_STATE, 1, PetalburgCity_EventScript_GymBlockerQuest
# 62 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_GymBlockerNormanAway, MSGBOX_DEFAULT
# 63 "data/maps/PetalburgCity/scripts.pory"
	release
# 64 "data/maps/PetalburgCity/scripts.pory"
	end
# 65 "data/maps/PetalburgCity/scripts.pory"

# 66 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_GymBlockerQuest::
# 67 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_GymBlockerFindEverstone, MSGBOX_DEFAULT
# 68 "data/maps/PetalburgCity/scripts.pory"
	release
# 69 "data/maps/PetalburgCity/scripts.pory"
	end
# 70 "data/maps/PetalburgCity/scripts.pory"

# 71 "data/maps/PetalburgCity/scripts.pory"
@ Fires once, on the tile in front of the gym door, after the Everstone quest.
# 72 "data/maps/PetalburgCity/scripts.pory"
@ A loss leaves VAR_SPLIT1_STATE at 2, so the rival challenges again next time.
# 73 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1Rival::
# 74 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 75 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_HIDE_PETALBURG_CITY_SPLIT1_RIVAL
# 76 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SPLIT1_RIVAL
# 77 "data/maps/PetalburgCity/scripts.pory"
	checkplayergender
# 78 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_RESULT, MALE, PetalburgCity_EventScript_Split1PlayMayMusic
# 79 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_RESULT, FEMALE, PetalburgCity_EventScript_Split1PlayBrendanMusic
# 80 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SPLIT1_RIVAL, PetalburgCity_Movement_Split1RivalApproach
# 81 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
# 82 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 83 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_Split1RivalChallenge, MSGBOX_DEFAULT
# 84 "data/maps/PetalburgCity/scripts.pory"
	checkplayergender
# 85 "data/maps/PetalburgCity/scripts.pory"
	goto_if_eq VAR_RESULT, MALE, PetalburgCity_EventScript_Split1BattleMay
# 86 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1BattleBrendan
# 87 "data/maps/PetalburgCity/scripts.pory"
	end
# 88 "data/maps/PetalburgCity/scripts.pory"

# 89 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1PlayMayMusic::
# 90 "data/maps/PetalburgCity/scripts.pory"
	playbgm MUS_ENCOUNTER_MAY, TRUE
# 91 "data/maps/PetalburgCity/scripts.pory"
	return
# 92 "data/maps/PetalburgCity/scripts.pory"

# 93 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1PlayBrendanMusic::
# 94 "data/maps/PetalburgCity/scripts.pory"
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
# 95 "data/maps/PetalburgCity/scripts.pory"
	return
# 96 "data/maps/PetalburgCity/scripts.pory"

# 97 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1BattleMay::
# 98 "data/maps/PetalburgCity/scripts.pory"
	switch VAR_STARTER_MON
# 99 "data/maps/PetalburgCity/scripts.pory"
	case 0, PetalburgCity_EventScript_Split1MayTreecko
# 100 "data/maps/PetalburgCity/scripts.pory"
	case 1, PetalburgCity_EventScript_Split1MayTorchic
# 101 "data/maps/PetalburgCity/scripts.pory"
	case 2, PetalburgCity_EventScript_Split1MayMudkip
# 102 "data/maps/PetalburgCity/scripts.pory"
	end
# 103 "data/maps/PetalburgCity/scripts.pory"

# 104 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1BattleBrendan::
# 105 "data/maps/PetalburgCity/scripts.pory"
	switch VAR_STARTER_MON
# 106 "data/maps/PetalburgCity/scripts.pory"
	case 0, PetalburgCity_EventScript_Split1BrendanTreecko
# 107 "data/maps/PetalburgCity/scripts.pory"
	case 1, PetalburgCity_EventScript_Split1BrendanTorchic
# 108 "data/maps/PetalburgCity/scripts.pory"
	case 2, PetalburgCity_EventScript_Split1BrendanMudkip
# 109 "data/maps/PetalburgCity/scripts.pory"
	end
# 110 "data/maps/PetalburgCity/scripts.pory"

# 111 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1MayTreecko::
# 112 "data/maps/PetalburgCity/scripts.pory"
	trainerbattle_no_intro TRAINER_MAY_PETALBURG_TREECKO, PetalburgCity_Text_Split1RivalDefeated
# 113 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1RivalAfter
# 114 "data/maps/PetalburgCity/scripts.pory"
	end
# 115 "data/maps/PetalburgCity/scripts.pory"

# 116 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1MayTorchic::
# 117 "data/maps/PetalburgCity/scripts.pory"
	trainerbattle_no_intro TRAINER_MAY_PETALBURG_TORCHIC, PetalburgCity_Text_Split1RivalDefeated
# 118 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1RivalAfter
# 119 "data/maps/PetalburgCity/scripts.pory"
	end
# 120 "data/maps/PetalburgCity/scripts.pory"

# 121 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1MayMudkip::
# 122 "data/maps/PetalburgCity/scripts.pory"
	trainerbattle_no_intro TRAINER_MAY_PETALBURG_MUDKIP, PetalburgCity_Text_Split1RivalDefeated
# 123 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1RivalAfter
# 124 "data/maps/PetalburgCity/scripts.pory"
	end
# 125 "data/maps/PetalburgCity/scripts.pory"

# 126 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1BrendanTreecko::
# 127 "data/maps/PetalburgCity/scripts.pory"
	trainerbattle_no_intro TRAINER_BRENDAN_PETALBURG_TREECKO, PetalburgCity_Text_Split1RivalDefeated
# 128 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1RivalAfter
# 129 "data/maps/PetalburgCity/scripts.pory"
	end
# 130 "data/maps/PetalburgCity/scripts.pory"

# 131 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1BrendanTorchic::
# 132 "data/maps/PetalburgCity/scripts.pory"
	trainerbattle_no_intro TRAINER_BRENDAN_PETALBURG_TORCHIC, PetalburgCity_Text_Split1RivalDefeated
# 133 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1RivalAfter
# 134 "data/maps/PetalburgCity/scripts.pory"
	end
# 135 "data/maps/PetalburgCity/scripts.pory"

# 136 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1BrendanMudkip::
# 137 "data/maps/PetalburgCity/scripts.pory"
	trainerbattle_no_intro TRAINER_BRENDAN_PETALBURG_MUDKIP, PetalburgCity_Text_Split1RivalDefeated
# 138 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Split1RivalAfter
# 139 "data/maps/PetalburgCity/scripts.pory"
	end
# 140 "data/maps/PetalburgCity/scripts.pory"

# 141 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Split1RivalAfter::
# 142 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_Split1RivalAfter, MSGBOX_DEFAULT
# 143 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 144 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SPLIT1_RIVAL, PetalburgCity_Movement_Split1RivalLeave
# 145 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 146 "data/maps/PetalburgCity/scripts.pory"
	removeobject LOCALID_SPLIT1_RIVAL
# 147 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_PETALBURG_CITY_SPLIT1_RIVAL
# 148 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_DEFEATED_PETALBURG_RIVAL
# 149 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_SPLIT1_STATE, 3
# 150 "data/maps/PetalburgCity/scripts.pory"
	savebgm MUS_DUMMY
# 151 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 152 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 153 "data/maps/PetalburgCity/scripts.pory"
	end
# 154 "data/maps/PetalburgCity/scripts.pory"

# 155 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_Split1RivalApproach:
# 156 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 157 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 158 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 159 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 160 "data/maps/PetalburgCity/scripts.pory"

# 161 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_Split1RivalLeave:
# 162 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 163 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 164 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 165 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 166 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 167 "data/maps/PetalburgCity/scripts.pory"

# 168 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_GymBlockerNormanAway:
# 169 "data/maps/PetalburgCity/scripts.pory"
	.string "Sorry, the GYM's closed right now.\p"
# 170 "data/maps/PetalburgCity/scripts.pory"
	.string "NORMAN went out to ROUTE 104, just\n"
# 171 "data/maps/PetalburgCity/scripts.pory"
	.string "west of town. Something about an\l"
# 172 "data/maps/PetalburgCity/scripts.pory"
	.string "EVERSTONE.\p"
# 173 "data/maps/PetalburgCity/scripts.pory"
	.string "If you want to battle him, you'll\n"
# 174 "data/maps/PetalburgCity/scripts.pory"
	.string "have to go find him there.$"
# 175 "data/maps/PetalburgCity/scripts.pory"

# 176 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_GymBlockerFindEverstone:
# 177 "data/maps/PetalburgCity/scripts.pory"
	.string "NORMAN won't open the GYM until\n"
# 178 "data/maps/PetalburgCity/scripts.pory"
	.string "someone finds that EVERSTONE.\p"
# 179 "data/maps/PetalburgCity/scripts.pory"
	.string "Head south through TRAINER GROVE\n"
# 180 "data/maps/PetalburgCity/scripts.pory"
	.string "to PETALBURG COAST.$"
# 181 "data/maps/PetalburgCity/scripts.pory"

# 182 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_Split1RivalChallenge:
# 183 "data/maps/PetalburgCity/scripts.pory"
	.string "{RIVAL}: {PLAYER}{KUN}! There you are!\p"
# 184 "data/maps/PetalburgCity/scripts.pory"
	.string "You're about to take on the GYM?\n"
# 185 "data/maps/PetalburgCity/scripts.pory"
	.string "Not before you get past me!$"
# 186 "data/maps/PetalburgCity/scripts.pory"

# 187 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_Split1RivalDefeated:
# 188 "data/maps/PetalburgCity/scripts.pory"
	.string "Whoa… You're really strong now!$"
# 189 "data/maps/PetalburgCity/scripts.pory"

# 190 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_Split1RivalAfter:
# 191 "data/maps/PetalburgCity/scripts.pory"
	.string "{RIVAL}: Okay, you're ready.\n"
# 192 "data/maps/PetalburgCity/scripts.pory"
	.string "Go show the GYM LEADER what you've got!$"
# 193 "data/maps/PetalburgCity/scripts.pory"

# 194 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_OnFrame:
# 195 "data/maps/PetalburgCity/scripts.pory"
	map_script_2 VAR_PETALBURG_CITY_STATE, 2, PetalburgCity_EventScript_WallyTutorial
# 196 "data/maps/PetalburgCity/scripts.pory"
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_EventScript_WalkToWallyHouse
# 197 "data/maps/PetalburgCity/scripts.pory"
	.2byte 0
# 198 "data/maps/PetalburgCity/scripts.pory"

# 199 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallyTutorial::
# 200 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 201 "data/maps/PetalburgCity/scripts.pory"
	special SavePlayerParty
# 202 "data/maps/PetalburgCity/scripts.pory"
	special LoadWallyZigzagoon
# 203 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLY, PetalburgCity_Movement_WallyTutorialWally
# 204 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_WallyTutorialPlayer
# 205 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 206 "data/maps/PetalburgCity/scripts.pory"
	msgbox Route102_Text_WatchMeCatchPokemon, MSGBOX_DEFAULT
# 207 "data/maps/PetalburgCity/scripts.pory"
	special StartWallyTutorialBattle
# 208 "data/maps/PetalburgCity/scripts.pory"
	waitstate
# 209 "data/maps/PetalburgCity/scripts.pory"
	msgbox Route102_Text_WallyIDidIt, MSGBOX_DEFAULT
# 210 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLY, Common_Movement_WalkInPlaceFasterLeft, MAP_PETALBURG_CITY
# 211 "data/maps/PetalburgCity/scripts.pory"
	waitmovement LOCALID_WALLY, MAP_PETALBURG_CITY
# 212 "data/maps/PetalburgCity/scripts.pory"
	msgbox Route102_Text_LetsGoBack, MSGBOX_DEFAULT
# 213 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 214 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_HIDE_MAP_NAME_POPUP
# 215 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_PETALBURG_CITY_STATE, 3
# 216 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 217 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 218 "data/maps/PetalburgCity/scripts.pory"
	special LoadPlayerParty
# 219 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_PETALBURG_GYM_STATE, 1
# 220 "data/maps/PetalburgCity/scripts.pory"
	warp MAP_PETALBURG_CITY_GYM, 4, 108
# 221 "data/maps/PetalburgCity/scripts.pory"
	waitstate
# 222 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 223 "data/maps/PetalburgCity/scripts.pory"
	end
# 224 "data/maps/PetalburgCity/scripts.pory"

# 225 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WalkToWallyHouse::
# 226 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 227 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
# 228 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLYS_DAD, PetalburgCity_Movement_WalkToWallyHouseWallysDad
# 229 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_WalkToWallyHousePlayer
# 230 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 231 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8004, 7
# 232 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8005, 5
# 233 "data/maps/PetalburgCity/scripts.pory"
	opendoor VAR_0x8004, VAR_0x8005
# 234 "data/maps/PetalburgCity/scripts.pory"
	waitdooranim
# 235 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLYS_DAD, PetalburgCity_Movement_WalkInsideHouseWallysDad
# 236 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_WalkInsideHousePlayer
# 237 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 238 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_PETALBURG_CITY_WALLYS_DAD
# 239 "data/maps/PetalburgCity/scripts.pory"
	hideobjectat OBJ_EVENT_ID_PLAYER, MAP_PETALBURG_CITY
# 240 "data/maps/PetalburgCity/scripts.pory"
	closedoor VAR_0x8004, VAR_0x8005
# 241 "data/maps/PetalburgCity/scripts.pory"
	waitdooranim
# 242 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_HIDE_MAP_NAME_POPUP
# 243 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 244 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 245 "data/maps/PetalburgCity/scripts.pory"
	warp MAP_PETALBURG_CITY_WALLYS_HOUSE, 2, 4
# 246 "data/maps/PetalburgCity/scripts.pory"
	waitstate
# 247 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 248 "data/maps/PetalburgCity/scripts.pory"
	end
# 249 "data/maps/PetalburgCity/scripts.pory"

# 250 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Boy::
# 251 "data/maps/PetalburgCity/scripts.pory"
	lock
# 252 "data/maps/PetalburgCity/scripts.pory"
	faceplayer
# 253 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WaterReflection, MSGBOX_DEFAULT
# 254 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 255 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_BOY, Common_Movement_FaceOriginalDirection
# 256 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 257 "data/maps/PetalburgCity/scripts.pory"
	release
# 258 "data/maps/PetalburgCity/scripts.pory"
	end
# 259 "data/maps/PetalburgCity/scripts.pory"

# 260 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallysMom::
# 261 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WhereIsWally, MSGBOX_NPC
# 262 "data/maps/PetalburgCity/scripts.pory"
	end
# 263 "data/maps/PetalburgCity/scripts.pory"

# 264 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkToWallyHousePlayer:
# 265 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 266 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 267 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 268 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 269 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 270 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 271 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 272 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 273 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 274 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 275 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 276 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 277 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 278 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 279 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 280 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 281 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 282 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 283 "data/maps/PetalburgCity/scripts.pory"

# 284 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkInsideHousePlayer:
# 285 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 286 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 287 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 288 "data/maps/PetalburgCity/scripts.pory"

# 289 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkToWallyHouseWallysDad:
# 290 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 291 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 292 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 293 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 294 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 295 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 296 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 297 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 298 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 299 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 300 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 301 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 302 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 303 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 304 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 305 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 306 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 307 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 308 "data/maps/PetalburgCity/scripts.pory"

# 309 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkInsideHouseWallysDad:
# 310 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 311 "data/maps/PetalburgCity/scripts.pory"
	set_invisible
# 312 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 313 "data/maps/PetalburgCity/scripts.pory"

# 314 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WallyTutorialPlayer:
# 315 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 316 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 317 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 318 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 319 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 320 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 321 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 322 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 323 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 324 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 325 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 326 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 327 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 328 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 329 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 330 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 331 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 332 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 333 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 334 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 335 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 336 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 337 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 338 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 339 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 340 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 341 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 342 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 343 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 344 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 345 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 346 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_right
# 347 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 348 "data/maps/PetalburgCity/scripts.pory"

# 349 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WallyTutorialWally:
# 350 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 351 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 352 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 353 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 354 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 355 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 356 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 357 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 358 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 359 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 360 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 361 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 362 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 363 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 364 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 365 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 366 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 367 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 368 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 369 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 370 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 371 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 372 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 373 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 374 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 375 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 376 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 377 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 378 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 379 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 380 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 381 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 382 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 383 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 384 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 385 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_right
# 386 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 387 "data/maps/PetalburgCity/scripts.pory"

# 388 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_GymSign::
# 389 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_GymSign, MSGBOX_SIGN
# 390 "data/maps/PetalburgCity/scripts.pory"
	end
# 391 "data/maps/PetalburgCity/scripts.pory"

# 392 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_CitySign::
# 393 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_CitySign, MSGBOX_SIGN
# 394 "data/maps/PetalburgCity/scripts.pory"
	end
# 395 "data/maps/PetalburgCity/scripts.pory"

# 396 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman::
# 397 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_FullPartyExplanation, MSGBOX_NPC
# 398 "data/maps/PetalburgCity/scripts.pory"
	end
# 399 "data/maps/PetalburgCity/scripts.pory"

# 400 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallyHouseSign::
# 401 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WallyHouseSign, MSGBOX_SIGN
# 402 "data/maps/PetalburgCity/scripts.pory"
	end
# 403 "data/maps/PetalburgCity/scripts.pory"

# 404 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer0::
# 405 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 406 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 0
# 407 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 408 "data/maps/PetalburgCity/scripts.pory"
	end
# 409 "data/maps/PetalburgCity/scripts.pory"

# 410 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer1::
# 411 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 412 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 1
# 413 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 414 "data/maps/PetalburgCity/scripts.pory"
	end
# 415 "data/maps/PetalburgCity/scripts.pory"

# 416 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer2::
# 417 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 418 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 2
# 419 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 420 "data/maps/PetalburgCity/scripts.pory"
	end
# 421 "data/maps/PetalburgCity/scripts.pory"

# 422 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer3::
# 423 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 424 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 3
# 425 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 426 "data/maps/PetalburgCity/scripts.pory"
	end
# 427 "data/maps/PetalburgCity/scripts.pory"

# 428 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer::
# 429 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_FacePlayer
# 430 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 431 "data/maps/PetalburgCity/scripts.pory"
	playbgm MUS_FOLLOW_ME, FALSE
# 432 "data/maps/PetalburgCity/scripts.pory"
	playse SE_PIN
# 433 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_ExclamationMark
# 434 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 435 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_Delay48
# 436 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 437 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 0, PetalburgCity_EventScript_BoyApproachPlayer0
# 438 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 1, PetalburgCity_EventScript_BoyApproachPlayer1
# 439 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 2, PetalburgCity_EventScript_BoyApproachPlayer2
# 440 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 3, PetalburgCity_EventScript_BoyApproachPlayer3
# 441 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_AreYouRookieTrainer, MSGBOX_DEFAULT
# 442 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 443 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 0, PetalburgCity_EventScript_LeadPlayerToGym0
# 444 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 1, PetalburgCity_EventScript_LeadPlayerToGym1
# 445 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 2, PetalburgCity_EventScript_LeadPlayerToGym2
# 446 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 3, PetalburgCity_EventScript_LeadPlayerToGym3
# 447 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_ThisIsPetalburgGym, MSGBOX_DEFAULT
# 448 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_WalkInPlaceFasterRight
# 449 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
# 450 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 451 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_ThisIsGymSign, MSGBOX_DEFAULT
# 452 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 453 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkAway
# 454 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 455 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 456 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 457 "data/maps/PetalburgCity/scripts.pory"
	end
# 458 "data/maps/PetalburgCity/scripts.pory"

# 459 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer0::
# 460 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer0
# 461 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 462 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
# 463 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 464 "data/maps/PetalburgCity/scripts.pory"
	return
# 465 "data/maps/PetalburgCity/scripts.pory"

# 466 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer1::
# 467 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer1
# 468 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 469 "data/maps/PetalburgCity/scripts.pory"
	return
# 470 "data/maps/PetalburgCity/scripts.pory"

# 471 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer2::
# 472 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer2
# 473 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 474 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
# 475 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 476 "data/maps/PetalburgCity/scripts.pory"
	return
# 477 "data/maps/PetalburgCity/scripts.pory"

# 478 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer3::
# 479 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer3
# 480 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 481 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
# 482 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 483 "data/maps/PetalburgCity/scripts.pory"
	return
# 484 "data/maps/PetalburgCity/scripts.pory"

# 485 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym0::
# 486 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym0
# 487 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym0
# 488 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 489 "data/maps/PetalburgCity/scripts.pory"
	return
# 490 "data/maps/PetalburgCity/scripts.pory"

# 491 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym1::
# 492 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym1
# 493 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym1
# 494 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 495 "data/maps/PetalburgCity/scripts.pory"
	return
# 496 "data/maps/PetalburgCity/scripts.pory"

# 497 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym2::
# 498 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym2
# 499 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym2
# 500 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 501 "data/maps/PetalburgCity/scripts.pory"
	return
# 502 "data/maps/PetalburgCity/scripts.pory"

# 503 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym3::
# 504 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym3
# 505 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym3
# 506 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 507 "data/maps/PetalburgCity/scripts.pory"
	return
# 508 "data/maps/PetalburgCity/scripts.pory"

# 509 "data/maps/PetalburgCity/scripts.pory"
@ Unused
# 510 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_Delay48::
# 511 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 512 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 513 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 514 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 515 "data/maps/PetalburgCity/scripts.pory"

# 516 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer0:
# 517 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 518 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 519 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 520 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 521 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 522 "data/maps/PetalburgCity/scripts.pory"

# 523 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer1:
# 524 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 525 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 526 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 527 "data/maps/PetalburgCity/scripts.pory"

# 528 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer2:
# 529 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 530 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 531 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 532 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 533 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 534 "data/maps/PetalburgCity/scripts.pory"

# 535 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer3:
# 536 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 537 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 538 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 539 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 540 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 541 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 542 "data/maps/PetalburgCity/scripts.pory"

# 543 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym0:
# 544 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 545 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 546 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 547 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 548 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 549 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 550 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 551 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 552 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 553 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 554 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 555 "data/maps/PetalburgCity/scripts.pory"

# 556 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym1:
# 557 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 558 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 559 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 560 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 561 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 562 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 563 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 564 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 565 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 566 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 567 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 568 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 569 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 570 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 571 "data/maps/PetalburgCity/scripts.pory"

# 572 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym2:
# 573 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 574 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 575 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 576 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 577 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 578 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 579 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 580 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 581 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 582 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 583 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 584 "data/maps/PetalburgCity/scripts.pory"

# 585 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym3:
# 586 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 587 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 588 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 589 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 590 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 591 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 592 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 593 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 594 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 595 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 596 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 597 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 598 "data/maps/PetalburgCity/scripts.pory"

# 599 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkAway:
# 600 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 601 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 602 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 603 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 604 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 605 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 606 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 607 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 608 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 609 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 610 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 611 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 612 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 613 "data/maps/PetalburgCity/scripts.pory"

# 614 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym0:
# 615 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 616 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 617 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 618 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 619 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 620 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 621 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 622 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 623 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 624 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 625 "data/maps/PetalburgCity/scripts.pory"

# 626 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym1:
# 627 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 628 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 629 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 630 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 631 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 632 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 633 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 634 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 635 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 636 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 637 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 638 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 639 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 640 "data/maps/PetalburgCity/scripts.pory"

# 641 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym2:
# 642 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 643 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 644 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 645 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 646 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 647 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 648 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 649 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 650 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 651 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 652 "data/maps/PetalburgCity/scripts.pory"

# 653 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym3:
# 654 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 655 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 656 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 657 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 658 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 659 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 660 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 661 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 662 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 663 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 664 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 665 "data/maps/PetalburgCity/scripts.pory"

# 666 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott0::
# 667 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 668 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 669 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 0
# 670 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 10
# 671 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 672 "data/maps/PetalburgCity/scripts.pory"
	end
# 673 "data/maps/PetalburgCity/scripts.pory"

# 674 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott1::
# 675 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 676 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 677 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 1
# 678 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 11
# 679 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 680 "data/maps/PetalburgCity/scripts.pory"
	end
# 681 "data/maps/PetalburgCity/scripts.pory"

# 682 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott2::
# 683 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 684 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 685 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 2
# 686 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 12
# 687 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 688 "data/maps/PetalburgCity/scripts.pory"
	end
# 689 "data/maps/PetalburgCity/scripts.pory"

# 690 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott3::
# 691 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 692 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 693 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 3
# 694 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 13
# 695 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 696 "data/maps/PetalburgCity/scripts.pory"
	end
# 697 "data/maps/PetalburgCity/scripts.pory"

# 698 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott::
# 699 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottStartWalkLeft
# 700 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 701 "data/maps/PetalburgCity/scripts.pory"
	playse SE_PIN
# 702 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_ExclamationMark
# 703 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 704 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_Delay48
# 705 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 706 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottApproachPlayer
# 707 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 708 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
# 709 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 710 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_SCOTT_STATE, 1
# 711 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_AreYouATrainer, MSGBOX_DEFAULT
# 712 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 713 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterRight
# 714 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 715 "data/maps/PetalburgCity/scripts.pory"
	delay 30
# 716 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WellMaybeNot, MSGBOX_DEFAULT
# 717 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 718 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterLeft
# 719 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 720 "data/maps/PetalburgCity/scripts.pory"
	delay 30
# 721 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_ImLookingForTalentedTrainers, MSGBOX_DEFAULT
# 722 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 723 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 0, PetalburgCity_EventScript_ScottExit0
# 724 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 1, PetalburgCity_EventScript_ScottExit1
# 725 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 2, PetalburgCity_EventScript_ScottExit2
# 726 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 3, PetalburgCity_EventScript_ScottExit3
# 727 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_SCOTT_PETALBURG_ENCOUNTER, 1
# 728 "data/maps/PetalburgCity/scripts.pory"
	removeobject LOCALID_SCOTT
# 729 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 730 "data/maps/PetalburgCity/scripts.pory"
	end
# 731 "data/maps/PetalburgCity/scripts.pory"

# 732 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit0::
# 733 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit0
# 734 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit0
# 735 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 736 "data/maps/PetalburgCity/scripts.pory"
	return
# 737 "data/maps/PetalburgCity/scripts.pory"

# 738 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit1::
# 739 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit1
# 740 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit1
# 741 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 742 "data/maps/PetalburgCity/scripts.pory"
	return
# 743 "data/maps/PetalburgCity/scripts.pory"

# 744 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit2::
# 745 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit2
# 746 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit2
# 747 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 748 "data/maps/PetalburgCity/scripts.pory"
	return
# 749 "data/maps/PetalburgCity/scripts.pory"

# 750 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit3::
# 751 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit3
# 752 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit3
# 753 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 754 "data/maps/PetalburgCity/scripts.pory"
	return
# 755 "data/maps/PetalburgCity/scripts.pory"

# 756 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottStartWalkLeft:
# 757 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 758 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 759 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 760 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 761 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 762 "data/maps/PetalburgCity/scripts.pory"

# 763 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottApproachPlayer:
# 764 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 765 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 766 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 767 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 768 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 769 "data/maps/PetalburgCity/scripts.pory"

# 770 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit0:
# 771 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 772 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 773 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 774 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 775 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 776 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 777 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 778 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 779 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 780 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 781 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 782 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 783 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 784 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 785 "data/maps/PetalburgCity/scripts.pory"

# 786 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit0:
# 787 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 788 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 789 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 790 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 791 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 792 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 793 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 794 "data/maps/PetalburgCity/scripts.pory"

# 795 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit1:
# 796 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 797 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 798 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 799 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 800 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 801 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 802 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 803 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 804 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 805 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 806 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 807 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 808 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 809 "data/maps/PetalburgCity/scripts.pory"

# 810 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit1:
# 811 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 812 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 813 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 814 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 815 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 816 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 817 "data/maps/PetalburgCity/scripts.pory"

# 818 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit2:
# 819 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 820 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 821 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 822 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 823 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 824 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 825 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 826 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 827 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 828 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 829 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 830 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 831 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 832 "data/maps/PetalburgCity/scripts.pory"

# 833 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit2:
# 834 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 835 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 836 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 837 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 838 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 839 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 840 "data/maps/PetalburgCity/scripts.pory"

# 841 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit3:
# 842 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 843 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 844 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 845 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 846 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 847 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 848 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 849 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 850 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 851 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 852 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 853 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 854 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 855 "data/maps/PetalburgCity/scripts.pory"

# 856 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit3:
# 857 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 858 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 859 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 860 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 861 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 862 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 863 "data/maps/PetalburgCity/scripts.pory"

# 864 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_GymBoy::
# 865 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_AreYouRookieTrainer, MSGBOX_NPC
# 866 "data/maps/PetalburgCity/scripts.pory"
	end
# 867 "data/maps/PetalburgCity/scripts.pory"

# 868 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WhereIsWally:
# 869 "data/maps/PetalburgCity/scripts.pory"
	.string "Where has our WALLY gone?\p"
# 870 "data/maps/PetalburgCity/scripts.pory"
	.string "We have to leave for VERDANTURF TOWN\n"
# 871 "data/maps/PetalburgCity/scripts.pory"
	.string "very soon…$"
# 872 "data/maps/PetalburgCity/scripts.pory"

# 873 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_AreYouRookieTrainer:
# 874 "data/maps/PetalburgCity/scripts.pory"
	.string "Hiya! Are you maybe…\n"
# 875 "data/maps/PetalburgCity/scripts.pory"
	.string "A rookie TRAINER?\p"
# 876 "data/maps/PetalburgCity/scripts.pory"
	.string "Do you know what POKéMON TRAINERS\n"
# 877 "data/maps/PetalburgCity/scripts.pory"
	.string "do when they reach a new town?\p"
# 878 "data/maps/PetalburgCity/scripts.pory"
	.string "They first check what kind of GYM\n"
# 879 "data/maps/PetalburgCity/scripts.pory"
	.string "is in the town.$"
# 880 "data/maps/PetalburgCity/scripts.pory"

# 881 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_ThisIsPetalburgGym:
# 882 "data/maps/PetalburgCity/scripts.pory"
	.string "See? This is PETALBURG CITY's GYM.$"
# 883 "data/maps/PetalburgCity/scripts.pory"

# 884 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_ThisIsGymSign:
# 885 "data/maps/PetalburgCity/scripts.pory"
	.string "This is the GYM's sign. Look for it\n"
# 886 "data/maps/PetalburgCity/scripts.pory"
	.string "whenever you're looking for a GYM.$"
# 887 "data/maps/PetalburgCity/scripts.pory"

# 888 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WaterReflection:
# 889 "data/maps/PetalburgCity/scripts.pory"
	.string "My face is reflected in the water.\p"
# 890 "data/maps/PetalburgCity/scripts.pory"
	.string "It's a shining grin full of hope…\p"
# 891 "data/maps/PetalburgCity/scripts.pory"
	.string "Or it could be a look of somber silence\n"
# 892 "data/maps/PetalburgCity/scripts.pory"
	.string "struggling with fear…\p"
# 893 "data/maps/PetalburgCity/scripts.pory"
	.string "What do you see reflected in your face?$"
# 894 "data/maps/PetalburgCity/scripts.pory"

# 895 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_FullPartyExplanation:
# 896 "data/maps/PetalburgCity/scripts.pory"
	.string "Let's say you have six POKéMON.\n"
# 897 "data/maps/PetalburgCity/scripts.pory"
	.string "If you catch another one…\p"
# 898 "data/maps/PetalburgCity/scripts.pory"
	.string "It is automatically sent to a STORAGE\n"
# 899 "data/maps/PetalburgCity/scripts.pory"
	.string "BOX over a PC connection.$"
# 900 "data/maps/PetalburgCity/scripts.pory"

# 901 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_GymSign:
# 902 "data/maps/PetalburgCity/scripts.pory"
	.string "PETALBURG CITY POKéMON GYM\n"
# 903 "data/maps/PetalburgCity/scripts.pory"
	.string "LEADER: NORMAN\l"
# 904 "data/maps/PetalburgCity/scripts.pory"
	.string "“A man in pursuit of power!”$"
# 905 "data/maps/PetalburgCity/scripts.pory"

# 906 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_CitySign:
# 907 "data/maps/PetalburgCity/scripts.pory"
	.string "PETALBURG CITY\n"
# 908 "data/maps/PetalburgCity/scripts.pory"
	.string "“Where people mingle with nature.”$"
# 909 "data/maps/PetalburgCity/scripts.pory"

# 910 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WallyHouseSign:
# 911 "data/maps/PetalburgCity/scripts.pory"
	.string "WALLY'S HOUSE$"
# 912 "data/maps/PetalburgCity/scripts.pory"

# 913 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_AreYouATrainer:
# 914 "data/maps/PetalburgCity/scripts.pory"
	.string "Excuse me!\p"
# 915 "data/maps/PetalburgCity/scripts.pory"
	.string "Let me guess, from the way you're\n"
# 916 "data/maps/PetalburgCity/scripts.pory"
	.string "dressed, are you a POKéMON TRAINER?$"
# 917 "data/maps/PetalburgCity/scripts.pory"

# 918 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WellMaybeNot:
# 919 "data/maps/PetalburgCity/scripts.pory"
	.string "… … … … … …\p"
# 920 "data/maps/PetalburgCity/scripts.pory"
	.string "Well, maybe not.\n"
# 921 "data/maps/PetalburgCity/scripts.pory"
	.string "Your clothes aren't all that dirty.\p"
# 922 "data/maps/PetalburgCity/scripts.pory"
	.string "You're either a rookie TRAINER,\n"
# 923 "data/maps/PetalburgCity/scripts.pory"
	.string "or maybe you're just an ordinary kid.$"
# 924 "data/maps/PetalburgCity/scripts.pory"

# 925 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_ImLookingForTalentedTrainers:
# 926 "data/maps/PetalburgCity/scripts.pory"
	.string "I'm roaming the land in search of\n"
# 927 "data/maps/PetalburgCity/scripts.pory"
	.string "talented TRAINERS.\p"
# 928 "data/maps/PetalburgCity/scripts.pory"
	.string "I'm sorry to have taken your time.$"
