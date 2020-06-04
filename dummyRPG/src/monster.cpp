#include "dummyrpg/monster.hpp"

namespace Dummy {

Monster::Monster(monster_id id, const std::string&& name, sprite_id spriteId)
    : m_id(id)
    , m_name(name)
    , m_spriteId(spriteId)
{}

///////////////////////////////////////////////////////////////////////////////

MonsterInstance::MonsterInstance() {}

} // namespace Dummy
