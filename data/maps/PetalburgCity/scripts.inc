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
.set LOCALID_SPLIT1_RIVAL, 11
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
@ Split 1: the first time the player walks into Petalburg from Route 102, Wally runs up with a Focus Sash.
# 195 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallyFocusSash::
# 196 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_TEMP_2, 1
# 197 "data/maps/PetalburgCity/scripts.pory"
	goto_if_set FLAG_RECEIVED_FOCUS_SASH_WALLY, PetalburgCity_EventScript_WallyFocusSashEnd
# 198 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 199 "data/maps/PetalburgCity/scripts.pory"
	getplayerxy VAR_0x8004, VAR_0x8005
# 200 "data/maps/PetalburgCity/scripts.pory"
	copyvar VAR_0x8006, VAR_0x8004
# 201 "data/maps/PetalburgCity/scripts.pory"
	subvar VAR_0x8006, 3
# 202 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_HIDE_PETALBURG_CITY_WALLY
# 203 "data/maps/PetalburgCity/scripts.pory"
	setobjectxyperm LOCALID_WALLY, VAR_0x8006, VAR_0x8005
# 204 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_WALLY
# 205 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLY, PetalburgCity_Movement_WallyRunUp
# 206 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 207 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
# 208 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 209 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WallyFocusSash, MSGBOX_DEFAULT
# 210 "data/maps/PetalburgCity/scripts.pory"
	giveitem ITEM_FOCUS_SASH
# 211 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_RECEIVED_FOCUS_SASH_WALLY
# 212 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WallyFocusSashAfter, MSGBOX_DEFAULT
# 213 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 214 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLY, PetalburgCity_Movement_WallyLeave
# 215 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 216 "data/maps/PetalburgCity/scripts.pory"
	removeobject LOCALID_WALLY
# 217 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_PETALBURG_CITY_WALLY
# 218 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 219 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallyFocusSashEnd::
# 220 "data/maps/PetalburgCity/scripts.pory"
	end
# 221 "data/maps/PetalburgCity/scripts.pory"

# 222 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WallyRunUp:
# 223 "data/maps/PetalburgCity/scripts.pory"
	walk_fast_right
# 224 "data/maps/PetalburgCity/scripts.pory"
	walk_fast_right
# 225 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 226 "data/maps/PetalburgCity/scripts.pory"

# 227 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WallyLeave:
# 228 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 229 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 230 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 231 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 232 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 233 "data/maps/PetalburgCity/scripts.pory"

# 234 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WallyFocusSash:
# 235 "data/maps/PetalburgCity/scripts.pory"
	.string "WALLY: Um… excuse me! You're a\n"
# 236 "data/maps/PetalburgCity/scripts.pory"
	.string "TRAINER, right?\p"
# 237 "data/maps/PetalburgCity/scripts.pory"
	.string "I'm WALLY. I want to be a TRAINER\n"
# 238 "data/maps/PetalburgCity/scripts.pory"
	.string "too, someday…\p"
# 239 "data/maps/PetalburgCity/scripts.pory"
	.string "My dad gave me this, but I don't have\n"
# 240 "data/maps/PetalburgCity/scripts.pory"
	.string "any POKéMON yet. Please, take it!$"
# 241 "data/maps/PetalburgCity/scripts.pory"

# 242 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WallyFocusSashAfter:
# 243 "data/maps/PetalburgCity/scripts.pory"
	.string "If a POKéMON holding a FOCUS SASH has\n"
# 244 "data/maps/PetalburgCity/scripts.pory"
	.string "full HP, it can survive any hit!\p"
# 245 "data/maps/PetalburgCity/scripts.pory"
	.string "Good luck… I'll catch up to you\n"
# 246 "data/maps/PetalburgCity/scripts.pory"
	.string "someday!$"
# 247 "data/maps/PetalburgCity/scripts.pory"

# 248 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman_Gift::
# 249 "data/maps/PetalburgCity/scripts.pory"
	goto_if_set FLAG_RECEIVED_SILK_SCARF_PETALBURG, PetalburgCity_EventScript_Gentleman_GiftDone
# 250 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_EventScript_Gentleman_Text_GiftIntro, MSGBOX_DEFAULT
# 251 "data/maps/PetalburgCity/scripts.pory"
	giveitem ITEM_SILK_SCARF
