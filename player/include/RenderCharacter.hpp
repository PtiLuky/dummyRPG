#ifndef DUMMYRPG_CHARACTERRENDER_HPP
#define DUMMYRPG_CHARACTERRENDER_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

#include "dummyrpg/game.hpp"

namespace DummyPlayer {
class GameRender;
///////////////////////////////////////////////////////////////////////////////

/// \brief MapRenderError: some error related to the MapRendering
class CharacterRenderError : public std::runtime_error
{
public:
    explicit CharacterRenderError(const std::string& msg)
        : std::runtime_error(msg)
    {}
};

///////////////////////////////////////////////////////////////////////////////

class PlayerRender
{
public:
    explicit PlayerRender(const Dummy::PlayerInstance&, const GameRender&);

    void render(sf::RenderWindow&);

private:
    const Dummy::PlayerInstance& m_playerRef;
    const Dummy::AnimatedSprite& m_spriteRef;

    const GameRender& m_gameRender;

    sf::Texture m_texture;
    sf::Sprite m_sprite;

    Dummy::CharState m_lastState = Dummy::CharState::Idle;
    uint8_t m_lastFrame          = 0;
};
} // namespace DummyPlayer

#endif // DUMMYRPG_CHARACTERRENDER_HPP
