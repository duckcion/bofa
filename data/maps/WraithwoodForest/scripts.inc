# 6 "data/maps/WraithwoodForest/scripts.pory"

# 7 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_MapScripts::
# 8 "data/maps/WraithwoodForest/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, WraithwoodForest_OnTransition
# 9 "data/maps/WraithwoodForest/scripts.pory"
	.byte 0
# 10 "data/maps/WraithwoodForest/scripts.pory"

# 11 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_OnTransition:
# 12 "data/maps/WraithwoodForest/scripts.pory"
	setflag FLAG_CLEARED_WRAITHWOOD_ROUTE
# 13 "data/maps/WraithwoodForest/scripts.pory"
	end
# 14 "data/maps/WraithwoodForest/scripts.pory"

# 15 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Desmond::
# 16 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_DESMOND, WraithwoodForest_Text_DesmondIntro, WraithwoodForest_Text_DesmondDefeat, WraithwoodForest_EventScript_TryRegisterDesmondAfterBattle
# 17 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 18 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, WraithwoodForest_EventScript_RematchDesmond
# 19 "data/maps/WraithwoodForest/scripts.pory"
	setvar VAR_0x8004, TRAINER_DESMOND
# 20 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 21 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, WraithwoodForest_EventScript_TryRegisterDesmond
# 22 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondPostBattle, MSGBOX_DEFAULT
# 23 "data/maps/WraithwoodForest/scripts.pory"
	release
# 24 "data/maps/WraithwoodForest/scripts.pory"
	end
# 25 "data/maps/WraithwoodForest/scripts.pory"

# 26 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_RematchDesmond::
# 27 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_rematch TRAINER_DESMOND, WraithwoodForest_Text_DesmondRematchIntro, WraithwoodForest_Text_DesmondDefeat
# 28 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondPostBattle, MSGBOX_DEFAULT
# 29 "data/maps/WraithwoodForest/scripts.pory"
	release
# 30 "data/maps/WraithwoodForest/scripts.pory"
	end
# 31 "data/maps/WraithwoodForest/scripts.pory"

# 32 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterDesmond::
# 33 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondRegister, MSGBOX_DEFAULT
# 34 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_DESMOND
# 35 "data/maps/WraithwoodForest/scripts.pory"
	release
# 36 "data/maps/WraithwoodForest/scripts.pory"
	end
# 37 "data/maps/WraithwoodForest/scripts.pory"

# 38 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterDesmondAfterBattle::
# 39 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_DesmondRegister, MSGBOX_DEFAULT
# 40 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_DESMOND
# 41 "data/maps/WraithwoodForest/scripts.pory"
	release
# 42 "data/maps/WraithwoodForest/scripts.pory"
	end
# 43 "data/maps/WraithwoodForest/scripts.pory"

# 44 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondIntro:
# 45 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: The bugs in this fog love\n"
# 46 "data/maps/WraithwoodForest/scripts.pory"
	.string "company! Come keep us busy!$"
# 47 "data/maps/WraithwoodForest/scripts.pory"

# 48 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondDefeat:
# 49 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: Aw, bug out!$"
# 50 "data/maps/WraithwoodForest/scripts.pory"

# 51 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondPostBattle:
# 52 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: This fog's thick, but my bugs\n"
# 53 "data/maps/WraithwoodForest/scripts.pory"
	.string "know the way around.$"
# 54 "data/maps/WraithwoodForest/scripts.pory"

# 55 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondRegister:
# 56 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: Let's swap numbers!$"
# 57 "data/maps/WraithwoodForest/scripts.pory"

# 58 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_DesmondRematchIntro:
# 59 "data/maps/WraithwoodForest/scripts.pory"
	.string "DESMOND: Back for more bug bites?$"
# 60 "data/maps/WraithwoodForest/scripts.pory"

# 61 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Ottoline::
# 62 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_OTTOLINE, WraithwoodForest_Text_OttolineIntro, WraithwoodForest_Text_OttolineDefeat, WraithwoodForest_EventScript_TryRegisterOttolineAfterBattle
# 63 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 64 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, WraithwoodForest_EventScript_RematchOttoline
# 65 "data/maps/WraithwoodForest/scripts.pory"
	setvar VAR_0x8004, TRAINER_OTTOLINE
# 66 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 67 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, WraithwoodForest_EventScript_TryRegisterOttoline
# 68 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolinePostBattle, MSGBOX_DEFAULT
# 69 "data/maps/WraithwoodForest/scripts.pory"
	release
# 70 "data/maps/WraithwoodForest/scripts.pory"
	end
# 71 "data/maps/WraithwoodForest/scripts.pory"

# 72 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_RematchOttoline::
# 73 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_rematch TRAINER_OTTOLINE, WraithwoodForest_Text_OttolineRematchIntro, WraithwoodForest_Text_OttolineDefeat
# 74 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolinePostBattle, MSGBOX_DEFAULT
# 75 "data/maps/WraithwoodForest/scripts.pory"
	release
# 76 "data/maps/WraithwoodForest/scripts.pory"
	end
# 77 "data/maps/WraithwoodForest/scripts.pory"

