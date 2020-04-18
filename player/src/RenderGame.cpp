#include "RenderGame.hpp"

#include <iostream>

namespace DummyPlayer {

GameRender::GameRender(const Dummy::GameStatic& game, const Dummy::GameInstance& instance,
                       sf::RenderWindow& window)
    : m_window(window)
    , m_game(game)
    , m_gameInstance(instance)
{}

void GameRender::setMap(const Dummy::Map& map)
{
    try {
        m_playerRender = std::make_unique<PlayerRender>(m_gameInstance.Player(), *this);
    } catch (const MapRenderError& e) {
        std::cerr << "Error : PlayerRender creation failed (" << e.what() << ")." << std::endl;
    }

    try {
        m_mapRender = std::make_unique<MapRender>(map, *this);
    } catch (const MapRenderError& e) {
        std::cerr << "Error : MapRender creation failed (" << e.what() << ")." << std::endl;
    }
}

void GameRender::render()
{
    auto& playerPos = m_gameInstance.Player().Pos();
    m_mapOffset.x =
        static_cast<int>(m_window.getSize().x / 2 - playerPos.coord.x * m_zoom * Dummy::TILE_SIZE);
    m_mapOffset.y =
        static_cast<int>(m_window.getSize().y / 2 - playerPos.coord.y * m_zoom * Dummy::TILE_SIZE);

    m_window.clear();

    if (m_mapRender)
        m_mapRender->renderBelow(m_window, playerPos.floorId);

    if (m_playerRender)
        m_playerRender->render(m_window);

    if (m_mapRender)
        m_mapRender->renderAbove(m_window, playerPos.floorId);

    m_window.display();
}

sf::Vector2f GameRender::itemPxPos(Dummy::Coord pos) const
{
    return {
        m_mapOffset.x + pos.x * Dummy::TILE_SIZE * m_zoom,
        m_mapOffset.y + pos.y * Dummy::TILE_SIZE * m_zoom,
    };
}
} // namespace DummyPlayer
