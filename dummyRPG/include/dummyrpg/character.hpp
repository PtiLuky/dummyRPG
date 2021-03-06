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
    Character(char_id, const std::string& name, sprite_id spriteId);

    char_id id() const;
    const std::string& name() const;
    sprite_id spriteId() const;

    void setName(const std::string& name);
    void setSprite(sprite_id);

private:
    char_id m_id;
    std::string m_name;
    sprite_id m_spriteId = 0;
};

///////////////////////////////////////////////////////////////////////////////

class CharacterInstance
{
    friend class GameInstance;

public:
    CharacterInstance(char_id id, const PositionChar& position);

    char_id characterId() const;
    event_id eventId() const;
    const PositionChar& pos() const;

    void setEvent(event_id);
    void setPos(const PositionChar&);
    void changeId(char_id id);

private:
    char_id m_charId = 0;
    PositionChar m_pos;

    event_id m_event = undefEvent;
};

///////////////////////////////////////////////////////////////////////////////

class PlayerInstance
{
    friend class GameInstance;

public:
    PlayerInstance(const std::string&& name, sprite_id spriteId, const PositionChar& position);

    sprite_id spriteId() const;
    const PositionChar& pos() const;

private:
    std::string m_name;
    sprite_id m_spriteId = 0;
    PositionChar m_pos;

    std::map<item_id, uint16_t> m_bag;
};

} // namespace Dummy

#endif // DUMMYRPG_CHARACTER_HPP
