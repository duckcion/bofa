// BOFA: Generation 4 (Platinum / Platinum Kaizo) trainer AI.
//
// Ports the Basic, Evaluate Attack, Expert and Risky flags from the Gen 4 trainer AI
// reference (Reference/Platnimu AI/gen4_trainer_ai.md), sourced from pokeplatinum's AI scripts,
// plus Platinum Kaizo's AI changes (1/4-recoil routine, Triple Axel / Fury Cutter are Risky).
// Like the original, it reads the real battle state (moves, abilities, items) of both sides.
//
// Moves are scored by their *effect*, so moves whose effect BOFA changed to match Platinum
// Kaizo (e.g. Hyper Beam is now plain 1/2 recoil, Dig no longer goes underground) are scored
// by their new effect, exactly like PK's AI does.
//
// Every move starts at the default score; the move with the highest score is chosen and ties
// are broken at random. Percent chances are rolled out of 256 as in the original scripts.

#include "global.h"
#include "battle.h"
#include "battle_ai_gen4.h"
#include "battle_ai_main.h"
#include "battle_ai_util.h"
#include "battle_util.h"
#include "battle_ai_switch_items.h"
#include "battle_controllers.h"
#include "item.h"
#include "random.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"

// ------------------------------------------------------------------------------------ helpers

static bool32 IsBattlerAlly(u32 battlerAtk, u32 battlerDef)
{
    return GetBattlerSide(battlerAtk) == GetBattlerSide(battlerDef);
}

static bool32 Roll(u32 outOf256)
{
    return (Random() % 256) < outOf256;
}

static u32 HpPct(u32 battler)
{
    if (gBattleMons[battler].maxHP == 0)
        return 0;
    return gBattleMons[battler].hp * 100 / gBattleMons[battler].maxHP;
}

static s32 Stage(u32 battler, u32 stat)
{
    return gBattleMons[battler].statStages[stat] - DEFAULT_STAT_STAGE;
}

static bool32 AttackerFaster(u32 battlerAtk, u32 battlerDef)
{
    return AI_DATA->speedStats[battlerAtk] > AI_DATA->speedStats[battlerDef];
}

static bool32 AttackerSlower(u32 battlerAtk, u32 battlerDef)
{
    return AI_DATA->speedStats[battlerAtk] < AI_DATA->speedStats[battlerDef];
}

static bool32 Knows(u32 battler, u32 move)
{
    u32 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (gBattleMons[battler].moves[i] == move)
            return TRUE;
    return FALSE;
}

static bool32 KnowsEffect(u32 battler, u32 effect)
{
    u32 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (gBattleMons[battler].moves[i] != MOVE_NONE && gMovesInfo[gBattleMons[battler].moves[i]].effect == effect)
            return TRUE;
    return FALSE;
}

static bool32 KnowsDamagingMove(u32 battler)
{
    u32 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (gBattleMons[battler].moves[i] != MOVE_NONE && !IS_MOVE_STATUS(gBattleMons[battler].moves[i]))
            return TRUE;
    return FALSE;
}

static bool32 KnowsProtect(u32 battler)
{
    return KnowsEffect(battler, EFFECT_PROTECT);
}

static bool32 KnowsFlatRecoveryOrDefenseCurl(u32 battler)
{
    return KnowsEffect(battler, EFFECT_RESTORE_HP) || KnowsEffect(battler, EFFECT_SOFTBOILED)
        || KnowsEffect(battler, EFFECT_ROOST) || KnowsEffect(battler, EFFECT_DEFENSE_CURL);
}

static u32 CurrentEffectiveness(u32 battlerAtk, u32 battlerDef)
{
    return AI_DATA->effectiveness[battlerAtk][battlerDef][AI_THINKING_STRUCT->movesetIndex];
}

static bool32 ResistsOrImmune(u32 battlerAtk, u32 battlerDef)
{
    return CurrentEffectiveness(battlerAtk, battlerDef) <= AI_EFFECTIVENESS_x0_5;
}

static bool32 HasMoldBreaker(u32 battler)
{
    u32 ability = GetBattlerAbility(battler);
    return ability == ABILITY_MOLD_BREAKER || ability == ABILITY_TERAVOLT || ability == ABILITY_TURBOBLAZE;
}

static bool32 DefAbility(u32 battlerAtk, u32 battlerDef, u32 ability)
{
    return GetBattlerAbility(battlerDef) == ability && !HasMoldBreaker(battlerAtk);
}

static bool32 IsLastMon(u32 battler)
{
    return CountUsablePartyMons(battler) == 0;
}

static bool32 HasStatus(u32 battler)
{
    return (gBattleMons[battler].status1 & STATUS1_ANY) != 0;
}

static bool32 HasSafeguard(u32 battler)
{
    return (gSideStatuses[GetBattlerSide(battler)] & SIDE_STATUS_SAFEGUARD) != 0;
}

static bool32 IsAsleep(u32 battler)
{
    return (gBattleMons[battler].status1 & STATUS1_SLEEP) != 0;
}

static bool32 IsConfusedOrInfatuated(u32 battler)
{
    return (gBattleMons[battler].status2 & (STATUS2_CONFUSION | STATUS2_INFATUATION)) != 0;
}

static bool32 IsAsleepConfusedOrInfatuated(u32 battler)
{
    return IsAsleep(battler) || IsConfusedOrInfatuated(battler);
}

static bool32 IsBadlyPoisoned(u32 battler)
{
    return (gBattleMons[battler].status1 & STATUS1_TOXIC_POISON) != 0;
}

static bool32 IsSeeded(u32 battler)
{
    return (gStatuses3[battler] & STATUS3_LEECHSEED) != 0;
}

static bool32 IsCursed(u32 battler)
{
    return (gBattleMons[battler].status2 & STATUS2_CURSED) != 0;
}

static bool32 IsRootedOrAquaRing(u32 battler)
{
    return (gStatuses3[battler] & (STATUS3_ROOTED | STATUS3_AQUA_RING)) != 0;
}

// Toxic / Curse / Perish Song / Attract (+ Leech Seed / Yawn when asked)
static bool32 HasDrainingCondition(u32 battler, bool32 includeSeedAndYawn)
{
    if (IsBadlyPoisoned(battler) || IsCursed(battler) || (gStatuses3[battler] & STATUS3_PERISH_SONG)
     || (gBattleMons[battler].status2 & STATUS2_INFATUATION))
        return TRUE;
    if (includeSeedAndYawn && (IsSeeded(battler) || (gStatuses3[battler] & STATUS3_YAWN)))
        return TRUE;
    return FALSE;
}

static bool32 IsTaunted(u32 battler)
{
    return gDisableStructs[battler].tauntTimer != 0;
}

static bool32 WeatherIs(u32 weatherFlags)
{
    return (AI_GetWeather(AI_DATA) & weatherFlags) != 0;
}

static bool32 TrickRoomActive(void)
{
    return (gFieldStatuses & STATUS_FIELD_TRICK_ROOM) != 0;
}

static u32 LastMoveOf(u32 battler)
{
    u32 move = gLastMoves[battler];
    if (move == MOVE_UNAVAILABLE)
        return MOVE_NONE;
    return move;
}

static bool32 LastMoveWasStatus(u32 battler)
{
    u32 move = LastMoveOf(battler);
    return move != MOVE_NONE && IS_MOVE_STATUS(move);
}

static bool32 LastMoveWasDamaging(u32 battler)
{
    u32 move = LastMoveOf(battler);
    return move != MOVE_NONE && !IS_MOVE_STATUS(move);
}

static bool32 AnyStageAtLeast(u32 battler, s32 stage)
{
    u32 i;
    for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
        if (Stage(battler, i) >= stage)
            return TRUE;
    return FALSE;
}

static bool32 AnyStageAtMost(u32 battler, s32 stage)
{
    u32 i;
    for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
        if (Stage(battler, i) <= stage)
            return TRUE;
    return FALSE;
}

static bool32 HasSuperEffectiveMove(u32 battlerAtk, u32 battlerDef)
{
    u32 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u32 move = gBattleMons[battlerAtk].moves[i];
        if (move != MOVE_NONE && !IS_MOVE_STATUS(move)
         && AI_DATA->effectiveness[battlerAtk][battlerDef][i] >= AI_EFFECTIVENESS_x2)
            return TRUE;
    }
    return FALSE;
}

// stat the effect raises (status moves only), or NUM_BATTLE_STATS
static u32 BoostedStat(u32 effect)
{
    switch (effect)
    {
    case EFFECT_ATTACK_UP: case EFFECT_ATTACK_UP_2:
        return STAT_ATK;
    case EFFECT_DEFENSE_UP: case EFFECT_DEFENSE_UP_2: case EFFECT_DEFENSE_UP_3: case EFFECT_DEFENSE_CURL:
        return STAT_DEF;
    case EFFECT_SPEED_UP: case EFFECT_SPEED_UP_2:
        return STAT_SPEED;
    case EFFECT_SPECIAL_ATTACK_UP: case EFFECT_SPECIAL_ATTACK_UP_2: case EFFECT_SPECIAL_ATTACK_UP_3: case EFFECT_GROWTH:
        return STAT_SPATK;
    case EFFECT_SPECIAL_DEFENSE_UP: case EFFECT_SPECIAL_DEFENSE_UP_2:
        return STAT_SPDEF;
    case EFFECT_ACCURACY_UP: case EFFECT_ACCURACY_UP_2:
        return STAT_ACC;
    case EFFECT_EVASION_UP: case EFFECT_EVASION_UP_2: case EFFECT_MINIMIZE:
        return STAT_EVASION;
    }
    return NUM_BATTLE_STATS;
}

static u32 ReducedStat(u32 effect)
{
    switch (effect)
    {
    case EFFECT_ATTACK_DOWN: case EFFECT_ATTACK_DOWN_2:
        return STAT_ATK;
    case EFFECT_DEFENSE_DOWN: case EFFECT_DEFENSE_DOWN_2:
        return STAT_DEF;
    case EFFECT_SPEED_DOWN: case EFFECT_SPEED_DOWN_2:
        return STAT_SPEED;
    case EFFECT_SPECIAL_ATTACK_DOWN: case EFFECT_SPECIAL_ATTACK_DOWN_2:
        return STAT_SPATK;
    case EFFECT_SPECIAL_DEFENSE_DOWN: case EFFECT_SPECIAL_DEFENSE_DOWN_2:
        return STAT_SPDEF;
    case EFFECT_ACCURACY_DOWN: case EFFECT_ACCURACY_DOWN_2:
        return STAT_ACC;
    case EFFECT_EVASION_DOWN: case EFFECT_EVASION_DOWN_2:
        return STAT_EVASION;
    }
    return NUM_BATTLE_STATS;
}

static bool32 IsRechargeMove(u32 move)
{
    return MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_RECHARGE);
}

// Moves the Gen 4 AI never regards for damage calculation (scored only by their routines).
static bool32 IsDamageIgnored(u32 move)
{
    switch (gMovesInfo[move].effect)
    {
    case EFFECT_EXPLOSION:
    case EFFECT_DREAM_EATER:
    case EFFECT_TWO_TURNS_ATTACK:
    case EFFECT_SOLAR_BEAM:
    case EFFECT_POWER_BASED_ON_USER_HP:
    case EFFECT_GYRO_BALL:
    case EFFECT_LOW_KICK:
    case EFFECT_LEVEL_DAMAGE:
    case EFFECT_RETURN:
    case EFFECT_FRUSTRATION:
    case EFFECT_FIXED_DAMAGE_ARG:
    case EFFECT_SPIT_UP:
    case EFFECT_FOCUS_PUNCH:
    case EFFECT_SUCKER_PUNCH:
    case EFFECT_HIDDEN_POWER:
    case EFFECT_NATURAL_GIFT:
    case EFFECT_CHANGE_TYPE_ON_ITEM: // Judgment
    case EFFECT_PSYWAVE:
        return TRUE;
    }
    return IsRechargeMove(move) || move == MOVE_HEAD_SMASH || MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_ATK_DEF_DOWN);
}

static s32 ExpectedDamage(u32 battlerAtk, u32 battlerDef, u32 moveIndex)
{
    return AI_DATA->simulatedDmg[battlerAtk][battlerDef][moveIndex].expected;
}

// ------------------------------------------------------------------------------- Basic flag