# 252 "data/maps/PetalburgCity/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
# 253 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_RECEIVED_SILK_SCARF_PETALBURG
# 254 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_EventScript_Gentleman_Text_GiftAfter, MSGBOX_DEFAULT
# 255 "data/maps/PetalburgCity/scripts.pory"
	release
# 256 "data/maps/PetalburgCity/scripts.pory"
	end
# 257 "data/maps/PetalburgCity/scripts.pory"

# 258 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman_GiftDone::
# 259 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_EventScript_Gentleman_Text_GiftDone, MSGBOX_DEFAULT
# 260 "data/maps/PetalburgCity/scripts.pory"
	release
# 261 "data/maps/PetalburgCity/scripts.pory"
	end
# 262 "data/maps/PetalburgCity/scripts.pory"

# 263 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman_Text_GiftIntro:
# 264 "data/maps/PetalburgCity/scripts.pory"
	.string "Ah, a young TRAINER. Let me share\n"
# 265 "data/maps/PetalburgCity/scripts.pory"
	.string "a little secret.\p"
# 266 "data/maps/PetalburgCity/scripts.pory"
	.string "Some held items make moves of one\n"
# 267 "data/maps/PetalburgCity/scripts.pory"
	.string "type hit harder.\p"
# 268 "data/maps/PetalburgCity/scripts.pory"
	.string "A SILK SCARF boosts NORMAL-type\n"
# 269 "data/maps/PetalburgCity/scripts.pory"
	.string "moves. Here, I'd like you to have it.$"
# 270 "data/maps/PetalburgCity/scripts.pory"

# 271 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman_Text_GiftAfter:
# 272 "data/maps/PetalburgCity/scripts.pory"
	.string "There are items like this for every\n"
# 273 "data/maps/PetalburgCity/scripts.pory"
	.string "type. Keep an eye out for them!$"
# 274 "data/maps/PetalburgCity/scripts.pory"

# 275 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman_Text_GiftDone:
# 276 "data/maps/PetalburgCity/scripts.pory"
	.string "Type-boosting items are simple, but\n"
# 277 "data/maps/PetalburgCity/scripts.pory"
	.string "they make a real difference.$"
# 278 "data/maps/PetalburgCity/scripts.pory"

# 279 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_OnFrame:
# 280 "data/maps/PetalburgCity/scripts.pory"
	map_script_2 VAR_PETALBURG_CITY_STATE, 2, PetalburgCity_EventScript_WallyTutorial
# 281 "data/maps/PetalburgCity/scripts.pory"
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_EventScript_WalkToWallyHouse
# 282 "data/maps/PetalburgCity/scripts.pory"
	.2byte 0
# 283 "data/maps/PetalburgCity/scripts.pory"

# 284 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallyTutorial::
# 285 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 286 "data/maps/PetalburgCity/scripts.pory"
	special SavePlayerParty
# 287 "data/maps/PetalburgCity/scripts.pory"
	special LoadWallyZigzagoon
# 288 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLY, PetalburgCity_Movement_WallyTutorialWally
# 289 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_WallyTutorialPlayer
# 290 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 291 "data/maps/PetalburgCity/scripts.pory"
	msgbox Route102_Text_WatchMeCatchPokemon, MSGBOX_DEFAULT
# 292 "data/maps/PetalburgCity/scripts.pory"
	special StartWallyTutorialBattle
# 293 "data/maps/PetalburgCity/scripts.pory"
	waitstate
# 294 "data/maps/PetalburgCity/scripts.pory"
	msgbox Route102_Text_WallyIDidIt, MSGBOX_DEFAULT
# 295 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLY, Common_Movement_WalkInPlaceFasterLeft, MAP_PETALBURG_CITY
# 296 "data/maps/PetalburgCity/scripts.pory"
	waitmovement LOCALID_WALLY, MAP_PETALBURG_CITY
# 297 "data/maps/PetalburgCity/scripts.pory"
	msgbox Route102_Text_LetsGoBack, MSGBOX_DEFAULT
# 298 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 299 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_HIDE_MAP_NAME_POPUP
# 300 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_PETALBURG_CITY_STATE, 3
# 301 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 302 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 303 "data/maps/PetalburgCity/scripts.pory"
	special LoadPlayerParty
