#ifndef DUMMYRPG_CHARACTER_HPP
#define DUMMYRPG_CHARACTER_HPP

#include <string>
#include <map>

#include "dummyrpg/dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct Character
{
    std::string name;
    sprite_id spriteSheetId;
};

///////////////////////////////////////////////////////////////////////////////

struct CharacterInstance
{
    char_id charId;
    Coord pos;
};

///////////////////////////////////////////////////////////////////////////////

struct PlayerInstance
{
    std::string name;
    sprite_id spriteId;
    Coord pos;

    std::map<item_id, uint16_t> bag;
};

} // namespace Dummy

#endif // DUMMYRPG_CHARACTER_HPP
