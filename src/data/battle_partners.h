//
// DO NOT MODIFY THIS FILE! It is auto-generated from src/data/battle_partners.party
//
// If you want to modify this file set COMPETITIVE_PARTY_SYNTAX to FALSE
// in include/config/general.h and remove this notice.
// Use sed -i '/^#line/d' 'src/data/battle_partners.h' to remove #line markers.
//

#line 1 "src/data/battle_partners.party"

#line 1
    [PARTNER_NONE] =
    {
#line 3
        .trainerClass = TRAINER_CLASS_PKMN_TRAINER_1,
#line 4
        .trainerPic = TRAINER_BACK_PIC_BRENDAN,
        .encounterMusic_gender = 
#line 6
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 0,
        .party = (const struct TrainerMon[])
        {
        },
    },
#line 8
    [PARTNER_STEVEN] =
    {
#line 9
        .trainerName = _("STEVEN"),
#line 10
        .trainerClass = TRAINER_CLASS_RIVAL,
#line 11
        .trainerPic = TRAINER_BACK_PIC_STEVEN,
        .encounterMusic_gender = 
#line 13
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 15
            .species = SPECIES_METANG,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 19
            .ev = TRAINER_PARTY_EVS(0, 252, 252, 0, 6, 0),
#line 18
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 17
            .lvl = 42,
#line 16
            .nature = NATURE_BRAVE,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 20
                MOVE_LIGHT_SCREEN,
                MOVE_PSYCHIC,
                MOVE_REFLECT,
                MOVE_METAL_CLAW,
            },
            },
            {
#line 25
            .species = SPECIES_SKARMORY,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 29
            .ev = TRAINER_PARTY_EVS(252, 0, 0, 0, 6, 252),
#line 28
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 27
            .lvl = 43,
#line 26
            .nature = NATURE_IMPISH,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 30
                MOVE_TOXIC,
                MOVE_AERIAL_ACE,
                MOVE_PROTECT,
                MOVE_STEEL_WING,
            },
            },
            {
#line 35
            .species = SPECIES_AGGRON,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 39
            .ev = TRAINER_PARTY_EVS(0, 252, 0, 0, 252, 6),
#line 38
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 37
            .lvl = 44,
#line 36
            .nature = NATURE_ADAMANT,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 40
                MOVE_THUNDER,
                MOVE_PROTECT,
                MOVE_SOLAR_BEAM,
                MOVE_DRAGON_CLAW,
            },
            },
        },
    },
#line 45
    [PARTNER_RIVAL_BRENDAN] =
    {
#line 46
        .trainerName = _("BRENDAN"),
#line 47
        .trainerClass = TRAINER_CLASS_RIVAL,
#line 48
        .trainerPic = TRAINER_BACK_PIC_BRENDAN,
        .encounterMusic_gender = 
#line 50
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 52
            .species = SPECIES_GRENINJA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 52
            .heldItem = ITEM_LIFE_ORB,
#line 56
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 53
            .ability = ABILITY_PROTEAN,
#line 55
            .lvl = 36,
#line 54
            .nature = NATURE_NAIVE,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 57
                MOVE_WATERFALL,
                MOVE_DARK_PULSE,
                MOVE_ICE_BEAM,
                MOVE_EXTRASENSORY,
            },
            },
            {
#line 62
            .species = SPECIES_CHESNAUGHT,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 62
            .heldItem = ITEM_LEFTOVERS,
#line 66
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 63
            .ability = ABILITY_BULLETPROOF,
#line 65
            .lvl = 36,
#line 64
            .nature = NATURE_ADAMANT,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 67
                MOVE_SEED_BOMB,
                MOVE_DRAIN_PUNCH,
                MOVE_ROCK_SLIDE,
                MOVE_SPIKY_SHIELD,
            },
            },
            {
#line 72
            .species = SPECIES_DELPHOX,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 72
            .heldItem = ITEM_EXPERT_BELT,
#line 76
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 73
            .ability = ABILITY_MAGICIAN,
#line 75
            .lvl = 36,
#line 74
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 77
                MOVE_FLAMETHROWER,
                MOVE_PSYCHIC,
                MOVE_SHADOW_BALL,
                MOVE_DAZZLING_GLEAM,
            },
            },
        },
    },
#line 82
    [PARTNER_RIVAL_MAY] =
    {
#line 83
        .trainerName = _("MAY"),
#line 84
        .trainerClass = TRAINER_CLASS_RIVAL,
#line 85
        .trainerPic = TRAINER_BACK_PIC_MAY,
        .encounterMusic_gender = 
#line 86
F_TRAINER_FEMALE | 
#line 87
            TRAINER_ENCOUNTER_MUSIC_FEMALE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 89
            .species = SPECIES_GRENINJA,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 89
            .heldItem = ITEM_LIFE_ORB,
#line 93
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 90
            .ability = ABILITY_PROTEAN,
#line 92
            .lvl = 36,
#line 91
            .nature = NATURE_NAIVE,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 94
                MOVE_WATERFALL,
                MOVE_DARK_PULSE,
                MOVE_ICE_BEAM,
                MOVE_EXTRASENSORY,
            },
            },
            {
#line 99
            .species = SPECIES_CHESNAUGHT,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 99
            .heldItem = ITEM_LEFTOVERS,
#line 103
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 100
            .ability = ABILITY_BULLETPROOF,
#line 102
            .lvl = 36,
#line 101
            .nature = NATURE_ADAMANT,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 104
                MOVE_SEED_BOMB,
                MOVE_DRAIN_PUNCH,
                MOVE_ROCK_SLIDE,
                MOVE_SPIKY_SHIELD,
            },
            },
            {
#line 109
            .species = SPECIES_DELPHOX,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 109
            .heldItem = ITEM_EXPERT_BELT,
#line 113
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 110
            .ability = ABILITY_MAGICIAN,
#line 112
            .lvl = 36,
#line 111
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 114
                MOVE_FLAMETHROWER,
                MOVE_PSYCHIC,
                MOVE_SHADOW_BALL,
                MOVE_DAZZLING_GLEAM,
            },
            },
        },
    },
