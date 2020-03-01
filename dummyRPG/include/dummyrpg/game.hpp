#ifndef DUMMYRPG_GAME
#define DUMMYRPG_GAME

#include <map>

#include "dummyrpg/map.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct Game
{
    std::map<uint16_t, std::unique_ptr<Map>> m_maps;
    std::map<uint8_t, std::string> m_chipsetPaths;
};

} // namespace Dummy


#endif
