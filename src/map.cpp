#include "dummyrpg/map.hpp"

#include "dummyrpg/floor.hpp"
#include "utils.hpp"

namespace Dummy {
Map::Map(uint16_t w, uint16_t h, chip_id firstChip)
    : m_chipsets({firstChip})
{
    m_floors.push_back(std::make_unique<Floor>(w, h));
}

Map::~Map() {}

uint16_t Map::width() const
{
    return m_floors[0]->width();
}

uint16_t Map::height() const
{
    return m_floors[0]->height();
}

bool Map::registerChipset(chip_id idx)
{
    if (m_chipsets.size() >= MAX_CHIPS_COUNT)
        return false;

    if (vec_contains(m_chipsets, idx))
        return false;

    m_chipsets.push_back(idx);
    return true;
}

bool Map::unregisterChipset(chip_id idx)
{
    if (m_chipsets.size() <= 1) // Cannot delete last one
        return false;

    auto chipIte = std::find(m_chipsets.begin(), m_chipsets.end(), idx);
    if (chipIte == m_chipsets.end())
        return false;

    m_chipsets.erase(chipIte);
    for (auto& pFloor : m_floors)
        pFloor->removeChipRef(idx);

    return true;
}

} // namespace Dummy
