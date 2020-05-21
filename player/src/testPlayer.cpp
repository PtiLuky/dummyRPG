#include <fstream>
#include <iostream>

#include "dummyrpg/floor.hpp"

#include "ControlGame.hpp"

using Dummy::GameInstance;
using Dummy::GameStatic;

///////////////////////////////////////////////////////////////////////////////
// This method is to be deleted later, so yes it's dirty, and no I don't want to see all
// related warnings about tuple casting loose of precision (msvc 4244)
#pragma warning(push)
#pragma warning(disable : 4244)
static GameStatic createFakeGame()
{
    GameStatic game(".");

    game.registerTileset("ClassicRPG_Sheet.png");
    game.registerSpriteSheet("ClassicRPG_Sheet.png");

    // Create sprite
    auto spriteId1         = game.registerSprite();
    auto* sprite1          = game.sprite(spriteId1);
    sprite1->spriteSheetId = 0;
    sprite1->width         = 16;
    sprite1->height        = 16;
    sprite1->nbFrames      = 4;

    auto spriteId2         = game.registerSprite();
    auto* sprite2          = game.sprite(spriteId2);
    sprite2->spriteSheetId = 0;
    sprite2->width         = 16;
    sprite2->height        = 16;
    sprite2->nbFrames      = 4;

    auto id = game.registerCharacter("Whity");
    game.character(id)->setSprite(1);
    auto id2 = game.registerCharacter("Whity2");
    game.character(id2)->setSprite(1);
    auto id3 = game.registerCharacter("Whity3");
    game.character(id3)->setSprite(1);
    auto id4 = game.registerCharacter("Whity4");
    game.character(id4)->setSprite(1);

    auto dialog1Id  = game.registerDialog(id, "Bonjour je suis 1.");
    auto dialogNId  = game.registerDialog(id, "Bah non...");
    auto dialogYId  = game.registerDialog(id, "Yesss bien jou\u00e9");
    auto choiceId   = game.registerChoice("Tu sais choisir l'option 3 ?");
    auto dialog2Id  = game.registerDialog(id2, "Bonjour je suis 2.");
    auto dialog2bId = game.registerDialog(id2, "... Et je continue de parler");
    auto dialog2cId = game.registerDialog(
        id2,
        "... Et je fais des phrases tr\u00e8s longues pour tester le d\u00e9bordement. :) :) :)\n"
        "... genre longues... et je fais des phrases tr\u00e8s longues pour tester le "
        "d\u00e9bordement. :) :) :) (again)");
    game.registerDialog(id3, "Bonjour je suis 3.");
    game.registerDialog(id4, "Bonjour je suis 4.");

    auto* dialog1  = game.dialog(game.event(dialog1Id)->idxPerType);
    auto* dialog2  = game.dialog(game.event(dialog2Id)->idxPerType);
    auto* dialog2b = game.dialog(game.event(dialog2bId)->idxPerType);
    auto* choice   = game.choice(game.event(choiceId)->idxPerType);
    dialog1->setNextEvent(choice->id());
    choice->addOption({"1. Uhm...", dialogNId});
    choice->addOption({"2. ?", dialogNId});
    choice->addOption({"3. Maybe", dialogYId});
    choice->addOption({"4. Uhm...", dialogNId});
    dialog2->setNextEvent(dialog2bId);
    dialog2b->setNextEvent(dialog2cId);

    return game;
}
static std::shared_ptr<Dummy::Map> createMap()
{
    const Dummy::chip_id fkId = 0;
    const uint16_t mapSize    = 30;

    auto pMap   = std::make_shared<Dummy::Map>(mapSize, mapSize, fkId);
    auto* floor = pMap->floorAt(0);
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

    Dummy::PositionChar pos1;
    pos1.coord               = {3, 3};
    auto pi1Id               = floor->registerNPC(0, pos1);
    Dummy::PositionChar pos2 = pos1;
    pos2.coord               = {4, 3};
    pos2.dir                 = Dummy::Direction::Left;
    auto pi2Id               = floor->registerNPC(0, pos2);
    Dummy::PositionChar pos3 = pos1;
    pos3.coord               = {3, 5};
    pos3.dir                 = Dummy::Direction::Right;
    auto pi3Id               = floor->registerNPC(0, pos3);
    Dummy::PositionChar pos4 = pos1;
    pos4.coord               = {6, 6};
    pos4.dir                 = Dummy::Direction::Right;
    auto pi4Id               = floor->registerNPC(0, pos4);

    auto& pi1 = floor->npc(pi1Id);
    auto& pi2 = floor->npc(pi2Id);
    auto& pi3 = floor->npc(pi3Id);
    auto& pi4 = floor->npc(pi4Id);

    pi1.setEvent(0);
    pi2.setEvent(4);
    pi3.setEvent(7);
    pi4.setEvent(8);

    return pMap;
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
// Implementation of log
void Dummy::Log(const std::string& msg)
{
    std::cout << msg << std::endl;
}
void Dummy::LogWarn(const std::string& msg)
{
    std::cout << "(warning) " << msg << std::endl;
}
void Dummy::LogErr(const std::string& msg)
{
    std::cerr << "(error) " << msg << std::endl;
}


///////////////////////////////////////////////////////////////////////////////

int main()
{
    const GameStatic game     = createFakeGame();
    GameInstance gameInstance = createFakeGameInstance(game);
    gameInstance.setCurrentMap(createMap());

    DummyPlayer::GameControl gameControl(game, gameInstance);
    return gameControl.run();
}
