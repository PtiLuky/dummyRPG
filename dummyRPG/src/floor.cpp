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

bool Floor::isWalkable(Coord coord) const
{
    if (m_blockingLayer.at(coord))
        return false;

    for (auto& npc : m_npcs)
        if (npc.pos().coord == coord)
            return false;

    return true;
}

const std::vector<GraphicLayer>& Floor::graphicLayers() const
{
    return m_layers;
}

const std::vector<CharacterInstance>& Floor::npcs() const
{
    return m_npcs;
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

void Floor::registerNPC(char_id id, const PositionChar& pos)
{
    m_npcs.push_back(CharacterInstance(id, pos));
}
} // namespace Dummy
