#include "dummyrpg/game.hpp"
#include "dummyrpg/floor.hpp"
#include "dummyrpg/serialize.hpp"

#include <fstream>
#include <iostream>

static const char* const MAP_SUBDIR = "maps/";
static const char* const IMG_SUBDIR = "images/";

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////
GameStatic::GameStatic(const std::string& path)
    : m_gameDataPath(path)
{}

const std::string& GameStatic::name() const
{
    return m_name;
}
const std::vector<std::string>& GameStatic::mapNames() const
{
    return m_mapsNames;
}
void GameStatic::renameMap(const std::string& oldName, const std::string& newName)
{
    for (auto& name : m_mapsNames)
        if (name == oldName)
            name = newName;
}
uint16_t GameStatic::registerMap(const std::string& mapName)
{
    const size_t nbMaps = m_mapsNames.size();
    if (nbMaps >= std::numeric_limits<uint16_t>::max())
        return 0;

    for (uint16_t i = 0; i < nbMaps; ++i)
        if (m_mapsNames[i] == mapName)
            return i;

    sprite_id nextMapId = static_cast<uint16_t>(nbMaps);
    m_mapsNames.push_back(mapName);
    return nextMapId;
}

///////////////////////////////////////////////

const std::vector<std::string>& GameStatic::tileSets() const
{
    return m_tileSets;
}
std::string GameStatic::tileset(chip_id id) const
{
    return id < m_tileSets.size() ? m_tileSets[id] : "";
}
std::string GameStatic::tilesetPath(chip_id id) const
{
    return id < m_tileSets.size() ? m_gameDataPath + "/" + IMG_SUBDIR + m_tileSets[id] : "";
}
chip_id GameStatic::registerTileset(const std::string& chipPath)
{
    const size_t nbTileset = m_tileSets.size();
    if (nbTileset >= std::numeric_limits<chip_id>::max())
        return 0;

    for (chip_id i = 0; i < nbTileset; ++i)
        if (m_tileSets[i] == chipPath)
            return i;

    chip_id nextChipId = static_cast<chip_id>(nbTileset);
    m_tileSets.push_back(chipPath);
    return nextChipId;
}

///////////////////////////////////////////////

const std::vector<std::string>& GameStatic::spriteSheets() const
{
    return m_spriteSheets;
}
std::string GameStatic::spriteSheet(sprite_id id) const
{
    return id < m_spriteSheets.size() ? m_spriteSheets[id] : "";
}
std::string GameStatic::spriteSheetPath(sprite_id id) const
{
    return id < m_spriteSheets.size() ? (m_gameDataPath + "/" + IMG_SUBDIR + m_spriteSheets[id])
                                      : "";
}
sprite_id GameStatic::registerSpriteSheet(const std::string& sheetPath)
{
    const size_t nbSheets = m_spriteSheets.size();
    if (nbSheets >= std::numeric_limits<sprite_id>::max())
        return undefSprite;

    for (sprite_id i = 0; i < nbSheets; ++i)
        if (m_spriteSheets[i] == sheetPath)
            return i;

    sprite_id nextSheetId = static_cast<sprite_id>(nbSheets);
    m_spriteSheets.push_back(sheetPath);
    return nextSheetId;
}

///////////////////////////////////////////////

const umap<sprite_id, AnimatedSprite>& GameStatic::sprites() const
{
    return m_sprites;
}
const AnimatedSprite* GameStatic::sprite(sprite_id id) const
{
    auto r = m_sprites.find(id);
    return r == m_sprites.end() ? nullptr : &r->second;
}
AnimatedSprite* GameStatic::sprite(sprite_id id)
{
    return id < m_sprites.size() ? &m_sprites[id] : nullptr;
}
sprite_id GameStatic::registerSprite()
{
    // use first unused idx. Not best way, but not critical here
    for (sprite_id id = 0; id < Dummy::undefSprite; ++id) {
        if (m_sprites.find(id) == m_sprites.end()) {
            Dummy::AnimatedSprite s;
            s.id = id;
            m_sprites.insert({id, s});
            return id;
        }
    }
    return Dummy::undefSprite;
}

///////////////////////////////////////////////

