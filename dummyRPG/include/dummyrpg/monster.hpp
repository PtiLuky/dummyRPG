#ifndef DUMMYRPG_MONSTER_HPP
#define DUMMYRPG_MONSTER_HPP

#include "dummyrpg/dummy_types.hpp"

#include <string>

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

class Monster
{
    friend class Serializer;

public:
    Monster(const std::string&& name, sprite_id spriteId);

private:
    std::string m_name;
    sprite_id m_spriteId = 0;
    Curve m_attacks;
    Curve m_defense;
    Curve m_hp;
};

///////////////////////////////////////////////////////////////////////////////

class MonsterInstance
{
    friend class GameInstance;

public:
    MonsterInstance();

private:
    monster_id monsterId = 0;
    uint8_t level        = 0;
    uint8_t curMissingHp = 0;
};

} // namespace Dummy

#endif // DUMMYRPG_MONSTER_HPP
