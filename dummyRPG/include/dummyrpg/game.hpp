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
    const std::vector<std::string>& mapNames() const;
    void renameMap(const std::string& oldName, const std::string& newName);
    uint16_t registerMap(const std::string& mapName);

    // Tile sets
    const std::vector<std::string>& tileSets() const;
    std::string tileset(chip_id) const;
    std::string tilesetPath(chip_id) const;
    chip_id registerTileset(const std::string& chipFilename);

    // Sprite sheets
    const std::vector<std::string>& spriteSheets() const;
    std::string spriteSheet(sprite_id) const;
    std::string spriteSheetPath(sprite_id) const;
    sprite_id registerSpriteSheet(const std::string& sheetFilename);

    // sprites
    const std::vector<AnimatedSprite>& sprites() const;
    const AnimatedSprite* sprite(sprite_id) const;
    AnimatedSprite* sprite(sprite_id);
    sprite_id registerSprite();

    // characters
    const std::vector<Character>& characters() const;
    Character* character(char_id);
    char_id registerCharacter(const std::string&& charName);

    // Events
    const Event* event(event_id) const;
    const DialogSentence* dialog(event_id dialogId) const;
    DialogSentence* dialog(event_id dialogId);
    const DialogChoice* choice(event_id choiceId) const;
    DialogChoice* choice(event_id choiceId);
    event_id registerDialog(const std::string& speaker, const std::string& sentence);
    event_id registerChoice(const std::string& question);

    // Misc
    /// returns true if all referenced files are present
    bool checkFilesIntegrity() const;
    const std::string& gameDataPath() const;

private:
    static bool assertFileExists(const std::string& filePath);

    uint64_t version = 0;
    std::string m_name;
    std::string m_gameDataPath = ".";
    std::vector<std::string> m_mapsNames;

    std::vector<std::string> m_tileSets;
    std::vector<std::string> m_spriteSheets;

    std::vector<Item> m_items;
    std::vector<Monster> m_monsters;

    std::vector<Character> m_characters;
    std::vector<AnimatedSprite> m_sprites;

    std::vector<Event> m_events;
    std::vector<DialogSentence> m_dialogs;
    std::vector<DialogChoice> m_dialogsChoices;
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
