#include "dummyrpg/serialize.hpp"

#include <algorithm>

#include "dummyrpg/floor.hpp"
#include "dummyrpg/game.hpp"

/// You can find a more detailled explanation in the file /doc/file-format.md
/// A game is splitted in 2 files : the game-definition file and a game-instance file
/// Each file is a binary file
/// File contains several sections each section has a specific tag


static const uint32_t FILE_SIGNATURE_GDUMMY = 0x00ca3e00; // .gdummy file must begin with this
static const uint32_t FILE_SIGNATURE_SDUMMY = 0x005afe00; // .sdummy file must begin with this

// we use "dd" before to have something more distinguishable than "0"
static const uint16_t TAG_NAME    = 0xdd01;
static const uint16_t TAG_VERSION = 0xdd02;

static const uint16_t TAG_END_OF_CONTENT = 0xddee;
static const uint16_t TAG_END_OF_HEADER  = 0xddff;

static const uint16_t TAG_CHIPSET = 0xdd05;

static const uint16_t TAG_MAP            = 0xdd10;
static const uint16_t TAG_FLOOR          = 0xdd11;
static const uint16_t TAG_ITEM           = 0xdd12;
static const uint16_t TAG_CHARACTER      = 0xdd13;
static const uint16_t TAG_MONSTER        = 0xdd14;
static const uint16_t TAG_SPRITE_SHEET   = 0xdd15;
static const uint16_t TAG_DYNAMIC_SPRITE = 0xdd16;

static const uint16_t TAG_EVENT        = 0xdd30;
static const uint16_t TAG_EVENT_DIALOG = 0xdd31;
static const uint16_t TAG_EVENT_CHOICE = 0xdd32;

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

bool Serializer::serializeGameToFile(const GameStatic& game, std::ostream& out)
{
    if (! out.good())
        return false;

    // Header
    write4B(out, FILE_SIGNATURE_GDUMMY);
    writeHeader(game, out);

    // Chipsets
    for (auto& chipPath : game.m_tileSets)
        writeStrElem(out, chipPath, TAG_CHIPSET);

    // Maps
    for (auto& map : game.m_mapsNames)
        writeStrElem(out, map, TAG_MAP);

    // Items
    for (auto& item : game.m_items)
        writeItem(out, item.second);

    // Events
    for (auto& e : game.m_events)
        writeEvent(out, e.second);
    for (auto& d : game.m_dialogs)
        writeEventDialog(out, d.second);
    for (auto& c : game.m_dialogsChoices)
        writeEventChoice(out, c.second);

    // Characters
    for (auto& charac : game.m_characters)
        writeCharacter(out, charac.second);

    // Monsters
    for (auto& monster : game.m_monsters)
        writeMonster(out, monster.second);

    // Sprites
    for (auto& sheet : game.m_spriteSheets)
        writeStrElem(out, sheet, TAG_SPRITE_SHEET);
    for (auto& sprite : game.m_sprites)
        writeSprite(out, sprite.second);

    // File end of content tag
    write2B(out, TAG_END_OF_CONTENT);

    // File signature
    write4B(out, FILE_SIGNATURE_GDUMMY);

    return out.good();
}

bool Serializer::serializeMapToFile(const Map& game, std::ostream& out)
{
    // TODO return an error value to have details on the failure
    if (! out.good())
        return false;

    // Header
    write4B(out, FILE_SIGNATURE_GDUMMY);

    writeMap(out, game);

    // File signature
    write4B(out, FILE_SIGNATURE_GDUMMY);

    return out.good();
}

///////////////////////////////////////////////////////////////////////////////

void Serializer::write1B(std::ostream& out, uint8_t val)
{
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
}

void Serializer::write2B(std::ostream& out, uint16_t val)
{
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
}

void Serializer::write4B(std::ostream& out, uint32_t val)
{
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
}

void Serializer::write8B(std::ostream& out, uint64_t val)
{
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
}

void Serializer::writeCurve(std::ostream& out, const Curve& val)
{
    write1B(out, val.p0);
    write1B(out, val.p1);
    write1B(out, val.p2);
    write1B(out, val.p3);
    write1B(out, val.p4);
}

void Serializer::writeStr(std::ostream& out, const std::string& val)
{
    size_t maxLength = std::numeric_limits<uint32_t>::max();
    uint32_t size    = static_cast<uint32_t>(std::min(maxLength, val.size()));
    write4B(out, size);
    out.write(val.data(), size);
}

