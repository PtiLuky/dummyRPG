#include <fstream>

#include "dummyrpg/floor.hpp"
#include "dummyrpg/game.hpp"
#include "dummyrpg/serialize.hpp"

#include "ControlGame.hpp"
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
    game.chipsetPaths.push_back("Resources/ClassicRPG_Sheet.png");
    Dummy::Map map(mapSize, mapSize, fkId);
    auto* floor = map.floorAt(0);
    for (uint16_t x = 0; x < mapSize; ++x)
        for (uint16_t y = 0; y < mapSize; ++y)
            floor->setGraphicCell(0, {x, y}, {7, 0, fkId});

    floor->setGraphicCell(2, {1, 1}, {3, 4, fkId});
    floor->setBlockCell({1, 1}, true);
    floor->setGraphicCell(2, {1, 2}, {3, 4, fkId});
    floor->setGraphicCell(2, {1, 3}, {3, 4, fkId});
    floor->setGraphicCell(2, {1, 4}, {10, 1, fkId});
    floor->setGraphicCell(2, {1, 5}, {3, 4, fkId});
    floor->setGraphicCell(2, {1, 6}, {3, 4, fkId});
    floor->setGraphicCell(2, {1, 7}, {3, 4, fkId});
    floor->setGraphicCell(2, {1, 8}, {3, 4, fkId});
    floor->setBlockCell({1, 8}, true);
    floor->setGraphicCell(2, {2, 1}, {7, 1, fkId});
    floor->setBlockCell({2, 1}, true);

    game.maps.push_back(std::move(map));

    // Create sprite
    Dummy::AnimatedSprite sprite1;
    sprite1.imgPath  = "Resources/ClassicRPG_Sheet.png";
    sprite1.width    = 16;
    sprite1.height   = 16;
    sprite1.nbFrames = 4;
    game.sprites.push_back(sprite1);

    Dummy::AnimatedSprite sprite2;
    sprite2.imgPath  = "Resources/ClassicRPG_Sheet.png";
    sprite2.width    = 16;
    sprite2.height   = 16;
    sprite2.nbFrames = 4;
    game.sprites.push_back(sprite2);


    Dummy::Character perso1("Whity", 1);
    game.characters.push_back(perso1);


    Dummy::PositionChar pos1;
    pos1.coord               = {3, 3};
    pos1.mapId               = 0;
    pos1.floorId             = 0;
    auto pi1Id               = game.RegisterNPC(0, pos1);
    Dummy::PositionChar pos2 = pos1;
    pos2.coord               = {4, 3};
    pos2.dir                 = Dummy::Direction::Left;
    auto pi2Id               = game.RegisterNPC(0, pos2);
    Dummy::PositionChar pos3 = pos1;
    pos3.coord               = {3, 5};
    pos3.dir                 = Dummy::Direction::Right;
    auto pi3Id               = game.RegisterNPC(0, pos3);
    Dummy::PositionChar pos4 = pos1;
    pos4.coord               = {6, 6};
    pos4.dir                 = Dummy::Direction::Right;
    auto pi4Id               = game.RegisterNPC(0, pos4);

    auto dialog1Id  = game.RegisterDialog("Number1", "Bonjour je suis 1.");
    auto dialogNId  = game.RegisterDialog("Number1", "Bah non...");
    auto dialogYId  = game.RegisterDialog("Number1", "Yesss bien jou\u00e9");
    auto choiceId   = game.RegisterChoice("Tu sais choisir l'option 3 ?");
    auto dialog2Id  = game.RegisterDialog("Number2", "Bonjour je suis 2.");
    auto dialog2bId = game.RegisterDialog("Number2", "... Et je continue de parler");
    auto dialog2cId = game.RegisterDialog(
        "Number2",
        "... Et je fais des phrases tr\u00e8s longues pour tester le d\u00e9bordement. :) :) :)\n"
        "... genre longues... et je fais des phrases tr\u00e8s longues pour tester le d\u00e9bordement. :) :) :) (again)");
    auto dialog3Id = game.RegisterDialog("Number3", "Bonjour je suis 3.");
    auto dialog4Id = game.RegisterDialog("Number4", "Bonjour je suis 4.");

    auto& dialog1  = game.dialogs[game.events[dialog1Id].idxPerType];
    auto& dialog2  = game.dialogs[game.events[dialog2Id].idxPerType];
    auto& dialog2b = game.dialogs[game.events[dialog2bId].idxPerType];
    auto& choice   = game.dialogsChoices[game.events[choiceId].idxPerType];
    dialog1.setNextEvent(choice.id());
    choice.addOption({"1. Uhm...", dialogNId});
    choice.addOption({"2. ?", dialogNId});
    choice.addOption({"3. Maybe", dialogYId});
    choice.addOption({"4. Uhm...", dialogNId});
    dialog2.setNextEvent(dialog2bId);
    dialog2b.setNextEvent(dialog2cId);

    auto& pi1 = floor->npc(pi1Id);
    auto& pi2 = floor->npc(pi2Id);
    auto& pi3 = floor->npc(pi3Id);
    auto& pi4 = floor->npc(pi4Id);

    pi1.setEvent(dialog1Id);
    pi2.setEvent(dialog2Id);
    pi3.setEvent(dialog3Id);
    pi4.setEvent(dialog4Id);

    return game;
}
static GameInstance createFakeGameInstance(const GameStatic& game)
{
    GameInstance gameInstance(game);
    Dummy::PositionChar pos;
    pos.coord = {5, 5};
    Dummy::PlayerInstance player("Toto", 0, pos);
    gameInstance.registerPlayer(player);

    return gameInstance;
}
#pragma warning(pop)
///////////////////////////////////////////////////////////////////////////////



int main()
{
    const GameStatic game     = createFakeGame();
    GameInstance gameInstance = createFakeGameInstance(game);
    DummyPlayer::GameControl gameControl(game, gameInstance);
    DummyPlayer::Keymap keymap;
    bool gameHasFocus = true;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), game.name);
    window.setFramerateLimit(WIN_FPS);

    DummyPlayer::GameRender renderer(game, gameInstance);
    auto* map = gameInstance.currentMap();
    if (map == nullptr)
        return 1; // corrupted data ???
    renderer.setMap(*map);

    while (window.isOpen()) {
        sf::Event event;

        // Process input events
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
            } else if (event.type == sf::Event::KeyPressed) {
                // split register and doing action to avoid multiple events
                gameControl.registerKeyPressed(event.key.code, keymap);
            }
        }

        // Do actions registered in previous loop
        gameControl.doAction();

        // Process game-sent events
        Dummy::event_id eventId = gameInstance.dequeEvent();
        while (eventId != Dummy::undefEvent) {
            gameControl.executeEvent(eventId);
            eventId = gameInstance.dequeEvent();
        }

        // Process pressed-states (events will only give the pressed moment)
        if (gameHasFocus) {
            gameControl.applyPlayerMovement(keymap);
        }

        // Render game
        window.clear();
        renderer.render(window);
        gameControl.renderOverlays(window);
        window.display();
    }

    return 0;
}
