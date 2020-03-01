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
} // namespace Dummy
