#ifndef DUMMYRPG_CHARACTER_HPP
#define DUMMYRPG_CHARACTER_HPP

#include <map>
#include <string>

#include "dummyrpg/dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct Character
{
    std::string name;
    sprite_id spriteSheetId = 0;
};

///////////////////////////////////////////////////////////////////////////////

struct CharacterInstance
{
    char_id charId = 0;
    Coord pos      = {0, 0};
};

///////////////////////////////////////////////////////////////////////////////

struct PlayerInstance
{
    std::string name;
    sprite_id spriteId = 0;

    Coord pos       = {0, 0};
    uint16_t mapId  = 0;
    uint8_t floorId = 0;
    Direction dir   = Direction::Bottom;
    CharState state = CharState::Idle;

    std::map<item_id, uint16_t> bag;
};

} // namespace Dummy

#endif // DUMMYRPG_CHARACTER_HPP
