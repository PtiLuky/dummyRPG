#include "dummyrpg/item.hpp"

namespace Dummy {

Item::Item(const std::string&& name, sprite_id spriteId)
    : m_name(name)
    , m_spriteId(spriteId)
{}

} // namespace Dummy
