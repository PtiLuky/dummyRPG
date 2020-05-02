#include "dummyrpg/game.hpp"
#include "dummyrpg/floor.hpp"

#include <fstream>
#include <iostream>

static const char* const MAP_SUBDIR    = "maps/";
static const char* const IMG_SUBDIR    = "images/";
static const char* const FONTS_SUBDIR  = "fonts/";  // unused
static const char* const SOUNDS_SUBDIR = "sounds/"; // unused

namespace Dummy {

bool GameStatic::checkFilesIntegrity() const
{
    bool success = true;

    for (const auto& mapPath : mapsNames)
        success = success && assertFileExists(m_gameDataPath + "/" + MAP_SUBDIR + mapPath);

    for (const auto& tileSet : tileSets)
        success = success && assertFileExists(m_gameDataPath + "/" + IMG_SUBDIR + tileSet);

    for (const auto& spriteSheet : spriteSheets)
        success = success && assertFileExists(m_gameDataPath + "/" + IMG_SUBDIR + spriteSheet);

    return success;
}

void GameStatic::setGameDataPath(const std::string& rootPath)
{
    m_gameDataPath = rootPath;
}

const std::string& GameStatic::gameDataPath() const
{
    return m_gameDataPath;
}

event_id GameStatic::registerDialog(const std::string& speaker, const std::string& sentence)
{
    uint32_t nextEventId = static_cast<uint32_t>(events.size());

    Dummy::DialogSentence dialog(speaker, sentence, nextEventId);

    uint32_t nextDialogId = static_cast<uint32_t>(dialogs.size());
    events.push_back({Dummy::EventType::Dialog, nextDialogId});

    dialogs.push_back(std::move(dialog));
    return nextEventId;
}

event_id GameStatic::registerChoice(const std::string& question)
{
    uint32_t nextEventId = static_cast<uint32_t>(events.size());

    Dummy::DialogChoice choice(question, nextEventId);

    uint32_t nextDialogId = static_cast<uint32_t>(dialogsChoices.size());
    events.push_back({Dummy::EventType::Choice, nextDialogId});

    dialogsChoices.push_back(choice);
    return nextEventId;
}

chip_id GameStatic::registerChipset(const std::string& chipPath)
{
    const size_t nbTileset = tileSets.size();
    if (nbTileset >= std::numeric_limits<chip_id>::max())
        return 0;

    for (chip_id i = 0; i < nbTileset; ++i)
        if (tileSets[i] == chipPath)
            return i;

    chip_id nextChipId = static_cast<chip_id>(nbTileset);
    tileSets.push_back(chipPath);
    return nextChipId;
}

sprite_id GameStatic::registerSpriteSheet(const std::string& sheetPath)
{
    const size_t nbSheets = spriteSheets.size();
    if (nbSheets >= std::numeric_limits<sprite_id>::max())
        return 0;

    for (sprite_id i = 0; i < nbSheets; ++i)
        if (spriteSheets[i] == sheetPath)
            return i;

    sprite_id nextSheetId = static_cast<sprite_id>(nbSheets);
    spriteSheets.push_back(sheetPath);
    return nextSheetId;
}

std::string GameStatic::spriteSheetPath(sprite_id id) const
{
    if (id >= spriteSheets.size())
        return "undefined";

    return m_gameDataPath + "/" + IMG_SUBDIR + spriteSheets[id];
}

std::string GameStatic::tileSetPath(chip_id id) const
{
    if (id >= tileSets.size())
        return "undefined";

    return m_gameDataPath + "/" + IMG_SUBDIR + tileSets[id];
}

bool GameStatic::assertFileExists(const std::string& path)
{
    std::ifstream f(path.c_str());
    if (f.good()) {
        return true;
    } else {
        std::cerr << "Missing file: " << path << std::endl;
        return false;
    }
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

    return currentMap()->floorAt(m_player.m_pos.floorId);
}

Dummy::Floor* GameInstance::currentFloor()
{
    auto* map = currentMap();
    if (map == nullptr)
        return nullptr;

    return currentMap()->floorAt(m_player.m_pos.floorId);
}

} // namespace Dummy
