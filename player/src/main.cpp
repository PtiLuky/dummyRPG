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
            floor->setGraphicCell(0, {x, y}, {0, 0, fkId});

    floor->setGraphicCell(2, {1, 1}, {3, 0, fkId});
    floor->setBlockCell({1, 1}, true);
    floor->setGraphicCell(2, {1, 2}, {3, 1, fkId});
    floor->setGraphicCell(2, {1, 3}, {3, 1, fkId});
    floor->setGraphicCell(2, {1, 4}, {3, 1, fkId});
    floor->setGraphicCell(2, {1, 5}, {3, 1, fkId});
    floor->setGraphicCell(2, {1, 6}, {3, 1, fkId});
    floor->setGraphicCell(2, {1, 7}, {3, 1, fkId});
    floor->setGraphicCell(2, {1, 8}, {3, 1, fkId});
    floor->setBlockCell({1, 8}, true);
    floor->setGraphicCell(2, {2, 1}, {1, 1, fkId});
    floor->setBlockCell({2, 1}, true);

    game.maps.push_back(std::move(map));

    // Create sprite
    Dummy::AnimatedSprite sprite1;
    sprite1.imgPath  = "Resources/bluewarrior.png";
    sprite1.width    = 24;
    sprite1.height   = 32;
    sprite1.nbFrames = 3;
    game.sprites.push_back(sprite1);

    Dummy::AnimatedSprite sprite2;
    sprite2.imgPath  = "Resources/WhiteScale.png";
    sprite2.width    = 24;
    sprite2.height   = 32;
    sprite2.nbFrames = 3;
    game.sprites.push_back(sprite2);

    Dummy::Character perso1("Whity", 1);
    game.characters.push_back(perso1);

    Dummy::PositionChar pos1;
    pos1.coord   = {3, 3};
    pos1.mapId   = 0;
    pos1.floorId = 0;
    game.RegisterNPC(0, pos1);

    Dummy::PositionChar pos2 = pos1;
    pos2.coord               = {4, 3};
    pos2.dir                 = Dummy::Direction::Left;
    game.RegisterNPC(0, pos2);

    Dummy::PositionChar pos3 = pos1;
    pos3.coord               = {3, 5};
    pos3.dir                 = Dummy::Direction::Right;
    game.RegisterNPC(0, pos3);

    auto& dialog1  = game.RegisterDialog("Number1", "Bonjour je suis 1.");
    auto& dialogY  = game.RegisterDialog("Number1", "Bah non...");
    auto& dialogN  = game.RegisterDialog("Number1", "Yesss bien joué");
    auto& choice = game.RegisterChoice("Tu sais choisir l'option 3 ?");
    dialog1.setNextEvent(choice.id());
    choice.addOption({"1. Uhm...", dialogN.id()});
    choice.addOption({"2. ?", dialogN.id()});
    choice.addOption({"3. Maybe", dialogY.id()});
    choice.addOption({"4. Uhm...", dialogN.id()});

    auto& dialog2  = game.RegisterDialog("Number2", "Bonjour je suis 2.");
    auto& dialog2b = game.RegisterDialog("Number2", "... Et je continue de parler");
    dialog2.setNextEvent(dialog2b.id());

    game.RegisterDialog("Number3", "Bonjour je suis 3.");
    game.RegisterDialog("Number4", "Bonjour je suis 4.");

    return game;
}
static GameInstance createFakeGameInstance(const GameStatic& game)
{
    GameInstance gameInstance(game);
    Dummy::PositionChar pos;
    pos.coord = {5, 5};
    Dummy::PlayerInstance player("Toto", 0, std::move(pos));
    gameInstance.registerPlayer(std::move(player));

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
    auto* map = gameInstance.currentMap();
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
            DummyPlayer::PlayerControl::applyMovement(gameInstance, keymap);
        }

        // Render game
        renderer.render();
    }

    return 0;
}
