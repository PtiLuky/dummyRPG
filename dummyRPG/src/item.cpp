#include "dummyrpg/item.hpp"

namespace Dummy {

Item::Item(item_id id, const std::string&& name, sprite_id spriteId)
    : m_id(id)
    , m_name(name)
    , m_spriteId(spriteId)
{}

} // namespace Dummy
