OldaleTown_House1_MapScripts::
	.byte 0

OldaleTown_House1_EventScript_Woman::
	msgbox OldaleTown_House1_Text_LeftPokemonGoesOutFirst, MSGBOX_NPC
	end

@ BOFA: one-time gift of 8 Persim Berries.
OldaleTown_House1_EventScript_SupplyLady::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_OLDALE_STARTER_SUPPLIES, OldaleTown_House1_EventScript_SupplyLadyAlreadyGiven
	msgbox OldaleTown_House1_Text_SupplyLadyIntro, MSGBOX_DEFAULT
	giveitem ITEM_PERSIM_BERRY, 8
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	setflag FLAG_RECEIVED_OLDALE_STARTER_SUPPLIES
	msgbox OldaleTown_House1_Text_SupplyLadyOutro, MSGBOX_DEFAULT
	release
	end

OldaleTown_House1_EventScript_SupplyLadyAlreadyGiven::
	msgbox OldaleTown_House1_Text_SupplyLadyAlreadyGiven, MSGBOX_NPC
	end

OldaleTown_House1_EventScript_SupplyLadyBagFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	release
	end

OldaleTown_House1_Text_SupplyLadyIntro:
	.string "Oh, a new trainer! Here, take these\n"
	.string "PERSIM BERRIES. They snap a POKéMON\l"
	.string "out of confusion.$"

OldaleTown_House1_Text_SupplyLadyOutro:
	.string "Take good care of your POKéMON out\n"
	.string "there, and don't be afraid to use what\l"
	.string "I gave you!$"

OldaleTown_House1_Text_SupplyLadyAlreadyGiven:
	.string "Make good use of those berries!$"

OldaleTown_House1_Text_LeftPokemonGoesOutFirst:
	.string "When a POKéMON battle starts, the one\n"
	.string "at the left of the list goes out first.\p"
	.string "So, when you get more POKéMON in your\n"
	.string "party, try switching around the order\l"
	.string "of your POKéMON.\p"
	.string "It could give you an advantage.$"

