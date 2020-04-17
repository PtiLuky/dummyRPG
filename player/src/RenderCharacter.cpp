#include "RenderCharacter.hpp"

#include "RenderGame.hpp"

using Dummy::CharState;
using Dummy::Direction;

namespace DummyPlayer {

PlayerRender::PlayerRender(const Dummy::PlayerInstance& player, const GameRender& gameRender)
    : m_playerRef(player)
    , m_spriteRef(gameRender.game().sprites[player.spriteId])
    , m_gameRender(gameRender)
{
    if (! m_texture.loadFromFile(m_spriteRef.imgPath))
        throw CharacterRenderError("Could not load a sprite texture: " + m_spriteRef.imgPath);

    sf::IntRect rect(m_spriteRef.x, m_spriteRef.y, m_spriteRef.width, m_spriteRef.height);
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(gameRender.zoom(), gameRender.zoom());
    m_sprite.setTextureRect(rect);
}

void PlayerRender::render(sf::RenderWindow& renderWindow)
{
    // Update Position
    // real player offset
    sf::Vector2f spritePos = m_gameRender.itemPxPos(m_playerRef.pos);
    // moving the sprite to have feet in the cell
    spritePos.x += (Dummy::TILE_SIZE - m_spriteRef.width) * m_gameRender.zoom() / 2;
    spritePos.y += -m_spriteRef.height * m_gameRender.zoom() / 2;
    m_sprite.setPosition(spritePos);

    // Update State and animation
    ++m_lastFrame;
    if (m_playerRef.state != m_lastState) {
        m_lastState = m_playerRef.state;
        // we can interrupt walk with idle and then walk again
        if (m_playerRef.state != CharState::Walking)
            m_lastFrame = 0;
    }

    // Offset depending of the animation
    uint16_t spriteOffsetX = m_spriteRef.x;
    uint16_t spriteOffsetY = m_spriteRef.y;
    if (m_playerRef.state == CharState::Walking) {
        m_lastFrame = m_lastFrame % m_spriteRef.nbFrames;
    } else if (m_playerRef.state == CharState::Attack) {
        spriteOffsetX = m_spriteRef.x2;
        spriteOffsetY = m_spriteRef.y2;
        m_lastFrame   = m_lastFrame % m_spriteRef.nbFrames2;
    } else if (m_playerRef.state == CharState::Dead) {
        spriteOffsetX = m_spriteRef.x3;
        spriteOffsetY = m_spriteRef.y3;
        m_lastFrame   = m_lastFrame % m_spriteRef.nbFrames3;
    } else {
        m_lastFrame = 0;
    }
    // Offset depending of the direction
    uint8_t lineIdx = 0;
    if (m_playerRef.dir == Direction::Right)
        lineIdx = 1;
    else if (m_playerRef.dir == Direction::Bottom)
        lineIdx = 2;
    else if (m_playerRef.dir == Direction::Left)
        lineIdx = 3;
    // Offset depending of the frame
    spriteOffsetX += m_lastFrame * m_spriteRef.width;
    spriteOffsetY += lineIdx * m_spriteRef.height;

    sf::IntRect rect(spriteOffsetX, spriteOffsetY, m_spriteRef.width, m_spriteRef.height);
    m_sprite.setTextureRect(rect);

    // Draw
    renderWindow.draw(m_sprite);
}

} // namespace DummyPlayer
