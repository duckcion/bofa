// BOFA: per-map land encounter rates. Maps listed here use these percentages (one per slot, summing to 100)
// instead of the standard 20/20/10/10/10/10/5/5/4/4/1/1. Slots with rate 0 are never picked.
// tools/docgen reads this file too, so keep one entry per line in this exact format.

struct CustomLandRates
{
    u16 map;
    u8 rates[LAND_WILD_COUNT];
};

static const struct CustomLandRates sCustomLandRates[] =
{
    { MAP_PETALBURG_WOODS,           { 20, 17, 15, 12, 10, 8, 6, 5, 4, 3, 0, 0 } },
    { MAP_RUSTBORO_CITY,             { 20, 20, 15, 15, 10, 8, 5, 3, 2, 2, 0, 0 } },
    { MAP_LOST_CAVE,                 { 15, 15, 13, 12, 12, 9, 7, 6, 6, 5, 0, 0 } },
    { MAP_ROUTE116,                  { 18, 16, 14, 12, 12, 10, 8, 5, 5, 0, 0, 0 } },
    { MAP_GRANITE_CAVE_1F,           { 20, 20, 14, 12, 10, 8, 6, 5, 3, 2, 0, 0 } },
    { MAP_GRANITE_CAVE_B1F,          { 20, 20, 14, 12, 10, 8, 6, 5, 3, 2, 0, 0 } },
    { MAP_GRANITE_CAVE_B2F,          { 20, 20, 14, 12, 10, 8, 6, 5, 3, 2, 0, 0 } },
    { MAP_GRANITE_CAVE_STEVENS_ROOM, { 18, 15, 13, 12, 11, 9, 6, 6, 4, 3, 2, 1 } },
    { MAP_RUSTURF_TUNNEL,             { 20, 20, 12, 10, 10, 8, 7, 5, 5, 3, 0, 0 } },
    { MAP_WRAITHWOOD_FOREST,          { 17, 15, 14, 12, 10, 9, 8, 7, 6, 2, 0, 0 } },
    { MAP_HOLLOWBROOK,                { 14, 14, 14, 12, 11, 10, 9, 8, 5, 3, 0, 0 } },
    { MAP_DEWFORD_TOWN,               { 18, 15, 15, 12, 10, 8, 7, 6, 5, 4, 0, 0 } },
};
