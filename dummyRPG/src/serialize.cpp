#include "dummyrpg/serialize.hpp"

#include <algorithm>

#include "dummyrpg/game.hpp"

/// You can find a more detailled explanation in the file /doc/file-format.md
/// A game is splitted in 2 files : the game-definition file and a game-instance file
/// Each file is a binary file
/// File contains several sections each section has a specific tag


static const uint32_t FILE_SIGNATURE_GDUMMY = 0x00ca3e00; // .gdummy file must begin with this
static const uint32_t FILE_SIGNATURE_SDUMMY = 0x005afe00; // .sdummy file must begin with this

// we use "d" or "dd" before to have something more distinguishable than "0"
static const uint16_t TAG_NAME    = 0xdd01;
static const uint16_t TAG_VERSION = 0xdd02;

static const uint16_t TAG_MAP_COUNT            = 0xdd03;
static const uint16_t TAG_ITEM_COUNT           = 0xdd04;
static const uint16_t TAG_CHARACTER_COUNT      = 0xdd05;
static const uint16_t TAG_MONSTER_COUNT        = 0xdd06;
static const uint16_t TAG_EVENT_COUNT          = 0xdd07;
static const uint16_t TAG_CHIPSET_COUNT        = 0xdd08;
static const uint16_t TAG_STATIC_SPRITE_COUNT  = 0xdd09;
static const uint16_t TAG_DYNAMIC_SPRITE_COUNT = 0xdd0a;

static const uint16_t TAG_END_OF_CONTENT = 0xddee;
static const uint16_t TAG_END_OF_HEADER  = 0xddff;

static const uint16_t TAG_CHIPSETS = 0xd100;
static const uint16_t TAG_CHIPSET  = 0xd101;

static const uint16_t TAG_MAPS  = 0xd102;
static const uint16_t TAG_MAP   = 0xd103;
static const uint16_t TAG_FLOOR = 0xd104;

static const uint16_t TAG_ITEMS = 0xd110;
static const uint16_t TAG_ITEM  = 0xd111;

static const uint16_t TAG_CHARACTERS = 0xd112;
static const uint16_t TAG_CHARACTER  = 0xd113;

static const uint16_t TAG_MONSTERS = 0xd114;
static const uint16_t TAG_MONSTER  = 0xd115;

static const uint16_t TAG_DYNAMIC_SPRITES = 0xd121;
static const uint16_t TAG_DYNAMIC_SPRITE  = 0xd123;

static const uint16_t TAG_EVENTS = 0xd130;

