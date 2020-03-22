#ifndef DUMMYRPG_GAME_HPP
#define DUMMYRPG_GAME_HPP

#include <map>

#include "dummyrpg/character.hpp"
#include "dummyrpg/map.hpp"
#include "dummyrpg/sprite.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct Game
{
    std::map<uint16_t, std::unique_ptr<Map>> maps;
    std::map<char_id, Character> characters;

    std::map<chip_id, std::string> chipsetPaths;
    std::map<sprite_id, AnimatedSprite> spriteSheetPaths;
};

} // namespace Dummy


#endif
