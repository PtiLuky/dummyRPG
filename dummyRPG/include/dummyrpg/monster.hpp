#ifndef DUMMYRPG_MONSTER_HPP
#define DUMMYRPG_MONSTER_HPP

#include "dummyrpg/dummy_types.hpp"

#include <string>

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct Monster
{
    std::string name;
    sprite_id spriteSheetId;
    Curve attacks;
    Curve defense;
    Curve hp;
};

///////////////////////////////////////////////////////////////////////////////

struct MonsterInstance
{
    monster_id monsterId;
    uint8_t level;
    uint8_t curMissingHp;
};

} // namespace Dummy

#endif // DUMMYRPG_MONSTER_HPP