namespace Dummy {

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
    for (auto& floor : map.floors()) {
        write2B(out, TAG_FLOOR);
        // blocking layer
        writeLayer(out, floor->blockingLayer());
        // graphic layers
        write1B(out, static_cast<uint8_t>(floor->graphicLayers().size()));
        for (auto& layer : floor->graphicLayers())
            writeLayer(out, layer);
    }
}
template <typename T> void Serializer::writeLayer(std::ostream& out, const Layer<T>& layer)
{
    out.write(layer.data(), static_cast<std::streamsize>(layer.size() * sizeof(T)));
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
    in.read(const_cast<char*>(layer.data()),
            static_cast<std::streamsize>(layer.size() * sizeof(T)));
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
        if (read2B(in) != TAG_MAP)
            return false;

        pFloor.reset(new Floor(w, h));
        readLayer(in, pFloor->m_blockingLayer);

        uint8_t nbGraphLayers = read1B(in);
        for (size_t i = 0; i < nbGraphLayers; ++i) {
            if (i >= pFloor->m_layers.size())
                pFloor->addLayerAbove();
            readLayer(in, pFloor->m_layers[i]);
        }
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////


bool Serializer::SerializeGameToFile(const GameStaticData& game, std::ostream& out)
{
    // TODO return an error value to have details on the failure
    if (! out.good())
        return false;

    // Header
    write4B(out, FILE_SIGNATURE_GDUMMY);

    write2B(out, TAG_NAME);
    writeStr(out, game.name);
    write2B(out, TAG_VERSION);
    write8B(out, game.version);
    write2B(out, TAG_MAP_COUNT);
    write2B(out, static_cast<uint16_t>(game.maps.size()));
    write2B(out, TAG_ITEM_COUNT);
    write2B(out, static_cast<uint16_t>(game.items.size()));
    write2B(out, TAG_CHARACTER_COUNT);
    write4B(out, static_cast<uint32_t>(game.characters.size()));
    write2B(out, TAG_MONSTER_COUNT);
    write4B(out, static_cast<uint32_t>(game.monsters.size()));
    // write2B(out, TAG_EVENT_COUNT);
    // write4B(out, static_cast<uint32_t>(game.monsters.size()));
    write2B(out, TAG_CHIPSET_COUNT);
    write1B(out, static_cast<uint8_t>(game.chipsetPaths.size()));
    write2B(out, TAG_DYNAMIC_SPRITE_COUNT);
    write2B(out, static_cast<uint16_t>(game.sprites.size()));
    write2B(out, TAG_END_OF_HEADER);

    // Chipsets
    write2B(out, TAG_CHIPSETS);
    for (auto& chipPath : game.chipsetPaths) {
        write2B(out, TAG_CHIPSET);
        writeStr(out, chipPath);
    }

    // Maps
    write2B(out, TAG_MAPS);
    for (auto& map : game.maps)
        writeMap(out, map);

    // Items
    write2B(out, TAG_ITEMS);
    for (auto& item : game.items) {
        write2B(out, TAG_ITEM);
        writeStr(out, item.name);
        write2B(out, item.spriteSheetId);
    }

    // Characters
    write2B(out, TAG_CHARACTERS);
    for (auto& charac : game.characters) {
        write2B(out, TAG_CHARACTER);
        writeStr(out, charac.name);
        write2B(out, charac.spriteSheetId);
    }

    // Monsters
    write2B(out, TAG_MONSTERS);
    for (auto& monster : game.monsters) {
        write2B(out, TAG_MONSTER);
        writeStr(out, monster.name);
        write2B(out, monster.spriteSheetId);
        writeCurve(out, monster.attacks);
        writeCurve(out, monster.defense);
        writeCurve(out, monster.hp);
    }

    // Sprites
    write2B(out, TAG_DYNAMIC_SPRITES);
    for (auto& sprite : game.sprites) {
        write2B(out, TAG_DYNAMIC_SPRITE);
        writeStr(out, sprite.imgPath);
        write2B(out, sprite.x);
        write2B(out, sprite.y);
        write2B(out, sprite.width);
        write2B(out, sprite.height);
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

    // File end of content tag
    write2B(out, TAG_END_OF_CONTENT);

    // File signature
    write4B(out, FILE_SIGNATURE_GDUMMY);

    return out.good();
}

bool Serializer::SerializeSaveToFile(const GameInstanceData& sav, std::ostream& out)
{
    return out.good();
}


bool Serializer::ParseGameFromFile(std::istream& in, GameStaticData& game)
{
    // TODO return an error value to have details on the failure
    if (! in.good())
        return false;

    // Header
    bool headerDone = false;
    if (read4B(in) != FILE_SIGNATURE_GDUMMY)
        return false;

    while (in.good() && ! headerDone) {
        uint16_t temp2B = read2B(in);
        switch (temp2B) {
        case TAG_NAME:
            game.name = readStr(in);
            break;
        case TAG_VERSION:
            game.version = read8B(in);
            break;
        case TAG_MAP_COUNT:
            game.maps.resize(read2B(in));
            break;
        case TAG_ITEM_COUNT:
            game.items.resize(read2B(in));
            break;
        case TAG_CHARACTER_COUNT:
            game.characters.resize(read4B(in));
            break;
        case TAG_MONSTER_COUNT:
            game.monsters.resize(read4B(in));
            break;
        // write2B(out, TAG_EVENT_COUNT);
        // read4B(out, static_cast<uint32_t>(game.monsters.size()));
        case TAG_CHIPSET_COUNT:
            game.chipsetPaths.resize(read1B(in));
            break;
        case TAG_DYNAMIC_SPRITE_COUNT:
            game.sprites.resize(read2B(in));
            break;
        case TAG_END_OF_HEADER:
            headerDone = true;
            break;
        default:
            return false;
        }
    }

    // Content
    bool contentDone = false;
    while (in.good() && ! contentDone) {
        uint16_t temp2B = read2B(in);
        switch (temp2B) {
        case TAG_END_OF_CONTENT:
            contentDone = true;
            break;
        case TAG_CHIPSETS:
            for (auto& chip : game.chipsetPaths) {
                if (read2B(in) != TAG_CHIPSET)
                    return false;
                chip = readStr(in);
            }
            break;
        case TAG_MAPS:
            for (auto& map : game.maps) {
                if (! readMap(in, map))
                    return false;
            }
            break;
        case TAG_ITEMS:
            for (auto& item : game.items) {
                if (read2B(in) != TAG_ITEM)
                    return false;
                item.name          = readStr(in);
                item.spriteSheetId = read2B(in);
            }
            break;
        case TAG_CHARACTERS:
            for (auto& charac : game.characters) {
                if (read2B(in) != TAG_CHARACTER)
                    return false;
                charac.name          = readStr(in);
                charac.spriteSheetId = read2B(in);
            }
            break;
        case TAG_MONSTERS:
            for (auto& monster : game.monsters) {
                if (read2B(in) != TAG_MONSTER)
                    return false;
                monster.name          = readStr(in);
                monster.spriteSheetId = read2B(in);
                monster.attacks       = readCurve(in);
                monster.defense       = readCurve(in);
                monster.hp            = readCurve(in);
            }
            break;
        case TAG_DYNAMIC_SPRITES:
            for (auto& sprite : game.sprites) {
                if (read2B(in) != TAG_DYNAMIC_SPRITE)
                    return false;
                sprite.imgPath  = readStr(in);
                sprite.x        = read2B(in);
                sprite.y        = read2B(in);
                sprite.width    = read2B(in);
                sprite.height   = read2B(in);
                sprite.nbFrames = read1B(in);

                sprite.x2        = read2B(in);
                sprite.y2        = read2B(in);
                sprite.nbFrames2 = read1B(in);

                sprite.x3        = read2B(in);
                sprite.y3        = read2B(in);
                sprite.nbFrames3 = read1B(in);

                sprite.x4        = read2B(in);
                sprite.y4        = read2B(in);
                sprite.nbFrames4 = read1B(in);
            }
            break;
        default:
            return false;
        }
    }

    if (read4B(in) != FILE_SIGNATURE_GDUMMY)
        return false;

    return contentDone && headerDone;
}

bool Serializer::ParseSaveFromFile(std::istream& in, GameInstanceData& sav)
{
    return false;
}

} // namespace Dummy