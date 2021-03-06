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
    Item(item_id, const std::string&& m_name, sprite_id spriteId);

    item_id id() const;
    sprite_id spriteId() const;
    const std::string& name() const;

    void setSprite(sprite_id);

private:
    item_id m_id;
    std::string m_name;
    sprite_id m_spriteId = 0;
};

} // namespace Dummy

#endif // DUMMYRPG_ITEM_HPP
