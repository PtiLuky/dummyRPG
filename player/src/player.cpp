#include <fstream>
#include <iostream>

#include "ControlGame.hpp"
#include "dummyrpg/floor.hpp"
#include "dummyrpg/serialize.hpp"

using Dummy::GameInstance;
using Dummy::GameStatic;

static GameInstance createFakeGameInstance(GameStatic& game)
{
    // Edit game to add temps necessary sprites
    game.registerSpriteSheet("ClassicRPG_Sheet.png");
    Dummy::AnimatedSprite sprite1;
    sprite1.spriteSheetId = 0;
    sprite1.width         = 16;
    sprite1.height        = 16;
    sprite1.nbFrames      = 4;
    game.m_sprites.push_back(sprite1);


    GameInstance gameInstance(game);
    Dummy::PositionChar pos;
    pos.coord = {1, 1};
    Dummy::PlayerInstance player("Toto", 0, pos);
    gameInstance.registerPlayer(player);

    return gameInstance;
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr
            << "This application must be called with 2 parameters : the game_path and the map_name"
            << std::endl;
        return 1;
    }

    std::string gameFilePath = std::string(argv[1]) + "/game-data.gdummy";
    std::ifstream gameFile(gameFilePath, std::ios::binary);
    GameStatic game;
    game.setGameDataPath(argv[1]);
    Dummy::Serializer::parseGameFromFile(gameFile, game);

    std::string mapFilePath = std::string(argv[1]) + "/maps/" + std::string(argv[2]) + ".mdummy";
    std::ifstream mapFile(mapFilePath, std::ios::binary);
    std::shared_ptr<Dummy::Map> pMap = std::make_shared<Dummy::Map>();
    Dummy::Serializer::parseMapFromFile(mapFile, *pMap);

    GameInstance gameInstance = createFakeGameInstance(game);
    gameInstance.setCurrentMap(pMap);

    DummyPlayer::GameControl gameControl(game, gameInstance);
    return gameControl.run();
}
