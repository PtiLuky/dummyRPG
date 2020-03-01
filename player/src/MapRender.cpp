#include "MapRender.hpp"

#include <map>
#include <stdexcept>

#include "dummyrpg/floor.hpp"

namespace DummyPlayer {

MapRender::MapRender(const Dummy::Map& map, const Dummy::Game& game)
{
    // Load shader
    if (! m_mapShader.loadFromFile("Resources/tilemap.vert", "Resources/tilemap.frag"))
        throw std::runtime_error("Could not load shader programs");

    std::map<chip_id, uint8_t> chipIdToIdx;

    // Read chipsets, and keep the change of indices
    const int nbChips = static_cast<int>(map.chipsetsUsed().size());
    for (uint8_t i = 0; i < nbChips; ++i) {
        auto chipId = map.chipsetsUsed()[i];
        sf::Texture chipTex;
        std::string path = game.m_chipsetPaths.at(chipId);
        if (! chipTex.loadFromFile(path))
            throw std::runtime_error("Could not load a chipset texture: " + path);

        m_chipsets.push_back(chipTex);

        chipIdToIdx[chipId] = i;
    }

    // Send chipset parameters to shader
    m_mapShader.setUniform("tilemapSize", sf::Glsl::Vec2(map.width(), map.height()));
    m_mapShader.setUniform("chipCount", nbChips);
    if (nbChips >= 1) {
        m_mapShader.setUniform("chip0Tiles", m_chipsets[0]);
        auto size = m_chipsets[0].getSize();
        m_mapShader.setUniform("chip0PxSize", sf::Glsl::Vec2(size));
    }
    if (nbChips >= 2) {
        m_mapShader.setUniform("chip1Tiles", m_chipsets[1]);
        auto size = m_chipsets[1].getSize();
        m_mapShader.setUniform("chip1PxSize", sf::Glsl::Vec2(size));
    }
    if (nbChips >= 3) {
        m_mapShader.setUniform("chip2Tiles", m_chipsets[2]);
        auto size = m_chipsets[2].getSize();
        m_mapShader.setUniform("chip2PxSize", sf::Glsl::Vec2(size));
    }
    if (nbChips >= 4) {
        m_mapShader.setUniform("chip3Tiles", m_chipsets[3]);
        auto size = m_chipsets[3].getSize();
        m_mapShader.setUniform("chip3PxSize", sf::Glsl::Vec2(size));
    }

    // Decode each floor, layers below the player
    uint16_t nbLayersDone = 0;
    for (auto& floor : map.floors()) {
        auto& layers = floor->graphicLayers();
        m_firstLayerIdx.push_back(nbLayersDone);
        m_abovePlayerIdx.push_back(nbLayersDone + floor->INF_LAYERS_COUNT);

        const size_t nbLayers = layers.size();
        for (uint8_t i = 0; i < nbLayers; ++i) {
            addLayer(layers[i], m_floors, chipIdToIdx);
        }
        nbLayersDone += nbLayers;
    }

    m_mapSprite.setTexture(m_floors[0]);
    m_mapSprite.setScale(32, 32);
    m_mapSprite.setTextureRect(sf::IntRect(0, 0, 600, 600));
}

void MapRender::renderBelow(sf::RenderWindow& renderWindow, uint8_t playerFloor)
{
    if (playerFloor >= m_abovePlayerIdx.size())
        return;

    m_mutex.lock();

    const size_t nbLayersBelow = m_abovePlayerIdx[playerFloor];

    for (size_t fIdx = 0; fIdx < nbLayersBelow; ++fIdx) {
        m_mapShader.setUniform("tilemap", m_floors[fIdx]);
        renderWindow.draw(m_mapSprite, &m_mapShader);
    }
    m_mutex.unlock();
}

void MapRender::renderAbove(sf::RenderWindow& renderWindow, uint8_t playerFloor)
{
     if (playerFloor >= m_abovePlayerIdx.size())
         return;

     m_mutex.lock();

     const size_t nbLayersBelow = m_abovePlayerIdx[playerFloor];
     uint16_t nextFloor         = playerFloor;
     ++nextFloor;
     const size_t nextFloorLayer =
         (nextFloor >= m_firstLayerIdx.size()) ? m_floors.size() : m_firstLayerIdx[nextFloor];

     for (size_t fIdx = nbLayersBelow; fIdx < nextFloorLayer; ++fIdx) {
         m_mapShader.setUniform("tilemap", m_floors[fIdx]);
         renderWindow.draw(m_mapSprite, &m_mapShader);
     }
     m_mutex.unlock();
}

void MapRender::addLayer(const Dummy::GraphicLayer& layer, std::vector<sf::Texture>& floorList,
                         const std::map<chip_id, uint8_t>& idMapping)
{
    m_mutex.lock();

    uint16_t w = layer.width();
    uint16_t h = layer.height();

    sf::Image tileMapImage;
    tileMapImage.create(w, h);
    for (uint16_t y = 0; y < h; ++y) {
        for (uint16_t x = 0; x < w; ++x) {
            tileaspect val = layer.at({x, y});
            uint8_t chipx  = std::get<0>(val);
            uint8_t chipy  = std::get<1>(val);

            uint8_t chipId       = std::get<2>(val);
            uint8_t chipLocalIdx = 0;
            if (idMapping.find(chipId) != idMapping.end())
                chipLocalIdx = idMapping.at(std::get<2>(val));

            sf::Color tileIndex(chipx, chipy, chipLocalIdx);
            tileMapImage.setPixel(x, y, tileIndex);
        }
    }

    sf::Texture textu;
    textu.loadFromImage(tileMapImage);
    floorList.push_back(textu);
    m_mutex.unlock();
}

} // namespace DummyPlayer