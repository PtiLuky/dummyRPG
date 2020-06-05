#include "dummyrpg/character.hpp"

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

Character::Character(char_id id, const std::string& name, sprite_id spriteId)
    : m_id(id)
    , m_name(name)
    , m_spriteId(spriteId)
{}

char_id Character::id() const
{
    return m_id;
}

const std::string& Character::name() const
{
    return m_name;
}

sprite_id Character::spriteId() const
{
    return m_spriteId;
}

void Character::setName(const std::string& name)
{
    m_name = name;
}

void Character::setSprite(sprite_id id)
{
    m_spriteId = id;
}

///////////////////////////////////////////////////////////////////////////////

CharacterInstance::CharacterInstance(char_id id, const PositionChar& position)
    : m_charId(id)
    , m_pos(position)
{}

event_id CharacterInstance::eventId() const
{
    return m_event;
}

char_id CharacterInstance::characterId() const
{
    return m_charId;
}

const PositionChar& CharacterInstance::pos() const
{
    return m_pos;
}

void CharacterInstance::setEvent(event_id event)
{
    m_event = event;
}

void CharacterInstance::setPos(const PositionChar& pos)
{
    m_pos = pos;
}

void CharacterInstance::changeId(char_id id)
{
    m_charId = id;
}

///////////////////////////////////////////////////////////////////////////////

PlayerInstance::PlayerInstance(const std::string&& name, sprite_id spriteId,
                               const PositionChar& position)
    : m_name(name)
    , m_spriteId(spriteId)
    , m_pos(position)
{}

const PositionChar& PlayerInstance::pos() const
{
    return m_pos;
}

sprite_id PlayerInstance::spriteId() const
{
    return m_spriteId;
}
} // namespace Dummy
