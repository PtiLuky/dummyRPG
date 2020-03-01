#ifndef DUMMYRPG_MAP
#define DUMMYRPG_MAP

#include <memory>
#include <vector>

#include "dummyrpg/dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {
class Floor;

///////////////////////////////////////////////////////////////////////////////

///
/// \brief A map is a graphically coherent part of a "level".
/// The map will contains one or several floor(s)
///
/// Some limitations:
///  - 255 (MAX_FLOORS_COUNT) floors max (1 min). You can only create a new floor
///     above any other floor, and you can delete any floor (except if there is only 1)
///  - 4 (MAX_CHIPS_COUNT) graphical chipsets max (1 min). Use (un)registerChipset
///     to add/remove a chipset from the list of used chipsets of this map
///
class Map
{
public:
    Map(uint16_t w, uint16_t h, chip_id firstChip);
    Map(const Map&) = delete;
    ~Map();

    /////////////////////////////////////
    // const Getters
    const std::vector<std::unique_ptr<Floor>>& floors() const { return m_floors; }
    Floor* floorAt(uint8_t floorIdx) const { return m_floors[floorIdx].get(); }
    const std::vector<chip_id>& chipsetsUsed() const { return m_chipsets; }
    uint16_t width() const;
    uint16_t height() const;

    /////////////////////////////////////
    // Setters
    /// \brief addFloor Adds a floor ontop of all others. May return
    /// \return ptr to the created floor or nullptr if max has been reached
    Floor* addFloor();
    bool removeFloorAt(uint8_t);     ///< returns true if removed
    bool registerChipset(chip_id);   ///< add chipset to list of chipsets to use
    bool unregisterChipset(chip_id); ///< unregister chip and delete all tiles from this chip

    /////////////////////////////////////
    // Consts
    static const uint8_t MAX_FLOORS_COUNT = 255; ///< total floors at most
    static const uint8_t MAX_CHIPS_COUNT  = 4;   ///< total chipsets at most

private:
    std::vector<std::unique_ptr<Floor>> m_floors; // Always contains at least 1 Floor
    std::vector<chip_id> m_chipsets;              // Always contains at least 1 chipset
};

} // namespace Dummy

#endif // DUMMYRPG_MAP