# 304 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_PETALBURG_GYM_STATE, 1
# 305 "data/maps/PetalburgCity/scripts.pory"
	warp MAP_PETALBURG_CITY_GYM, 4, 108
# 306 "data/maps/PetalburgCity/scripts.pory"
	waitstate
# 307 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 308 "data/maps/PetalburgCity/scripts.pory"
	end
# 309 "data/maps/PetalburgCity/scripts.pory"

# 310 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WalkToWallyHouse::
# 311 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 312 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
# 313 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLYS_DAD, PetalburgCity_Movement_WalkToWallyHouseWallysDad
# 314 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_WalkToWallyHousePlayer
# 315 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 316 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8004, 7
# 317 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8005, 5
# 318 "data/maps/PetalburgCity/scripts.pory"
	opendoor VAR_0x8004, VAR_0x8005
# 319 "data/maps/PetalburgCity/scripts.pory"
	waitdooranim
# 320 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_WALLYS_DAD, PetalburgCity_Movement_WalkInsideHouseWallysDad
# 321 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_WalkInsideHousePlayer
# 322 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 323 "data/maps/PetalburgCity/scripts.pory"
	setflag FLAG_HIDE_PETALBURG_CITY_WALLYS_DAD
# 324 "data/maps/PetalburgCity/scripts.pory"
	hideobjectat OBJ_EVENT_ID_PLAYER, MAP_PETALBURG_CITY
# 325 "data/maps/PetalburgCity/scripts.pory"
	closedoor VAR_0x8004, VAR_0x8005
# 326 "data/maps/PetalburgCity/scripts.pory"
	waitdooranim
# 327 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_HIDE_MAP_NAME_POPUP
# 328 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 329 "data/maps/PetalburgCity/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 330 "data/maps/PetalburgCity/scripts.pory"
	warp MAP_PETALBURG_CITY_WALLYS_HOUSE, 2, 4
# 331 "data/maps/PetalburgCity/scripts.pory"
	waitstate
# 332 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 333 "data/maps/PetalburgCity/scripts.pory"
	end
# 334 "data/maps/PetalburgCity/scripts.pory"

# 335 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Boy::
# 336 "data/maps/PetalburgCity/scripts.pory"
	lock
# 337 "data/maps/PetalburgCity/scripts.pory"
	faceplayer
# 338 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WaterReflection, MSGBOX_DEFAULT
# 339 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 340 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_BOY, Common_Movement_FaceOriginalDirection
# 341 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 342 "data/maps/PetalburgCity/scripts.pory"
	release
# 343 "data/maps/PetalburgCity/scripts.pory"
	end
# 344 "data/maps/PetalburgCity/scripts.pory"

# 345 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallysMom::
# 346 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WhereIsWally, MSGBOX_NPC
# 347 "data/maps/PetalburgCity/scripts.pory"
	end
# 348 "data/maps/PetalburgCity/scripts.pory"

# 349 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkToWallyHousePlayer:
# 350 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 351 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 352 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 353 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 354 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 355 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 356 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 357 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 358 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 359 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 360 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 361 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 362 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 363 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 364 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 365 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 366 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 367 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 368 "data/maps/PetalburgCity/scripts.pory"

# 369 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkInsideHousePlayer:
# 370 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 371 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 372 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 373 "data/maps/PetalburgCity/scripts.pory"

# 374 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkToWallyHouseWallysDad:
# 375 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 376 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 377 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 378 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 379 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 380 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 381 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 382 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 383 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 384 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 385 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 386 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 387 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 388 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 389 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 390 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 391 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 392 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 393 "data/maps/PetalburgCity/scripts.pory"

# 394 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WalkInsideHouseWallysDad:
# 395 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 396 "data/maps/PetalburgCity/scripts.pory"
	set_invisible
# 397 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 398 "data/maps/PetalburgCity/scripts.pory"

# 399 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WallyTutorialPlayer:
# 400 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 401 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 402 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 403 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 404 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 405 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 406 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 407 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 408 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 409 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 410 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 411 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 412 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 413 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 414 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 415 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 416 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 417 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 418 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 419 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 420 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 421 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 422 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 423 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 424 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 425 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 426 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 427 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 428 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 429 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 430 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 431 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_right
# 432 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 433 "data/maps/PetalburgCity/scripts.pory"

