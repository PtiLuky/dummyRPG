#include "dummyrpg/game.hpp"

#include "dummyrpg/floor.hpp"

namespace Dummy {

bool GameStatic::RegisterNPC(char_id id, const PositionChar& pos)
{
    if (id >= characters.size())
        return false;

    if (pos.mapId >= maps.size())
        return false;

    auto* floor = maps[pos.mapId].floorAt(pos.floorId);
    if (floor == nullptr)
        return false;

    floor->registerNPC(id, pos);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

GameInstance::GameInstance(const GameStatic& game)
    : m_game(game)
{}

bool GameInstance::RegisterPlayer(const PlayerInstance& player)
{
    m_player = player;
    // Some check to do to return false sometimes ?
    return true;
}

const Dummy::Map* GameInstance::CurrentMap() const
{
    uint16_t mapIdx = m_player.m_pos.mapId;
    if (mapIdx >= m_game.maps.size())
        return nullptr;

    return &m_game.maps[mapIdx];
}

const Dummy::Floor* GameInstance::CurrentFloor() const
{
    auto* map = CurrentMap();
    if (map == nullptr)
        return nullptr;

    return CurrentMap()->floorAt(m_player.m_pos.floorId);
}

void GameInstance::MovePlayer(Direction dir)
{
    auto* floor = CurrentFloor();
    if (floor == nullptr)
        return;

    auto newCoord = m_player.m_pos.coord;

    if (dir == Direction::Top) {
        if (newCoord.y > 0)
            --newCoord.y;
        m_player.m_pos.state = Dummy::CharState::Walking;

    } else if (dir == Direction::Left) {
        if (newCoord.x > 0)
            --newCoord.x;
        m_player.m_pos.state = Dummy::CharState::Walking;

    } else if (dir == Direction::Bottom) {
        if (newCoord.y < floor->height() - 1)
            ++newCoord.y;
        m_player.m_pos.state = Dummy::CharState::Walking;

    } else if (dir == Direction::Right) {
        if (newCoord.x < floor->width() - 1)
            ++newCoord.x;
        m_player.m_pos.state = Dummy::CharState::Walking;
    }

    // Note: when changing direction the first step is in place

    if (m_player.m_pos.dir != dir)
        m_player.m_pos.dir = dir;
    else if (floor->isWalkable(newCoord))
        m_player.m_pos.coord = newCoord;
}

void GameInstance::StopPlayer()
{
    if (m_player.m_pos.state == Dummy::CharState::Walking)
        m_player.m_pos.state = Dummy::CharState::Idle;
}

} // namespace Dummy
