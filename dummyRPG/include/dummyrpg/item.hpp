#ifndef DUMMYRPG_ITEM_HPP
#define DUMMYRPG_ITEM_HPP

#include "dummyrpg/dummy_types.hpp"
#include "dummyrpg/sprite.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

struct Item
{
    std::string name;
    sprite_id spriteSheetId;
};

} // namespace Dummy

#endif // DUMMYRPG_ITEM_HPP
