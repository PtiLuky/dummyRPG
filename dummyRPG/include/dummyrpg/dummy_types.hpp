#ifndef DUMMYRPG_DUMMYTYPES_HPP
#define DUMMYRPG_DUMMYTYPES_HPP

#include <cstdint>
#include <tuple>

///////////////////////////////////////////////////////////////////////////////
/// Types definitions

/// The id of a chipset. A game has 255 chipsets max.
using chip_id    = uint8_t;
/// tilecoords represents coordinates of a tile. As a pair of <x, y>
using tilecoords = std::pair<uint16_t, uint16_t>;         // x, y
/// tilecoords represents coordinates of a chipset tile. As a tuple of <x, y, chipset_id>
using tileaspect = std::tuple<uint8_t, uint8_t, chip_id>; // x, y, chipset_id


///////////////////////////////////////////////////////////////////////////////
/// CONST definitions

/// Default tile aspect value. Will always be considered as undefined.
const tileaspect undefAspect {static_cast<uint8_t>(-1), static_cast<uint8_t>(-1),
                              static_cast<chip_id>(0)};
/// The size in px of a tile (on the chipset and on the map)
const int TILE_SIZE = 16;

#endif // DUMMYRPG_DUMMYTYPES_HPP
