LostCave_MapScripts::
	.byte 0

@ Deepest point of Lost Cave: choose ONE of Shiny Stone / Moon Stone.
LostCave_EventScript_StoneKeeper::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_LOST_CAVE_STONE, LostCave_EventScript_StoneKeeperDone
	msgbox LostCave_Text_StoneKeeperIntro, MSGBOX_DEFAULT
	msgbox LostCave_Text_AskLight, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LostCave_EventScript_GiveShinyStone
	msgbox LostCave_Text_AskMoon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LostCave_EventScript_GiveMoonStone
	msgbox LostCave_Text_StoneKeeperWait, MSGBOX_DEFAULT
	release
	end

LostCave_EventScript_GiveShinyStone::
	giveitem ITEM_SHINY_STONE
	goto LostCave_EventScript_StoneKeeperFinish

LostCave_EventScript_GiveMoonStone::
	giveitem ITEM_MOON_STONE
	goto LostCave_EventScript_StoneKeeperFinish

LostCave_EventScript_StoneKeeperFinish::
	goto_if_eq VAR_0x8007, FALSE, LostCave_EventScript_StoneKeeperRelease
	setflag FLAG_RECEIVED_LOST_CAVE_STONE
	msgbox LostCave_Text_StoneKeeperAfter, MSGBOX_DEFAULT
LostCave_EventScript_StoneKeeperRelease::
	release
	end

LostCave_EventScript_StoneKeeperDone::
	msgbox LostCave_Text_StoneKeeperAfter, MSGBOX_DEFAULT
	release
	end

LostCave_Text_StoneKeeperIntro:
	.string "Strange, isn't it? Two stones born\n"
	.string "for change… yet they seem almost\l"
	.string "opposed.\p"
	.string "One catches the light.\p"
	.string "The other seems to swallow it.\p"
	.string "Perhaps that's why they shouldn't\n"
	.string "remain together.\p"
	.string "Tell me, traveler… which one calls\n"
	.string "to you?$"

LostCave_Text_AskLight:
	.string "The stone of light?$"

LostCave_Text_AskMoon:
	.string "Then the stone of the moon?$"

LostCave_Text_StoneKeeperWait:
	.string "Take your time. They aren't going\n"
	.string "anywhere.$"

LostCave_Text_StoneKeeperAfter:
	.string "Interesting choice.\p"
	.string "But remember… choosing when to change\n"
	.string "can matter just as much as choosing\l"
	.string "how.$"
