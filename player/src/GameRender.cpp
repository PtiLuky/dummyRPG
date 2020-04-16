#include "GameRender.hpp"

#include <iostream>

namespace DummyPlayer {

GameRender::GameRender(const Dummy::GameStaticData& game, const Dummy::GameInstanceData& instance,
                       sf::RenderWindow& window)
    : m_window(window)
    , m_game(game)
    , m_gameInstance(instance)
{}

void GameRender::setMap(const Dummy::Map& map)
{
    m_mutex.lock();

    try {
        m_playerRender = std::make_unique<PlayerRender>(m_gameInstance.player, *this);
    } catch (const MapRenderError& e) {
        m_mutex.unlock();
        std::cerr << "Error : PlayerRender creation failed (" << e.what() << ")." << std::endl;
    }

    try {
        m_mapRender = std::make_unique<MapRender>(map, *this);
    } catch (const MapRenderError& e) {
        m_mutex.unlock();
        std::cerr << "Error : MapRender creation failed (" << e.what() << ")." << std::endl;
    }
    m_mutex.unlock();
}

void GameRender::renderingThread()
{
    // activate the window's context
    m_window.setActive(true);


    // the rendering loop
    while (m_window.isOpen()) {

        m_mutex.lock();

        // Update offset and player pos
        const uint8_t playerFloor = 0;
        m_mapOffset.x             = static_cast<int>(m_window.getSize().x) / 2 - m_gameInstance.player.pos.x;
        m_mapOffset.y             = static_cast<int>(m_window.getSize().y) / 2 - m_gameInstance.player.pos.y;

        m_window.clear();

        if (m_mapRender)
            m_mapRender->renderBelow(m_window, playerFloor);

        if (m_playerRender)
            m_playerRender->render(m_window);

        // player and chars

        if (m_mapRender)
            m_mapRender->renderAbove(m_window, playerFloor);

        m_window.display();

        m_mutex.unlock();
    }
}
} // namespace DummyPlayer
