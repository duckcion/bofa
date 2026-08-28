OldaleTown_House1_MapScripts::
	.byte 0

OldaleTown_House1_EventScript_Woman::
	msgbox OldaleTown_House1_Text_LeftPokemonGoesOutFirst, MSGBOX_NPC
	end

@ Gives the player a one-time starting stockpile of status-healing berries and
@ general consumables so they aren't stuck relying on the Mart alone early on.
OldaleTown_House1_EventScript_SupplyLady::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_OLDALE_STARTER_SUPPLIES, OldaleTown_House1_EventScript_SupplyLadyAlreadyGiven
	msgbox OldaleTown_House1_Text_SupplyLadyIntro, MSGBOX_DEFAULT
	giveitem ITEM_CHERI_BERRY, 12
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_CHESTO_BERRY, 12
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_PECHA_BERRY, 12
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_PERSIM_BERRY, 12
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_ORAN_BERRY, 100
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_MAX_REPEL, 900
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_RARE_CANDY, 900
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_FULL_RESTORE, 900
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_MAX_POTION, 900
	goto_if_eq VAR_RESULT, FALSE, OldaleTown_House1_EventScript_SupplyLadyBagFull
	giveitem ITEM_MAX_ELIXIR, 900
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
	.string "Oh, a new trainer! Here, take these --\n"
	.string "every trainer should set out with a\l"
	.string "full stock of berries and supplies.$"

OldaleTown_House1_Text_SupplyLadyOutro:
	.string "Take good care of your POKéMON out\n"
	.string "there, and don't be afraid to use what\l"
	.string "I gave you!$"

OldaleTown_House1_Text_SupplyLadyAlreadyGiven:
	.string "Make good use of those supplies!$"

OldaleTown_House1_Text_LeftPokemonGoesOutFirst:
	.string "When a POKéMON battle starts, the one\n"
	.string "at the left of the list goes out first.\p"
	.string "So, when you get more POKéMON in your\n"
	.string "party, try switching around the order\l"
	.string "of your POKéMON.\p"
	.string "It could give you an advantage.$"