static bool32 Gen4CanBePoisoned(u32 battlerAtk, u32 battlerDef)
{
    u32 ability = GetBattlerAbility(battlerDef);
    if (IS_BATTLER_OF_TYPE(battlerDef, TYPE_STEEL) || IS_BATTLER_OF_TYPE(battlerDef, TYPE_POISON))
        return FALSE;
    if (ability == ABILITY_IMMUNITY || ability == ABILITY_MAGIC_GUARD || ability == ABILITY_POISON_HEAL)
        return FALSE;
    if (ability == ABILITY_LEAF_GUARD && WeatherIs(B_WEATHER_SUN))
        return FALSE;
    if (ability == ABILITY_HYDRATION && WeatherIs(B_WEATHER_RAIN))
        return FALSE;
    if (HasStatus(battlerDef) || HasSafeguard(battlerDef))
        return FALSE;
    return TRUE;
}

static bool32 Gen4CanBeBurned(u32 battlerDef)
{
    u32 ability = GetBattlerAbility(battlerDef);
    if (IS_BATTLER_OF_TYPE(battlerDef, TYPE_FIRE) || ability == ABILITY_WATER_VEIL || ability == ABILITY_MAGIC_GUARD)
        return FALSE;
    return !HasStatus(battlerDef) && !HasSafeguard(battlerDef);
}

static s32 Gen4BasicEffect(u32 battlerAtk, u32 battlerDef, u32 move)
{
    u32 effect = gMovesInfo[move].effect;
    u32 defAbility = GetBattlerAbility(battlerDef);
    u32 atkAbility = GetBattlerAbility(battlerAtk);
    u32 stat;
    u32 atkSide = GetBattlerSide(battlerAtk), defSide = GetBattlerSide(battlerDef);

    switch (effect)
    {
    case EFFECT_SLEEP:
    case EFFECT_YAWN:
    case EFFECT_DARK_VOID:
        if (HasStatus(battlerDef) || HasSafeguard(battlerDef)
         || defAbility == ABILITY_INSOMNIA || defAbility == ABILITY_VITAL_SPIRIT)
            return -10;
        break;
    case EFFECT_POISON:
    case EFFECT_TOXIC:
        if (!Gen4CanBePoisoned(battlerAtk, battlerDef))
            return -10;
        break;
    case EFFECT_PARALYZE:
        if ((gMovesInfo[move].type == TYPE_ELECTRIC && IS_BATTLER_OF_TYPE(battlerDef, TYPE_GROUND))
         || IS_BATTLER_OF_TYPE(battlerDef, TYPE_ELECTRIC)
         || defAbility == ABILITY_LIMBER || defAbility == ABILITY_MAGIC_GUARD
         || (move == MOVE_THUNDER_WAVE && (DefAbility(battlerAtk, battlerDef, ABILITY_MOTOR_DRIVE) || DefAbility(battlerAtk, battlerDef, ABILITY_VOLT_ABSORB)))
         || HasStatus(battlerDef) || HasSafeguard(battlerDef))
            return -10;
        break;
    case EFFECT_WILL_O_WISP:
        if (!Gen4CanBeBurned(battlerDef))
            return -10;
        break;
    case EFFECT_CONFUSE:
    case EFFECT_SWAGGER:
    case EFFECT_FLATTER:
        if (gBattleMons[battlerDef].status2 & STATUS2_CONFUSION)
            return -5;
        if (defAbility == ABILITY_OWN_TEMPO || HasSafeguard(battlerDef))
            return -10;
        break;
    case EFFECT_ATTRACT:
        if ((gBattleMons[battlerDef].status2 & STATUS2_INFATUATION) || defAbility == ABILITY_OBLIVIOUS
         || !AreBattlersOfOppositeGender(battlerAtk, battlerDef))
            return -10;
        break;
    case EFFECT_EXPLOSION:
        if (CurrentEffectiveness(battlerAtk, battlerDef) == AI_EFFECTIVENESS_x0 || DefAbility(battlerAtk, battlerDef, ABILITY_DAMP))
            return -10;
        if (IsLastMon(battlerAtk))
            return IsLastMon(battlerDef) ? -1 : -10;
        break;
    case EFFECT_NIGHTMARE:
        if ((gBattleMons[battlerDef].status2 & STATUS2_NIGHTMARE) || defAbility == ABILITY_MAGIC_GUARD)
            return -10;
        if (!IsAsleep(battlerDef))
            return -8;
        break;
    case EFFECT_DREAM_EATER:
        if (!IsAsleep(battlerDef))
            return -8;
        if (CurrentEffectiveness(battlerAtk, battlerDef) == AI_EFFECTIVENESS_x0)
            return -10;
        break;
    case EFFECT_BELLY_DRUM:
        if (HpPct(battlerAtk) < 51)
            return -10;
        break;
    case EFFECT_HAZE:
    case EFFECT_PSYCH_UP:
        if (!AnyStageAtMost(battlerAtk, -1) && !AnyStageAtLeast(battlerDef, 1))
            return -10;
        break;
    case EFFECT_ROAR:
        if (IsLastMon(battlerDef) || DefAbility(battlerAtk, battlerDef, ABILITY_SUCTION_CUPS))
            return -10;
        break;
    case EFFECT_RESTORE_HP:
    case EFFECT_SOFTBOILED:
    case EFFECT_ROOST:
    case EFFECT_MORNING_SUN:
    case EFFECT_SYNTHESIS:
    case EFFECT_MOONLIGHT:
    case EFFECT_SHORE_UP:
        if (gBattleMons[battlerAtk].hp >= gBattleMons[battlerAtk].maxHP)
            return -8;
        break;
    case EFFECT_REFLECT:
        if (gSideStatuses[atkSide] & SIDE_STATUS_REFLECT)
            return -8;
        break;
    case EFFECT_LIGHT_SCREEN:
        if (gSideStatuses[atkSide] & SIDE_STATUS_LIGHTSCREEN)
            return -8;
        break;
    case EFFECT_MIST:
        if (gSideStatuses[atkSide] & SIDE_STATUS_MIST)
            return -8;
        break;
    case EFFECT_SAFEGUARD:
        if (gSideStatuses[atkSide] & SIDE_STATUS_SAFEGUARD)
            return -8;
        break;
    case EFFECT_OHKO:
        if (CurrentEffectiveness(battlerAtk, battlerDef) == AI_EFFECTIVENESS_x0 || DefAbility(battlerAtk, battlerDef, ABILITY_STURDY)
         || gBattleMons[battlerDef].level > gBattleMons[battlerAtk].level)
            return -10;
        break;
    case EFFECT_FOCUS_ENERGY:
        if (gBattleMons[battlerAtk].status2 & STATUS2_FOCUS_ENERGY_ANY)
            return -10;
        break;
    case EFFECT_INGRAIN:
        if (gStatuses3[battlerAtk] & STATUS3_ROOTED)
            return -10;
        break;
    case EFFECT_MUD_SPORT:
        if (gFieldStatuses & STATUS_FIELD_MUDSPORT)
            return -10;
        break;
    case EFFECT_WATER_SPORT:
        if (gFieldStatuses & STATUS_FIELD_WATERSPORT)
            return -10;
        break;
    case EFFECT_POWER_TRICK:
        if (gStatuses3[battlerAtk] & STATUS3_POWER_TRICK)
            return -10;
        break;
    case EFFECT_LUCKY_CHANT:
        if (gSideStatuses[atkSide] & SIDE_STATUS_LUCKY_CHANT)
            return -10;
        break;
    case EFFECT_AQUA_RING:
        if (gStatuses3[battlerAtk] & STATUS3_AQUA_RING)
            return -10;
        break;
    case EFFECT_MAGNET_RISE:
        if ((gStatuses3[battlerAtk] & STATUS3_MAGNET_RISE) || atkAbility == ABILITY_LEVITATE || IS_BATTLER_OF_TYPE(battlerAtk, TYPE_FLYING))
            return -10;
        break;
    case EFFECT_SUBSTITUTE:
        if (gBattleMons[battlerAtk].status2 & STATUS2_SUBSTITUTE)
            return -8;
        if (HpPct(battlerAtk) < 26)
            return -10;
        break;
    case EFFECT_LEECH_SEED:
        if (IsSeeded(battlerDef) || IS_BATTLER_OF_TYPE(battlerDef, TYPE_GRASS) || defAbility == ABILITY_MAGIC_GUARD)
            return -10;
        break;
    case EFFECT_DISABLE:
        if (gDisableStructs[battlerDef].disabledMove != MOVE_NONE)
            return -8;
        break;
    case EFFECT_ENCORE:
        if (gDisableStructs[battlerDef].encoredMove != MOVE_NONE)
            return -8;
        break;
    case EFFECT_SNORE:
    case EFFECT_SLEEP_TALK:
        if (!IsAsleep(battlerAtk))
            return -8;
        break;
    case EFFECT_LOCK_ON:
        if ((gStatuses3[battlerDef] & STATUS3_ALWAYS_HITS) || defAbility == ABILITY_NO_GUARD || atkAbility == ABILITY_NO_GUARD)
            return -10;
        break;
    case EFFECT_MEAN_LOOK:
        if (gBattleMons[battlerDef].status2 & STATUS2_ESCAPE_PREVENTION)
            return -10;
        break;
    case EFFECT_FORESIGHT:
        if (gBattleMons[battlerDef].status2 & STATUS2_FORESIGHT)
            return -10;
        break;
    case EFFECT_PERISH_SONG:
        if (gStatuses3[battlerDef] & STATUS3_PERISH_SONG)
            return -10;
        break;
    case EFFECT_TORMENT:
        if (gBattleMons[battlerDef].status2 & STATUS2_TORMENT)
            return -10;
        break;
    case EFFECT_MIRACLE_EYE:
        if (gStatuses3[battlerDef] & STATUS3_MIRACLE_EYED)
            return -10;
        break;
    case EFFECT_HEAL_BLOCK:
        if (gStatuses3[battlerDef] & STATUS3_HEAL_BLOCK)
            return -10;
        break;
    case EFFECT_GASTRO_ACID:
        if ((gStatuses3[battlerDef] & STATUS3_GASTRO_ACID) || defAbility == ABILITY_MULTITYPE || defAbility == ABILITY_TRUANT
         || defAbility == ABILITY_SLOW_START || defAbility == ABILITY_STENCH || defAbility == ABILITY_RUN_AWAY
         || defAbility == ABILITY_PICKUP || defAbility == ABILITY_HONEY_GATHER)
            return -10;
        break;
    case EFFECT_CURSE:
        if (IS_BATTLER_OF_TYPE(battlerAtk, TYPE_GHOST))
        {
            if (IsCursed(battlerDef) || defAbility == ABILITY_MAGIC_GUARD)
                return -10;
        }
        else
        {
            if (atkAbility == ABILITY_SIMPLE && (Stage(battlerAtk, STAT_ATK) >= 2 || Stage(battlerAtk, STAT_DEF) >= 2))
                return -10;
            if (Stage(battlerAtk, STAT_ATK) >= 6 || Stage(battlerAtk, STAT_DEF) >= 6)
                return -10;
        }
        break;
    case EFFECT_SPIKES:
        if (gSideTimers[defSide].spikesAmount >= 3 || IsLastMon(battlerDef))
            return -10;
        break;
    case EFFECT_TOXIC_SPIKES:
        if (gSideTimers[defSide].toxicSpikesAmount >= 2 || IsLastMon(battlerDef))
            return -10;
        break;
    case EFFECT_STEALTH_ROCK:
        if ((gSideStatuses[defSide] & SIDE_STATUS_STEALTH_ROCK) || IsLastMon(battlerDef))
            return -10;
        break;
    case EFFECT_SANDSTORM:
        if (WeatherIs(B_WEATHER_SANDSTORM))
            return -8;
        break;
    case EFFECT_RAIN_DANCE:
        if (WeatherIs(B_WEATHER_RAIN))
            return -8;
        if (atkAbility != ABILITY_SWIFT_SWIM && atkAbility != ABILITY_HYDRATION && defAbility == ABILITY_HYDRATION && HasStatus(battlerDef))
            return -8;
        break;
    case EFFECT_SUNNY_DAY:
        if (WeatherIs(B_WEATHER_SUN))
            return -8;
        if (atkAbility != ABILITY_FLOWER_GIFT && atkAbility != ABILITY_LEAF_GUARD && atkAbility != ABILITY_SOLAR_POWER
         && defAbility == ABILITY_HYDRATION && HasStatus(battlerDef))
            return -10;
        break;
    case EFFECT_HAIL:
    case EFFECT_SNOWSCAPE:
        if (WeatherIs(B_WEATHER_HAIL | B_WEATHER_SNOW))
            return -8;
        if (defAbility == ABILITY_ICE_BODY && atkAbility != ABILITY_ICE_BODY)
            return -8;
        break;
    case EFFECT_FUTURE_SIGHT:
        if ((gSideStatuses[atkSide] & SIDE_STATUS_FUTUREATTACK) || (gSideStatuses[defSide] & SIDE_STATUS_FUTUREATTACK))
            return -12;
        break;
    case EFFECT_BATON_PASS:
        if (IsLastMon(battlerAtk))
            return -10;
        break;
    case EFFECT_FIRST_TURN_ONLY:
        if (!gDisableStructs[battlerAtk].isFirstTurn)
            return -10;
        break;
    case EFFECT_STOCKPILE:
        if (gDisableStructs[battlerAtk].stockpileCounter >= 3)
            return -10;
        break;
    case EFFECT_SPIT_UP:
        if (CurrentEffectiveness(battlerAtk, battlerDef) == AI_EFFECTIVENESS_x0 || gDisableStructs[battlerAtk].stockpileCounter == 0)
            return -10;
        break;
    case EFFECT_SWALLOW:
        if (gDisableStructs[battlerAtk].stockpileCounter == 0 || IS_BATTLER_OF_TYPE(battlerDef, TYPE_GHOST))
            return -10;
        if (gBattleMons[battlerAtk].hp >= gBattleMons[battlerAtk].maxHP)
            return -8;
        break;
    case EFFECT_MEMENTO:
        if (DefAbility(battlerAtk, battlerDef, ABILITY_CLEAR_BODY) || DefAbility(battlerAtk, battlerDef, ABILITY_WHITE_SMOKE)
         || Stage(battlerDef, STAT_ATK) <= -6)
            return -10;
        if (Stage(battlerDef, STAT_SPATK) <= -6)
            return -8;
        if (IsLastMon(battlerAtk))
            return -10;
        break;
    case EFFECT_HELPING_HAND:
        if (!IsDoubleBattle())
            return -10;
        break;
    case EFFECT_TRICK:
        if ((defAbility == ABILITY_STICKY_HOLD && !HasMoldBreaker(battlerAtk)) || gBattleMons[battlerDef].item == ITEM_NONE)
            return -10;
        break;
    case EFFECT_KNOCK_OFF:
        if (defAbility == ABILITY_STICKY_HOLD || gBattleMons[battlerDef].item == ITEM_NONE)
            return -10;
        break;
    case EFFECT_IMPRISON:
        if ((gStatuses3[battlerAtk] & STATUS3_IMPRISONED_OTHERS) || (gStatuses3[battlerDef] & STATUS3_IMPRISONED_OTHERS))
            return -10;
        break;
    case EFFECT_REFRESH:
        if (!(gBattleMons[battlerAtk].status1 & (STATUS1_BURN | STATUS1_PSN_ANY | STATUS1_PARALYSIS)))
            return -10;
        break;
    case EFFECT_TICKLE:
        if (DefAbility(battlerAtk, battlerDef, ABILITY_CLEAR_BODY) || DefAbility(battlerAtk, battlerDef, ABILITY_WHITE_SMOKE)
         || Stage(battlerDef, STAT_ATK) <= -6)
            return -10;
        if (Stage(battlerDef, STAT_DEF) <= -6)
            return -8;
        break;
    case EFFECT_COSMIC_POWER:
    case EFFECT_BULK_UP:
    case EFFECT_CALM_MIND:
    case EFFECT_DRAGON_DANCE:
    {
        u32 first = STAT_DEF, second = STAT_SPDEF;
        if (effect == EFFECT_BULK_UP)       first = STAT_ATK, second = STAT_DEF;
        if (effect == EFFECT_CALM_MIND)     first = STAT_SPATK, second = STAT_SPDEF;
        if (effect == EFFECT_DRAGON_DANCE)  first = STAT_ATK, second = STAT_SPEED;
        if (effect == EFFECT_DRAGON_DANCE && TrickRoomActive())
            return -10;
        if (atkAbility == ABILITY_SIMPLE && (Stage(battlerAtk, first) >= 3 || Stage(battlerAtk, second) >= 3))
            return -10;
        if (Stage(battlerAtk, first) >= 6)
            return -10;
        if (Stage(battlerAtk, second) >= 6)
            return -8;
        break;
    }
    case EFFECT_GRAVITY:
        if (gFieldStatuses & STATUS_FIELD_GRAVITY)
            return -10;
        break;
    case EFFECT_TAILWIND:
        if ((gSideStatuses[atkSide] & SIDE_STATUS_TAILWIND) || TrickRoomActive())
            return -10;
        break;
    case EFFECT_TRICK_ROOM:
        // BOFA: Trick Room lasts the whole battle and using it again would end it
        if (TrickRoomActive())
            return -10;
        if (!AttackerSlower(battlerAtk, battlerDef)
         || (IsDoubleBattle() && IsBattlerAlive(BATTLE_PARTNER(battlerDef)) && !AttackerSlower(battlerAtk, BATTLE_PARTNER(battlerDef))))
            return -10;
        break;
    case EFFECT_HEALING_WISH:
    {
        s32 penalty = -20;
        u32 i, needsHealing = FALSE;
        struct Pokemon *party = GetBattlerParty(battlerAtk);
        if (IsLastMon(battlerAtk))
            penalty -= 10;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (i == gBattlerPartyIndexes[battlerAtk] || GetMonData(&party[i], MON_DATA_SPECIES) == SPECIES_NONE
             || GetMonData(&party[i], MON_DATA_HP) == 0)
                continue;
            if (GetMonData(&party[i], MON_DATA_STATUS) || GetMonData(&party[i], MON_DATA_HP) < GetMonData(&party[i], MON_DATA_MAX_HP))
                needsHealing = TRUE;
        }
        if (!needsHealing)
            penalty -= 10;
        return penalty;
    }
    case EFFECT_NATURAL_GIFT:
        if (ItemId_GetPocket(gBattleMons[battlerAtk].item) != POCKET_BERRIES)
            return -10;
        break;
    case EFFECT_ACUPRESSURE:
        if (AnyStageAtLeast(battlerAtk, atkAbility == ABILITY_SIMPLE ? 3 : 6))
            return -10;
        break;
    case EFFECT_METAL_BURST:
        if (defAbility == ABILITY_STALL)
            return -10;
        if (atkAbility == ABILITY_STALL)
            return +10;
        if (AttackerFaster(battlerAtk, battlerDef))
            return -10;
        break;
    case EFFECT_EMBARGO:
        if (gStatuses3[battlerDef] & STATUS3_EMBARGO)
            return -10;
        break;
    case EFFECT_FLING:
        if (ItemId_GetFlingPower(gBattleMons[battlerAtk].item) == 0 || atkAbility == ABILITY_MULTITYPE)
            return -10;
        break;
    case EFFECT_PSYCHO_SHIFT:
    {
        u32 status = gBattleMons[battlerAtk].status1;
        if (!(status & STATUS1_ANY) || HasStatus(battlerDef) || HasSafeguard(battlerDef))
            return -10;
        if ((status & STATUS1_PSN_ANY) && (atkAbility == ABILITY_POISON_HEAL || !Gen4CanBePoisoned(battlerAtk, battlerDef)))
            return -10;
        if ((status & STATUS1_BURN) && !Gen4CanBeBurned(battlerDef))
            return -10;
        if ((status & STATUS1_PARALYSIS) && defAbility == ABILITY_LIMBER)
            return -10;
        break;
    }
    case EFFECT_COPYCAT:
        if (gDisableStructs[battlerAtk].isFirstTurn && AttackerFaster(battlerAtk, battlerDef))
            return -10;
        break;
    case EFFECT_POWER_SWAP:
        if (Stage(battlerDef, STAT_ATK) < Stage(battlerAtk, STAT_ATK) && Stage(battlerDef, STAT_SPATK) < Stage(battlerAtk, STAT_SPATK))
            return -10;
        break;
    case EFFECT_GUARD_SWAP:
        if (Stage(battlerDef, STAT_DEF) < Stage(battlerAtk, STAT_DEF) && Stage(battlerDef, STAT_SPDEF) < Stage(battlerAtk, STAT_SPDEF))
            return -10;
        break;
    case EFFECT_LAST_RESORT:
        if (!CanUseLastResort(battlerAtk))
            return -10;
        break;
    case EFFECT_WORRY_SEED:
        if (defAbility == ABILITY_TRUANT || defAbility == ABILITY_INSOMNIA || defAbility == ABILITY_VITAL_SPIRIT
         || defAbility == ABILITY_MULTITYPE)
            return -10;
        if (IsAsleep(battlerDef) && !KnowsEffect(battlerDef, EFFECT_SLEEP_TALK) && !KnowsEffect(battlerDef, EFFECT_SNORE))
            return -10;
        break;
    case EFFECT_DEFOG:
        if (Stage(battlerDef, STAT_EVASION) > -6 || (gSideStatuses[defSide] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN)))
            break;
        if (WeatherIs(B_WEATHER_FOG))
            break;
        if (IsLastMon(battlerDef) || !(gSideStatuses[defSide] & (SIDE_STATUS_SPIKES | SIDE_STATUS_STEALTH_ROCK | SIDE_STATUS_TOXIC_SPIKES)))
            return -10;
        break;
    case EFFECT_CAPTIVATE:
        if (DefAbility(battlerAtk, battlerDef, ABILITY_OBLIVIOUS) || DefAbility(battlerAtk, battlerDef, ABILITY_CLEAR_BODY)
         || DefAbility(battlerAtk, battlerDef, ABILITY_WHITE_SMOKE) || !AreBattlersOfOppositeGender(battlerAtk, battlerDef)
         || Stage(battlerDef, STAT_SPATK) <= -6)
            return -10;
        break;
    }

    // Stat boosting status moves
    stat = BoostedStat(effect);
    if (stat != NUM_BATTLE_STATS)
    {
        if (stat == STAT_SPEED && TrickRoomActive())
            return -10;
        if ((stat == STAT_ACC || stat == STAT_EVASION) && atkAbility == ABILITY_NO_GUARD)
            return -10;
        if (atkAbility == ABILITY_SIMPLE && Stage(battlerAtk, stat) >= 3)
            return -10;
        if (Stage(battlerAtk, stat) >= 6)
            return -10;
    }

    // Stat reducing status moves
    stat = ReducedStat(effect);
    if (stat != NUM_BATTLE_STATS)
    {
        if (stat == STAT_SPEED && TrickRoomActive())
            return -10;
        if (stat == STAT_ATK && DefAbility(battlerAtk, battlerDef, ABILITY_HYPER_CUTTER))
            return -10;
        if (stat == STAT_SPEED && defAbility == ABILITY_SPEED_BOOST)
            return -10;
        if ((stat == STAT_ACC || stat == STAT_EVASION) && (atkAbility == ABILITY_NO_GUARD || defAbility == ABILITY_NO_GUARD))
            return -10;
        if (stat == STAT_ACC && defAbility == ABILITY_KEEN_EYE)
            return -10;
        if (DefAbility(battlerAtk, battlerDef, ABILITY_CLEAR_BODY) || DefAbility(battlerAtk, battlerDef, ABILITY_WHITE_SMOKE))
            return -10;
        if (Stage(battlerDef, stat) <= -6)
            return -10;
    }
    return 0;
}

