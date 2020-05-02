#ifndef DUMMYRPG_GAME_HPP
#define DUMMYRPG_GAME_HPP

#include <deque>
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
    void setGameDataPath(const std::string& rootPath);
    event_id registerDialog(const std::string& speaker, const std::string& sentence);
    event_id registerChoice(const std::string& question);
    chip_id registerChipset(const std::string& chipPath);
    sprite_id registerSpriteSheet(const std::string& sheetPath);

    std::string spriteSheetPath(sprite_id) const;
    std::string tileSetPath(chip_id) const;

    /// returns true if all referenced files are present
    bool checkFilesIntegrity() const;
    const std::string& gameDataPath() const;

public:
    uint64_t version = 0;
    std::string name;

    std::vector<Item> items;

    std::vector<Character> characters;
    std::vector<Monster> monsters;

    std::vector<Event> events;
    std::vector<DialogSentence> dialogs;
    std::vector<DialogChoice> dialogsChoices;

    std::vector<std::string> mapsNames;
    std::vector<std::string> tileSets;
    std::vector<std::string> spriteSheets;
    std::vector<AnimatedSprite> sprites;

private:
    static bool assertFileExists(const std::string& filePath);
    std::string m_gameDataPath = ".";
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

    void blockEvents(bool);
    void registerEvent(event_id);
    event_id dequeEvent();
    bool eventBlocked() const;
    void setCurrentMap(const std::shared_ptr<Map>& pMap);

    const PlayerInstance& player() const;
    const Dummy::Map* currentMap() const;
    const Dummy::Floor* currentFloor() const;

private:
    Dummy::Floor* currentFloor();

private:
    const GameStatic& m_game;
    PlayerInstance m_player = {"Unnamed", 0, PositionChar()};

    std::shared_ptr<Map> m_currMap;

    bool m_eventsBlocked = false;
    std::deque<event_id> m_eventsQueue;
};

} // namespace Dummy


#endif
