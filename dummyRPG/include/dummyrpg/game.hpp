#ifndef DUMMYRPG_GAME_HPP
#define DUMMYRPG_GAME_HPP

#include <deque>
#include <map>
#include <set>

#include "dummyrpg/character.hpp"
#include "dummyrpg/dialog.hpp"
#include "dummyrpg/item.hpp"
#include "dummyrpg/map.hpp"
#include "dummyrpg/monster.hpp"
#include "dummyrpg/sprite.hpp"

static const char* const MAP_FILE_EXT = ".mdummy";

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

template <typename T, typename TT> using umap = std::map<T, TT>;

///////////////////////////////////////////////////////////////////////////////

class GameStatic
{
    friend class Serializer;

public:
    explicit GameStatic(const std::string& path = ".");
    const std::string& name() const;

    // Maps
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
    const umap<sprite_id, AnimatedSprite>& sprites() const;
    const AnimatedSprite* sprite(sprite_id) const;
    AnimatedSprite* sprite(sprite_id);
    sprite_id registerSprite();
    void unregisterSprite(sprite_id);

    // characters
    const umap<char_id, Character>& characters() const;
    const Character* character(char_id) const;
    Character* character(char_id);
    char_id registerCharacter(const std::string&& charName);
    void unregisterCharacter(char_id);

    // Events
    const Event* event(event_id) const;
    const DialogSentence* dialog(event_id eventId) const;
    DialogSentence* dialog(event_id eventId);
    const DialogChoice* choice(event_id eventId) const;
    DialogChoice* choice(event_id eventId);
    event_id registerDialog(char_id speaker, const std::string& sentence);
    event_id registerChoice(const std::string& question);

    // Misc
    /// returns true if all referenced files are present
    bool checkFilesIntegrity() const;
    void cleanupUnused(); ///< remove all unreference events, tilesets, spritesheets...

private:
    static bool assertFileExists(const std::string& filePath);
    void extendUsageEvents(std::set<event_id>& usedEvents, event_id id);

    uint64_t version = 0; // unused
    std::string m_name;   // unused
    std::string m_gameDataPath;
    std::vector<std::string> m_mapsNames;

    std::vector<std::string> m_tileSets;
    std::vector<std::string> m_spriteSheets;

    umap<item_id, Item> m_items;
    umap<monster_id, Monster> m_monsters;

    umap<char_id, Character> m_characters;
    umap<sprite_id, AnimatedSprite> m_sprites;

    umap<event_id, Event> m_events;
    umap<event_id, DialogSentence> m_dialogs;
    umap<event_id, DialogChoice> m_dialogsChoices;
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
    uint8_t m_currFloor = 0;

    bool m_eventsBlocked = false;
    std::deque<event_id> m_eventsQueue;
};

} // namespace Dummy


#endif
