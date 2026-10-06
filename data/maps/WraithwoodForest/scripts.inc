# 6 "data/maps/WraithwoodForest/scripts.pory"

# 7 "data/maps/WraithwoodForest/scripts.pory"
.set LOCALID_ARCHIE, 5
# 8 "data/maps/WraithwoodForest/scripts.pory"

# 9 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_MapScripts::
# 10 "data/maps/WraithwoodForest/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, WraithwoodForest_OnTransition
# 11 "data/maps/WraithwoodForest/scripts.pory"
	.byte 0
# 12 "data/maps/WraithwoodForest/scripts.pory"

# 13 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_OnTransition:
# 14 "data/maps/WraithwoodForest/scripts.pory"
	setflag FLAG_CLEARED_WRAITHWOOD_ROUTE
# 15 "data/maps/WraithwoodForest/scripts.pory"
	end
# 16 "data/maps/WraithwoodForest/scripts.pory"

# 17 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Desmond::
# 18 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_DESMOND, WraithwoodForest_Text_DesmondIntro, WraithwoodForest_Text_DesmondDefeat, WraithwoodForest_EventScript_TryRegisterDesmondAfterBattle
# 19 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 20 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, WraithwoodForest_EventScript_RematchDesmond
# 21 "data/maps/WraithwoodForest/scripts.pory"
	setvar VAR_0x8004, TRAINER_DESMOND
# 22 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 23 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, WraithwoodForest_EventScript_TryRegisterDesmond
# 24 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondPostBattle, MSGBOX_DEFAULT
# 25 "data/maps/WraithwoodForest/scripts.pory"
	release
# 26 "data/maps/WraithwoodForest/scripts.pory"
	end
# 27 "data/maps/WraithwoodForest/scripts.pory"

# 28 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_RematchDesmond::
# 29 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_rematch TRAINER_DESMOND, WraithwoodForest_Text_DesmondRematchIntro, WraithwoodForest_Text_DesmondDefeat
# 30 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondPostBattle, MSGBOX_DEFAULT
# 31 "data/maps/WraithwoodForest/scripts.pory"
	release
# 32 "data/maps/WraithwoodForest/scripts.pory"
	end
# 33 "data/maps/WraithwoodForest/scripts.pory"

# 34 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterDesmond::
# 35 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondRegister, MSGBOX_DEFAULT
# 36 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_DESMOND
# 37 "data/maps/WraithwoodForest/scripts.pory"
	release
# 38 "data/maps/WraithwoodForest/scripts.pory"
	end
# 39 "data/maps/WraithwoodForest/scripts.pory"

# 40 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterDesmondAfterBattle::
# 41 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondRegister, MSGBOX_DEFAULT
# 42 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_DESMOND
# 43 "data/maps/WraithwoodForest/scripts.pory"
	release
# 44 "data/maps/WraithwoodForest/scripts.pory"
	end
# 45 "data/maps/WraithwoodForest/scripts.pory"

# 46 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondIntro:
# 47 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: The bugs in this fog love\n"
# 48 "data/maps/WraithwoodForest/scripts.pory"
	.string "company! Come keep us busy!$"
# 49 "data/maps/WraithwoodForest/scripts.pory"

# 50 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondDefeat:
# 51 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: Aw, bug out!$"
# 52 "data/maps/WraithwoodForest/scripts.pory"

# 53 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondPostBattle:
# 54 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: This fog's thick, but my bugs\n"
# 55 "data/maps/WraithwoodForest/scripts.pory"
	.string "know the way around.$"
# 56 "data/maps/WraithwoodForest/scripts.pory"

# 57 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondRegister:
# 58 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: Let's swap numbers!$"
# 59 "data/maps/WraithwoodForest/scripts.pory"

# 60 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondRematchIntro:
# 61 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: Back for more bug bites?$"
# 62 "data/maps/WraithwoodForest/scripts.pory"

# 63 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Ottoline::
# 64 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_OTTOLINE, WraithwoodForest_Text_OttolineIntro, WraithwoodForest_Text_OttolineDefeat, WraithwoodForest_EventScript_TryRegisterOttolineAfterBattle
# 65 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 66 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, WraithwoodForest_EventScript_RematchOttoline
# 67 "data/maps/WraithwoodForest/scripts.pory"
	setvar VAR_0x8004, TRAINER_OTTOLINE
# 68 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 69 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, WraithwoodForest_EventScript_TryRegisterOttoline
# 70 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolinePostBattle, MSGBOX_DEFAULT
# 71 "data/maps/WraithwoodForest/scripts.pory"
	release
# 72 "data/maps/WraithwoodForest/scripts.pory"
	end
# 73 "data/maps/WraithwoodForest/scripts.pory"

