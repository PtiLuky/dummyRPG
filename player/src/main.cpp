#include "MapRender.hpp"

#include "GameRender.hpp"
#include "dummyrpg/floor.hpp"
#include "dummyrpg/game.hpp"

using Dummy::Game;
static const int WIN_WIDTH  = 600;
static const int WIN_HEIGHT = 600;
static const int WIN_FPS    = 30;

///////////////////////////////////////////////////////////////////////////////
// This method is to be deleted later, so yes it's dirty, and no I don't want to see all
// related warnings about tuple casting loose of precision
#pragma warning(push)
#pragma warning(disable : 4244)
static Game createFakeGame()
{
    const chip_id fkId     = 42;
    const uint16_t mapSize = 30;

    Game game;
    game.m_chipsetPaths.insert({fkId, "Resources/chip1.png"});
    auto pMap   = std::make_unique<Dummy::Map>(mapSize, mapSize, fkId);
    auto* floor = pMap->floorAt(0);
    for (uint16_t x = 0; x < mapSize; ++x)
        for (uint16_t y = 0; y < mapSize; ++y)
            floor->graphicLayerAt(0).set({x, y}, {0, 0, fkId});

    floor->graphicLayerAt(2).set({1, 1}, {3, 0, fkId});
    floor->graphicLayerAt(2).set({1, 2}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 3}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 4}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 5}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 6}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 7}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 8}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({2, 1}, {1, 1, fkId});
    floor->graphicLayerAt(2).set({4, 4}, {0, 0, fkId});

    game.m_maps.insert(std::make_pair(0, std::move(pMap)));
    return game;
}
#pragma warning(pop)
///////////////////////////////////////////////////////////////////////////////



int main()
{
    Game game = createFakeGame();

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "My super game");
    window.setFramerateLimit(WIN_FPS);

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
                sf::FloatRect visibleArea(0.F, 0.F, static_cast<float>(event.size.width),
                                          static_cast<float>(event.size.height));
                window.setView(sf::View(visibleArea));
            }
        }
    }

    return 0;
}
