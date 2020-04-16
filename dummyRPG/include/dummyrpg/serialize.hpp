#ifndef DUMMYRPG_SERIALIZE_HPP
#define DUMMYRPG_SERIALIZE_HPP

#include <iostream>

#include "dummyrpg/floor.hpp"
#include "dummyrpg/map.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

struct GameStaticData;
struct GameInstanceData;

///////////////////////////////////////////////////////////////////////////////

class Serializer
{
public:
    static bool SerializeGameToFile(const GameStaticData&, std::ostream&);
    static bool SerializeSaveToFile(const GameInstanceData&, std::ostream&);

    static bool ParseGameFromFile(std::istream&, GameStaticData&);
    static bool ParseSaveFromFile(std::istream&, GameInstanceData&);

private:
    static uint8_t Serializer::read1B(std::istream& in);
    static uint16_t Serializer::read2B(std::istream& in);
    static uint32_t Serializer::read4B(std::istream& in);
    static uint64_t Serializer::read8B(std::istream& in);
    static Curve Serializer::readCurve(std::istream& in);
    static std::string Serializer::readStr(std::istream& in);
    static bool readMap(std::istream& in, Map&);
    template <typename T> static void readLayer(std::istream& in, Layer<T>& layer);

    static void write1B(std::ostream& out, uint8_t val);
    static void write2B(std::ostream& out, uint16_t val);
    static void write4B(std::ostream& out, uint32_t val);
    static void write8B(std::ostream& out, uint64_t val);
    static void writeCurve(std::ostream& out, const Curve& val);
    static void writeStr(std::ostream& out, const std::string& val);
    static void writeMap(std::ostream& out, const Map& val);
    template <typename T> static void writeLayer(std::ostream& out, const Layer<T>& layer);
};

} // namespace Dummy



#endif // DUMMYRPG_SERIALIZE_HPP