template <typename T> void Serializer::writeLayer(std::ostream& out, const Layer<T>& layer)
{
    out.write(layer.data(), static_cast<std::streamsize>(layer.size() * sizeof(T)));
}

void Serializer::writeHeader(const GameStatic& game, std::ostream& out)
{
    write2B(out, TAG_NAME);
    writeStr(out, game.m_name);
    write2B(out, TAG_VERSION);
    write8B(out, game.version);
    write2B(out, TAG_END_OF_HEADER);
}

void Serializer::writeStrElem(std::ostream& out, const std::string& str, uint16_t tag)
{
    write2B(out, tag);
    writeStr(out, str);
}

void Serializer::writeMap(std::ostream& out, const Map& map)
{
    write2B(out, TAG_MAP);

    write2B(out, map.width());
    write2B(out, map.height());

    write1B(out, static_cast<uint8_t>(map.chipsetsUsed().size()));
    for (uint8_t chipId : map.chipsetsUsed())
        write1B(out, chipId);
    // floors
    write1B(out, static_cast<uint8_t>(map.floors().size()));
    for (const auto& floor : map.floors()) {
        write2B(out, TAG_FLOOR);
        // blocking layer
        writeLayer(out, floor->blockingLayer());
        // graphic layers
        write1B(out, static_cast<uint8_t>(floor->graphicLayers().size()));
        for (const auto& layer : floor->graphicLayers())
            writeLayer(out, layer);

        // NPC
        for (const auto& npc : floor->m_npcs) {
            write2B(out, TAG_CHARACTER);
            write4B(out, npc.characterId());
            write4B(out, npc.eventId());
            write2B(out, npc.pos().coord.x);
            write2B(out, npc.pos().coord.y);
            write1B(out, static_cast<uint8_t>(npc.pos().dir));
            write1B(out, static_cast<uint8_t>(npc.pos().state));
        }
    }
}

void Serializer::writeItem(std::ostream& out, const Item& item)
{
    write2B(out, TAG_ITEM);
    write2B(out, item.m_id);
    writeStr(out, item.m_name);
    write2B(out, item.m_spriteId);
}

void Serializer::writeCharacter(std::ostream& out, const Character& character)
{
    write2B(out, TAG_CHARACTER);
    write4B(out, character.m_id);
    writeStr(out, character.m_name);
    write2B(out, character.m_spriteId);
}

void Serializer::writeMonster(std::ostream& out, const Monster& monster)
{
    write2B(out, TAG_MONSTER);
    write2B(out, monster.m_id);
    writeStr(out, monster.m_name);
    write2B(out, monster.m_spriteId);
    writeCurve(out, monster.m_attacks);
    writeCurve(out, monster.m_defense);
    writeCurve(out, monster.m_hp);
}

void Serializer::writeSprite(std::ostream& out, const AnimatedSprite& sprite)
{
    write2B(out, TAG_DYNAMIC_SPRITE);
    write2B(out, sprite.id);
    write2B(out, sprite.spriteSheetId);
    write2B(out, sprite.x);
    write2B(out, sprite.y);
    write2B(out, sprite.width);
    write2B(out, sprite.height);
    write1B(out, sprite.has4Directions ? 1 : 0);
    write1B(out, sprite.nbFrames);

    write2B(out, sprite.x2);
    write2B(out, sprite.y2);
    write1B(out, sprite.nbFrames2);

    write2B(out, sprite.x3);
    write2B(out, sprite.y3);
    write1B(out, sprite.nbFrames3);

    write2B(out, sprite.x4);
    write2B(out, sprite.y4);
    write1B(out, sprite.nbFrames4);
}

void Serializer::writeEvent(std::ostream& out, const Event& e)
{
    write2B(out, TAG_EVENT);
    write4B(out, e.id);
    write1B(out, static_cast<uint8_t>(e.type));
}

void Serializer::writeEventChoice(std::ostream& out, const DialogChoice& c)
{
    write2B(out, TAG_EVENT_CHOICE);
    write4B(out, c.id());
    writeStr(out, c.m_question);
    write1B(out, static_cast<uint8_t>(c.m_options.size()));
    for (auto& opt : c.m_options) {
        writeStr(out, opt.option);
        write4B(out, opt.nextEvent);
    }
}