# 74 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_RematchOttoline::
# 75 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_rematch TRAINER_OTTOLINE, WraithwoodForest_Text_OttolineRematchIntro, WraithwoodForest_Text_OttolineDefeat
# 76 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolinePostBattle, MSGBOX_DEFAULT
# 77 "data/maps/WraithwoodForest/scripts.pory"
	release
# 78 "data/maps/WraithwoodForest/scripts.pory"
	end
# 79 "data/maps/WraithwoodForest/scripts.pory"

# 80 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterOttoline::
# 81 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolineRegister, MSGBOX_DEFAULT
# 82 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_OTTOLINE
# 83 "data/maps/WraithwoodForest/scripts.pory"
	release
# 84 "data/maps/WraithwoodForest/scripts.pory"
	end
# 85 "data/maps/WraithwoodForest/scripts.pory"

# 86 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterOttolineAfterBattle::
# 87 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolineRegister, MSGBOX_DEFAULT
# 88 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_OTTOLINE
# 89 "data/maps/WraithwoodForest/scripts.pory"
	release
# 90 "data/maps/WraithwoodForest/scripts.pory"
	end
# 91 "data/maps/WraithwoodForest/scripts.pory"

# 92 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineIntro:
# 93 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: Something's watching us from\n"
# 94 "data/maps/WraithwoodForest/scripts.pory"
	.string "the fog… oh, it's just me!$"
# 95 "data/maps/WraithwoodForest/scripts.pory"

# 96 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineDefeat:
# 97 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: My spirits… they've fled!$"
# 98 "data/maps/WraithwoodForest/scripts.pory"

# 99 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolinePostBattle:
# 100 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: This place is thick with\n"
# 101 "data/maps/WraithwoodForest/scripts.pory"
	.string "restless spirits.$"
# 102 "data/maps/WraithwoodForest/scripts.pory"

# 103 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineRegister:
# 104 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: Let's stay in touch!$"
# 105 "data/maps/WraithwoodForest/scripts.pory"

# 106 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineRematchIntro:
# 107 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: The spirits are restless again!$"
# 108 "data/maps/WraithwoodForest/scripts.pory"

# 109 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Rufus::
# 110 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_RUFUS, WraithwoodForest_Text_RufusIntro, WraithwoodForest_Text_RufusDefeat, WraithwoodForest_EventScript_TryRegisterRufusAfterBattle
# 111 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 112 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, WraithwoodForest_EventScript_RematchRufus
# 113 "data/maps/WraithwoodForest/scripts.pory"
	setvar VAR_0x8004, TRAINER_RUFUS
# 114 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 115 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, WraithwoodForest_EventScript_TryRegisterRufus
# 116 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusPostBattle, MSGBOX_DEFAULT
# 117 "data/maps/WraithwoodForest/scripts.pory"
	release
# 118 "data/maps/WraithwoodForest/scripts.pory"
	end
# 119 "data/maps/WraithwoodForest/scripts.pory"

# 120 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_RematchRufus::
# 121 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_rematch TRAINER_RUFUS, WraithwoodForest_Text_RufusRematchIntro, WraithwoodForest_Text_RufusDefeat
# 122 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusPostBattle, MSGBOX_DEFAULT
# 123 "data/maps/WraithwoodForest/scripts.pory"
	release
# 124 "data/maps/WraithwoodForest/scripts.pory"
	end
# 125 "data/maps/WraithwoodForest/scripts.pory"

# 126 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterRufus::
# 127 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusRegister, MSGBOX_DEFAULT
# 128 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_RUFUS
# 129 "data/maps/WraithwoodForest/scripts.pory"
	release
# 130 "data/maps/WraithwoodForest/scripts.pory"
	end
# 131 "data/maps/WraithwoodForest/scripts.pory"

# 132 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterRufusAfterBattle::
# 133 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusRegister, MSGBOX_DEFAULT
# 134 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_RUFUS
# 135 "data/maps/WraithwoodForest/scripts.pory"
	release
# 136 "data/maps/WraithwoodForest/scripts.pory"
	end
# 137 "data/maps/WraithwoodForest/scripts.pory"

# 138 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusIntro:
# 139 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Heading for GRANITE CAVE?\n"
# 140 "data/maps/WraithwoodForest/scripts.pory"
	.string "Not before you beat me!$"
# 141 "data/maps/WraithwoodForest/scripts.pory"

# 142 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusDefeat:
# 143 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Solid hit, that.$"
# 144 "data/maps/WraithwoodForest/scripts.pory"

# 145 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusPostBattle:
# 146 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: The cave entrance is just\n"
# 147 "data/maps/WraithwoodForest/scripts.pory"
	.string "south of here.$"
# 148 "data/maps/WraithwoodForest/scripts.pory"