# 434 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_WallyTutorialWally:
# 435 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 436 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 437 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 438 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 439 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 440 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 441 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 442 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 443 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 444 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 445 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 446 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 447 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 448 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 449 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 450 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 451 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 452 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 453 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 454 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 455 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 456 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 457 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 458 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 459 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 460 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 461 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 462 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 463 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 464 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 465 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 466 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 467 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 468 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 469 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 470 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_right
# 471 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 472 "data/maps/PetalburgCity/scripts.pory"

# 473 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_GymSign::
# 474 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_GymSign, MSGBOX_SIGN
# 475 "data/maps/PetalburgCity/scripts.pory"
	end
# 476 "data/maps/PetalburgCity/scripts.pory"

# 477 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_CitySign::
# 478 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_CitySign, MSGBOX_SIGN
# 479 "data/maps/PetalburgCity/scripts.pory"
	end
# 480 "data/maps/PetalburgCity/scripts.pory"

# 481 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Gentleman::
# 482 "data/maps/PetalburgCity/scripts.pory"
	lock
# 483 "data/maps/PetalburgCity/scripts.pory"
	faceplayer
# 484 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Gentleman_Gift
# 485 "data/maps/PetalburgCity/scripts.pory"
	end
# 486 "data/maps/PetalburgCity/scripts.pory"

# 487 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_WallyHouseSign::
# 488 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WallyHouseSign, MSGBOX_SIGN
# 489 "data/maps/PetalburgCity/scripts.pory"
	end
# 490 "data/maps/PetalburgCity/scripts.pory"

# 491 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer0::
# 492 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 493 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 0
# 494 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 495 "data/maps/PetalburgCity/scripts.pory"
	end
# 496 "data/maps/PetalburgCity/scripts.pory"

# 497 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer1::
# 498 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 499 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 1
# 500 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 501 "data/maps/PetalburgCity/scripts.pory"
	end
# 502 "data/maps/PetalburgCity/scripts.pory"

# 503 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer2::
# 504 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 505 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 2
# 506 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 507 "data/maps/PetalburgCity/scripts.pory"
	end
# 508 "data/maps/PetalburgCity/scripts.pory"

# 509 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer3::
# 510 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 511 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 3
# 512 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_ShowGymToPlayer
# 513 "data/maps/PetalburgCity/scripts.pory"
	end
# 514 "data/maps/PetalburgCity/scripts.pory"

# 515 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ShowGymToPlayer::
# 516 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_FacePlayer
# 517 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 518 "data/maps/PetalburgCity/scripts.pory"
	playbgm MUS_FOLLOW_ME, FALSE
# 519 "data/maps/PetalburgCity/scripts.pory"
	playse SE_PIN
# 520 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_ExclamationMark
# 521 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 522 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_Delay48
# 523 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 524 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 0, PetalburgCity_EventScript_BoyApproachPlayer0
# 525 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 1, PetalburgCity_EventScript_BoyApproachPlayer1
# 526 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 2, PetalburgCity_EventScript_BoyApproachPlayer2
# 527 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 3, PetalburgCity_EventScript_BoyApproachPlayer3
# 528 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_AreYouRookieTrainer, MSGBOX_DEFAULT
# 529 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 530 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 0, PetalburgCity_EventScript_LeadPlayerToGym0
# 531 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 1, PetalburgCity_EventScript_LeadPlayerToGym1
# 532 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 2, PetalburgCity_EventScript_LeadPlayerToGym2
# 533 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 3, PetalburgCity_EventScript_LeadPlayerToGym3
# 534 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_ThisIsPetalburgGym, MSGBOX_DEFAULT
# 535 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, Common_Movement_WalkInPlaceFasterRight
# 536 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
# 537 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 538 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_ThisIsGymSign, MSGBOX_DEFAULT
# 539 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 540 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkAway
# 541 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 542 "data/maps/PetalburgCity/scripts.pory"
	fadedefaultbgm
# 543 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 544 "data/maps/PetalburgCity/scripts.pory"
	end
# 545 "data/maps/PetalburgCity/scripts.pory"

# 546 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer0::
# 547 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer0
# 548 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 549 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
# 550 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 551 "data/maps/PetalburgCity/scripts.pory"
	return
