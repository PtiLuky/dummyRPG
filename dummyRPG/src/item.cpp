#include "dummyrpg/item.hpp"

namespace Dummy {

Item::Item(item_id id, const std::string&& name, sprite_id spriteId)
    : m_id(id)
    , m_name(name)
    , m_spriteId(spriteId)
{}

item_id Item::id() const
{
    return m_id;
}

sprite_id Item::spriteId() const
{
    return m_spriteId;
}

const std::string& Item::name() const
{
    return m_name;
}

void Item::setSprite(sprite_id id)
{
    m_id = id;
}

} // namespace Dummy
