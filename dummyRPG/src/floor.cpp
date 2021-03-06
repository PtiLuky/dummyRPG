#include "dummyrpg/floor.hpp"

namespace Dummy {

Floor::Floor(uint16_t w, uint16_t h)
    : m_blockingLayer(w, h, false)
{
    m_layers.reserve(MAX_LAYERS_COUNT);

    // Default : 2 layers below, 2 layers above
    addLayerAbove(); // below player
    addLayerAbove(); // below player

    addLayerAbove(); // above player
    addLayerAbove(); // above player
}

void Floor::resize(uint16_t w, uint16_t h)
{
    m_blockingLayer.resize(w, h, false);

    for (auto& layer : m_layers)
        layer.resize(w, h, undefAspect);

    for (auto it = m_npcs.begin(); it != m_npcs.end();) {
        auto coord = it->pos().coord;
        if (coord.x >= w || coord.y >= h)
            it = m_npcs.erase(it);
        else
            ++it;
    }
}

uint16_t Floor::width() const
{
    return m_blockingLayer.width();
}

uint16_t Floor::height() const
{
    return m_blockingLayer.height();
}

const BlockingLayer& Floor::blockingLayer() const
{
    return m_blockingLayer;
}

BlockingLayer& Floor::blockingLayer()
{
    return m_blockingLayer;
}

bool Floor::isWalkable(Coord coord) const
{
    if (m_blockingLayer.at(coord) > 0) // 0 is false but encoded on uint8... can be improved
        return false;

    for (auto& npc : m_npcs)
        if (npc.pos().coord == coord)
            return false;

    return true;
}

GraphicLayer& Floor::graphicLayersAt(uint8_t idx)
{
    return m_layers[idx];
}

const std::vector<GraphicLayer>& Floor::graphicLayers() const
{
    return m_layers;
}

const std::vector<CharacterInstance>& Floor::npcs() const
{
    return m_npcs;
}

CharacterInstance& Floor::npc(char_id id)
{
    return m_npcs[id];
}

void Floor::deleteNpcAt(Coord coord)
{
    size_t nbNpc = m_npcs.size();
    for (size_t i = 0; i < nbNpc; ++i)
        if (m_npcs[i].pos().coord == coord) {
            m_npcs.erase(m_npcs.begin() + static_cast<long long>(i));
            break;
        }
}

void Floor::replaceCharactedId(char_id oldId, char_id newId)
{
    if (newId == undefChar)
        deleteNpc(oldId);
    else {
        size_t nbNpc = m_npcs.size();
        for (size_t i = 0; i < nbNpc; ++i)
            if (m_npcs[i].characterId() == oldId)
                m_npcs[i].changeId(newId);
    }
}

void Floor::deleteNpc(char_id characterId)
{
    for (auto it = m_npcs.begin(); it != m_npcs.end();)
        if (it->characterId() == characterId)
            it = m_npcs.erase(it);
        else
            ++it;
}

void Floor::setBlockCell(Coord coord, bool val)
{
    m_blockingLayer.set(coord, val);
}

void Floor::setGraphicCell(uint8_t layerIdx, Coord coord, Tileaspect val)
{
    m_layers[layerIdx].set(coord, val);
}

bool Floor::addLayerAbove()
{
    if (m_layers.size() >= MAX_LAYERS_COUNT)
        return false;

    m_layers.push_back(GraphicLayer(width(), height(), undefAspect));
    return true;
}

bool Floor::removeLayerAbove()
{
    if (m_layers.size() <= INF_LAYERS_COUNT) // cannot delete inferior layers
        return false;

    m_layers.pop_back();
    return true;
}

void Floor::removeChipRef(chip_id idx)
{
    for (auto& layer : m_layers) {
        const uint16_t w = layer.width();
        const uint16_t h = layer.height();

        for (uint16_t y = 0; y < h; ++y) {
            for (uint16_t x = 0; x < w; ++x) {
                auto val = layer.at({x, y});
                if (val.chipId == idx)
                    layer.set({x, y}, undefAspect);
            }
        }
    }
}

char_id Floor::registerNPC(char_id id, const PositionChar& pos)
{
    m_npcs.push_back(CharacterInstance(id, pos));
    return static_cast<char_id>(m_npcs.size() - 1);
}
} // namespace Dummy
