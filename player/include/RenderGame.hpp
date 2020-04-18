#ifndef DUMMYRPG_GAMERENDER_HPP
#define DUMMYRPG_GAMERENDER_HPP

#include "RenderCharacter.hpp"
#include "RenderMap.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace DummyPlayer {

///////////////////////////////////////////////////////////////////////////////

///
/// \brief GameRender is the classe to instantiate (once) to run the render-loop
/// of the game.
/// It can handle a change of map on the fly
///
/// renderingThread() is the function to call when creating a sf::Thread, and only
/// at this moment.
///
class GameRender
{
public:
    ///
    /// \brief GameRender create a Game Render and link it to a window
    /// \param window the current rendering windows. Must have a longer
    /// lifespan than this object.
    ///
    explicit GameRender(const Dummy::GameStatic&, const Dummy::GameInstance&,
                        sf::RenderWindow& window);

    const Dummy::GameStatic& game() const;
    const sf::Vector2i& offset() const;
    sf::Vector2f itemPxPos(Dummy::Coord pos) const;
    float zoom() const;

    void setMap(const Dummy::Map&);    ///< set the current map
    void changeFloor(uint8_t floorId); ///< update floor-dependent elements
    void render();                     ///< method to call in the rendering thread

private:
    std::vector<uint32_t> getCharacterDrawOrder();

private:
    sf::RenderWindow& m_window;

    std::unique_ptr<MapRender> m_mapRender;
    std::vector<std::unique_ptr<CharacterRender>> m_npcRenders;

    const Dummy::GameStatic& m_game;
    const Dummy::GameInstance& m_gameInstance;

    float m_zoom = 2.F;
    sf::Vector2i m_mapOffset; // in pixels
};

} // namespace DummyPlayer

#endif // DUMMYRPG_GAMERENDER_HPP
