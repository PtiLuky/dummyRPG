#ifndef DUMMYRPG_FLOOR
#define DUMMYRPG_FLOOR

#include "dummyrpg/layer.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

///
/// \brief A Floor has one blocking layer and several graphic layers
///
class Floor
{
public:
    Floor(uint16_t w, uint16_t h);

    // Const getters
    uint16_t width() const { return m_blockingLayer.width(); }
    uint16_t height() const { return m_blockingLayer.height(); }
    const BlockingLayer& blockingLayer() const { return m_blockingLayer; }
    const std::vector<GraphicLayer>& graphicLayers() const { return m_layers; }

    // Getter
    GraphicLayer& graphicLayerAt(uint8_t idx) { return m_layers[idx]; }

    // Modif methods
    bool addLayerAbove();        ///< add a new top layer, in the limit of MAX_LAYERS_COUNT
    bool removeLayerAbove();     ///< remove the top layer. Cannot remove a inf layer
    void removeChipRef(chip_id); ///< unset all tiles from this chip

    // Consts
    static const uint8_t MAX_LAYERS_COUNT = 6; ///< total layers at most
    static const uint8_t INF_LAYERS_COUNT = 2; ///< number of inferior layers

private:
    // Layers
    BlockingLayer m_blockingLayer;
    std::vector<GraphicLayer> m_layers; // MAX_LAYERS_COUNT max
};

} // namespace Dummy

#endif // DUMMYRPG_FLOOR