# 552 "data/maps/PetalburgCity/scripts.pory"

# 553 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer1::
# 554 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer1
# 555 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 556 "data/maps/PetalburgCity/scripts.pory"
	return
# 557 "data/maps/PetalburgCity/scripts.pory"

# 558 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer2::
# 559 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer2
# 560 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 561 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
# 562 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 563 "data/maps/PetalburgCity/scripts.pory"
	return
# 564 "data/maps/PetalburgCity/scripts.pory"

# 565 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_BoyApproachPlayer3::
# 566 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyApproachPlayer3
# 567 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 568 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
# 569 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 570 "data/maps/PetalburgCity/scripts.pory"
	return
# 571 "data/maps/PetalburgCity/scripts.pory"

# 572 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym0::
# 573 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym0
# 574 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym0
# 575 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 576 "data/maps/PetalburgCity/scripts.pory"
	return
# 577 "data/maps/PetalburgCity/scripts.pory"

# 578 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym1::
# 579 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym1
# 580 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym1
# 581 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 582 "data/maps/PetalburgCity/scripts.pory"
	return
# 583 "data/maps/PetalburgCity/scripts.pory"

# 584 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym2::
# 585 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym2
# 586 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym2
# 587 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 588 "data/maps/PetalburgCity/scripts.pory"
	return
# 589 "data/maps/PetalburgCity/scripts.pory"

# 590 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_LeadPlayerToGym3::
# 591 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_GYM_BOY, PetalburgCity_Movement_BoyWalkToGym3
# 592 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWalkToGym3
# 593 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 594 "data/maps/PetalburgCity/scripts.pory"
	return
# 595 "data/maps/PetalburgCity/scripts.pory"

# 596 "data/maps/PetalburgCity/scripts.pory"
@ Unused
# 597 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_Delay48::
# 598 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 599 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 600 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 601 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 602 "data/maps/PetalburgCity/scripts.pory"

# 603 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer0:
# 604 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 605 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 606 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 607 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 608 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 609 "data/maps/PetalburgCity/scripts.pory"

# 610 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer1:
# 611 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 612 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 613 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 614 "data/maps/PetalburgCity/scripts.pory"

# 615 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer2:
# 616 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 617 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 618 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 619 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 620 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 621 "data/maps/PetalburgCity/scripts.pory"

# 622 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyApproachPlayer3:
# 623 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 624 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 625 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 626 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 627 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 628 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 629 "data/maps/PetalburgCity/scripts.pory"

# 630 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym0:
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
	walk_right
# 638 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 639 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 640 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 641 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 642 "data/maps/PetalburgCity/scripts.pory"

# 643 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym1:
# 644 "data/maps/PetalburgCity/scripts.pory"
	walk_down
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
	walk_right
# 651 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 652 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 653 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 654 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 655 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 656 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 657 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 658 "data/maps/PetalburgCity/scripts.pory"

# 659 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym2:
# 660 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 661 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 662 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 663 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 664 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 665 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 666 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 667 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 668 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 669 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 670 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 671 "data/maps/PetalburgCity/scripts.pory"

# 672 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkToGym3:
# 673 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 674 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 675 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 676 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 677 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 678 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 679 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 680 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 681 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 682 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 683 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 684 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 685 "data/maps/PetalburgCity/scripts.pory"

# 686 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_BoyWalkAway:
# 687 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 688 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 689 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 690 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 691 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 692 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 693 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 694 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 695 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 696 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 697 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 698 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 699 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 700 "data/maps/PetalburgCity/scripts.pory"

# 701 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym0:
# 702 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 703 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 704 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 705 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 706 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 707 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 708 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 709 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 710 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 711 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 712 "data/maps/PetalburgCity/scripts.pory"

# 713 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym1:
# 714 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 715 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 716 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 717 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 718 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 719 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 720 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 721 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 722 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 723 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 724 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 725 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 726 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 727 "data/maps/PetalburgCity/scripts.pory"

# 728 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym2:
# 729 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 730 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 731 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 732 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 733 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 734 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 735 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 736 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 737 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 738 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 739 "data/maps/PetalburgCity/scripts.pory"

# 740 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWalkToGym3:
# 741 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 742 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 743 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 744 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 745 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 746 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 747 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 748 "data/maps/PetalburgCity/scripts.pory"
	walk_right
