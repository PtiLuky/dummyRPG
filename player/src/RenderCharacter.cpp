#include "RenderCharacter.hpp"

#include "RenderGame.hpp"

using Dummy::CharState;
using Dummy::Direction;

static const uint8_t LINE_TOP_VIEW    = 0;
static const uint8_t LINE_RIGHT_VIEW  = 1;
static const uint8_t LINE_BOTTOM_VIEW = 2;
static const uint8_t LINE_LEFT_VIEW   = 3;

namespace DummyPlayer {



CharacterRender::CharacterRender(const Dummy::AnimatedSprite& spriteRef,
                                 const Dummy::PositionChar& posRef, const GameRender& gameRender)
    : m_posRef(posRef)
    , m_spriteRef(spriteRef)
    , m_gameRender(gameRender)
{
    if (! m_texture.loadFromFile(m_spriteRef.imgPath))
        throw CharacterRenderError("Could not load a sprite texture: " + m_spriteRef.imgPath);

    sf::IntRect rect(m_spriteRef.x, m_spriteRef.y, m_spriteRef.width, m_spriteRef.height);
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(gameRender.zoom(), gameRender.zoom());
    m_sprite.setTextureRect(rect);
}

const Dummy::PositionChar& CharacterRender::Pos() const
{
    return m_posRef;
}

void CharacterRender::render(sf::RenderWindow& renderWindow)
{
    // Update Position
    // real player offset
    sf::Vector2f spritePos = m_gameRender.itemPxPos(m_posRef.coord);
    // moving the sprite to have feet in the cell
    spritePos.x += (Dummy::TILE_SIZE - m_spriteRef.width) * m_gameRender.zoom() / 2;
    spritePos.y += -m_spriteRef.height * m_gameRender.zoom() / 2;
    m_sprite.setPosition(spritePos);


    // Update State and animation
    auto& currState = m_posRef.state;
    auto& currDir   = m_posRef.dir;

    ++m_lastFrame;
    if (currState != m_lastState) {
        m_lastState = currState;
        // we can interrupt walk with idle and then walk again
        if (currState != CharState::Walking)
            m_lastFrame = 0;
    }

    // Offset depending of the animation
    uint16_t spriteOffsetX = m_spriteRef.x;
    uint16_t spriteOffsetY = m_spriteRef.y;
    if (currState == CharState::Walking) {
        m_lastFrame = m_lastFrame % m_spriteRef.nbFrames;
    } else if (currState == CharState::Attack) {
        spriteOffsetX = m_spriteRef.x2;
        spriteOffsetY = m_spriteRef.y2;
        m_lastFrame   = m_lastFrame % m_spriteRef.nbFrames2;
    } else if (currState == CharState::Dead) {
        spriteOffsetX = m_spriteRef.x3;
        spriteOffsetY = m_spriteRef.y3;
        m_lastFrame   = m_lastFrame % m_spriteRef.nbFrames3;
    } else {
        m_lastFrame = 0;
    }

    // Offset depending of the direction
    uint8_t lineIdx = LINE_TOP_VIEW;
    if (currDir == Direction::Right)
        lineIdx = LINE_RIGHT_VIEW;
    else if (currDir == Direction::Bottom)
        lineIdx = LINE_BOTTOM_VIEW;
    else if (currDir == Direction::Left)
        lineIdx = LINE_LEFT_VIEW;

    // Offset depending of the frame
    spriteOffsetX += m_lastFrame * m_spriteRef.width;
    spriteOffsetY += lineIdx * m_spriteRef.height;

    sf::IntRect rect(spriteOffsetX, spriteOffsetY, m_spriteRef.width, m_spriteRef.height);
    m_sprite.setTextureRect(rect);

    // Draw
    renderWindow.draw(m_sprite);
}

} // namespace DummyPlayer
