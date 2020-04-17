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
    explicit GameRender(const Dummy::GameStaticData&, const Dummy::GameInstanceData&,
                        sf::RenderWindow& window);

    const Dummy::GameStaticData& game() const { return m_game; }
    const sf::Vector2i& offset() const { return m_mapOffset; }
    sf::Vector2f itemPxPos(Dummy::Coord pos) const;
    float zoom() const { return m_zoom; }

    void setMap(const Dummy::Map&); ///< set the current map
    void render();                  ///< method to call in the rendering thread

private:
    sf::Clock m_clock;
    sf::RenderWindow& m_window;
    std::unique_ptr<MapRender> m_mapRender;
    std::unique_ptr<PlayerRender> m_playerRender;

    const Dummy::GameStaticData& m_game;
    const Dummy::GameInstanceData& m_gameInstance;

    float m_zoom = 2.F;
    sf::Vector2i m_mapOffset;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_GAMERENDER_HPP
