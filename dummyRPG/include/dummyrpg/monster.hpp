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
    sprite_id spriteSheetId = 0;
    Curve attacks;
    Curve defense;
    Curve hp;
};

///////////////////////////////////////////////////////////////////////////////

struct MonsterInstance
{
    monster_id monsterId = 0;
    uint8_t level        = 0;
    uint8_t curMissingHp = 0;
};

} // namespace Dummy

#endif // DUMMYRPG_MONSTER_HPP