s32 AI_Gen4Basic(u32 battlerAtk, u32 battlerDef, u32 move, s32 score)
{
    s32 change;

    if (IsBattlerAlly(battlerAtk, battlerDef))
        return score;

    // Step 1: immunities (type, absorbing abilities, Wonder Guard, Soundproof)
    if (!IS_MOVE_STATUS(move))
    {
        if (CurrentEffectiveness(battlerAtk, battlerDef) == AI_EFFECTIVENESS_x0)
            return score - 10;
    }
    if (gMovesInfo[move].soundMove && DefAbility(battlerAtk, battlerDef, ABILITY_SOUNDPROOF))
        return score - 10;

    // Step 2: by effect
    change = Gen4BasicEffect(battlerAtk, battlerDef, move);
    return score + change;
}

// ---------------------------------------------------------------------- Evaluate Attack flag

s32 AI_Gen4EvaluateAttack(u32 battlerAtk, u32 battlerDef, u32 move, s32 score)
{
    u32 effect = gMovesInfo[move].effect;
    u32 index = AI_THINKING_STRUCT->movesetIndex;
    u32 i;

    if (IsBattlerAlly(battlerAtk, battlerDef))
        return score;

    if (!IS_MOVE_STATUS(move) && !IsDamageIgnored(move))
    {
        s32 dmg = ExpectedDamage(battlerAtk, battlerDef, index);
        s32 maxDmg = dmg * 108 / 100; // top damage roll
        bool32 highest = TRUE;

        if (maxDmg >= (s32)gBattleMons[battlerDef].hp && dmg > 0)
        {
            if (effect == EFFECT_EXPLOSION)
                ;
            else if (effect == EFFECT_FOCUS_PUNCH || effect == EFFECT_SUCKER_PUNCH || effect == EFFECT_FUTURE_SIGHT)
            {
                if (Roll(86))
                    score += 4;
            }
            else if (GetMovePriority(battlerAtk, move) == 1 && effect != EFFECT_FIRST_TURN_ONLY)
                score += 6;
            else
                score += 4;
        }

        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            u32 other = gBattleMons[battlerAtk].moves[i];
            if (i == index || other == MOVE_NONE || gBattleMons[battlerAtk].pp[i] == 0 || IS_MOVE_STATUS(other) || IsDamageIgnored(other))
                continue;
            if (ExpectedDamage(battlerAtk, battlerDef, i) > dmg)
                highest = FALSE;
        }
        if (!highest)
            score -= 1;
    }

    if ((effect == EFFECT_EXPLOSION || effect == EFFECT_FOCUS_PUNCH || effect == EFFECT_SUCKER_PUNCH) && Roll(205))
        score -= 2;

    if (!IS_MOVE_STATUS(move) && CurrentEffectiveness(battlerAtk, battlerDef) >= AI_EFFECTIVENESS_x4 && Roll(80))
        score += 2;

    return score;
}

