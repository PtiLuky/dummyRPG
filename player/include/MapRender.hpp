#ifndef DUMMYRPG_MAPRENDER
#define DUMMYRPG_MAPRENDER

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>

#include "dummyrpg/game.hpp"
#include "dummyrpg/layer.hpp"

namespace DummyPlayer {

///////////////////////////////////////////////////////////////////////////////

/// \brief MapRenderError: some error related to the MapRendering
class MapRenderError : public std::runtime_error
{
public:
    explicit MapRenderError(const std::string& msg)
        : std::runtime_error(msg)
    {}
};

///////////////////////////////////////////////////////////////////////////////

///
/// \brief MapRender prepares and exposes methods to render a map.
/// The rendering thread/method/classes (GameRender ?) will call
/// renderBelow() before drawing the characters and renderAbove() after the characters
///
/// This class is thought to render a static map.
/// Objects, Characters and other dynamic sprite should be rendered outside this class.
///
/// This class cannot "update" the currently rendered map, if the map changed, another
/// MapRender should be created.
///
class MapRender
{
public:
    ///
    /// \brief MapRender objects construction needs a Map and the Game where it belongs
    ///
    MapRender(const Dummy::Map&, const Dummy::Game&);
    /// \brief renderBelow Render all the floor below the playerFloor, and only layers
    /// below the player for the current floor
    /// \param playerFloor the index of the current floor
    void renderBelow(sf::RenderWindow&, uint8_t playerFloor);
    /// \brief renderBelow Render only layers above the player for the current floor
    /// \param playerFloor the index of the current floor
    void renderAbove(sf::RenderWindow&, uint8_t playerFloor);

private:
    void addLayer(const Dummy::GraphicLayer& layer, std::vector<sf::Texture>& floorList,
                  const std::map<chip_id, uint8_t>& idMapping);

private:
    float m_zoom = 2.F;
    sf::Mutex m_mutex;
    sf::Shader m_mapShader;
    sf::Sprite m_mapSprite;
    std::vector<sf::Texture> m_floors;
    std::vector<uint16_t> m_abovePlayerIdx; ///< indices of each layer "above" the player, per floor
    std::vector<uint16_t> m_firstLayerIdx;  ///< indices of the first layer of each floor
    std::vector<sf::Texture> m_chipsets;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_MAPRENDER
