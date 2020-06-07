#include "dummyrpg/monster.hpp"

namespace Dummy {

Monster::Monster(monster_id id, const std::string&& name, sprite_id spriteId)
    : m_id(id)
    , m_name(name)
    , m_spriteId(spriteId)
{}

monster_id Monster::id() const
{
    return m_id;
}

const std::string& Monster::name() const
{
    return m_name;
}

sprite_id Monster::spriteId() const
{
    return m_spriteId;
}

void Monster::setSprite(sprite_id id)
{
    m_spriteId = id;
}


///////////////////////////////////////////////////////////////////////////////

MonsterInstance::MonsterInstance() {}

} // namespace Dummy