// -------------------------------------------------------------------------------- Risky flag

s32 AI_Gen4Risky(u32 battlerAtk, u32 battlerDef, u32 move, s32 score)
{
    bool32 risky = FALSE;

    if (IsBattlerAlly(battlerAtk, battlerDef))
        return score;

    switch (gMovesInfo[move].effect)
    {
    case EFFECT_SLEEP:
    case EFFECT_YAWN:
    case EFFECT_DARK_VOID:
    case EFFECT_EXPLOSION:
    case EFFECT_MIRROR_MOVE:
    case EFFECT_OHKO:
    case EFFECT_CONFUSE:
    case EFFECT_METRONOME:
    case EFFECT_PSYWAVE:
    case EFFECT_COUNTER:
    case EFFECT_MIRROR_COAT:
    case EFFECT_METAL_BURST:
    case EFFECT_DESTINY_BOND:
    case EFFECT_SWAGGER:
    case EFFECT_ATTRACT:
    case EFFECT_PRESENT:
    case EFFECT_BELLY_DRUM:
    case EFFECT_FOCUS_PUNCH:
    case EFFECT_GYRO_BALL:
    case EFFECT_ACUPRESSURE:
    case EFFECT_PAYBACK:
    case EFFECT_ME_FIRST:
    case EFFECT_SUCKER_PUNCH:
        risky = TRUE;
        break;
    }
    if (!IS_MOVE_STATUS(move) && gMovesInfo[move].criticalHitStage > 0)
        risky = TRUE;
    if (MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_ALL_STATS_UP))
        risky = TRUE;
    // Platinum Kaizo: Triple Axel and Fury Cutter count as Risky (not Triple Kick / Rock Wrecker)
    if (move == MOVE_TRIPLE_AXEL || move == MOVE_FURY_CUTTER)
        risky = TRUE;

    if (risky && Roll(128))
        score += 2;
    return score;
}

// ------------------------------------------------------------------------------- Expert flag

static s32 ExpertSpeedDownStatus(u32 battlerAtk, u32 battlerDef)
{
    if (AttackerSlower(battlerAtk, battlerDef))
        return Roll(186) ? 2 : 0;
    return -3;
}

static s32 ExpertStatBoost(u32 battlerAtk, u32 battlerDef, u32 stat)
{
    s32 s = 0;
    u32 hp = HpPct(battlerAtk);

    switch (stat)
    {
    case STAT_ATK:
    case STAT_SPATK:
        if (Stage(battlerAtk, stat) >= 3 && Roll(156))
            s -= 1;
        if (hp >= 100 && Roll(128))
            s += 2;
        if (hp > 70)
            return s;
        if (hp < 40)
            return s - 2;
        if (Roll(216))
            s -= 2;
        return s;
    case STAT_DEF:
    case STAT_SPDEF:
        if (Stage(battlerAtk, stat) >= 3 && Roll(156))
            s -= 1;
        if (hp >= 100 && Roll(128))
            s += 2;
        if (hp >= 70 && Roll(200))
            return s;
        if (hp < 40)
            return s - 2;
        if (LastMoveWasStatus(battlerDef))
            return s - (Roll(196) ? 2 : 0);
        if (LastMoveWasDamaging(battlerDef)
         && ((stat == STAT_DEF && IS_MOVE_PHYSICAL(LastMoveOf(battlerDef))) || (stat == STAT_SPDEF && IS_MOVE_SPECIAL(LastMoveOf(battlerDef)))))
            return s - (Roll(150) ? 2 : 0);
        return s - 2;
    case STAT_SPEED:
        if (AttackerFaster(battlerAtk, battlerDef))
            return -3;
        return Roll(186) ? 3 : 0;
    case STAT_ACC:
        if (Stage(battlerAtk, stat) >= 3 && Roll(206))
            s -= 2;
        if (hp < 70)
            s -= 2;
        return s;
    case STAT_EVASION:
        if (hp >= 90 && Roll(156))
            s += 3;
        if (Stage(battlerAtk, stat) >= 3 && Roll(128))
            s -= 1;
        if (IsBadlyPoisoned(battlerDef))
        {
            if (hp > 50 ? Roll(206) : Roll(141))
                s += 3;
        }
        if (IsSeeded(battlerDef) && Roll(186))
            s += 3;
        if (IsRootedOrAquaRing(battlerAtk) && Roll(128))
            s += 2;
        if (IsCursed(battlerDef) && Roll(186))
            s += 3;
        if (hp > 70)
            return s;
        if (Stage(battlerAtk, stat) == 0)
            return s;
        if (hp < 40 || HpPct(battlerDef) < 40)
            return s - 2;
        if (Roll(186))
            s -= 2;
        return s;
    }
    return 0;
}

static s32 ExpertStatReduce(u32 battlerAtk, u32 battlerDef, u32 stat)
{
    s32 s = 0;
    u32 hp = HpPct(battlerAtk), defHp = HpPct(battlerDef);

    switch (stat)
    {
    case STAT_ATK:
    case STAT_SPATK:
        if (Stage(battlerDef, stat) != 0)
        {
            s -= 1;
            if (hp <= 90)
                s -= 1;
            if (Stage(battlerDef, stat) <= -3 && Roll(206))
                s -= 2;
        }
        if (defHp <= 70)
            s -= 2;
        if (LastMoveOf(battlerDef) != MOVE_NONE
         && !((stat == STAT_ATK && IS_MOVE_PHYSICAL(LastMoveOf(battlerDef))) || (stat == STAT_SPATK && IS_MOVE_SPECIAL(LastMoveOf(battlerDef))))
         && Roll(128))
            s -= 2;
        return s;
    case STAT_DEF:
    case STAT_SPDEF:
        if (hp < 70 && Roll(206))
            s -= 2;
        else if (Stage(battlerDef, stat) <= -3 && Roll(206))
            s -= 2;
        if (defHp < 70)
            s -= 2;
        return s;
    case STAT_SPEED:
        return ExpertSpeedDownStatus(battlerAtk, battlerDef);
    case STAT_ACC:
        if (defHp <= 70 && !(hp >= 70) && Roll(156))
            s -= 1;
        if (Stage(battlerAtk, STAT_ACC) <= -2 && Roll(176))
            s -= 2;
        if (IsBadlyPoisoned(battlerDef) && Roll(186))
            s += 2;
        if (IsSeeded(battlerDef) && Roll(186))
            s += 2;
        if (IsRootedOrAquaRing(battlerAtk) && Roll(128))
            s += 1;
        if (IsCursed(battlerDef) && Roll(186))
            s += 2;
        if (hp > 70)
            return s;
        if (Stage(battlerDef, stat) == 0)
            return s;
        if (hp < 40 || defHp < 40)
            return s - 2;
        if (Roll(186))
            s -= 2;
        return s;
    case STAT_EVASION:
        if (hp < 70 && Roll(206))
            s -= 2;
        else if (Stage(battlerDef, stat) <= -3 && Roll(206))
            s -= 2;
        if (defHp <= 70)
            s -= 2;
        return s;
    }
    return 0;
}

static bool32 IsMirrorMoveEncouraged(u32 move)
{
    switch (move)
    {
    case MOVE_SLEEP_POWDER: case MOVE_LOVELY_KISS: case MOVE_SPORE: case MOVE_HYPNOSIS: case MOVE_SING:
    case MOVE_GRASS_WHISTLE: case MOVE_SHADOW_PUNCH: case MOVE_SAND_ATTACK: case MOVE_SMOKESCREEN: case MOVE_TOXIC:
    case MOVE_GUILLOTINE: case MOVE_HORN_DRILL: case MOVE_FISSURE: case MOVE_SHEER_COLD: case MOVE_CROSS_CHOP:
    case MOVE_AEROBLAST: case MOVE_CONFUSE_RAY: case MOVE_SWEET_KISS: case MOVE_SCREECH: case MOVE_COTTON_SPORE:
    case MOVE_SCARY_FACE: case MOVE_FAKE_TEARS: case MOVE_METAL_SOUND: case MOVE_THUNDER_WAVE: case MOVE_GLARE:
    case MOVE_POISON_POWDER: case MOVE_SHADOW_BALL: case MOVE_DYNAMIC_PUNCH: case MOVE_HYPER_BEAM: case MOVE_EXTREME_SPEED:
    case MOVE_THIEF: case MOVE_COVET: case MOVE_ATTRACT: case MOVE_SWAGGER: case MOVE_TORMENT: case MOVE_FLATTER:
    case MOVE_TRICK: case MOVE_SUPERPOWER: case MOVE_SKILL_SWAP: case MOVE_PSYCHO_SHIFT: case MOVE_POWER_SWAP:
    case MOVE_GUARD_SWAP: case MOVE_SUCKER_PUNCH: case MOVE_HEART_SWAP: case MOVE_SWITCHEROO: case MOVE_CAPTIVATE:
        return TRUE;
    }
    return FALSE;
}

static bool32 IsDesirableAbility(u32 ability)
{
    switch (ability)
    {
    case ABILITY_SPEED_BOOST: case ABILITY_BATTLE_ARMOR: case ABILITY_SAND_VEIL: case ABILITY_STATIC: case ABILITY_FLASH_FIRE:
    case ABILITY_WONDER_GUARD: case ABILITY_EFFECT_SPORE: case ABILITY_SWIFT_SWIM: case ABILITY_HUGE_POWER: case ABILITY_RAIN_DISH:
    case ABILITY_CUTE_CHARM: case ABILITY_SHED_SKIN: case ABILITY_MARVEL_SCALE: case ABILITY_PURE_POWER: case ABILITY_CHLOROPHYLL:
    case ABILITY_SHIELD_DUST: case ABILITY_ADAPTABILITY: case ABILITY_MAGIC_GUARD: case ABILITY_MOLD_BREAKER: case ABILITY_SUPER_LUCK:
    case ABILITY_UNAWARE: case ABILITY_TINTED_LENS: case ABILITY_FILTER: case ABILITY_SOLID_ROCK: case ABILITY_RECKLESS:
        return TRUE;
    }
    return FALSE;
}

static bool32 IsBadTradeItem(u32 holdEffect)
{
    switch (holdEffect)
    {
    case HOLD_EFFECT_MACHO_BRACE: case HOLD_EFFECT_CHOICE_BAND: case HOLD_EFFECT_CHOICE_SCARF: case HOLD_EFFECT_CHOICE_SPECS:
    case HOLD_EFFECT_IRON_BALL: case HOLD_EFFECT_LAGGING_TAIL: case HOLD_EFFECT_STICKY_BARB: case HOLD_EFFECT_POWER_ITEM:
    case HOLD_EFFECT_TOXIC_ORB: case HOLD_EFFECT_FLAME_ORB: case HOLD_EFFECT_BLACK_SLUDGE:
        return TRUE;
    }
    return FALSE;
}

static bool32 IsDisruptiveItem(u32 holdEffect)
{
    return holdEffect != HOLD_EFFECT_MACHO_BRACE && holdEffect != HOLD_EFFECT_TOXIC_ORB && holdEffect != HOLD_EFFECT_FLAME_ORB
        && holdEffect != HOLD_EFFECT_BLACK_SLUDGE && IsBadTradeItem(holdEffect);
}

