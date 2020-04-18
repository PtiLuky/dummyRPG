#include "dummyrpg/character.hpp"

namespace Dummy {
Character::Character(const std::string&& name, sprite_id spriteId)
    : m_name(name)
    , m_spriteId(spriteId)
{}

CharacterInstance::CharacterInstance(char_id id, const PositionChar&& position)
    : m_charId(id)
    , m_pos(position)
{}

PlayerInstance::PlayerInstance(const std::string&& name, sprite_id spriteId,
                               const PositionChar&& position)
    : m_name(name)
    , m_spriteId(spriteId)
    , m_pos(position)
{}

} // namespace Dummy
