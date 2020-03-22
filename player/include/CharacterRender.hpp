#ifndef DUMMYRPG_CHARACTERRENDER_HPP
#define DUMMYRPG_CHARACTERRENDER_HPP

#include <SFML/Graphics.hpp>

#include "dummyrpg/character.hpp"

namespace DummyPlayer {}
class CharacterRender
{
public:
    explicit CharacterRender(const Dummy::Character&);

    void updatePosDir(const Dummy::Character&); ///< update position and direction

    void render(sf::RenderWindow&);
};
}

#endif // DUMMYRPG_CHARACTERRENDER_HPP
