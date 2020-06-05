#include <fstream>
#include <iostream>

#include "ControlGame.hpp"
#include "dummyrpg/floor.hpp"
#include "dummyrpg/serialize.hpp"

using Dummy::GameInstance;
using Dummy::GameStatic;

static GameInstance createFakeGameInstance(const GameStatic& game)
{
    GameInstance gameInstance(game);
    Dummy::PositionChar pos;
    pos.coord = {1, 1};
    Dummy::PlayerInstance player("Toto", 0, pos); // Will use first sprite if there is one
    gameInstance.registerPlayer(player);

    return gameInstance;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of logs
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

int main(int argc, char* argv[])
{
    if (argc != 3) {
        Dummy::LogErr("This application must be called with 2 parameters : " //
                      "\n\t 1. game_path"                                    //
                      "\n\t 2. map_name");
        return 1;
    }

    std::string gameFilePath = std::string(argv[1]) + "/game-data.gdummy";
    std::ifstream gameFile(gameFilePath, std::ios::binary);
    GameStatic game(argv[1]);
    Dummy::Serializer::parseGameFromFile(gameFile, game);

    std::string mapFilePath = std::string(argv[1]) + "/maps/" + std::string(argv[2]) + MAP_FILE_EXT;
    std::ifstream mapFile(mapFilePath, std::ios::binary);
    std::shared_ptr<Dummy::Map> pMap = std::make_shared<Dummy::Map>();
    Dummy::Serializer::parseMapFromFile(mapFile, *pMap);

    GameInstance gameInstance = createFakeGameInstance(game);
    gameInstance.setCurrentMap(pMap);

    DummyPlayer::GameControl gameControl(game, gameInstance);
    return gameControl.run();
}