void Serializer::writeEventDialog(std::ostream& out, const DialogSentence& d)
{
    write2B(out, TAG_EVENT_DIALOG);
    write4B(out, d.m_id);
    write4B(out, d.m_speaker);
    writeStr(out, d.m_sentence);
    write4B(out, d.m_nextEvent);
}

///////////////////////////////////////////////////////////////////////////////

bool Serializer::parseGameFromFile(std::istream& in, GameStatic& game)
{
    // TODO return an error value to have details on the failure
    if (! in.good())
        return false;

    // Header
    if (! readHeader(in, game))
        return false;

    // Content
    bool contentDone = false;
    while (in.good() && ! contentDone) {
        uint16_t tag = read2B(in);
        if (tag == TAG_END_OF_CONTENT) {
            contentDone = true;
        } else if (tag == TAG_CHIPSET) {
            game.m_tileSets.push_back(readStr(in));
        } else if (tag == TAG_MAP) {
            game.m_mapsNames.push_back(readStr(in));
        } else if (tag == TAG_ITEM) {
            Item i = readItem(in);
            game.m_items.insert({i.m_id, i});
        } else if (tag == TAG_EVENT) {
            Event e = readEvent(in);
            game.m_events.insert({e.id, e});
        } else if (tag == TAG_EVENT_CHOICE) {
            DialogChoice c = readEventChoice(in);
            game.m_dialogsChoices.insert({c.m_id, c});
        } else if (tag == TAG_EVENT_DIALOG) {
            DialogSentence d = readEventDialog(in);
            game.m_dialogs.insert({d.m_id, d});
        } else if (tag == TAG_CHARACTER) {
            Character c = readCharacter(in);
            game.m_characters.insert({c.m_id, c});
        } else if (tag == TAG_MONSTER) {
            Monster m = readMonster(in);
            game.m_monsters.insert({m.m_id, m});
        } else if (tag == TAG_SPRITE_SHEET) {
            game.m_spriteSheets.push_back(readStr(in));
        } else if (tag == TAG_DYNAMIC_SPRITE) {
            AnimatedSprite s = readSprite(in);
            game.m_sprites.insert({s.id, s});
        } else
            return false;
    }

    if (read4B(in) != FILE_SIGNATURE_GDUMMY)
        return false;

    return true;
} // namespace Dummy

