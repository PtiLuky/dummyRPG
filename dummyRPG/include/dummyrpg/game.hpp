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
    friend class Serializer;

public:
    void setGameDataPath(const std::string& rootPath);

    const std::string& name() const;

    // Tile sets
    const std::vector<std::string>& tileSets() const;
    std::string tileset(chip_id) const;
    std::string tilesetPath(chip_id) const;
    // Sprite sheets
    const std::vector<std::string>& spriteSheets() const;
    std::string spriteSheet(sprite_id) const;
    std::string spriteSheetPath(sprite_id) const;
    // sprites
    const std::vector<AnimatedSprite>& sprites() const;
    AnimatedSprite* sprite(sprite_id);
    // characters
    const std::vector<Character>& characters() const;
    Character* character(char_id);

    event_id registerDialog(const std::string& speaker, const std::string& sentence);
    event_id registerChoice(const std::string& question);
    chip_id registerTileset(const std::string& chipFilename);
    sprite_id registerSpriteSheet(const std::string& sheetFilename);
    char_id registerCharacter(const std::string&& charName);


    /// returns true if all referenced files are present
    bool checkFilesIntegrity() const;
    const std::string& gameDataPath() const;

public:
    std::vector<Event> events;
    std::vector<DialogSentence> dialogs;
    std::vector<DialogChoice> dialogsChoices;

    std::vector<std::string> mapsNames;
    std::vector<AnimatedSprite> m_sprites;

private:
    static bool assertFileExists(const std::string& filePath);

    uint64_t version = 0;
    std::string m_name;
    std::string m_gameDataPath = ".";

    std::vector<std::string> m_tileSets;
    std::vector<std::string> m_spriteSheets;

    std::vector<Item> m_items;
    std::vector<Monster> m_monsters;

    std::vector<Character> m_characters;
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