const umap<char_id, Character>& GameStatic::characters() const
{
    return m_characters;
}
const Character* GameStatic::character(char_id id) const
{
    auto r = m_characters.find(id);
    return r == m_characters.end() ? nullptr : &r->second;
}
Character* GameStatic::character(char_id id)
{
    auto r = m_characters.find(id);
    return r == m_characters.end() ? nullptr : &r->second;
}
char_id GameStatic::registerCharacter(const std::string&& charName)
{
    for (char_id id = 0; id < Dummy::undefChar; ++id) {
        if (m_characters.find(id) == m_characters.end()) {
            m_characters.insert({id, Character(id, charName, Dummy::undefSprite)});
            return id;
        }
    }
    return undefChar;
}

///////////////////////////////////////////////

const Event* GameStatic::event(event_id id) const
{
    auto r = m_events.find(id);
    return r == m_events.end() ? nullptr : &r->second;
}
const DialogSentence* GameStatic::dialog(event_id id) const
{
    auto r = m_dialogs.find(id);
    return r == m_dialogs.end() ? nullptr : &r->second;
}
DialogSentence* GameStatic::dialog(event_id id)
{
    auto r = m_dialogs.find(id);
    return r == m_dialogs.end() ? nullptr : &r->second;
}
const DialogChoice* GameStatic::choice(event_id id) const
{
    auto r = m_dialogsChoices.find(id);
    return r == m_dialogsChoices.end() ? nullptr : &r->second;
}
DialogChoice* GameStatic::choice(event_id id)
{
    auto r = m_dialogsChoices.find(id);
    return r == m_dialogsChoices.end() ? nullptr : &r->second;
}

event_id GameStatic::registerDialog(char_id speaker, const std::string& sentence)
{
    // we use first unused idx. Not best way, but not critical here
    for (event_id id = 0; id < Dummy::undefEvent; ++id) {
        if (m_events.find(id) == m_events.end()) {
            m_events.insert({id, {id, EventType::Dialog}});
            m_dialogs.insert({id, Dummy::DialogSentence(id, speaker, sentence)});
            return id;
        }
    }
    return undefEvent;
}

event_id GameStatic::registerChoice(const std::string& question)
{
    // we use first unused idx. Not best way, but not critical here
    for (event_id id = 0; id < Dummy::undefEvent; ++id) {
        if (m_events.find(id) == m_events.end()) {
            m_events.insert({id, {id, EventType::Choice}});
            m_dialogsChoices.insert({id, Dummy::DialogChoice(id, question)});
            return id;
        }
    }
    return undefEvent;
}
///////////////////////////////////////////////////////////////////////////////

bool GameStatic::checkFilesIntegrity() const
{
    bool success = true;

    for (const auto& mapPath : m_mapsNames)
        success = success && assertFileExists(m_gameDataPath + "/" + MAP_SUBDIR + mapPath);

    for (const auto& tileSet : m_tileSets)
        success = success && assertFileExists(m_gameDataPath + "/" + IMG_SUBDIR + tileSet);

    for (const auto& spriteSheet : m_spriteSheets)
        success = success && assertFileExists(m_gameDataPath + "/" + IMG_SUBDIR + spriteSheet);

    return success;
}

bool GameStatic::assertFileExists(const std::string& path)
{
    std::ifstream f(path.c_str());
    if (f.good()) {
        return true;
    } else {
        LogErr("Missing file: " + path);
        return false;
    }
}

void GameStatic::cleanupUnused()
{
    std::set<event_id> usedEventsIds;

    // Check usages
    //
    // Check all maps for events and tilesheets
    for (const auto& mapName : m_mapsNames) {
        auto pMap    = std::make_unique<Dummy::Map>();
        auto mapPath = m_gameDataPath + "/" + MAP_SUBDIR + mapName + MAP_FILE_EXT;
        std::ifstream mapDataFile(mapPath, std::ios::binary);
        bool bRes = Dummy::Serializer::parseMapFromFile(mapDataFile, *pMap);
        if (! bRes) {
            LogErr("Error while loading the map " + mapPath);
            continue;
        }
        // check use of events
        for (const auto& floor : pMap->floors())
            // TODO later : check use for other kind of event source than NPC
            for (const auto& npc : floor->npcs())
                extendUsageEvents(usedEventsIds, npc.eventId());
    }

    // Clean up
    //
    // Clean up events
    for (auto it = m_events.cbegin(); it != m_events.cend();) {
        if (usedEventsIds.find(it->first) == usedEventsIds.end())
            it = m_events.erase(it);
        else
            ++it;
    }
}

