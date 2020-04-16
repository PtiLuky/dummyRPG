#include "CharacterRender.hpp"

#include "GameRender.hpp"

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
    auto offset = m_gameRender.offset();
    sf::Vector2f spriteOffset;
    // real player offset
    spriteOffset.x = offset.x + m_playerRef.pos.x * Dummy::TILE_SIZE * m_gameRender.zoom();
    spriteOffset.y = offset.y + m_playerRef.pos.y * Dummy::TILE_SIZE * m_gameRender.zoom();
    // moving the sprite to have feet in the cell
    spriteOffset.x += (Dummy::TILE_SIZE - m_spriteRef.width) * m_gameRender.zoom() / 2;
    spriteOffset.y += -m_spriteRef.height * m_gameRender.zoom() / 2;

    m_sprite.setPosition(spriteOffset);
    renderWindow.draw(m_sprite);
}

} // namespace DummyPlayer
