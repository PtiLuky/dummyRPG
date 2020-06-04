#include "dummyrpg/map.hpp"

#include "dummyrpg/floor.hpp"
#include "utils.hpp"

namespace Dummy {
Map::Map() {}
Map::Map(uint16_t w, uint16_t h, chip_id firstChip)
    : m_chipsets({firstChip})
{
    m_floors.push_back(std::make_unique<Floor>(w, h));
}

Map::~Map() {}

void Map::reset(uint16_t w, uint16_t h, chip_id firstChip)
{
    m_chipsets = {firstChip};
    m_floors.clear();
    m_floors.push_back(std::make_unique<Floor>(w, h));
}

void Map::resize(uint16_t w, uint16_t h)
{
    for (const auto& floor : m_floors)
        floor->resize(w, h);
}

const std::vector<std::unique_ptr<Floor>>& Map::floors() const
{
    return m_floors;
}

Floor* Map::floorAt(uint8_t floorIdx) const
{
    if (floorIdx >= m_floors.size())
        return nullptr;
    return m_floors[floorIdx].get();
}

const std::vector<chip_id>& Map::chipsetsUsed() const
{
    return m_chipsets;
}

uint16_t Map::width() const
{
    if (m_floors.empty())
        return 0;
    return m_floors[0]->width();
}

uint16_t Map::height() const
{
    if (m_floors.empty())
        return 0;
    return m_floors[0]->height();
}

Floor* Map::addFloor()
{
    if (m_floors.size() >= MAX_FLOORS_COUNT)
        return nullptr;

    m_floors.push_back(std::make_unique<Floor>(width(), height()));
    return m_floors.back().get();
}

bool Map::removeFloorAt(uint8_t idx)
{
    if (m_floors.size() == 1 || idx >= m_floors.size())
        return false;

    m_floors.erase(m_floors.begin() + idx);
    return true;
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
    for (const auto& pFloor : m_floors)
        pFloor->removeChipRef(idx);

    return true;
}

void Map::unregisterCharacter(char_id id)
{
    for (const auto& floor : m_floors)
        floor->deleteNpc(id);
}
void Map::replaceCharactedId(char_id oldId, char_id newId)
{
    for (const auto& floor : m_floors)
        floor->replaceCharactedId(oldId, newId);
}

} // namespace Dummy