// does any party member (bench) deal more damage to battlerDef than battlerAtk's best move?
static bool32 BenchOutdamages(u32 battlerAtk, u32 battlerDef)
{
    u32 i, j;
    s32 best = 0;
    struct Pokemon *party = GetBattlerParty(battlerAtk);

    for (i = 0; i < MAX_MON_MOVES; i++)
        if (ExpectedDamage(battlerAtk, battlerDef, i) > best)
            best = ExpectedDamage(battlerAtk, battlerDef, i);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (i == gBattlerPartyIndexes[battlerAtk] || !IsValidForBattle(&party[i]))
            continue;
        InitializeSwitchinCandidate(&party[i]);
        for (j = 0; j < MAX_MON_MOVES; j++)
        {
            u32 move = GetMonData(&party[i], MON_DATA_MOVE1 + j);
            if (move == MOVE_NONE || IS_MOVE_STATUS(move))
                continue;
            if (AI_CalcPartyMonDamage(move, battlerAtk, battlerDef, AI_DATA->switchinCandidate.battleMon, TRUE, DMG_ROLL_DEFAULT) > best)
                return TRUE;
        }
    }
    return FALSE;
}

// does battlerAtk's best move deal more damage to battlerDef than battlerDef's best move deals back?
static bool32 AttackerOutdamages(u32 battlerAtk, u32 battlerDef)
{
    u32 i;
    s32 mine = 0, theirs = 0;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (ExpectedDamage(battlerAtk, battlerDef, i) > mine)
            mine = ExpectedDamage(battlerAtk, battlerDef, i);
        if (AI_DATA->simulatedDmg[battlerDef][battlerAtk][i].expected > theirs)
            theirs = AI_DATA->simulatedDmg[battlerDef][battlerAtk][i].expected;
    }
    return mine > theirs;
}

static s32 TieredRoll(u32 tiers)
{
    // 50% top, 25% next ... down to 0 (Power Swap / Guard Swap / Punishment tables)
    u32 r = Random() % 256;
    s32 value = tiers;
    u32 threshold = 128;
    while (value > 0)
    {
        if (r < threshold)
            return value;
        r -= threshold;
        threshold /= 2;
        value--;
    }
    return 0;
}

static s32 SwapScore(s32 diffMain, s32 diffSecond)
{
    s32 base;
    if (diffMain > 3)      base = 3;
    else if (diffMain > 1) base = 2;
    else if (diffMain > 0) base = 1;
    else if (diffMain == 0) base = 0;
    else return 0;

    if (diffSecond > 3)       return TieredRoll(base + 2);
    else if (diffSecond > 1)  return TieredRoll(base + 1);
    else if (diffSecond == 0) return base == 0 ? 0 : TieredRoll(base);
    else if (diffSecond > 0 && base == 0) return TieredRoll(1);
    return 0;
}

