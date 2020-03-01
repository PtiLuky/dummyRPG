#ifndef DUMMYRPG_GAMERENDER
#define DUMMYRPG_GAMERENDER

#include "MapRender.hpp"

namespace DummyPlayer {

class GameRender
{
public:
    GameRender(sf::RenderWindow& window);
    void setMap(const Dummy::Map&, const Dummy::Game&);
    void renderingThread();

private:
    sf::RenderWindow& m_window;
    std::unique_ptr<MapRender> m_mapRender;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_GAMERENDER
