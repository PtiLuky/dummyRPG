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

class GameStatic
{
public:
    bool RegisterNPC(char_id id, const PositionChar& pos);

public:
    uint64_t version = 0;
    std::string name;
    std::vector<Map> maps;

    std::vector<Item> items;

    std::vector<Character> characters;
    std::vector<Monster> monsters;

    std::vector<std::string> chipsetPaths;
    std::vector<AnimatedSprite> sprites;
};

///////////////////////////////////////////////////////////////////////////////

class GameInstance
{
public:
    explicit GameInstance(const GameStatic& game);

    bool RegisterPlayer(const PlayerInstance& player);
    void MovePlayer(Direction);
    void StopPlayer();

    const PlayerInstance& Player() const { return m_player; }
    const Dummy::Map* CurrentMap() const;
    const Dummy::Floor* CurrentFloor() const;

private:
    const GameStatic& m_game;
    PlayerInstance m_player = {"Unnamed", 0, PositionChar()};
};

} // namespace Dummy


#endif
