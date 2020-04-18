#ifndef DUMMYRPG_ITEM_HPP
#define DUMMYRPG_ITEM_HPP

#include "dummyrpg/dummy_types.hpp"
#include "dummyrpg/sprite.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

class Item
{
    friend class Serializer;

public:
    Item(const std::string&& m_name, sprite_id spriteId);

private:
    std::string m_name;
    sprite_id m_spriteId = 0;
};

} // namespace Dummy

#endif // DUMMYRPG_ITEM_HPP