# 149 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusRegister:
# 150 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Let's trade numbers!$"
# 151 "data/maps/WraithwoodForest/scripts.pory"

# 152 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusRematchIntro:
# 153 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Fancy another round?$"
# 154 "data/maps/WraithwoodForest/scripts.pory"

# 155 "data/maps/WraithwoodForest/scripts.pory"
@ ---- Split 3: Team Aqua in Wraithwood (4 grunts, then Archie)
# 156 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Grunt1::
# 157 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_WRAITHWOOD_GRUNT_1, WraithwoodForest_Text_Grunt1Intro, WraithwoodForest_Text_Grunt1Defeat
# 158 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_Grunt1Post, MSGBOX_AUTOCLOSE
# 159 "data/maps/WraithwoodForest/scripts.pory"
	end
# 160 "data/maps/WraithwoodForest/scripts.pory"

# 161 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Grunt2::
# 162 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_WRAITHWOOD_GRUNT_2, WraithwoodForest_Text_Grunt2Intro, WraithwoodForest_Text_Grunt2Defeat
# 163 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_Grunt2Post, MSGBOX_AUTOCLOSE
# 164 "data/maps/WraithwoodForest/scripts.pory"
	end
# 165 "data/maps/WraithwoodForest/scripts.pory"

# 166 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Grunt3::
# 167 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_WRAITHWOOD_GRUNT_3, WraithwoodForest_Text_Grunt3Intro, WraithwoodForest_Text_Grunt3Defeat
# 168 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_Grunt3Post, MSGBOX_AUTOCLOSE
# 169 "data/maps/WraithwoodForest/scripts.pory"
	end
# 170 "data/maps/WraithwoodForest/scripts.pory"

# 171 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Grunt4::
# 172 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_WRAITHWOOD_GRUNT_4, WraithwoodForest_Text_Grunt4Intro, WraithwoodForest_Text_Grunt4Defeat
# 173 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_Grunt4Post, MSGBOX_AUTOCLOSE
# 174 "data/maps/WraithwoodForest/scripts.pory"
	end
# 175 "data/maps/WraithwoodForest/scripts.pory"

# 176 "data/maps/WraithwoodForest/scripts.pory"
@ Archie stands in the north exit; stepping onto either exit tile starts the battle.
# 177 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_ArchieTrigger::
# 178 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_defeated TRAINER_ARCHIE_WRAITHWOOD, WraithwoodForest_EventScript_ArchieEnd
# 179 "data/maps/WraithwoodForest/scripts.pory"
	lockall
# 180 "data/maps/WraithwoodForest/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
# 181 "data/maps/WraithwoodForest/scripts.pory"
	waitmovement 0
# 182 "data/maps/WraithwoodForest/scripts.pory"
	goto WraithwoodForest_EventScript_ArchieBattle
# 183 "data/maps/WraithwoodForest/scripts.pory"

# 184 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Archie::
# 185 "data/maps/WraithwoodForest/scripts.pory"
	lockall
# 186 "data/maps/WraithwoodForest/scripts.pory"
	faceplayer
# 187 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_ArchieBattle::
# 188 "data/maps/WraithwoodForest/scripts.pory"
	playbgm MUS_ENCOUNTER_AQUA, FALSE
# 189 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_ArchieIntro, MSGBOX_DEFAULT
# 190 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_no_intro TRAINER_ARCHIE_WRAITHWOOD, WraithwoodForest_Text_ArchieDefeat
# 191 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_ArchiePost, MSGBOX_DEFAULT
# 192 "data/maps/WraithwoodForest/scripts.pory"
	closemessage
# 193 "data/maps/WraithwoodForest/scripts.pory"
	fadescreen FADE_TO_BLACK
# 194 "data/maps/WraithwoodForest/scripts.pory"
	removeobject LOCALID_ARCHIE
# 195 "data/maps/WraithwoodForest/scripts.pory"
	setflag FLAG_HIDE_WRAITHWOOD_ARCHIE
# 196 "data/maps/WraithwoodForest/scripts.pory"
	fadescreen FADE_FROM_BLACK
# 197 "data/maps/WraithwoodForest/scripts.pory"
	releaseall
# 198 "data/maps/WraithwoodForest/scripts.pory"
	end
# 199 "data/maps/WraithwoodForest/scripts.pory"

# 200 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_ArchieEnd::
# 201 "data/maps/WraithwoodForest/scripts.pory"
	end
# 202 "data/maps/WraithwoodForest/scripts.pory"

# 203 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt1Intro:
# 204 "data/maps/WraithwoodForest/scripts.pory"
	.string "Lost, kid? This forest swallows\n"
# 205 "data/maps/WraithwoodForest/scripts.pory"
	.string "people whole. Team Aqua's business\l"
