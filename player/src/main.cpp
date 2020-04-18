#include <fstream>

#include "dummyrpg/floor.hpp"
#include "dummyrpg/game.hpp"
#include "dummyrpg/serialize.hpp"

#include "ControlPlayer.hpp"
#include "Keymap.hpp"
#include "RenderGame.hpp"
#include "RenderMap.hpp"

using Dummy::GameInstance;
using Dummy::GameStatic;
static const int WIN_WIDTH  = 600;
static const int WIN_HEIGHT = 600;
static const int WIN_FPS    = 6;

///////////////////////////////////////////////////////////////////////////////
// This method is to be deleted later, so yes it's dirty, and no I don't want to see all
// related warnings about tuple casting loose of precision (msvc 4244)
#pragma warning(push)
#pragma warning(disable : 4244)
static GameStatic createFakeGame()
{
    const Dummy::chip_id fkId = 0;
    const uint16_t mapSize    = 30;

    GameStatic game;

    // Create a map :)
    game.chipsetPaths.push_back("Resources/chip1.png");
    Dummy::Map map(mapSize, mapSize, fkId);
    auto* floor = map.floorAt(0);
    for (uint16_t x = 0; x < mapSize; ++x)
        for (uint16_t y = 0; y < mapSize; ++y)
            floor->graphicLayerAt(0).set({x, y}, {0, 0, fkId});

    floor->graphicLayerAt(2).set({1, 1}, {3, 0, fkId});
    floor->blockingLayer().set({1, 1}, true);
    floor->graphicLayerAt(2).set({1, 2}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 3}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 4}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 5}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 6}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 7}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({1, 8}, {3, 1, fkId});
    floor->graphicLayerAt(2).set({2, 1}, {1, 1, fkId});
    floor->graphicLayerAt(2).set({4, 4}, {0, 0, fkId});

    game.maps.push_back(std::move(map));

    // Create sprite
    Dummy::AnimatedSprite perso1;
    perso1.imgPath  = "Resources/bluewarrior.png";
    perso1.width    = 24;
    perso1.height   = 32;
    perso1.nbFrames = 3;
    game.sprites.push_back(perso1);

    return game;
}
static GameInstance createFakeGameInstance(const GameStatic& game)
{
    GameInstance gameInstance(game);
    Dummy::PlayerInstance player("Toto", 0, {5, 5});
    gameInstance.RegisterPlayer(std::move(player));

    return gameInstance;
}
#pragma warning(pop)
///////////////////////////////////////////////////////////////////////////////



int main()
{
    GameStatic game           = createFakeGame();
    GameInstance gameInstance = createFakeGameInstance(game);
    DummyPlayer::Keymap keymap;
    bool gameHasFocus = true;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), game.name);
    window.setFramerateLimit(WIN_FPS);

    DummyPlayer::GameRender renderer(game, gameInstance, window);
    auto* map = gameInstance.CurrentMap();
    if (map == nullptr)
        return 1; // corrupted data ???
    renderer.setMap(*map);

    while (window.isOpen()) {
        sf::Event event;

        // Process events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.F, 0.F, static_cast<float>(event.size.width),
                                          static_cast<float>(event.size.height));
                window.setView(sf::View(visibleArea));
            } else if (event.type == sf::Event::GainedFocus) {
                gameHasFocus = true;
            } else if (event.type == sf::Event::LostFocus) {
                gameHasFocus = false;
            }
        }

        // Process pressed-states (events will only give the pressed moment)
        if (gameHasFocus) {
            DummyPlayer::PlayerControl::ApplyMovement(gameInstance, keymap);
        }

        // Render game
        renderer.render();
    }

    return 0;
}
