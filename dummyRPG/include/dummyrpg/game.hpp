#ifndef DUMMYRPG_GAME_HPP
#define DUMMYRPG_GAME_HPP

#include <map>

#include "dummyrpg/character.hpp"
#include "dummyrpg/dialog.hpp"
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

    DialogSentence& RegisterDialog(const std::string& speaker, const std::string& sentence);
    DialogChoice& RegisterChoice(const std::string& question);

public:
    uint64_t version = 0;
    std::string name;
    std::vector<Map> maps;

    std::vector<Item> items;

    std::vector<Character> characters;
    std::vector<Monster> monsters;

    std::vector<Event> events;
    std::vector<DialogSentence> dialogs;
    std::vector<DialogChoice> dialogsChoices;

    std::vector<std::string> chipsetPaths;
    std::vector<AnimatedSprite> sprites;
};

///////////////////////////////////////////////////////////////////////////////

class GameInstance
{
public:
    explicit GameInstance(const GameStatic& game);

    bool registerPlayer(const PlayerInstance& player);
    void movePlayer(Direction);
    void stopPlayer();
    void interractInFrontOfPlayer();

    const PlayerInstance& player() const;
    const Dummy::Map* currentMap() const;
    const Dummy::Floor* currentFloor() const;

private:
    Dummy::Floor* currentFloor();

private:
    const GameStatic& m_game;
    PlayerInstance m_player = {"Unnamed", 0, PositionChar()};
};

} // namespace Dummy


#endif
