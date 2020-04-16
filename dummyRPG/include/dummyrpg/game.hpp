#ifndef DUMMYRPG_GAME_HPP
#define DUMMYRPG_GAME_HPP

#include <map>

#include "dummyrpg/character.hpp"
#include "dummyrpg/item.hpp"
#include "dummyrpg/map.hpp"
#include "dummyrpg/monster.hpp"
#include "dummyrpg/sprite.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct GameStaticData
{
    uint64_t version;
    std::string name;
    std::vector<Map> maps;

    std::vector<Item> items;

    std::vector<Character> characters;
    std::vector<Monster> monsters;

    std::vector<std::string> chipsetPaths;
    std::vector<AnimatedSprite> sprites;


};

///////////////////////////////////////////////////////////////////////////////

struct GameInstanceData
{
    PlayerInstance player;
};

} // namespace Dummy


#endif
