#include "dummyrpg/monster.hpp"

namespace Dummy {

Monster::Monster(const std::string&& name, sprite_id spriteId)
    : m_name(name)
    , m_spriteId(spriteId)
{}

///////////////////////////////////////////////////////////////////////////////

MonsterInstance::MonsterInstance() {}

} // namespace Dummy