s32 AI_Gen4Expert(u32 battlerAtk, u32 battlerDef, u32 move, s32 score)
{
    u32 effect = gMovesInfo[move].effect;
    u32 hp = HpPct(battlerAtk), defHp = HpPct(battlerDef);
    bool32 faster = AttackerFaster(battlerAtk, battlerDef), slower = AttackerSlower(battlerAtk, battlerDef);
    u32 atkAbility = GetBattlerAbility(battlerAtk), defAbility = GetBattlerAbility(battlerDef);
    u32 atkHold = GetBattlerHoldEffect(battlerAtk, TRUE), defHold = GetBattlerHoldEffect(battlerDef, TRUE);
    u32 stat;
    s32 s = 0;

    if (IsBattlerAlly(battlerAtk, battlerDef))
        return score;

    // ---- moves identified by their (damaging) properties
    if (!IS_MOVE_STATUS(move))
    {
        // Platinum Kaizo 1/4-recoil routine (Brave Bird, Close Combat, Take Down)
        if (gMovesInfo[move].recoil == 25)
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (slower && hp <= 80)
                s -= 1;
            if (faster && hp <= 60)
                s -= 1;
        }
        else if (gMovesInfo[move].recoil > 0 && move != MOVE_HEAD_SMASH)
        {
            if (!ResistsOrImmune(battlerAtk, battlerDef) && (atkAbility == ABILITY_ROCK_HEAD || atkAbility == ABILITY_MAGIC_GUARD))
                s += 1;
        }
        if (gMovesInfo[move].accuracy == 0 && effect != EFFECT_DRAGON_DARTS)
        {
            if (Stage(battlerDef, STAT_EVASION) >= 5 || Stage(battlerAtk, STAT_ACC) <= -5)
                s += Roll(156) ? 2 : 1;
            else if (Stage(battlerDef, STAT_EVASION) >= 3 || Stage(battlerAtk, STAT_ACC) <= -3)
                s += 1;
        }
        if (gMovesInfo[move].criticalHitStage > 0)
        {
            u32 eff = CurrentEffectiveness(battlerAtk, battlerDef);
            if (eff >= AI_EFFECTIVENESS_x2 && Roll(128))
                s += 1;
            else if (eff == AI_EFFECTIVENESS_x1 && Roll(64))
                s += 1;
        }
        if (MoveHasAdditionalEffect(move, MOVE_EFFECT_WRAP)
         && HasDrainingCondition(battlerDef, FALSE) && Roll(128))
            s += 1;
        if (MoveHasAdditionalEffect(move, MOVE_EFFECT_SPD_MINUS_1) && !ResistsOrImmune(battlerAtk, battlerDef)
         && (move == MOVE_ICY_WIND || move == MOVE_ROCK_TOMB || move == MOVE_MUD_SHOT))
            s += ExpertSpeedDownStatus(battlerAtk, battlerDef);
        if (IsRechargeMove(move))
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (atkAbility == ABILITY_TRUANT && Roll(176))
                s += 1;
            if (slower && hp >= 60)
                s -= 1;
            if (faster && hp > 40)
                s -= 1;
        }
        if (MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_SP_ATK_MINUS_2))
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (faster && hp <= 60)
                s -= 1;
            if (slower && hp <= 80)
                s -= 1;
        }
        if (MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_ATK_DEF_DOWN)) // Superpower
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (Stage(battlerAtk, STAT_ATK) <= -1)
                s -= 1;
            if (slower && hp >= 60)
                s -= 1;
            if (faster && hp > 40)
                s -= 1;
        }
        if (MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_DEF_SPDEF_DOWN)) // Close Combat (vanilla)
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (slower && hp <= 80)
                s -= 1;
            if (faster && hp <= 60)
                s -= 1;
        }
        if (MoveHasAdditionalEffectSelf(move, MOVE_EFFECT_SPD_MINUS_1)) // Hammer Arm
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (slower)
                s += 1;
        }
        if (MoveHasAdditionalEffect(move, MOVE_EFFECT_FEINT))
        {
            if (!KnowsProtect(battlerDef) && Roll(192))
                return score + s;
            if (HasDrainingCondition(battlerAtk, TRUE))
            {
                if (Roll(128))
                    s += 1;
            }
            else if (gBattleMons[battlerDef].hp < gBattleMons[battlerDef].maxHP
                  && (defHold == HOLD_EFFECT_LEFTOVERS || defHold == HOLD_EFFECT_BLACK_SLUDGE) && Roll(128))
                s += 1;
            if (gDisableStructs[battlerDef].protectUses == 0)
                s += Roll(128) ? 1 : 0;
            else if (gDisableStructs[battlerDef].protectUses == 1)
                s += Roll(64) ? 1 : 0;
            else
                s -= 2;
        }
        if (MoveHasAdditionalEffect(move, MOVE_EFFECT_BUG_BITE))
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (gDisableStructs[battlerAtk].isFirstTurn && Roll(192))
                s += 1;
            if (Roll(128))
                s += 1;
        }
        if (MoveHasAdditionalEffect(move, MOVE_EFFECT_STEAL_ITEM)) // Thief, Covet
        {
            if (defHold == HOLD_EFFECT_NONE || IsBadTradeItem(defHold))
                s -= 2;
            else if (Roll(206))
                s += 1;
        }
        if (move == MOVE_VITAL_THROW && !slower && hp <= 60)
        {
            if (hp < 40)
                s -= Roll(206) ? 1 : 0;
            else
                s -= Roll(61) ? 1 : 0;
        }
    }

    // ---- by effect
    switch (effect)
    {
    case EFFECT_SLEEP:
    case EFFECT_DARK_VOID:
        if ((KnowsEffect(battlerAtk, EFFECT_DREAM_EATER) || KnowsEffect(battlerAtk, EFFECT_NIGHTMARE)) && Roll(128))
            s += 1;
        break;
    case EFFECT_POISON:
        if (hp < 50 || defHp <= 50)
            s -= 1;
        break;
    case EFFECT_PARALYZE:
        if (slower && Roll(236))
            s += 3;
        if (hp <= 70)
            s -= 1;
        break;
    case EFFECT_CONFUSE:
    case EFFECT_FLATTER:
    case EFFECT_SWAGGER:
        if (effect == EFFECT_SWAGGER && Knows(battlerAtk, MOVE_PSYCH_UP))
        {
            if (Stage(battlerDef, STAT_ATK) >= -3)
                return score + s - 5;
            s += gDisableStructs[battlerAtk].isFirstTurn ? 5 : 3;
            break;
        }
        if (defHp <= 70)
        {
            if (Roll(128))
                s -= 1;
            if (defHp <= 50)
            {
                s -= 1;
                if (defHp <= 30)
                    s -= 1;
            }
        }
        if ((effect == EFFECT_FLATTER || effect == EFFECT_SWAGGER) && Roll(128))
            s += 1;
        break;
    case EFFECT_ABSORB:
        if (ResistsOrImmune(battlerAtk, battlerDef) && Roll(206))
            s -= 3;
        break;
    case EFFECT_DREAM_EATER:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        else if (IsAsleep(battlerDef) && Roll(205))
            s += 3;
        break;
    case EFFECT_EXPLOSION:
    case EFFECT_MEMENTO:
        if (Stage(battlerDef, STAT_EVASION) >= 1)
            s -= 1;
        if (Stage(battlerDef, STAT_EVASION) >= 3 && Roll(128))
            s -= 1;
        if (hp >= 80)
            s -= Roll(206) ? (faster ? 3 : 1) : 0;
        else if (hp > 50)
            s -= Roll(206) ? 1 : 0;
        else if (hp > 30)
            s += Roll(128) ? 1 : 0;
        else
            s += Roll(206) ? 1 : 0;
        break;
    case EFFECT_HEALING_WISH:
        if (hp >= 80 && faster && Roll(64))
            s -= 5;
        if (hp > 50 && Roll(206))
            s -= 1;
        if (!Roll(192))
        {
            s += 1;
            if (!HasSuperEffectiveMove(battlerAtk, battlerDef) && Roll(64))
                s += 1;
            if (BenchOutdamages(battlerAtk, battlerDef) && Roll(128))
                s += 1;
        }
        if (hp <= 30 && Roll(128))
            s += 1;
        break;
    case EFFECT_MIRROR_MOVE:
        if (faster && IsMirrorMoveEncouraged(LastMoveOf(battlerDef)))
        {
            if (Roll(128))
                s += 2;
        }
        else if (!IsMirrorMoveEncouraged(LastMoveOf(battlerDef)) && Roll(176))
            s -= 1;
        break;
    case EFFECT_DRAGON_DANCE:
        if (slower && Roll(128))
            s += 1;
        if (hp <= 50 && Roll(186))
            s -= 1;
        break;
    case EFFECT_ACUPRESSURE:
        if (hp <= 50)
            s -= 1;
        else if (hp > 90)
            s += Roll(192) ? 1 : 0;
        else
            s += Roll(96) ? 1 : 0;
        break;
    case EFFECT_BULK_UP:
        s += ExpertStatBoost(battlerAtk, battlerDef, STAT_DEF);
        break;
    case EFFECT_CALM_MIND:
    case EFFECT_COSMIC_POWER:
        s += ExpertStatBoost(battlerAtk, battlerDef, STAT_SPDEF);
        break;
    case EFFECT_HAZE:
        if (AnyStageAtLeast(battlerAtk, 3) || AnyStageAtMost(battlerDef, -3))
            s -= Roll(206) ? 3 : 0;
        if (AnyStageAtMost(battlerAtk, -3) || AnyStageAtLeast(battlerDef, 3))
            s += Roll(206) ? 3 : 0;
        else if (!(AnyStageAtLeast(battlerAtk, 3) || AnyStageAtMost(battlerDef, -3)))
            s -= 1;
        break;
    case EFFECT_BIDE:
        if (hp <= 90)
            s -= 2;
        break;
    case EFFECT_ROAR:
        if (gDisableStructs[battlerDef].bofaTurnsOnField > 3 && Roll(192))
            s += 2;
        else if ((gSideStatuses[GetBattlerSide(battlerDef)] & (SIDE_STATUS_SPIKES | SIDE_STATUS_STEALTH_ROCK | SIDE_STATUS_TOXIC_SPIKES)) && Roll(128))
            s += 2;
        else if ((Stage(battlerDef, STAT_ATK) >= 3 || Stage(battlerDef, STAT_DEF) >= 3 || Stage(battlerDef, STAT_SPATK) >= 3
               || Stage(battlerDef, STAT_SPDEF) >= 3 || Stage(battlerDef, STAT_EVASION) >= 3) && Roll(128))
            s += 2;
        else
            s -= 3;
        break;
    case EFFECT_CONVERSION:
        if (hp <= 90)
            s -= 2;
        if (gBattleResults.battleTurnCounter != 0 && Roll(200))
            s -= 2;
        break;
    case EFFECT_RESTORE_HP:
    case EFFECT_SOFTBOILED:
    case EFFECT_ROOST:
    case EFFECT_MORNING_SUN:
    case EFFECT_SYNTHESIS:
    case EFFECT_MOONLIGHT:
    case EFFECT_SHORE_UP:
    case EFFECT_SWALLOW:
        if (gBattleMons[battlerAtk].hp >= gBattleMons[battlerAtk].maxHP)
            return score + s - 3;
        if (faster)
            return score + s - 8;
        if (hp >= 70 && Roll(226))
            return score + s - 3;
        if (!Knows(battlerDef, MOVE_SNATCH) ? Roll(236) : Roll(144))
            s += 2;
        if ((effect == EFFECT_MORNING_SUN || effect == EFFECT_SYNTHESIS || effect == EFFECT_MOONLIGHT)
         && WeatherIs(B_WEATHER_HAIL | B_WEATHER_SNOW | B_WEATHER_RAIN | B_WEATHER_SANDSTORM))
            s -= 2;
        break;
    case EFFECT_REST:
        if (faster)
        {
            if (gBattleMons[battlerAtk].hp >= gBattleMons[battlerAtk].maxHP && Roll(156))
                return score + s - 8;
            if (hp > 50)
                return score + s - 3;
            if (hp >= 40 && Roll(186))
                return score + s - 3;
        }
        else
        {
            if (hp > 70)
                return score + s - 3;
            if (hp >= 60 && Roll(206))
                return score + s - 3;
        }
        if (!Knows(battlerDef, MOVE_SNATCH) ? Roll(246) : Roll(198))
            s += 3;
        break;
    case EFFECT_TOXIC:
    case EFFECT_LEECH_SEED:
        if (KnowsDamagingMove(battlerAtk))
        {
            if (hp <= 50 && Roll(206))
                s -= 3;
            if (defHp <= 50 && Roll(206))
                s -= 3;
        }
        if ((KnowsEffect(battlerAtk, EFFECT_SPECIAL_DEFENSE_UP) || KnowsProtect(battlerAtk)) && Roll(196))
            s += 2;
        break;
    case EFFECT_LIGHT_SCREEN:
    case EFFECT_REFLECT:
        if (hp < 50)
            s -= 2;
        if (hp >= 90 && Roll(128))
            s += 1;
        if (LastMoveWasDamaging(battlerDef)
         && ((effect == EFFECT_LIGHT_SCREEN && IS_MOVE_SPECIAL(LastMoveOf(battlerDef))) || (effect == EFFECT_REFLECT && IS_MOVE_PHYSICAL(LastMoveOf(battlerDef))))
         && Roll(192))
            s += 1;
        break;
    case EFFECT_OHKO:
        if (Roll(64))
            s += 1;
        break;
    case EFFECT_TWO_TURNS_ATTACK:
    case EFFECT_SOLAR_BEAM:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            return score + s - 2;
        if (effect == EFFECT_SOLAR_BEAM && WeatherIs(B_WEATHER_SUN))
            s += 2;
        if (atkHold == HOLD_EFFECT_POWER_HERB)
            s += 2;
        if (KnowsProtect(battlerDef))
            s -= 2;
        if (hp <= 38)
            s -= 1;
        break;
    case EFFECT_SEMI_INVULNERABLE:
        if (atkHold == HOLD_EFFECT_POWER_HERB)
            s += 2;
        if (KnowsProtect(battlerDef))
            s -= 1;
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s += 1;
        if (IsBadlyPoisoned(battlerDef) || IsCursed(battlerDef) || IsSeeded(battlerDef))
            s += 1;
        if (((WeatherIs(B_WEATHER_SANDSTORM) && (IS_BATTLER_OF_TYPE(battlerAtk, TYPE_ROCK) || IS_BATTLER_OF_TYPE(battlerAtk, TYPE_GROUND) || IS_BATTLER_OF_TYPE(battlerAtk, TYPE_STEEL)))
          || (WeatherIs(B_WEATHER_HAIL) && IS_BATTLER_OF_TYPE(battlerAtk, TYPE_ICE))) && Roll(176))
            s += 1;
        if (faster && !(LastMoveOf(battlerDef) != MOVE_NONE && gMovesInfo[LastMoveOf(battlerDef)].accuracy == 0) && Roll(176))
            s += 1;
        break;
    case EFFECT_FIRST_TURN_ONLY:
        s += 2;
        break;
    case EFFECT_SPIT_UP:
        if (gDisableStructs[battlerAtk].stockpileCounter >= 2 && Roll(176))
            s += 2;
        break;
    case EFFECT_SUPER_FANG:
        if (defHp <= 50)
            s -= 1;
        break;
    case EFFECT_DISABLE:
        if (slower)
            break;
        if (LastMoveWasStatus(battlerDef))
            s -= Roll(156) ? 1 : 0;
        else if (LastMoveWasDamaging(battlerDef))
            s += 1;
        break;
    case EFFECT_ENCORE:
        if (gDisableStructs[battlerDef].disabledMove != MOVE_NONE && Roll(226))
            s += 3;
        else if (slower)
            s -= 2;
        else if (!LastMoveWasStatus(battlerDef))
            s -= 2;
        else if (Roll(226))
            s += 3;
        break;
    case EFFECT_COUNTER:
    case EFFECT_MIRROR_COAT:
    {
        bool32 counter = (effect == EFFECT_COUNTER);
        if (IsAsleepConfusedOrInfatuated(battlerDef))
            return score + s - 1;
        if (hp <= 30 && Roll(246))
            s -= 1;
        if (hp <= 50 && Roll(156))
            s -= 1;
        if (Knows(battlerAtk, MOVE_COUNTER) && Knows(battlerAtk, MOVE_MIRROR_COAT) && Roll(156))
            s += 4;
        if (LastMoveWasStatus(battlerDef))
        {
            if (IsTaunted(battlerDef) && Roll(156))
                s += 1;
            if (Roll(125))
                s += 4;
        }
        else if (LastMoveWasDamaging(battlerDef))
        {
            u32 last = LastMoveOf(battlerDef);
            if (IsTaunted(battlerDef) && Roll(156))
                s += 1;
            if ((counter && !IS_MOVE_PHYSICAL(last)) || (!counter && !IS_MOVE_SPECIAL(last)))
                s -= 1;
            else if (Roll(156))
                s += 1;
        }
        break;
    }
    case EFFECT_METAL_BURST:
        if (IsAsleepConfusedOrInfatuated(battlerDef) || KnowsEffect(battlerDef, EFFECT_REVENGE)
         || KnowsEffect(battlerDef, EFFECT_FOCUS_PUNCH) || Knows(battlerDef, MOVE_VITAL_THROW))
            return score + s - 1;
        if (hp <= 30 && Roll(246))
            s -= 1;
        if (hp <= 50 && Roll(156))
            s -= 1;
        if (hp > 50 && Roll(64))
            s += 1;
        if (!LastMoveWasStatus(battlerDef) && !IsTaunted(battlerDef) && Roll(156))
            s += 1;
        if (!IsTaunted(battlerDef) && Roll(156))
            s += 1;
        break;
    case EFFECT_PAIN_SPLIT:
        if (defHp < 80)
            s -= 1;
        else if (slower)
            s += (hp > 60) ? -1 : 1;
        else
            s -= 1;
        break;
    case EFFECT_NIGHTMARE:
        s += 2;
        break;
    case EFFECT_LOCK_ON:
        if (Roll(128))
            s += 2;
        break;
    case EFFECT_SLEEP_TALK:
        s += IsAsleep(battlerAtk) ? 10 : -5;
        break;
    case EFFECT_DESTINY_BOND:
        s -= 1;
        if (slower || hp > 70)
            break;
        if (Roll(128))
            s += 1;
        if (hp > 50)
            break;
        if (Roll(128))
            s += 1;
        if (hp > 30)
            break;
        if (Roll(156))
            s += 2;
        break;
    case EFFECT_FLAIL:
        if (slower)
        {
            if (hp > 60)      s -= 1;
            else if (hp > 40) ;
            else if (Roll(156)) s += 1;
        }
        else
        {
            if (hp > 33)      s -= 1;
            else if (hp > 20) ;
            else if (hp >= 8) s += Roll(156) ? 1 : 0;
            else              s += Roll(156) ? 2 : 1;
        }
        break;
    case EFFECT_HEAL_BELL:
    {
        u32 i;
        bool32 any = HasStatus(battlerAtk);
        struct Pokemon *party = GetBattlerParty(battlerAtk);
        for (i = 0; i < PARTY_SIZE && !any; i++)
            if (GetMonData(&party[i], MON_DATA_HP) && GetMonData(&party[i], MON_DATA_STATUS))
                any = TRUE;
        if (!any)
            s -= 5;
        break;
    }
    case EFFECT_CURSE:
        if (IS_BATTLER_OF_TYPE(battlerAtk, TYPE_GHOST))
        {
            if (hp <= 80)
                s -= 1;
            break;
        }
        if (Stage(battlerAtk, STAT_DEF) >= 3)
            break;
        if ((KnowsEffect(battlerAtk, EFFECT_GYRO_BALL) || KnowsEffect(battlerAtk, EFFECT_TRICK_ROOM)) && Roll(224))
            s += 1;
        if (Roll(128))
        {
            s += 1;
            if (Stage(battlerAtk, STAT_DEF) <= 1 && Roll(128))
                s += 1;
            if (Stage(battlerAtk, STAT_DEF) <= 0 && Roll(128))
                s += 1;
        }
        break;
    case EFFECT_PROTECT:
    {
        bool32 lockedOn = (gStatuses3[battlerAtk] & STATUS3_ALWAYS_HITS) != 0;
        if ((Knows(battlerDef, MOVE_FEINT) || Knows(battlerDef, MOVE_SHADOW_FORCE)) && Roll(128))
            s -= 2;
        if (gDisableStructs[battlerAtk].protectUses > 1)
            return score + s - 2;
        if (HasDrainingCondition(battlerAtk, TRUE) && !lockedOn)
            return score + s - 2;
        if ((KnowsEffect(battlerDef, EFFECT_RESTORE_HP) || KnowsEffect(battlerDef, EFFECT_SOFTBOILED)
          || KnowsEffect(battlerDef, EFFECT_ROOST) || KnowsEffect(battlerDef, EFFECT_DEFENSE_CURL)) && !lockedOn)
            return score + s - 2;
        if (HasDrainingCondition(battlerDef, TRUE))
            s += 2;
        else if (IsDoubleBattle())
            s += 2;
        else if (lockedOn)
            s += 2;
        else if (Roll(85))
            s += 2;
        if (Roll(128))
            s -= 1;
        if (gDisableStructs[battlerAtk].protectUses == 1)
        {
            s -= 1;
            if (Roll(128))
                s -= 1;
        }
        break;
    }
    case EFFECT_SPIKES:
    case EFFECT_TOXIC_SPIKES:
    case EFFECT_STEALTH_ROCK:
        if (Roll(128))
            break;
        s += 1;
        if ((Knows(battlerAtk, MOVE_ROAR) || Knows(battlerAtk, MOVE_WHIRLWIND)) && Roll(192))
            s += 1;
        break;
    case EFFECT_FORESIGHT:
        if (IS_BATTLER_OF_TYPE(battlerAtk, TYPE_GHOST) && Roll(121)) // sic: checks the attacker, as in the original
            s += 2;
        else if (Stage(battlerDef, STAT_EVASION) >= 3 && Roll(176))
            s += 2;
        else
            s -= 2;
        break;
    case EFFECT_MIRACLE_EYE:
        if (IS_BATTLER_OF_TYPE(battlerDef, TYPE_DARK) && Roll(121))
            s += 2;
        else if (Stage(battlerDef, STAT_EVASION) >= 3 && Roll(176))
            s += 2;
        else
            s -= 2;
        break;
    case EFFECT_ENDURE:
        if (hp < 4)
            s -= 1;
        else if (hp < 35 && Roll(186))
            s += 1;
        break;
    case EFFECT_SUBSTITUTE:
    {
        u32 rolls = 0, i;
        if (Knows(battlerAtk, MOVE_FOCUS_PUNCH) && Roll(160))
            s += 1;
        if (hp <= 90)
            rolls = (hp > 70) ? 1 : (hp > 50) ? 2 : 3;
        for (i = 0; i < rolls; i++)
            if (Roll(156))
                s -= 1;
        if (faster)
        {
            u32 last = LastMoveOf(battlerDef);
            if (last != MOVE_NONE && IS_MOVE_STATUS(last))
            {
                u32 le = gMovesInfo[last].effect;
                if ((le == EFFECT_SLEEP || le == EFFECT_POISON || le == EFFECT_TOXIC || le == EFFECT_PARALYZE || le == EFFECT_WILL_O_WISP)
                 && HasStatus(battlerDef) && Roll(156))
                    s += 1;
                else if (le == EFFECT_CONFUSE && (gBattleMons[battlerDef].status2 & STATUS2_CONFUSION) && Roll(156))
                    s += 1;
                else if (le == EFFECT_LEECH_SEED && IsSeeded(battlerDef) && Roll(156))
                    s += 1;
            }
        }
        break;
    }
    case EFFECT_BATON_PASS:
    {
        bool32 bad = (slower && hp <= 70) || (faster && hp <= 60);
        if (AnyStageAtLeast(battlerAtk, 3))
        {
            if (bad && Roll(176))
                s += 2;
        }
        else if (AnyStageAtLeast(battlerAtk, 2))
        {
            if (bad)
                s -= 2;
        }
        else
            s -= 2;
        break;
    }
    case EFFECT_PURSUIT:
        if (gDisableStructs[battlerAtk].isFirstTurn)
        {
            if (Roll(128))
                s += 1;
        }
        else if ((IS_BATTLER_OF_TYPE(battlerDef, TYPE_GHOST) || IS_BATTLER_OF_TYPE(battlerDef, TYPE_PSYCHIC)) && Roll(128))
            s += 1;
        if (Knows(battlerDef, MOVE_U_TURN) && Roll(128))
            s += 1;
        break;
    case EFFECT_RAIN_DANCE:
        if (slower && atkAbility == ABILITY_SWIFT_SWIM)
            return score + s + 1;
        if (hp < 40)
            s -= 1;
        if (WeatherIs(B_WEATHER_HAIL | B_WEATHER_SUN | B_WEATHER_SANDSTORM))
            s += 1;
        if (atkAbility == ABILITY_RAIN_DISH || (HasStatus(battlerAtk) && atkAbility == ABILITY_HYDRATION))
            s += 1;
        break;
    case EFFECT_SUNNY_DAY:
        if (hp < 40)
            s -= 1;
        if (WeatherIs(B_WEATHER_HAIL | B_WEATHER_RAIN | B_WEATHER_SANDSTORM))
            s += 1;
        if (atkAbility == ABILITY_FLOWER_GIFT || (HasStatus(battlerAtk) && atkAbility == ABILITY_LEAF_GUARD))
            s += 1;
        break;
    case EFFECT_HAIL:
    case EFFECT_SNOWSCAPE:
        if (hp < 40)
            return score + s - 1;
        if (WeatherIs(B_WEATHER_SUN | B_WEATHER_RAIN | B_WEATHER_SANDSTORM))
        {
            s += 1;
            if (KnowsEffect(battlerAtk, EFFECT_BLIZZARD))
                s += 2;
        }
        if (atkAbility == ABILITY_ICE_BODY)
            s += 2;
        break;
    case EFFECT_GRAVITY:
        if ((defAbility == ABILITY_LEVITATE || (gStatuses3[battlerDef] & STATUS3_MAGNET_RISE) || IS_BATTLER_OF_TYPE(battlerDef, TYPE_FLYING)) && Roll(192))
            s += 1;
        if (hp >= 60 && Roll(96))
            s += 1;
        break;
    case EFFECT_TAILWIND:
        if (Roll(64))
            break;
        if (faster)
            s -= 1;
        else if (hp <= 30)
            s -= 1;
        else if (hp > 75)
            s += 1;
        else if (Roll(192))
            s += 1;
        break;
    case EFFECT_BELLY_DRUM:
        if (hp < 90)
            s -= 2;
        break;
    case EFFECT_PSYCH_UP:
        if (Stage(battlerDef, STAT_ATK) >= 3 || Stage(battlerDef, STAT_DEF) >= 3 || Stage(battlerDef, STAT_SPATK) >= 3
         || Stage(battlerDef, STAT_SPDEF) >= 3 || Stage(battlerDef, STAT_EVASION) >= 3)
        {
            if (Stage(battlerAtk, STAT_EVASION) <= 0)
                s += 2;
            else if (Stage(battlerAtk, STAT_ATK) <= 0 || Stage(battlerAtk, STAT_DEF) <= 0 || Stage(battlerAtk, STAT_SPATK) <= 0 || Stage(battlerAtk, STAT_SPDEF) <= 0)
                s += 1;
            else if (Roll(206))
                s -= 2;
        }
        else
            s -= 2;
        break;
    case EFFECT_FACADE:
        if (gBattleMons[battlerDef].status1 & (STATUS1_BURN | STATUS1_PSN_ANY | STATUS1_PARALYSIS)) // sic: checks the target
            s += 1;
        break;
    case EFFECT_FOCUS_PUNCH:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (gBattleMons[battlerAtk].status2 & STATUS2_SUBSTITUTE)
            s += 5;
        if (IsAsleep(battlerDef))
            s += 1;
        if (IsConfusedOrInfatuated(battlerDef) && Roll(156))
            s += 1;
        if (!gDisableStructs[battlerAtk].isFirstTurn && Roll(56))
            s += 1;
        break;
    case EFFECT_DOUBLE_POWER_ON_ARG_STATUS: // Smelling Salts, Wake-Up Slap
        if (gMovesInfo[move].argument & STATUS1_PARALYSIS)
        {
            if (gBattleMons[battlerDef].status1 & STATUS1_PARALYSIS)
                s += 2;
        }
        else
        {
            if (ResistsOrImmune(battlerAtk, battlerDef))
                s -= 1;
            if (IsAsleep(battlerDef))
                s += 1;
        }
        break;
    case EFFECT_TRICK:
    {
        bool32 badTrade = IsBadTradeItem(defHold);
        if (IsDisruptiveItem(atkHold))
            s += badTrade ? -3 : 5;
        else if (atkHold == HOLD_EFFECT_TOXIC_ORB)
        {
            if (badTrade)
                s -= 3;
            else if (Gen4CanBePoisoned(battlerAtk, battlerDef))
                s += 5;
            else if (HasStatus(battlerAtk) || HasSafeguard(battlerAtk) || IS_BATTLER_OF_TYPE(battlerAtk, TYPE_STEEL) || IS_BATTLER_OF_TYPE(battlerAtk, TYPE_POISON)
                  || atkAbility == ABILITY_IMMUNITY || atkAbility == ABILITY_MAGIC_GUARD || atkAbility == ABILITY_POISON_HEAL || atkAbility == ABILITY_KLUTZ)
                s -= 3;
            else
                s += 5;
        }
        else if (atkHold == HOLD_EFFECT_FLAME_ORB)
        {
            if (badTrade)
                s -= 3;
            else if (Gen4CanBeBurned(battlerDef))
                s += 5;
            else if (HasStatus(battlerAtk) || HasSafeguard(battlerAtk) || IS_BATTLER_OF_TYPE(battlerAtk, TYPE_FIRE)
                  || atkAbility == ABILITY_WATER_VEIL || atkAbility == ABILITY_MAGIC_GUARD || atkAbility == ABILITY_KLUTZ)
                s -= 3;
            else
                s += 5;
        }
        else if (atkHold == HOLD_EFFECT_BLACK_SLUDGE)
        {
            if (badTrade)
                s -= 3;
            else if (!IS_BATTLER_OF_TYPE(battlerDef, TYPE_POISON) && defAbility != ABILITY_MAGIC_GUARD)
                s += 5;
            else if (IS_BATTLER_OF_TYPE(battlerAtk, TYPE_POISON) || atkAbility == ABILITY_MAGIC_GUARD || atkAbility == ABILITY_KLUTZ)
                s -= 3;
            else
                s += 5;
        }
        else if (atkHold == HOLD_EFFECT_CONFUSE_SPICY || atkHold == HOLD_EFFECT_CONFUSE_DRY || atkHold == HOLD_EFFECT_CONFUSE_SWEET
              || atkHold == HOLD_EFFECT_CONFUSE_BITTER || atkHold == HOLD_EFFECT_CONFUSE_SOUR)
        {
            if (badTrade || (defHold >= HOLD_EFFECT_CONFUSE_SPICY && defHold <= HOLD_EFFECT_CONFUSE_SOUR))
                s -= 3;
            else if (Roll(206))
                s += 2;
        }
        break;
    }
    case EFFECT_ROLE_PLAY:
    case EFFECT_SKILL_SWAP:
        if (IsDesirableAbility(atkAbility))
            s -= 1;
        else if (IsDesirableAbility(defAbility) && Roll(206))
            s += 2;
        break;
    case EFFECT_POWER_BASED_ON_USER_HP: // Water Spout, Eruption
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (slower && defHp <= 70) // sic: checks the target's HP
            s -= 1;
        if (faster && defHp <= 50)
            s -= 1;
        break;
    case EFFECT_MAGIC_COAT:
        if (defHp <= 30 && Roll(156))
            s -= 1;
        if (gDisableStructs[battlerAtk].isFirstTurn)
            s += Roll(106) ? 1 : 0;
        else
            s -= Roll(226) ? 1 : 0;
        break;
    case EFFECT_RECYCLE:
    {
        u32 item = GetUsedHeldItem(battlerAtk);
        if (item != ITEM_CHESTO_BERRY && item != ITEM_LUM_BERRY && item != ITEM_STARF_BERRY)
            s -= 2;
        else if (Roll(206))
            s += 1;
        break;
    }
    case EFFECT_REVENGE:
        if (IsAsleepConfusedOrInfatuated(battlerDef))
            s -= 2;
        else
            s += Roll(180) ? -2 : 2;
        break;
    case EFFECT_BRICK_BREAK:
        if (gSideStatuses[GetBattlerSide(battlerDef)] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN))
            s += 1;
        break;
    case EFFECT_KNOCK_OFF:
        if (defHp >= 30 && !gDisableStructs[battlerAtk].isFirstTurn && Roll(76))
            s += 1;
        break;
    case EFFECT_ENDEAVOR:
        if (defHp < 70)
            return score + s - 1;
        if (slower)
            s += (hp > 50) ? -1 : 1;
        else
            s += (hp > 40) ? -1 : 1;
        break;
    case EFFECT_IMPRISON:
        if (!gDisableStructs[battlerAtk].isFirstTurn && Roll(156))
            s += 2;
        break;
    case EFFECT_REFRESH:
        if (defHp < 50)
            s -= 1;
        break;
    case EFFECT_SNATCH:
        if (gDisableStructs[battlerAtk].isFirstTurn && Roll(106))
            s += 2;
        if (Roll(30))
            break;
        if (slower)
        {
            if (defHp > 25 && Roll(226))
                s -= 2;
            else if (KnowsFlatRecoveryOrDefenseCurl(battlerDef) && Roll(106))
                s += 2;
            else if (Roll(26))
                s += 1;
        }
        else
        {
            if (gBattleMons[battlerAtk].hp < gBattleMons[battlerAtk].maxHP && Roll(226))
                s -= 2;
            else if (defHp < 70 && Roll(226))
                s -= 2;
            else if (Roll(173))
                s -= 2;
        }
        break;
    case EFFECT_MUD_SPORT:
    case EFFECT_WATER_SPORT:
        if (hp < 50)
            s -= 1;
    {
        u32 sportType = (effect == EFFECT_MUD_SPORT) ? TYPE_ELECTRIC : TYPE_FIRE;
        if (IS_BATTLER_OF_TYPE(battlerDef, sportType))
            s += 1;
        break;
    }
    case EFFECT_BRINE:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (defHp <= 50)
            s += Roll(128) ? 1 : 2;
        break;
    case EFFECT_HIT_ESCAPE: // U-turn
        if (ResistsOrImmune(battlerAtk, battlerDef))
            return score + s - 1;
        if (IsLastMon(battlerAtk))
            return score + s + 2;
        if (HasSuperEffectiveMove(battlerAtk, battlerDef) && Roll(192))
            s -= 2;
        if (!BenchOutdamages(battlerAtk, battlerDef) && Roll(192))
            return score + s - 2;
        if (defHp > 70 && Roll(192))
            s += 1;
        if (defHp > 30)
        {
            if (Roll(128))
                s += 1;
        }
        else if (Roll(64))
            s += 1;
        if (faster || Roll(128))
            s += 1;
        break;
    case EFFECT_PAYBACK:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (slower && hp >= 30 && Roll(192))
            s += 1;
        break;
    case EFFECT_ASSURANCE:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (slower)
        {
            if (atkAbility == ABILITY_ROUGH_SKIN)
                s += Roll(128) ? 1 : 0;
            else if (atkHold == HOLD_EFFECT_JABOCA_BERRY || atkHold == HOLD_EFFECT_ROWAP_BERRY)
                s += Roll(128) ? 1 : 0;
            else
                s += Roll(64) ? 1 : 0;
        }
        break;
    case EFFECT_EMBARGO:
        if (Roll(128))
            s += 1;
        break;
    case EFFECT_FLING:
    {
        u32 power = ItemId_GetFlingPower(gBattleMons[battlerAtk].item);
        if (ResistsOrImmune(battlerAtk, battlerDef) && atkHold != HOLD_EFFECT_FLINCH && atkHold != HOLD_EFFECT_POISON_POWER
         && atkHold != HOLD_EFFECT_TOXIC_ORB && atkHold != HOLD_EFFECT_FLAME_ORB && atkHold != HOLD_EFFECT_LIGHT_BALL)
            s -= 1;
        if (power < 30)
            s -= 2;
        else if (power > 90)
        {
            if (Roll(192))
                s += 1;
            if (CurrentEffectiveness(battlerAtk, battlerDef) >= AI_EFFECTIVENESS_x2)
                s += 4;
            else if (Roll(128))
                s += 1;
        }
        else if (power > 60)
            s += Roll(192) ? 1 : 0;
        else
            s -= Roll(128) ? 1 : 0;
        break;
    }
    case EFFECT_PSYCHO_SHIFT:
        if (!HasStatus(battlerAtk))
            return score + s - 10;
        if (defHp >= 30 && Roll(128))
            s += 1;
        break;
    case EFFECT_TRUMP_CARD:
    {
        u32 pp = gBattleMons[battlerAtk].pp[AI_THINKING_STRUCT->movesetIndex];
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (pp == 1)
            s += 3;
        else if (pp == 2)
            s += Roll(156) ? 2 : 1;
        else if (pp == 3)
            s += Roll(156) ? 1 : 0;
        if (defAbility == ABILITY_PRESSURE && Roll(226))
            s += 1;
        if (Stage(battlerDef, STAT_EVASION) >= 5 || Stage(battlerAtk, STAT_ACC) <= -5)
            s += Roll(156) ? 2 : 1;
        else if ((Stage(battlerDef, STAT_EVASION) >= 3 || Stage(battlerAtk, STAT_ACC) <= -3) && Roll(156))
            s += 1;
        break;
    }
    case EFFECT_HEAL_BLOCK:
        if ((KnowsEffect(battlerDef, EFFECT_DREAM_EATER) || KnowsEffect(battlerDef, EFFECT_RESTORE_HP) || KnowsEffect(battlerDef, EFFECT_ROOST)
          || KnowsEffect(battlerDef, EFFECT_MORNING_SUN) || KnowsEffect(battlerDef, EFFECT_SYNTHESIS) || KnowsEffect(battlerDef, EFFECT_MOONLIGHT)
          || KnowsEffect(battlerDef, EFFECT_REST) || KnowsEffect(battlerDef, EFFECT_SWALLOW) || KnowsEffect(battlerDef, EFFECT_ABSORB)
          || KnowsEffect(battlerDef, EFFECT_INGRAIN) || KnowsEffect(battlerDef, EFFECT_AQUA_RING) || KnowsEffect(battlerDef, EFFECT_LEECH_SEED)
          || KnowsEffect(battlerDef, EFFECT_HEALING_WISH)) && Roll(231))
            s += 1;
        else if ((IsSeeded(battlerAtk) || IsRootedOrAquaRing(battlerDef)) && Roll(231))
            s += 1;
        else if (Roll(144))
            s += 1;
        break;
    case EFFECT_POWER_BASED_ON_TARGET_HP: // Crush Grip / Wring Out
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (defHp < 50)
            s -= 1;
        if (gBattleMons[battlerDef].hp >= gBattleMons[battlerDef].maxHP)
        {
            if (Roll(231))
                s += 1;
            s += faster ? 2 : (slower ? 1 : 0);
        }
        else if (defHp > 85 && Roll(231))
            s += 1;
        break;
    case EFFECT_POWER_TRICK:
        if (hp > 90)
            s += Roll(160) ? 1 : 0;
        else if (hp > 60)
            s += Roll(128) ? 1 : 0;
        else if (hp > 30)
            s += Roll(92) ? 1 : 0;
        break;
    case EFFECT_GASTRO_ACID:
        if (Roll(64))
            break;
        if (defHp > 70)
            s += 1;
        else if (defHp > 50)
            s -= Roll(128) ? 0 : 1;
        else if (defHp > 30)
            s -= Roll(128) ? 1 : 2;
        else
            s -= Roll(128) ? 2 : 3;
        break;
    case EFFECT_LUCKY_CHANT:
    {
        u32 i;
        bool32 highCrit = FALSE;
        if (hp < 70)
            s -= 1;
        for (i = 0; i < MAX_MON_MOVES; i++)
            if (gBattleMons[battlerDef].moves[i] != MOVE_NONE && gMovesInfo[gBattleMons[battlerDef].moves[i]].criticalHitStage > 0)
                highCrit = TRUE;
        if (highCrit)
            s += 1;
        else if (Roll(64))
            s += 1;
        break;
    }
    case EFFECT_ME_FIRST:
        if (slower)
            s -= 2;
        if (AttackerOutdamages(battlerAtk, battlerDef) && Roll(224))
            s += 1;
        if (LastMoveWasDamaging(battlerDef) && Roll(128))
            s += 1;
        if (Roll(192))
            s += 1;
        break;
    case EFFECT_COPYCAT:
        if (slower && !AttackerOutdamages(battlerAtk, battlerDef) && !IsMirrorMoveEncouraged(LastMoveOf(battlerDef)) && Roll(176))
            s -= 1;
        else if (faster)
        {
            if (AttackerOutdamages(battlerAtk, battlerDef) && Roll(224))
                s += 2;
            else if (IsMirrorMoveEncouraged(LastMoveOf(battlerDef)) && Roll(128))
                s += 2;
        }
        break;
    case EFFECT_POWER_SWAP:
        s += SwapScore(Stage(battlerDef, STAT_ATK) - Stage(battlerAtk, STAT_ATK), Stage(battlerDef, STAT_SPATK) - Stage(battlerAtk, STAT_SPATK));
        break;
    case EFFECT_GUARD_SWAP:
        s += SwapScore(Stage(battlerDef, STAT_DEF) - Stage(battlerAtk, STAT_DEF), Stage(battlerDef, STAT_SPDEF) - Stage(battlerAtk, STAT_SPDEF));
        break;
    case EFFECT_PUNISHMENT:
    {
        u32 i;
        s32 total = 0;
        if (ResistsOrImmune(battlerAtk, battlerDef))
            break;
        for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
            if (Stage(battlerDef, i) > 0)
                total += Stage(battlerDef, i);
        if (total > 6)       s += TieredRoll(4);
        else if (total == 6) s += TieredRoll(3);
        else if (total == 5) s += TieredRoll(2);
        else if (total > 2)  s += TieredRoll(1);
        break;
    }
    case EFFECT_LAST_RESORT:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (CanUseLastResort(battlerAtk))
            s += 1;
        break;
    case EFFECT_WORRY_SEED:
        if (KnowsEffect(battlerDef, EFFECT_REST))
            s += 1;
        if (hp >= 50 && Roll(128))
            s += 1;
        if (Roll(192))
            s += 1;
        break;
    case EFFECT_SUCKER_PUNCH:
        if (ResistsOrImmune(battlerAtk, battlerDef))
            s -= 1;
        if (Roll(192))
            s += 1;
        break;
    case EFFECT_HEART_SWAP:
        if (!(Stage(battlerDef, STAT_ATK) >= 2 || Stage(battlerDef, STAT_DEF) >= 2 || Stage(battlerDef, STAT_SPATK) >= 2
           || Stage(battlerDef, STAT_SPDEF) >= 2 || Stage(battlerDef, STAT_EVASION) >= 2)
         && !(gBattleMons[battlerDef].status2 & STATUS2_FOCUS_ENERGY_ANY))
            return score + s - 2;
        if (Stage(battlerAtk, STAT_ATK) <= 0 || Stage(battlerAtk, STAT_DEF) <= 0 || Stage(battlerAtk, STAT_SPATK) <= 0
         || Stage(battlerAtk, STAT_SPDEF) <= 0 || !(gBattleMons[battlerAtk].status2 & STATUS2_FOCUS_ENERGY_ANY))
            s += 1;
        else if (Stage(battlerAtk, STAT_EVASION) <= 0)
            s += 2;
        else if (Roll(206))
            s -= 2;
        break;
    case EFFECT_AQUA_RING:
        if (hp >= 30 && Roll(128))
            s += 1;
        break;
    case EFFECT_MAGNET_RISE:
        if (hp < 50)
            break;
        if (Knows(battlerDef, MOVE_EARTHQUAKE) || Knows(battlerDef, MOVE_EARTH_POWER) || Knows(battlerDef, MOVE_FISSURE))
            s += 1;
        if (IS_BATTLER_OF_TYPE(battlerDef, TYPE_GROUND))
            s += 1;
        else if (Roll(128))
            s += 1;
        break;
    case EFFECT_DEFOG:
    {
        u32 defSide = GetBattlerSide(battlerDef);
        bool32 hazards = (gSideStatuses[defSide] & (SIDE_STATUS_SPIKES | SIDE_STATUS_STEALTH_ROCK | SIDE_STATUS_TOXIC_SPIKES)) != 0;
        if (gSideStatuses[defSide] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN))
        {
            if (hp < 30 && IsLastMon(battlerAtk))
            {
                if (Roll(206))
                    s -= 2;
                if (defHp > 70)
                    s -= 2;
            }
            s += 1;
            if (!IsLastMon(battlerDef) && hazards && Roll(128))
                s -= 1;
        }
        else if (hazards)
            s -= 2;
        if (hp >= 70 && Stage(battlerDef, STAT_EVASION) >= -2 && defHp <= 70)
            s -= 2;
        else
        {
            if (Roll(206))
                s -= 2;
            if (defHp <= 70)
                s -= 2;
        }
        break;
    }
    case EFFECT_TRICK_ROOM:
        if (IsDoubleBattle())
            break;
        if (hp <= 30 && IsLastMon(battlerAtk))
            break;
        if (faster)
            s -= 1;
        else if (slower && Roll(192))
            s += 3;
        break;
    case EFFECT_BLIZZARD:
        if (ResistsOrImmune(battlerAtk, battlerDef) && Roll(206))
            s -= 3;
        if (WeatherIs(B_WEATHER_HAIL | B_WEATHER_SNOW))
            s += 1;
        break;
    case EFFECT_CAPTIVATE:
        if (Stage(battlerDef, STAT_SPATK) != 0)
        {
            s -= 1;
            if (hp <= 90)
                s -= 1;
            if (Stage(battlerDef, STAT_SPATK) <= -3 && Roll(206))
                s -= 2;
        }
        if (defHp <= 70)
            s -= 2;
        if (LastMoveWasDamaging(battlerDef) && IS_MOVE_PHYSICAL(LastMoveOf(battlerDef)) && Roll(192))
            s -= 1;
        break;
    }

    // stat-changing status moves
    if (IS_MOVE_STATUS(move) && effect != EFFECT_BULK_UP && effect != EFFECT_CALM_MIND && effect != EFFECT_COSMIC_POWER)
    {
        stat = BoostedStat(effect);
        if (stat != NUM_BATTLE_STATS)
            s += ExpertStatBoost(battlerAtk, battlerDef, stat);
        stat = ReducedStat(effect);
        if (stat != NUM_BATTLE_STATS)
            s += ExpertStatReduce(battlerAtk, battlerDef, stat);
    }

    return score + s;
}