# 206 "data/maps/WraithwoodForest/scripts.pory"
	.string "here is none of yours!$"
# 207 "data/maps/WraithwoodForest/scripts.pory"

# 208 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt1Defeat:
# 209 "data/maps/WraithwoodForest/scripts.pory"
	.string "Tch… you're not just lost.$"
# 210 "data/maps/WraithwoodForest/scripts.pory"

# 211 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt1Post:
# 212 "data/maps/WraithwoodForest/scripts.pory"
	.string "The boss is up ahead. You really\n"
# 213 "data/maps/WraithwoodForest/scripts.pory"
	.string "want to meet him? Your funeral.$"
# 214 "data/maps/WraithwoodForest/scripts.pory"

# 215 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt2Intro:
# 216 "data/maps/WraithwoodForest/scripts.pory"
	.string "Feel that drizzle? Out here the\n"
# 217 "data/maps/WraithwoodForest/scripts.pory"
	.string "rain answers to Team Aqua!$"
# 218 "data/maps/WraithwoodForest/scripts.pory"

# 219 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt2Defeat:
# 220 "data/maps/WraithwoodForest/scripts.pory"
	.string "My storm… washed out?!$"
# 221 "data/maps/WraithwoodForest/scripts.pory"

# 222 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt2Post:
# 223 "data/maps/WraithwoodForest/scripts.pory"
	.string "Rain or no rain, the sea will take\n"
# 224 "data/maps/WraithwoodForest/scripts.pory"
	.string "this land back someday.$"
# 225 "data/maps/WraithwoodForest/scripts.pory"

# 226 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt3Intro:
# 227 "data/maps/WraithwoodForest/scripts.pory"
	.string "Nobody leaves this clearing once\n"
# 228 "data/maps/WraithwoodForest/scripts.pory"
	.string "I've got them. Nobody!$"
# 229 "data/maps/WraithwoodForest/scripts.pory"

# 230 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt3Defeat:
# 231 "data/maps/WraithwoodForest/scripts.pory"
	.string "You slipped right through my trap…$"
# 232 "data/maps/WraithwoodForest/scripts.pory"

# 233 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt3Post:
# 234 "data/maps/WraithwoodForest/scripts.pory"
	.string "Fine, go on. Archie will deal with\n"
# 235 "data/maps/WraithwoodForest/scripts.pory"
	.string "you himself.$"
# 236 "data/maps/WraithwoodForest/scripts.pory"

# 237 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt4Intro:
# 238 "data/maps/WraithwoodForest/scripts.pory"
	.string "Walls up, kid! Let's see you break\n"
# 239 "data/maps/WraithwoodForest/scripts.pory"
	.string "through Team Aqua's defenses!$"
# 240 "data/maps/WraithwoodForest/scripts.pory"

# 241 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt4Defeat:
# 242 "data/maps/WraithwoodForest/scripts.pory"
	.string "Our screens… shattered…$"
# 243 "data/maps/WraithwoodForest/scripts.pory"

# 244 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_Grunt4Post:
# 245 "data/maps/WraithwoodForest/scripts.pory"
	.string "Hollowbrook's just past the boss.\n"
# 246 "data/maps/WraithwoodForest/scripts.pory"
	.string "If you get that far.$"
# 247 "data/maps/WraithwoodForest/scripts.pory"

# 248 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_ArchieIntro:
# 249 "data/maps/WraithwoodForest/scripts.pory"
	.string "ARCHIE: So you're the one who tore\n"
# 250 "data/maps/WraithwoodForest/scripts.pory"
	.string "through my crew back there.\p"
# 251 "data/maps/WraithwoodForest/scripts.pory"
	.string "All life was born in the sea, and\n"
# 252 "data/maps/WraithwoodForest/scripts.pory"
	.string "to the sea it should return.\p"
# 253 "data/maps/WraithwoodForest/scripts.pory"
	.string "Let me show you the strength of\n"
# 254 "data/maps/WraithwoodForest/scripts.pory"
	.string "the ocean!$"
# 255 "data/maps/WraithwoodForest/scripts.pory"

# 256 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_ArchieDefeat:
# 257 "data/maps/WraithwoodForest/scripts.pory"
	.string "ARCHIE: Ha! You've got the spirit\n"
# 258 "data/maps/WraithwoodForest/scripts.pory"
	.string "of a rogue wave!$"
# 259 "data/maps/WraithwoodForest/scripts.pory"

# 260 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_ArchiePost:
# 261 "data/maps/WraithwoodForest/scripts.pory"
	.string "ARCHIE: Remember my name, kid.\n"
# 262 "data/maps/WraithwoodForest/scripts.pory"
	.string "Team Aqua isn't finished with you.$"