void GameStatic::extendUsageEvents(std::set<event_id>& usedEvents, event_id id)
{
    if (usedEvents.find(id) != usedEvents.end())
        return; // already marked as "used", nothing to do

    usedEvents.insert(id);
    auto type = m_events[id].type;

    if (type == EventType::Dialog) {
        const auto& it = m_dialogs.find(id);
        if (it == m_dialogs.end())
            return;

        extendUsageEvents(usedEvents, it->second.nextEvent());

    } else if (type == EventType::Choice) {
        const auto& it = m_dialogsChoices.find(id);
        if (it == m_dialogsChoices.end())
            return;

        size_t nbOpt = it->second.nbOptions();
        for (uint8_t opt = 0; opt < nbOpt; ++opt)
            extendUsageEvents(usedEvents, it->second.optionAt(opt).nextEvent);
    }
    // TODO other type of events
}

///////////////////////////////////////////////////////////////////////////////

GameInstance::GameInstance(const GameStatic& game)
    : m_game(game)
{}

bool GameInstance::registerPlayer(const PlayerInstance& player)
{
    m_player = player;
    // Some check to do to return false sometimes ?
    return true;
}

void GameInstance::movePlayer(Direction dir)
{
    const auto* floor = currentFloor();
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

void GameInstance::stopPlayer()
{
    if (m_player.m_pos.state == Dummy::CharState::Walking)
        m_player.m_pos.state = Dummy::CharState::Idle;
}

void GameInstance::interractInFrontOfPlayer()
{
    Coord target        = m_player.pos().coord;
    Direction dirTarget = Direction::Top; // where the NPC should look at
    const Direction dir = m_player.pos().dir;

    if (dir == Direction::Top) {
        --target.y; // we don't really care about overflow, an empty cell will be met anyway
        dirTarget = Direction::Bottom;
    } else if (dir == Direction::Left) {
        --target.x;
        dirTarget = Direction::Right;
    } else if (dir == Direction::Bottom) {
        ++target.y;
        dirTarget = Direction::Top;
    } else if (dir == Direction::Right) {
        ++target.x;
        dirTarget = Direction::Left;
    }

    auto* floor = currentFloor();
    if (floor == nullptr)
        return;

    for (auto& npc : floor->m_npcs) {
        if (npc.pos().coord == target) {
            npc.m_pos.dir = dirTarget;
            registerEvent(npc.eventId());
        }
    }
}

void GameInstance::blockEvents(bool block)
{
    m_eventsBlocked = block;
}

void GameInstance::registerEvent(event_id eventId)
{
    m_eventsQueue.push_back(eventId);
}

event_id GameInstance::dequeEvent()
{
    event_id event = undefEvent;
    if (! m_eventsBlocked && m_eventsQueue.size() > 0) {
        event = m_eventsQueue.front();
        m_eventsQueue.pop_front();
    }
    return event;
}

bool GameInstance::eventBlocked() const
{
    return m_eventsBlocked;
}

void GameInstance::setCurrentMap(const std::shared_ptr<Map>& pMap)
{
    m_currMap = pMap;
}

const PlayerInstance& GameInstance::player() const
{
    return m_player;
}

const Dummy::Map* GameInstance::currentMap() const
{
    return m_currMap.get();
}

const Dummy::Floor* GameInstance::currentFloor() const
{
    auto* map = currentMap();
    if (map == nullptr)
        return nullptr;

    return currentMap()->floorAt(m_currFloor);
}

Dummy::Floor* GameInstance::currentFloor()
{
    auto* map = currentMap();
    if (map == nullptr)
        return nullptr;

    return currentMap()->floorAt(m_currFloor);
}

} // namespace Dummy
