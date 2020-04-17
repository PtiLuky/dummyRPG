#ifndef DUMMYRPG_MAPRENDER_HPP
#define DUMMYRPG_MAPRENDER_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>

#include "dummyrpg/game.hpp"
#include "dummyrpg/layer.hpp"

namespace DummyPlayer {
class GameRender;
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
    MapRender(const Dummy::Map&, const GameRender&);

    /// \brief renderBelow Render all the floor below the playerFloor, and only layers
    /// below the player for the current floor
    /// \param playerFloor the index of the current floor
    void renderBelow(sf::RenderWindow&, uint8_t playerFloor);

    /// \brief renderBelow Render only layers above the player for the current floor
    /// \param playerFloor the index of the current floor
    void renderAbove(sf::RenderWindow&, uint8_t playerFloor);

private:
    void layerToImage(const Dummy::GraphicLayer&, sf::Image&, const std::map<Dummy::chip_id, uint8_t>&);

private:
    const GameRender& m_gameRender;

    sf::Shader m_mapShader;
    sf::Sprite m_mapSprite;
    std::vector<sf::Texture> m_tilemaps; ///< Each floor has up to 6 textures,
    std::vector<size_t> m_firstTexsIdx;  ///< index of the first texture of each floor in m_tilemaps
    std::vector<sf::Texture> m_chipsets;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_MAPRENDER_HPP