# 78 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterOttoline::
# 79 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolineRegister, MSGBOX_DEFAULT
# 80 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_OTTOLINE
# 81 "data/maps/WraithwoodForest/scripts.pory"
	release
# 82 "data/maps/WraithwoodForest/scripts.pory"
	end
# 83 "data/maps/WraithwoodForest/scripts.pory"

# 84 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterOttolineAfterBattle::
# 85 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_OttolineRegister, MSGBOX_DEFAULT
# 86 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_OTTOLINE
# 87 "data/maps/WraithwoodForest/scripts.pory"
	release
# 88 "data/maps/WraithwoodForest/scripts.pory"
	end
# 89 "data/maps/WraithwoodForest/scripts.pory"

# 90 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineIntro:
# 91 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: Something's watching us from\n"
# 92 "data/maps/WraithwoodForest/scripts.pory"
	.string "the fog… oh, it's just me!$"
# 93 "data/maps/WraithwoodForest/scripts.pory"

# 94 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineDefeat:
# 95 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: My spirits… they've fled!$"
# 96 "data/maps/WraithwoodForest/scripts.pory"

# 97 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolinePostBattle:
# 98 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: This place is thick with\n"
# 99 "data/maps/WraithwoodForest/scripts.pory"
	.string "restless spirits.$"
# 100 "data/maps/WraithwoodForest/scripts.pory"

# 101 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineRegister:
# 102 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: Let's stay in touch!$"
# 103 "data/maps/WraithwoodForest/scripts.pory"

# 104 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_OttolineRematchIntro:
# 105 "data/maps/WraithwoodForest/scripts.pory"
	.string "OTTOLINE: The spirits are restless again!$"
# 106 "data/maps/WraithwoodForest/scripts.pory"

# 107 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_Rufus::
# 108 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_single TRAINER_RUFUS, WraithwoodForest_Text_RufusIntro, WraithwoodForest_Text_RufusDefeat, WraithwoodForest_EventScript_TryRegisterRufusAfterBattle
# 109 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, ShouldTryRematchBattle
# 110 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, TRUE, WraithwoodForest_EventScript_RematchRufus
# 111 "data/maps/WraithwoodForest/scripts.pory"
	setvar VAR_0x8004, TRAINER_RUFUS
# 112 "data/maps/WraithwoodForest/scripts.pory"
	specialvar VAR_RESULT, IsTrainerRegistered
# 113 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, FALSE, WraithwoodForest_EventScript_TryRegisterRufus
# 114 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusPostBattle, MSGBOX_DEFAULT
# 115 "data/maps/WraithwoodForest/scripts.pory"
	release
# 116 "data/maps/WraithwoodForest/scripts.pory"
	end
# 117 "data/maps/WraithwoodForest/scripts.pory"

# 118 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_RematchRufus::
# 119 "data/maps/WraithwoodForest/scripts.pory"
	trainerbattle_rematch TRAINER_RUFUS, WraithwoodForest_Text_RufusRematchIntro, WraithwoodForest_Text_RufusDefeat
# 120 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusPostBattle, MSGBOX_DEFAULT
# 121 "data/maps/WraithwoodForest/scripts.pory"
	release
# 122 "data/maps/WraithwoodForest/scripts.pory"
	end
# 123 "data/maps/WraithwoodForest/scripts.pory"

# 124 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterRufus::
# 125 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusRegister, MSGBOX_DEFAULT
# 126 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_RUFUS
# 127 "data/maps/WraithwoodForest/scripts.pory"
	release
# 128 "data/maps/WraithwoodForest/scripts.pory"
	end
# 129 "data/maps/WraithwoodForest/scripts.pory"

# 130 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_TryRegisterRufusAfterBattle::
# 131 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_RufusRegister, MSGBOX_DEFAULT
# 132 "data/maps/WraithwoodForest/scripts.pory"
	register_matchcall TRAINER_RUFUS
# 133 "data/maps/WraithwoodForest/scripts.pory"
	release
# 134 "data/maps/WraithwoodForest/scripts.pory"
	end
# 135 "data/maps/WraithwoodForest/scripts.pory"

# 136 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusIntro:
# 137 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Heading for GRANITE CAVE?\n"
# 138 "data/maps/WraithwoodForest/scripts.pory"
	.string "Not before you beat me!$"
# 139 "data/maps/WraithwoodForest/scripts.pory"

# 140 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusDefeat:
# 141 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Solid hit, that.$"
# 142 "data/maps/WraithwoodForest/scripts.pory"

# 143 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusPostBattle:
# 144 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: The cave entrance is just\n"
# 145 "data/maps/WraithwoodForest/scripts.pory"
	.string "south of here.$"
# 146 "data/maps/WraithwoodForest/scripts.pory"

# 147 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusRegister:
# 148 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Let's trade numbers!$"
# 149 "data/maps/WraithwoodForest/scripts.pory"

# 150 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_RufusRematchIntro:
# 151 "data/maps/WraithwoodForest/scripts.pory"
	.string "RUFUS: Fancy another round?$"
# 152 "data/maps/WraithwoodForest/scripts.pory"

