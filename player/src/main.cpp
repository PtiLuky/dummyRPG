#include "MapRender.hpp"

#include "GameRender.hpp"
#include "dummyrpg/floor.hpp"
#include "dummyrpg/game.hpp"

using Dummy::Game;

Game createFakeGame()
{
    Game game;
    game.m_chipsetPaths.insert({42, "Resources/chip1.png"});
    auto pMap   = std::make_unique<Dummy::Map>(20, 20, 42);
    auto* floor = pMap->floorAt(0);
    for (uint16_t x = 0; x < 20; ++x)
        for (uint16_t y = 0; y < 20; ++y)
            floor->graphicLayerAt(0).set({x, y}, {0, 0, 42});

    floor->graphicLayerAt(2).set({1, 1}, {3, 0, 42});
    floor->graphicLayerAt(2).set({1, 2}, {3, 1, 42});
    floor->graphicLayerAt(2).set({2, 1}, {1, 1, 42});
    floor->graphicLayerAt(2).set({4, 4}, {0, 0, 42});

    game.m_maps.insert(std::make_pair(0, std::move(pMap)));
    return game;
}

int main()
{
    Game game = createFakeGame();

    sf::RenderWindow window(sf::VideoMode(600, 600), "My super game");
    window.setFramerateLimit(30);

    DummyPlayer::GameRender renderer(window);
    renderer.setMap(*game.m_maps[0], game);

    // deactivate its OpenGL context
    window.setActive(false);

    // launch the rendering thread
    sf::Thread thread(&DummyPlayer::GameRender::renderingThread, &renderer);
    thread.launch();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width),
                                          static_cast<float>(event.size.height));
                window.setView(sf::View(visibleArea));
            }
        }
    }

    return 0;
}
