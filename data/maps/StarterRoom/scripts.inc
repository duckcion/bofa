StarterRoom_MapScripts::
	.byte 0

@ Split 1: free Johto starter. The player picks ONE of the three balls on the table.
@ All three balls share FLAG_RECEIVED_STARTER_ROOM_JOHTO_MON, so the other two
@ disappear once a choice is made.
StarterRoom_EventScript_Chikorita::
	lock
	goto_if_set FLAG_RECEIVED_STARTER_ROOM_JOHTO_MON, StarterRoom_EventScript_AlreadyChose
	goto_if_unset FLAG_RECEIVED_POKEDEX_FROM_BIRCH, StarterRoom_EventScript_NotYet
	showmonpic SPECIES_CHIKORITA, 10, 3
	msgbox StarterRoom_Text_TakeChikorita, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, StarterRoom_EventScript_Decline
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHIKORITA
	getpartysize
	copyvar VAR_0x8008, VAR_RESULT
	givemon SPECIES_CHIKORITA, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, StarterRoom_EventScript_ToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, StarterRoom_EventScript_ToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

StarterRoom_EventScript_Cyndaquil::
	lock
	goto_if_set FLAG_RECEIVED_STARTER_ROOM_JOHTO_MON, StarterRoom_EventScript_AlreadyChose
	goto_if_unset FLAG_RECEIVED_POKEDEX_FROM_BIRCH, StarterRoom_EventScript_NotYet
	showmonpic SPECIES_CYNDAQUIL, 10, 3
	msgbox StarterRoom_Text_TakeCyndaquil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, StarterRoom_EventScript_Decline
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CYNDAQUIL
	getpartysize
	copyvar VAR_0x8008, VAR_RESULT
	givemon SPECIES_CYNDAQUIL, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, StarterRoom_EventScript_ToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, StarterRoom_EventScript_ToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

StarterRoom_EventScript_Totodile::
	lock
	goto_if_set FLAG_RECEIVED_STARTER_ROOM_JOHTO_MON, StarterRoom_EventScript_AlreadyChose
	goto_if_unset FLAG_RECEIVED_POKEDEX_FROM_BIRCH, StarterRoom_EventScript_NotYet
	showmonpic SPECIES_TOTODILE, 10, 3
	msgbox StarterRoom_Text_TakeTotodile, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, StarterRoom_EventScript_Decline
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TOTODILE
	getpartysize
	copyvar VAR_0x8008, VAR_RESULT
	givemon SPECIES_TOTODILE, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, StarterRoom_EventScript_ToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, StarterRoom_EventScript_ToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon
	end

StarterRoom_EventScript_ToParty::
	setmonmetlocation VAR_0x8008, MAPSEC_STARTER_ROOM
	call StarterRoom_EventScript_Received
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, StarterRoom_EventScript_Done
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto StarterRoom_EventScript_Done
	end

StarterRoom_EventScript_ToPC::
	call StarterRoom_EventScript_Received
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, StarterRoom_EventScript_SentToPC
	call Common_EventScript_NameReceivedBoxMon
	goto StarterRoom_EventScript_SentToPC
	end

StarterRoom_EventScript_SentToPC::
	call Common_EventScript_TransferredToPC
	goto StarterRoom_EventScript_Done
	end

StarterRoom_EventScript_Received::
	hidemonpic
	setflag FLAG_RECEIVED_STARTER_ROOM_JOHTO_MON
	removeobject 1
	removeobject 2
	removeobject 3
	playfanfare MUS_OBTAIN_ITEM
	message StarterRoom_Text_ReceivedStarter
	waitmessage
	waitfanfare
	return

StarterRoom_EventScript_Done::
	release
	end

StarterRoom_EventScript_Decline::
	hidemonpic
	msgbox StarterRoom_Text_TakeYourTime, MSGBOX_DEFAULT
	release
	end

StarterRoom_EventScript_AlreadyChose::
	msgbox StarterRoom_Text_AlreadyChose, MSGBOX_DEFAULT
	release
	end

StarterRoom_EventScript_NotYet::
	msgbox StarterRoom_Text_NotYet, MSGBOX_DEFAULT
	release
	end

StarterRoom_Text_TakeChikorita:
	.string "It's CHIKORITA, a POKéMON from\n"
	.string "the JOHTO region.\p"
	.string "Will you take CHIKORITA?$"

StarterRoom_Text_TakeCyndaquil:
	.string "It's CYNDAQUIL, a POKéMON from\n"
	.string "the JOHTO region.\p"
	.string "Will you take CYNDAQUIL?$"

StarterRoom_Text_TakeTotodile:
	.string "It's TOTODILE, a POKéMON from\n"
	.string "the JOHTO region.\p"
	.string "Will you take TOTODILE?$"

StarterRoom_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_1}!$"

StarterRoom_Text_TakeYourTime:
	.string "Take your time and choose\n"
	.string "the one you like.$"

StarterRoom_Text_NotYet:
	.string "These POKé BALLS belong to\n"
	.string "PROF. BIRCH. Better see him first.$"

StarterRoom_Text_AlreadyChose:
	.string "You already chose your JOHTO\n"
	.string "POKéMON.$"
