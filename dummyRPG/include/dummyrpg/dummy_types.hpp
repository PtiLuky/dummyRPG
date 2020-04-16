#ifndef DUMMYRPG_DUMMYTYPES_HPP
#define DUMMYRPG_DUMMYTYPES_HPP

#include <cstdint>

namespace Dummy {
///////////////////////////////////////////////////////////////////////////////
/// Types definitions

/// The id of a chipset. A game has 255 chipsets max.
using chip_id = uint8_t;
/// The id of a sprite. A game has 65'535 sprite max.
using sprite_id = uint16_t;
/// The id of an item. A game has 65'535 items max.
using item_id = uint16_t;
/// The id of a monster. A game has 65'535 monsters max.
using monster_id = uint16_t;
/// The id of an character. A game has 4'294'967'295 charaters max.
using char_id = uint32_t;
/// The id of an event. A game has 4'294'967'295 events max.
using event_id = uint32_t;


/// tilecoords represents coordinates of a tile. As a pair of <x, y>
struct xy
{
    uint16_t x;
    uint16_t y;
};

/// tilecoords represents coordinates of a chipset tile. As a tuple of <x, y, chipset_id>
struct Tileaspect
{
    uint8_t x;
    uint8_t y;
    chip_id chipId;
};

/// Attack curve, def curve, etc. This is a simplification with a 4-segments "curve"
struct Curve
{
    uint8_t p0 = 0;
    uint8_t p1 = 0;
    uint8_t p2 = 0;
    uint8_t p3 = 0;
    uint8_t p4 = 0;
};

enum class Direction
{
    Top = 0,
    Right,
    Bottom,
    Left
};

///////////////////////////////////////////////////////////////////////////////
/// CONST definitions

/// Default tile aspect value. Will always be considered as undefined.
const Tileaspect undefAspect {static_cast<uint8_t>(-1), static_cast<uint8_t>(-1),
                              static_cast<chip_id>(0)};
/// The size in px of a tile (on the chipset and on the map)
const int TILE_SIZE = 16;

} // namespace Dummy

#endif // DUMMYRPG_DUMMYTYPES_HPP
