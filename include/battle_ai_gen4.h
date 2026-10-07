#ifndef GUARD_BATTLE_AI_GEN4_H
#define GUARD_BATTLE_AI_GEN4_H

// BOFA: Generation 4 (Platinum Kaizo) trainer AI flags. See src/battle_ai_gen4.c.
s32 AI_Gen4Basic(u32 battlerAtk, u32 battlerDef, u32 move, s32 score);
s32 AI_Gen4EvaluateAttack(u32 battlerAtk, u32 battlerDef, u32 move, s32 score);
s32 AI_Gen4Expert(u32 battlerAtk, u32 battlerDef, u32 move, s32 score);
s32 AI_Gen4Risky(u32 battlerAtk, u32 battlerDef, u32 move, s32 score);

#endif // GUARD_BATTLE_AI_GEN4_H