# 153 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_SkorupiGift::
# 154 "data/maps/WraithwoodForest/scripts.pory"
	lock
# 155 "data/maps/WraithwoodForest/scripts.pory"
	faceplayer
# 156 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_set FLAG_RECEIVED_SKORUPI_GIFT, WraithwoodForest_EventScript_SkorupiGiftAlreadyGiven
# 157 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_SkorupiGiftIntro, MSGBOX_DEFAULT
# 158 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_SkorupiGiftOffer, MSGBOX_YESNO
# 159 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_eq VAR_RESULT, NO, WraithwoodForest_EventScript_SkorupiGiftDecline
# 160 "data/maps/WraithwoodForest/scripts.pory"
	getpartysize
# 161 "data/maps/WraithwoodForest/scripts.pory"
	copyvar VAR_0x8008, VAR_RESULT
# 162 "data/maps/WraithwoodForest/scripts.pory"
	givemon SPECIES_SKORUPI, 22, nature=NATURE_JOLLY, abilityNum=0, hpIv=31, atkIv=31, defIv=31, speedIv=31, spAtkIv=31, spDefIv=31, move1=MOVE_POISON_JAB, move2=MOVE_SLASH, move3=MOVE_BITE, move4=MOVE_PIN_MISSILE
# 163 "data/maps/WraithwoodForest/scripts.pory"
	goto_if_ne VAR_RESULT, MON_GIVEN_TO_PARTY, WraithwoodForest_EventScript_SkorupiGiftSkipLocation
# 164 "data/maps/WraithwoodForest/scripts.pory"
	setmonmetlocation VAR_0x8008, MAPSEC_WRAITHWOOD_FOREST
# 165 "data/maps/WraithwoodForest/scripts.pory"

# 166 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_SkorupiGiftSkipLocation::
# 167 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_SkorupiGiftGiven, MSGBOX_DEFAULT
# 168 "data/maps/WraithwoodForest/scripts.pory"
	setflag FLAG_RECEIVED_SKORUPI_GIFT
# 169 "data/maps/WraithwoodForest/scripts.pory"
	release
# 170 "data/maps/WraithwoodForest/scripts.pory"
	end
# 171 "data/maps/WraithwoodForest/scripts.pory"

# 172 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_SkorupiGiftDecline::
# 173 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_SkorupiGiftDecline, MSGBOX_DEFAULT
# 174 "data/maps/WraithwoodForest/scripts.pory"
	release
# 175 "data/maps/WraithwoodForest/scripts.pory"
	end
# 176 "data/maps/WraithwoodForest/scripts.pory"

# 177 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_EventScript_SkorupiGiftAlreadyGiven::
# 178 "data/maps/WraithwoodForest/scripts.pory"
	msgbox WraithwoodForest_Text_SkorupiGiftAlreadyGiven, MSGBOX_DEFAULT
# 179 "data/maps/WraithwoodForest/scripts.pory"
	release
# 180 "data/maps/WraithwoodForest/scripts.pory"
	end
# 181 "data/maps/WraithwoodForest/scripts.pory"

# 182 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_SkorupiGiftIntro:
# 183 "data/maps/WraithwoodForest/scripts.pory"
	.string "Careful where you step. There's a\n"
# 184 "data/maps/WraithwoodForest/scripts.pory"
	.string "nest of them under the roots here.\p"
# 185 "data/maps/WraithwoodForest/scripts.pory"
	.string "I've been raising one of the young\n"
# 186 "data/maps/WraithwoodForest/scripts.pory"
	.string "ones. Armored shell, thick as bark.\l"
# 187 "data/maps/WraithwoodForest/scripts.pory"
	.string "Nothing gets a clean hit on it.$"
# 188 "data/maps/WraithwoodForest/scripts.pory"

# 189 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_SkorupiGiftOffer:
# 190 "data/maps/WraithwoodForest/scripts.pory"
	.string "It's outgrowing my care. Would you\n"
# 191 "data/maps/WraithwoodForest/scripts.pory"
	.string "take it with you?$"
# 192 "data/maps/WraithwoodForest/scripts.pory"

# 193 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_SkorupiGiftGiven:
# 194 "data/maps/WraithwoodForest/scripts.pory"
	.string "Keep it fed and it'll grow into\n"
# 195 "data/maps/WraithwoodForest/scripts.pory"
	.string "something fierce.$"
# 196 "data/maps/WraithwoodForest/scripts.pory"

# 197 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_SkorupiGiftDecline:
# 198 "data/maps/WraithwoodForest/scripts.pory"
	.string "Suit yourself. It'll be here if you\n"
# 199 "data/maps/WraithwoodForest/scripts.pory"
	.string "change your mind.$"
# 200 "data/maps/WraithwoodForest/scripts.pory"

# 201 "data/maps/WraithwoodForest/scripts.pory"
WraithwoodForest_Text_SkorupiGiftAlreadyGiven:
# 202 "data/maps/WraithwoodForest/scripts.pory"
	.string "How's that SKORUPI treating you?\n"
# 203 "data/maps/WraithwoodForest/scripts.pory"
	.string "Mind its tail, now.$"
