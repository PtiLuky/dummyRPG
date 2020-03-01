#include "GameRender.hpp"

namespace DummyPlayer {

GameRender::GameRender(sf::RenderWindow& window)
    : m_window(window)
{}

void GameRender::setMap(const Dummy::Map& map, const Dummy::Game& game)
{
    m_mapRender = std::make_unique<MapRender>(map, game);
}

void GameRender::renderingThread()
{
    // activate the window's context
    m_window.setActive(true);

    sf::CircleShape shape(200.f);
    shape.setFillColor(sf::Color::Green);

    sf::Texture texture;
    if (! texture.loadFromFile("Resources/chip1.png"))
        return;

    sf::Sprite sprite;
    sprite.setTexture(texture);

    // the rendering loop
    while (m_window.isOpen()) {

        uint8_t playerFloor = 0;

        m_window.clear();

        if (m_mapRender)
            m_mapRender->renderBelow(m_window, playerFloor);

        // TODO player and chars

        if (m_mapRender)
            m_mapRender->renderAbove(m_window, playerFloor);

        m_window.display();
    }
}
} // namespace DummyPlayer