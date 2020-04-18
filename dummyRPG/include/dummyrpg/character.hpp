#ifndef DUMMYRPG_CHARACTER_HPP
#define DUMMYRPG_CHARACTER_HPP

#include <map>
#include <string>

#include "dummyrpg/dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

class Character
{
    friend class Serializer;

public:
    Character(const std::string&& name, sprite_id spriteId);

    const std::string& name() const;
    sprite_id spriteId() const;

private:
    std::string m_name;
    sprite_id m_spriteId = 0;
};

///////////////////////////////////////////////////////////////////////////////

class CharacterInstance
{
public:
    CharacterInstance(char_id id, const PositionChar& position);

    char_id characterId() const;
    const PositionChar& Pos() const;

private:
    char_id m_charId = 0;
    PositionChar m_pos;
};

///////////////////////////////////////////////////////////////////////////////

class PlayerInstance
{
    friend class GameInstance;

public:
    PlayerInstance(const std::string&& name, sprite_id spriteId, const PositionChar& position);

    sprite_id SpriteId() const;
    const PositionChar& Pos() const;

private:
    std::string m_name;
    sprite_id m_spriteId = 0;
    PositionChar m_pos;

    std::map<item_id, uint16_t> m_bag;
};

} // namespace Dummy

#endif // DUMMYRPG_CHARACTER_HPP