# 749 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 750 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 751 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 752 "data/maps/PetalburgCity/scripts.pory"

# 753 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott0::
# 754 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 755 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 756 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 0
# 757 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 10
# 758 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 759 "data/maps/PetalburgCity/scripts.pory"
	end
# 760 "data/maps/PetalburgCity/scripts.pory"

# 761 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott1::
# 762 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 763 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 764 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 1
# 765 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 11
# 766 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 767 "data/maps/PetalburgCity/scripts.pory"
	end
# 768 "data/maps/PetalburgCity/scripts.pory"

# 769 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott2::
# 770 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 771 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 772 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 2
# 773 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 12
# 774 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 775 "data/maps/PetalburgCity/scripts.pory"
	end
# 776 "data/maps/PetalburgCity/scripts.pory"

# 777 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott3::
# 778 "data/maps/PetalburgCity/scripts.pory"
	lockall
# 779 "data/maps/PetalburgCity/scripts.pory"
	addobject LOCALID_SCOTT
# 780 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_0x8008, 3
# 781 "data/maps/PetalburgCity/scripts.pory"
	setobjectxy LOCALID_SCOTT, 13, 13
# 782 "data/maps/PetalburgCity/scripts.pory"
	goto PetalburgCity_EventScript_Scott
# 783 "data/maps/PetalburgCity/scripts.pory"
	end
# 784 "data/maps/PetalburgCity/scripts.pory"

# 785 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_Scott::
# 786 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottStartWalkLeft
# 787 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 788 "data/maps/PetalburgCity/scripts.pory"
	playse SE_PIN
# 789 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_ExclamationMark
# 790 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 791 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_Delay48
# 792 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 793 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottApproachPlayer
# 794 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 795 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
# 796 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 797 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_SCOTT_STATE, 1
# 798 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_AreYouATrainer, MSGBOX_DEFAULT
# 799 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 800 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterRight
# 801 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 802 "data/maps/PetalburgCity/scripts.pory"
	delay 30
# 803 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_WellMaybeNot, MSGBOX_DEFAULT
# 804 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 805 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, Common_Movement_WalkInPlaceFasterLeft
# 806 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 807 "data/maps/PetalburgCity/scripts.pory"
	delay 30
# 808 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_ImLookingForTalentedTrainers, MSGBOX_DEFAULT
# 809 "data/maps/PetalburgCity/scripts.pory"
	closemessage
# 810 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 0, PetalburgCity_EventScript_ScottExit0
# 811 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 1, PetalburgCity_EventScript_ScottExit1
# 812 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 2, PetalburgCity_EventScript_ScottExit2
# 813 "data/maps/PetalburgCity/scripts.pory"
	call_if_eq VAR_0x8008, 3, PetalburgCity_EventScript_ScottExit3
# 814 "data/maps/PetalburgCity/scripts.pory"
	setvar VAR_SCOTT_PETALBURG_ENCOUNTER, 1
# 815 "data/maps/PetalburgCity/scripts.pory"
	removeobject LOCALID_SCOTT
# 816 "data/maps/PetalburgCity/scripts.pory"
	releaseall
# 817 "data/maps/PetalburgCity/scripts.pory"
	end
# 818 "data/maps/PetalburgCity/scripts.pory"

# 819 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit0::
# 820 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit0
# 821 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit0
# 822 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 823 "data/maps/PetalburgCity/scripts.pory"
	return
# 824 "data/maps/PetalburgCity/scripts.pory"

# 825 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit1::
# 826 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit1
# 827 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit1
# 828 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 829 "data/maps/PetalburgCity/scripts.pory"
	return
# 830 "data/maps/PetalburgCity/scripts.pory"

# 831 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit2::
# 832 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit2
# 833 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit2
# 834 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 835 "data/maps/PetalburgCity/scripts.pory"
	return
# 836 "data/maps/PetalburgCity/scripts.pory"

# 837 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_ScottExit3::
# 838 "data/maps/PetalburgCity/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, PetalburgCity_Movement_PlayerWatchScottExit3
# 839 "data/maps/PetalburgCity/scripts.pory"
	applymovement LOCALID_SCOTT, PetalburgCity_Movement_ScottExit3
