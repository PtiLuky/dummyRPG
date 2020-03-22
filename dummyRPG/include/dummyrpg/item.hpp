#ifndef DUMMYRPG_ITEM_HPP
#define DUMMYRPG_ITEM_HPP

#include "dummyrpg/dummy_types.hpp"
#include "dummyrpg/sprite.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

struct Item
{
    sprite_id spriteSheetId;
    std::string name;
};

} // namespace Dummy

#endif // DUMMYRPG_ITEM_HPP
