#ifndef DUMMYRPG_DUMMYTYPES_HPP
#define DUMMYRPG_DUMMYTYPES_HPP

#include <cstdint>
#include <string>

namespace Dummy {
///////////////////////////////////////////////////////////////////////////////
/// Types definitions

/// The id of a chipset. A game has 255 chipsets max.
using chip_id = uint8_t;
/// The id of a sprite. A game has 65'535 sprite max.
using sprite_id = uint16_t;
/// The id of an item. A game has 65'535 items max.
using item_id = uint16_t;
/// The id of a monster. A game has 65'535 monsters max.
using monster_id = uint16_t;
/// The id of an character. A game has 4'294'967'295 charaters max.
using char_id = uint32_t;
/// The id of an event. A game has 4'294'967'295 events max.
using event_id = uint32_t;


/// Coord represents coordinates of a tile. As a pair of <x, y>
struct Coord
{
    uint16_t x = 0;
    uint16_t y = 0;
};

inline bool operator==(const Coord& lhs, const Coord& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

enum class Direction
{
    Top = 0,
    Right,
    Bottom,
    Left
};

enum class CharState
{
    Idle = 0,
    Walking,
    Attack,
    Dead,
};

enum class EventType : uint8_t
{
    Dialog = 0,
    Choice,
};

/// Position of a character/item on a
struct PositionChar
{
    Coord coord     = {0, 0};
    Direction dir   = Direction::Bottom;
    CharState state = CharState::Idle;
};

/// tilecoords represents coordinates of a chipset tile. As a tuple of <x, y, chipset_id>
struct Tileaspect
{
    uint8_t x      = 0;
    uint8_t y      = 0;
    chip_id chipId = 0;
};

inline bool operator==(const Tileaspect& lhs, const Tileaspect& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.chipId == rhs.chipId;
}

/// Attack curve, def curve, etc. This is a simplification with a 4-segments "curve"
struct Curve
{
    uint8_t p0 = 0;
    uint8_t p1 = 0;
    uint8_t p2 = 0;
    uint8_t p3 = 0;
    uint8_t p4 = 0;
};

struct Event
{
    EventType type;
    uint32_t idxPerType;
};


///////////////////////////////////////////////////////////////////////////////
/// Logger DECLARATIONS. Those methods must be implemented in a .cpp of your project
void Log(const std::string&);
void LogWarn(const std::string&);
void LogErr(const std::string&);

///////////////////////////////////////////////////////////////////////////////
/// CONST definitions

/// Default tile aspect value. Will always be considered as undefined.
const Tileaspect undefAspect {static_cast<uint8_t>(-1), static_cast<uint8_t>(-1),
                              static_cast<chip_id>(0)};

const char_id undefChar {static_cast<char_id>(-1)};
const sprite_id undefSprite {static_cast<sprite_id>(-1)};
const event_id undefEvent {static_cast<event_id>(-1)};
/// The size in px of a tile (on the chipset and on the map)
const int TILE_SIZE = 16;

} // namespace Dummy

#endif // DUMMYRPG_DUMMYTYPES_HPP