# 840 "data/maps/PetalburgCity/scripts.pory"
	waitmovement 0
# 841 "data/maps/PetalburgCity/scripts.pory"
	return
# 842 "data/maps/PetalburgCity/scripts.pory"

# 843 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottStartWalkLeft:
# 844 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 845 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 846 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 847 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 848 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 849 "data/maps/PetalburgCity/scripts.pory"

# 850 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottApproachPlayer:
# 851 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 852 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 853 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 854 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 855 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 856 "data/maps/PetalburgCity/scripts.pory"

# 857 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit0:
# 858 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 859 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 860 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 861 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 862 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 863 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 864 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 865 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 866 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 867 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 868 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 869 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 870 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 871 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 872 "data/maps/PetalburgCity/scripts.pory"

# 873 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit0:
# 874 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 875 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 876 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 877 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 878 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 879 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 880 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 881 "data/maps/PetalburgCity/scripts.pory"

# 882 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit1:
# 883 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 884 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 885 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 886 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 887 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 888 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 889 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 890 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 891 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 892 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 893 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 894 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 895 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 896 "data/maps/PetalburgCity/scripts.pory"

# 897 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit1:
# 898 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 899 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 900 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 901 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 902 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 903 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 904 "data/maps/PetalburgCity/scripts.pory"

# 905 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit2:
# 906 "data/maps/PetalburgCity/scripts.pory"
	walk_down
# 907 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 908 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 909 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 910 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 911 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 912 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 913 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 914 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 915 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 916 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 917 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 918 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 919 "data/maps/PetalburgCity/scripts.pory"

# 920 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit2:
# 921 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 922 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_down
# 923 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 924 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 925 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 926 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 927 "data/maps/PetalburgCity/scripts.pory"

# 928 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_ScottExit3:
# 929 "data/maps/PetalburgCity/scripts.pory"
	walk_up
# 930 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 931 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 932 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 933 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 934 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 935 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 936 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 937 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 938 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 939 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 940 "data/maps/PetalburgCity/scripts.pory"
	walk_left
# 941 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 942 "data/maps/PetalburgCity/scripts.pory"

# 943 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Movement_PlayerWatchScottExit3:
# 944 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 945 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_up
# 946 "data/maps/PetalburgCity/scripts.pory"
	delay_16
# 947 "data/maps/PetalburgCity/scripts.pory"
	delay_8
# 948 "data/maps/PetalburgCity/scripts.pory"
	walk_in_place_faster_left
# 949 "data/maps/PetalburgCity/scripts.pory"
	step_end
# 950 "data/maps/PetalburgCity/scripts.pory"

# 951 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_EventScript_GymBoy::
# 952 "data/maps/PetalburgCity/scripts.pory"
	msgbox PetalburgCity_Text_AreYouRookieTrainer, MSGBOX_NPC
# 953 "data/maps/PetalburgCity/scripts.pory"
	end
# 954 "data/maps/PetalburgCity/scripts.pory"

# 955 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WhereIsWally:
# 956 "data/maps/PetalburgCity/scripts.pory"
	.string "Where has our WALLY gone?\p"
# 957 "data/maps/PetalburgCity/scripts.pory"
	.string "We have to leave for VERDANTURF TOWN\n"
# 958 "data/maps/PetalburgCity/scripts.pory"
	.string "very soon…$"
# 959 "data/maps/PetalburgCity/scripts.pory"

# 960 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_AreYouRookieTrainer:
# 961 "data/maps/PetalburgCity/scripts.pory"
	.string "Hiya! Are you maybe…\n"
# 962 "data/maps/PetalburgCity/scripts.pory"
	.string "A rookie TRAINER?\p"
# 963 "data/maps/PetalburgCity/scripts.pory"
	.string "Do you know what POKéMON TRAINERS\n"
# 964 "data/maps/PetalburgCity/scripts.pory"
	.string "do when they reach a new town?\p"
# 965 "data/maps/PetalburgCity/scripts.pory"
	.string "They first check what kind of GYM\n"
# 966 "data/maps/PetalburgCity/scripts.pory"
	.string "is in the town.$"
# 967 "data/maps/PetalburgCity/scripts.pory"

# 968 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_ThisIsPetalburgGym:
# 969 "data/maps/PetalburgCity/scripts.pory"
	.string "See? This is PETALBURG CITY's GYM.$"
