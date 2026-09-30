ViridianForest_MapScripts::
	.byte 0

ViridianForest_EventScript_StarterTrader::
	lock
	faceplayer
	goto_if_set FLAG_JOHTO_STARTER_TRADE_DONE, ViridianForest_EventScript_StarterTradeDone
	msgbox ViridianForest_Text_StarterTradeIntro, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_StarterTradeOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianForest_EventScript_StarterTradeDecline
	multichoice 0, 0, MULTI_JOHTO_STARTER_TRADE, FALSE
	switch VAR_RESULT
	case 0, ViridianForest_EventScript_StarterTradeChikorita
	case 1, ViridianForest_EventScript_StarterTradeCyndaquil
	case 2, ViridianForest_EventScript_StarterTradeTotodile
	goto ViridianForest_EventScript_StarterTradeDecline

ViridianForest_EventScript_StarterTradeChikorita::
	setvar VAR_0x8008, INGAME_TRADE_CHIKORITA
	goto ViridianForest_EventScript_StarterTradeDoTrade

ViridianForest_EventScript_StarterTradeCyndaquil::
	setvar VAR_0x8008, INGAME_TRADE_CYNDAQUIL
	goto ViridianForest_EventScript_StarterTradeDoTrade

ViridianForest_EventScript_StarterTradeTotodile::
	setvar VAR_0x8008, INGAME_TRADE_TOTODILE
	goto ViridianForest_EventScript_StarterTradeDoTrade

@ Unlike the vanilla in-game trades, this one accepts ANY species: the
@ goto_if_ne VAR_RESULT, VAR_0x8009 species check those scripts perform after
@ GetTradeSpecies is deliberately omitted here.
ViridianForest_EventScript_StarterTradeDoTrade::
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, ViridianForest_EventScript_StarterTradeDecline
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	msgbox ViridianForest_Text_StarterTradeThanks, MSGBOX_DEFAULT
	setflag FLAG_JOHTO_STARTER_TRADE_DONE
	release
	end

ViridianForest_EventScript_StarterTradeDecline::
	msgbox ViridianForest_Text_StarterTradeDecline, MSGBOX_DEFAULT
	release
	end

ViridianForest_EventScript_StarterTradeDone::
	msgbox ViridianForest_Text_StarterTradeDone, MSGBOX_DEFAULT
	release
	end

ViridianForest_Text_StarterTradeIntro:
	.string "You came all the way through the\n"
	.string "forest? Then you've earned a look\l"
	.string "at what I'm carrying.\p"
	.string "Three of them. Raised from eggs.\n"
	.string "Not from around here, any of them.$"

ViridianForest_Text_StarterTradeOffer:
	.string "I'm not fussy about what I get back.\n"
	.string "Any POKéMON will do. But I'll only\l"
	.string "part with one of the three.\p"
	.string "Care to trade?$"

ViridianForest_Text_StarterTradeThanks:
	.string "A fair deal. Raise it well.\n"
	.string "It's a long way from home.$"

ViridianForest_Text_StarterTradeDecline:
	.string "Take your time. I'm not going\n"
	.string "anywhere.$"

ViridianForest_Text_StarterTradeDone:
	.string "One trade, that was the deal.\n"
	.string "The other two are staying with me.$"
