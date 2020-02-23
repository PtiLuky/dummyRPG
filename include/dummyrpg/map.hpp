#pragma once

#include <memory>
#include <vector>

#include "dummyrpg/dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {
class Floor;

///////////////////////////////////////////////////////////////////////////////

class Map
{
public:
    Map(uint16_t w, uint16_t h, chip_id firstChip);
    ~Map();

    // const Getters
    const std::vector<std::unique_ptr<Floor>>& floors() const { return m_floors; }
    const std::vector<chip_id>& chipsetsUsed() const { return m_chipsets; }
    uint16_t width() const;
    uint16_t height() const;

    bool registerChipset(chip_id);   ///< add chipset to list of chipsets to use
    bool unregisterChipset(chip_id); ///< unregister chip and delete all tiles from this chip

    // Consts
    const uint8_t MAX_FLOORS_COUNT = 255; ///< total floors at most
    const uint8_t MAX_CHIPS_COUNT  = 8;   ///< total chipsets at most

private:
    std::vector<std::unique_ptr<Floor>> m_floors; // Always contains at least 1 Floor
    std::vector<chip_id> m_chipsets;              // Always contains at least 1 chipset
};

} // namespace Dummy
