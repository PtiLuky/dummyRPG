#ifndef DUMMYRPG_GAMERENDER_HPP
#define DUMMYRPG_GAMERENDER_HPP

#include "MapRender.hpp"

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
    explicit GameRender(sf::RenderWindow& window);

    void setMap(const Dummy::Map&, const Dummy::Game&); ///< set the current map
    void renderingThread();                             ///< method to call in the rendering thread

private:
    sf::Mutex m_mutex;
    sf::RenderWindow& m_window;
    std::unique_ptr<MapRender> m_mapRender;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_GAMERENDER_HPP