# 970 "data/maps/PetalburgCity/scripts.pory"

# 971 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_ThisIsGymSign:
# 972 "data/maps/PetalburgCity/scripts.pory"
	.string "This is the GYM's sign. Look for it\n"
# 973 "data/maps/PetalburgCity/scripts.pory"
	.string "whenever you're looking for a GYM.$"
# 974 "data/maps/PetalburgCity/scripts.pory"

# 975 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WaterReflection:
# 976 "data/maps/PetalburgCity/scripts.pory"
	.string "My face is reflected in the water.\p"
# 977 "data/maps/PetalburgCity/scripts.pory"
	.string "It's a shining grin full of hope…\p"
# 978 "data/maps/PetalburgCity/scripts.pory"
	.string "Or it could be a look of somber silence\n"
# 979 "data/maps/PetalburgCity/scripts.pory"
	.string "struggling with fear…\p"
# 980 "data/maps/PetalburgCity/scripts.pory"
	.string "What do you see reflected in your face?$"
# 981 "data/maps/PetalburgCity/scripts.pory"

# 982 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_FullPartyExplanation:
# 983 "data/maps/PetalburgCity/scripts.pory"
	.string "Let's say you have six POKéMON.\n"
# 984 "data/maps/PetalburgCity/scripts.pory"
	.string "If you catch another one…\p"
# 985 "data/maps/PetalburgCity/scripts.pory"
	.string "It is automatically sent to a STORAGE\n"
# 986 "data/maps/PetalburgCity/scripts.pory"
	.string "BOX over a PC connection.$"
# 987 "data/maps/PetalburgCity/scripts.pory"

# 988 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_GymSign:
# 989 "data/maps/PetalburgCity/scripts.pory"
	.string "PETALBURG CITY POKéMON GYM\n"
# 990 "data/maps/PetalburgCity/scripts.pory"
	.string "LEADER: NORMAN\l"
# 991 "data/maps/PetalburgCity/scripts.pory"
	.string "“A man in pursuit of power!”$"
# 992 "data/maps/PetalburgCity/scripts.pory"

# 993 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_CitySign:
# 994 "data/maps/PetalburgCity/scripts.pory"
	.string "PETALBURG CITY\n"
# 995 "data/maps/PetalburgCity/scripts.pory"
	.string "“Where people mingle with nature.”$"
# 996 "data/maps/PetalburgCity/scripts.pory"

# 997 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WallyHouseSign:
# 998 "data/maps/PetalburgCity/scripts.pory"
	.string "WALLY'S HOUSE$"
# 999 "data/maps/PetalburgCity/scripts.pory"

# 1000 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_AreYouATrainer:
# 1001 "data/maps/PetalburgCity/scripts.pory"
	.string "Excuse me!\p"
# 1002 "data/maps/PetalburgCity/scripts.pory"
	.string "Let me guess, from the way you're\n"
# 1003 "data/maps/PetalburgCity/scripts.pory"
	.string "dressed, are you a POKéMON TRAINER?$"
# 1004 "data/maps/PetalburgCity/scripts.pory"

# 1005 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_WellMaybeNot:
# 1006 "data/maps/PetalburgCity/scripts.pory"
	.string "… … … … … …\p"
# 1007 "data/maps/PetalburgCity/scripts.pory"
	.string "Well, maybe not.\n"
# 1008 "data/maps/PetalburgCity/scripts.pory"
	.string "Your clothes aren't all that dirty.\p"
# 1009 "data/maps/PetalburgCity/scripts.pory"
	.string "You're either a rookie TRAINER,\n"
# 1010 "data/maps/PetalburgCity/scripts.pory"
	.string "or maybe you're just an ordinary kid.$"
# 1011 "data/maps/PetalburgCity/scripts.pory"

# 1012 "data/maps/PetalburgCity/scripts.pory"
PetalburgCity_Text_ImLookingForTalentedTrainers:
# 1013 "data/maps/PetalburgCity/scripts.pory"
	.string "I'm roaming the land in search of\n"
# 1014 "data/maps/PetalburgCity/scripts.pory"
	.string "talented TRAINERS.\p"
# 1015 "data/maps/PetalburgCity/scripts.pory"
	.string "I'm sorry to have taken your time.$"
