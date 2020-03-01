#ifndef DUMMYRPG_MAPRENDER
#define DUMMYRPG_MAPRENDER

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>

#include "dummyrpg/game.hpp"
#include "dummyrpg/layer.hpp"

namespace DummyPlayer {

///////////////////////////////////////////////////////////////////////////////

class MapRenderError : public std::runtime_error
{
public:
    MapRenderError(const std::string& msg)
        : std::runtime_error(msg)
    {}
};

///////////////////////////////////////////////////////////////////////////////

class MapRender
{
public:
    MapRender(const Dummy::Map&, const Dummy::Game&);
    void renderBelow(sf::RenderWindow&, uint8_t playerFloor);
    void renderAbove(sf::RenderWindow&, uint8_t playerFloor);

private:
    void addLayer(const Dummy::GraphicLayer& layer, std::vector<sf::Texture>& floorList,
                  const std::map<chip_id, uint8_t>& idMapping);

private:
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
