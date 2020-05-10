#include "RenderGame.hpp"

#include <iostream>

#include "dummyrpg/floor.hpp"

namespace DummyPlayer {

GameRender::GameRender(const Dummy::GameStatic& game, const Dummy::GameInstance& instance)
    : m_game(game)
    , m_gameInstance(instance)
{}

const Dummy::GameStatic& GameRender::game() const
{
    return m_game;
}

const sf::Vector2i& GameRender::offset() const
{
    return m_mapOffset;
}

sf::Vector2f GameRender::itemPxPos(Dummy::Coord pos) const
{
    return {
        m_mapOffset.x + pos.x * Dummy::TILE_SIZE * m_zoom,
        m_mapOffset.y + pos.y * Dummy::TILE_SIZE * m_zoom,
    };
}

float GameRender::zoom() const
{
    return m_zoom;
}

void GameRender::setMap(const Dummy::Map& map)
{
    try {
        m_mapRender = std::make_unique<MapRender>(map, *this);
    } catch (const MapRenderError& e) {
        Dummy::LogErr(std::string("Error : MapRender creation failed (") + e.what() + ").");
        return;
    }

    changeFloor(map, 0);
}

void GameRender::changeFloor(const Dummy::Map& map, uint8_t floorId)
{
    const auto* floor = map.floorAt(floorId);
    if (floor == nullptr)
        return;

    try {
        // Player
        auto* spritePlayer = m_game.sprite(m_gameInstance.player().spriteId());
        if (spritePlayer != nullptr) {
            auto& positionP = m_gameInstance.player().pos();
            m_npcRenders.push_back(
                std::make_unique<CharacterRender>(*spritePlayer, positionP, *this));
        }

        // NPC
        for (auto& npc : floor->npcs()) {
            auto& charac   = m_game.characters()[npc.characterId()];
            auto& sprite   = m_game.sprites()[charac.spriteId()];
            auto& position = npc.pos();
            m_npcRenders.push_back(std::make_unique<CharacterRender>(sprite, position, *this));
        }
    } catch (const CharacterRenderError& e) {
        Dummy::LogErr(std::string("Error : Character Render creation failed (") + e.what() + ").");
    }
}

void GameRender::render(sf::RenderWindow& window)
{
    auto& playerPos       = m_gameInstance.player().pos();
    const float tileToPix = m_zoom * Dummy::TILE_SIZE;
    m_mapOffset.x = static_cast<int>(window.getSize().x / 2 - playerPos.coord.x * tileToPix);
    m_mapOffset.y = static_cast<int>(window.getSize().y / 2 - playerPos.coord.y * tileToPix);

    auto characOrder = getCharacterDrawOrder();

    uint8_t floorIdx = 0;
    if (m_mapRender)
        m_mapRender->renderBelow(window, floorIdx);

    for (const auto& characIdx : characOrder) {
        // Note: do we want to display NPC of inferior floors? If yes, we should handle that another
        // way
        m_npcRenders[characIdx]->render(window);
    }

    if (m_mapRender)
        m_mapRender->renderAbove(window, floorIdx);
}

std::vector<uint32_t> GameRender::getCharacterDrawOrder()
{
    std::vector<uint32_t> order;
    uint32_t nbCharac = static_cast<uint32_t>(m_npcRenders.size());

    // Poor ordering complexity, can be optimized
    for (uint32_t i = 0; i < nbCharac; ++i) {
        uint32_t insertIdx = 0;
        for (insertIdx = 0; insertIdx < i; ++insertIdx)
            if (m_npcRenders[i]->pos().coord.y <= m_npcRenders[insertIdx]->pos().coord.y)
                break;
        order.insert(order.begin() + insertIdx, i);
    }

    return order;
}

} // namespace DummyPlayer
