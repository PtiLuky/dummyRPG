#include "RenderGame.hpp"

#include <iostream>

#include "dummyrpg/floor.hpp"

namespace DummyPlayer {

GameRender::GameRender(const Dummy::GameStatic& game, const Dummy::GameInstance& instance,
                       sf::RenderWindow& window)
    : m_window(window)
    , m_game(game)
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
        std::cerr << "Error : MapRender creation failed (" << e.what() << ")." << std::endl;
    }

    changeFloor(m_gameInstance.Player().Pos().floorId);
}

void GameRender::changeFloor(uint8_t floorId)
{
    auto& playerPos = m_gameInstance.Player().Pos();
    auto* floor     = m_game.maps[playerPos.mapId].floorAt(floorId);
    if (floor == nullptr)
        return;

    try {
        // Player
        auto& spriteP   = m_game.sprites[m_gameInstance.Player().SpriteId()];
        auto& positionP = m_gameInstance.Player().Pos();
        m_npcRenders.push_back(std::make_unique<CharacterRender>(spriteP, positionP, *this));

        // NPC
        for (auto& npc : floor->npcs()) {
            auto& charac   = m_game.characters[npc.characterId()];
            auto& sprite   = m_game.sprites[charac.spriteId()];
            auto& position = npc.Pos();
            m_npcRenders.push_back(std::make_unique<CharacterRender>(sprite, position, *this));
        }
    } catch (const CharacterRenderError& e) {
        std::cerr << "Error : Character Render creation failed (" << e.what() << ")." << std::endl;
    }
}

void GameRender::render()
{
    auto& playerPos = m_gameInstance.Player().Pos();
    m_mapOffset.x =
        static_cast<int>(m_window.getSize().x / 2 - playerPos.coord.x * m_zoom * Dummy::TILE_SIZE);
    m_mapOffset.y =
        static_cast<int>(m_window.getSize().y / 2 - playerPos.coord.y * m_zoom * Dummy::TILE_SIZE);

    auto characOrder = getCharacterDrawOrder();

    m_window.clear();

    if (m_mapRender)
        m_mapRender->renderBelow(m_window, playerPos.floorId);

    for (auto& characIdx : characOrder) {
        // Note: do we want to display NPC of inferior floors? If yes, we should handle that another
        // way
        m_npcRenders[characIdx]->render(m_window);
    }

    if (m_mapRender)
        m_mapRender->renderAbove(m_window, playerPos.floorId);

    m_window.display();
}

std::vector<uint32_t> GameRender::getCharacterDrawOrder()
{
    std::vector<uint32_t> order;
    uint32_t nbCharac = static_cast<uint32_t>(m_npcRenders.size());

    // Poor ordering complexity, can be optimized
    for (uint32_t i = 0; i < nbCharac; ++i) {
        uint32_t insertIdx;
        for (insertIdx = 0; insertIdx < i; ++insertIdx)
            if (m_npcRenders[i]->Pos().coord.y <= m_npcRenders[insertIdx]->Pos().coord.y)
                break;
        order.insert(order.begin() + insertIdx, i);
    }

    return order;
}

} // namespace DummyPlayer
