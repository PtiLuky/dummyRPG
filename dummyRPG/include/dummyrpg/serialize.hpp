#ifndef DUMMYRPG_SERIALIZE_HPP
#define DUMMYRPG_SERIALIZE_HPP

#include <iostream>
#include <map>

#include "layer.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

struct AnimatedSprite;
struct Curve;

class Character;
class DialogChoice;
class DialogSentence;
class GameInstance;
class GameStatic;
class Item;
class Map;
class Monster;

template <typename T, typename TT> using umap = std::map<T, TT>;

///////////////////////////////////////////////////////////////////////////////

class Serializer
{
public:
    static bool serializeGameToFile(const GameStatic&, std::ostream&);
    static bool serializeMapToFile(const Map&, std::ostream&);

    static bool parseGameFromFile(std::istream&, GameStatic&);
    static bool parseMapFromFile(std::istream&, Map&);

private:
    // Read and return simple types
    static uint8_t read1B(std::istream&);
    static uint16_t read2B(std::istream&);
    static uint32_t read4B(std::istream&);
    static uint64_t read8B(std::istream&);
    static Curve readCurve(std::istream&);
    static std::string readStr(std::istream&);
    // Read and complete complexe types
    template <typename T> static void readLayer(std::istream&, Layer<T>&);
    static bool readHeader(std::istream&, GameStatic&);
    static bool readMap(std::istream&, Map&);
    static Item readItem(std::istream&);
    static Character readCharacter(std::istream&);
    static Monster readMonster(std::istream&);
    static AnimatedSprite readSprite(std::istream&);
    static Event readEvent(std::istream&);
    static DialogSentence readEventDialog(std::istream&);
    static DialogChoice readEventChoice(std::istream&);

    // Write simple types
    static void write1B(std::ostream&, uint8_t);
    static void write2B(std::ostream&, uint16_t);
    static void write4B(std::ostream&, uint32_t);
    static void write8B(std::ostream&, uint64_t);
    static void writeCurve(std::ostream&, const Curve&);
    static void writeStr(std::ostream&, const std::string&);
    // Write complexe types
    template <typename T> static void writeLayer(std::ostream&, const Layer<T>&);
    static void writeHeader(const GameStatic&, std::ostream&);
    static void writeStrElem(std::ostream&, const std::string&, uint16_t tag);
    static void writeMap(std::ostream&, const Map&);
    static void writeItem(std::ostream&, const Item&);
    static void writeCharacter(std::ostream&, const Character&);
    static void writeMonster(std::ostream&, const Monster&);
    static void writeSprite(std::ostream&, const AnimatedSprite&);
    static void writeEvent(std::ostream&, const Event&);
    static void writeEventDialog(std::ostream&, const DialogSentence&);
    static void writeEventChoice(std::ostream&, const DialogChoice&);
};

} // namespace Dummy



#endif // DUMMYRPG_SERIALIZE_HPP
