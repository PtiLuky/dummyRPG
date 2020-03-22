#ifndef DUMMYRPG_CHARACTER_HPP
#define DUMMYRPG_CHARACTER_HPP

#include "dummyrpg/dummy_types.hpp"

#include <string>

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

enum class Direction
{
    Top = 0,
    Right,
    Bottom,
    Left
};

///////////////////////////////////////////////////////////////////////////////

struct Character
{
    Tilecoords pos;
    Direction dir;
    sprite_id spriteSheetId;
};

} // namespace Dummy

#endif // DUMMYRPG_CHARACTER_HPP
