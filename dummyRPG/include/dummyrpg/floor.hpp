#ifndef DUMMYRPG_FLOOR
#define DUMMYRPG_FLOOR

#include "dummyrpg/layer.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

/// \brief A Floor is a level of interactivity with the player. It correspond
/// to a "real floor", i.e. objects at a same altitude.
///
/// Some limitations:
///  - 1 blocking layer only
///  - 2 (INF_LAYERS_COUNT) graphic layers below the player (let's say the floor
///     some objects on the floor)
///  - 4 (MAX_LAYERS_COUNT - INF_LAYERS_COUNT) graphic layers above the player (
///     trees, builds and other high suff)
///  - with <= 1024 (MAX_LAYER_BORDER_SIZE)
///  - height <= 1024 (MAX_LAYER_BORDER_SIZE)
///
class Floor
{
public:
    ///
    /// \brief Floor create a floor with by default :
    ///  - 1 blocking layer
    ///  - 2 layers below player
    ///  - 2 layers above player
    /// \param w width of the floor. 1024 max.
    /// \param h height of the floor. 1024 max.
    Floor(uint16_t w, uint16_t h);

    /////////////////////////////////////
    // Const getters
    uint16_t width() const { return m_blockingLayer.width(); }
    uint16_t height() const { return m_blockingLayer.height(); }
    const BlockingLayer& blockingLayer() const { return m_blockingLayer; }
    const std::vector<GraphicLayer>& graphicLayers() const { return m_layers; }

    /////////////////////////////////////
    // Editable ref getters
    GraphicLayer& graphicLayerAt(uint8_t idx) { return m_layers[idx]; }

    /////////////////////////////////////
    // Edition methods
    bool addLayerAbove();    ///< add a new top layer, in the limit of MAX_LAYERS_COUNT
    bool removeLayerAbove(); ///< remove the top layer. Cannot remove a inf layer

    /// \brief removeChipRef unset all tiles from this chip. Use this method for erase
    /// layers' references to any old chipset still in use
    void removeChipRef(chip_id);

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