bool Serializer::parseMapFromFile(std::istream& in, Map& map)
{
    // TODO return an error value to have details on the failure
    if (! in.good())
        return false;

    if (read4B(in) != FILE_SIGNATURE_GDUMMY)
        return false;

    if (! readMap(in, map))
        return false;

    if (read4B(in) != FILE_SIGNATURE_GDUMMY)
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////

uint8_t Serializer::read1B(std::istream& in)
{
    uint8_t val = 0;
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

uint16_t Serializer::read2B(std::istream& in)
{
    uint16_t val = 0;
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

uint32_t Serializer::read4B(std::istream& in)
{
    uint32_t val = 0;
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

uint64_t Serializer::read8B(std::istream& in)
{
    uint64_t val = 0;
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

Curve Serializer::readCurve(std::istream& in)
{
    Curve val;
    val.p0 = read1B(in);
    val.p1 = read1B(in);
    val.p2 = read1B(in);
    val.p3 = read1B(in);
    val.p4 = read1B(in);
    return val;
}

std::string Serializer::readStr(std::istream& in)
{
    size_t length = read4B(in);
    std::string val;
    val.resize(length);
    in.read(&val[0], static_cast<std::streamsize>(length));
    return val;
}

template <typename T> void Serializer::readLayer(std::istream& in, Layer<T>& layer)
{
    in.read(layer.data(), static_cast<std::streamsize>(layer.size() * sizeof(T)));
}

bool Serializer::readHeader(std::istream& in, GameStatic& game)
{
    if (read4B(in) != FILE_SIGNATURE_GDUMMY)
        return false;

    while (in.good()) {
        uint16_t temp2B = read2B(in);
        switch (temp2B) {
        case TAG_NAME:
            game.m_name = readStr(in);
            break;
        case TAG_VERSION:
            game.version = read8B(in);
            break;
        case TAG_END_OF_HEADER:
            return true;
        default:
            return false;
        }
    }

    return false; // exited because of !in.good() without having TAG_END_OF_HEADER
}

bool Serializer::readMap(std::istream& in, Map& map)
{
    if (read2B(in) != TAG_MAP)
        return false;

    uint16_t w = read2B(in);
    uint16_t h = read2B(in);

    map.reset(w, h, 0);

    map.m_chipsets.resize(read1B(in));
    for (auto& chipId : map.m_chipsets)
        chipId = read1B(in);

    map.m_floors.resize(read1B(in));
    for (auto& pFloor : map.m_floors) {
        if (read2B(in) != TAG_FLOOR)
            return false;

        pFloor.reset(new Floor(w, h));
        readLayer(in, pFloor->m_blockingLayer);

        uint8_t nbGraphLayers = read1B(in);
        for (size_t i = 0; i < nbGraphLayers; ++i) {
            if (i >= pFloor->m_layers.size())
                pFloor->addLayerAbove();
            readLayer(in, pFloor->m_layers[i]);
        }

        auto posBeforeTag = in.tellg();
        uint16_t tag      = read2B(in);
        while (tag == TAG_CHARACTER) {
            uint32_t id    = read4B(in);
            uint32_t event = read4B(in);
            PositionChar pos;
            pos.coord.x = read2B(in);
            pos.coord.y = read2B(in);
            pos.dir     = static_cast<Direction>(read1B(in));
            pos.state   = static_cast<CharState>(read1B(in));
            CharacterInstance chara(id, pos);
            chara.setEvent(event);
            pFloor->m_npcs.push_back(chara);

            posBeforeTag = in.tellg();
            tag          = read2B(in);
        }
        in.seekg(posBeforeTag);
    }
    return true;
}

Item Serializer::readItem(std::istream& in)
{
    Item item(0, "", 0);
    item.m_id       = read2B(in);
    item.m_name     = readStr(in);
    item.m_spriteId = read2B(in);
    return item;
}

Character Serializer::readCharacter(std::istream& in)
{
    Character charac(0, "", 0);
    charac.m_id       = read4B(in);
    charac.m_name     = readStr(in);
    charac.m_spriteId = read2B(in);
    return charac;
}

Monster Serializer::readMonster(std::istream& in)
{
    Monster monster(0, "", 0);
    monster.m_id       = read2B(in);
    monster.m_name     = readStr(in);
    monster.m_spriteId = read2B(in);
    monster.m_attacks  = readCurve(in);
    monster.m_defense  = readCurve(in);
    monster.m_hp       = readCurve(in);
    return monster;
}

AnimatedSprite Serializer::readSprite(std::istream& in)
{
    AnimatedSprite sprite;
    sprite.id             = read2B(in);
    sprite.spriteSheetId  = read2B(in);
    sprite.x              = read2B(in);
    sprite.y              = read2B(in);
    sprite.width          = read2B(in);
    sprite.height         = read2B(in);
    sprite.has4Directions = read1B(in);
    sprite.nbFrames       = read1B(in);

    sprite.x2        = read2B(in);
    sprite.y2        = read2B(in);
    sprite.nbFrames2 = read1B(in);

    sprite.x3        = read2B(in);
    sprite.y3        = read2B(in);
    sprite.nbFrames3 = read1B(in);

    sprite.x4        = read2B(in);
    sprite.y4        = read2B(in);
    sprite.nbFrames4 = read1B(in);
    return sprite;
}

Event Serializer::readEvent(std::istream& in)
{
    Event e;
    e.id   = read4B(in);
    e.type = static_cast<EventType>(read1B(in));
    return e;
}

DialogSentence Serializer::readEventDialog(std::istream& in)
{
    DialogSentence d(0, 0, "");
    d.m_id        = read4B(in);
    d.m_speaker   = read4B(in);
    d.m_sentence  = readStr(in);
    d.m_nextEvent = read4B(in);
    return d;
}

DialogChoice Serializer::readEventChoice(std::istream& in)
{
    DialogChoice c(0, "");
    c.m_id       = read4B(in);
    c.m_question = readStr(in);
    c.m_options.resize(read1B(in));
    for (auto& opt : c.m_options) {
        opt.option    = readStr(in);
        opt.nextEvent = read4B(in);
    }
    return c;
}

} // namespace Dummy
