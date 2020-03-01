#ifndef DUMMYRPG_DUMMYTYPES_HPP
#define DUMMYRPG_DUMMYTYPES_HPP

#include <cstdint>
#include <tuple>

using chip_id    = uint8_t;
using tilecoords = std::pair<uint16_t, uint16_t>;         // x, y
using tileaspect = std::tuple<uint8_t, uint8_t, chip_id>; // x, y, chipset_id

const tileaspect undefAspect {static_cast<uint8_t>(-1), static_cast<uint8_t>(-1), static_cast<chip_id>(0)};

#endif // DUMMYRPG_DUMMYTYPES_HPP
