#include "MapRender.hpp"

#include <map>

#include "dummyrpg/floor.hpp"

///
/// The global idea behind this class is to encode all floor visible data into textures
/// those "data textures" are saved in m_tilemaps. They are encoded as following:
///  - original data is <chip X, chip Y, chip Id>
///  encoded to
///  - color data color(r = chip X ; g = chip Y ; b = chip Id) (color alpha is 255)
///
/// Then to render a floor, we need to use the tilemap shader, and send:
///  - the tilemap texture
///  - the count of chipset textures
///  - from 1 up to 4 chipset textures (the orginal chipset textures)
///
/// And the shader does the drawing stuff itself
///

namespace DummyPlayer {

MapRender::MapRender(const Dummy::Map& map, const Dummy::Game& game)
{
    // Load shader
    if (! m_mapShader.loadFromFile("Resources/tilemap.vert", "Resources/tilemap.frag"))
        throw MapRenderError("Could not load shader programs");

    std::map<chip_id, uint8_t> chipIdToIdx;

    // Read chipsets, and keep the change of indices
    const uint8_t nbChips = static_cast<uint8_t>(map.chipsetsUsed().size());
    for (uint8_t i = 0; i < nbChips; ++i) {
        chip_id chipId = map.chipsetsUsed()[i];
        sf::Texture chipTex;
        std::string path = game.m_chipsetPaths.at(chipId);
        if (! chipTex.loadFromFile(path))
            throw MapRenderError("Could not load a chipset texture: " + path);

        m_chipsets.push_back(chipTex);

        chipIdToIdx[chipId] = i;
    }

    // Send chipset parameters to shader
    m_mapShader.setUniform("tilemapSize", sf::Glsl::Vec2(map.width(), map.height()));
    m_mapShader.setUniform("chipCount", static_cast<int>(nbChips));

    for (uint8_t i = 0; i < nbChips; ++i) {
        std::string strIdx = std::to_string(i);
        sf::Vector2u size  = m_chipsets[i].getSize();

        m_mapShader.setUniform("chip" + strIdx + "Tiles", m_chipsets[i]);
        m_mapShader.setUniform("chip" + strIdx + "PxSize", sf::Glsl::Vec2(size));
    }


    const uint16_t w = map.width();
    const uint16_t h = map.height();
    const sf::Color neutralColor(255, 255, 0);

    // Create below/above textures
    for (auto& floor : map.floors()) {
        sf::Image tileMapBelow;
        sf::Image tileMapAbove;
        tileMapBelow.create(w, h, neutralColor);
        tileMapAbove.create(w, h, neutralColor);
        auto& layers = floor->graphicLayers();

        const uint8_t nbLayers = static_cast<uint8_t>(layers.size());
        for (uint8_t i = 0; i < nbLayers; ++i) {
            if (i < Dummy::Floor::INF_LAYERS_COUNT)
                layerToImage(layers[i], tileMapBelow, chipIdToIdx);
            else
                layerToImage(layers[i], tileMapAbove, chipIdToIdx);
        }

        sf::Texture texBelow;
        sf::Texture texAbove;
        texBelow.loadFromImage(tileMapBelow);
        texAbove.loadFromImage(tileMapAbove);
        m_tilemaps.push_back(texBelow);
        m_tilemaps.push_back(texAbove);
    }

    m_mapSprite.setTexture(m_tilemaps[0]);
    m_mapSprite.setScale(TILE_SIZE * m_zoom, TILE_SIZE * m_zoom);
}

void MapRender::renderBelow(sf::RenderWindow& renderWindow, uint8_t playerFloor)
{
    size_t nbTexturesBelow = playerFloor * 2 + 1;
    if (nbTexturesBelow >= m_tilemaps.size())
        return;

    for (size_t fIdx = 0; fIdx < nbTexturesBelow; ++fIdx) {
        m_mapShader.setUniform("tilemap", m_tilemaps[fIdx]);
        renderWindow.draw(m_mapSprite, &m_mapShader);
    }
}

void MapRender::renderAbove(sf::RenderWindow& renderWindow, uint8_t playerFloor)
{
    size_t nbTexturesBelow = playerFloor * 2 + 1;
    if (nbTexturesBelow >= m_tilemaps.size())
        return;

    m_mapShader.setUniform("tilemap", m_tilemaps[nbTexturesBelow]);
    renderWindow.draw(m_mapSprite, &m_mapShader);
}

void MapRender::layerToImage(const Dummy::GraphicLayer& lay, sf::Image& img,
                             const std::map<chip_id, uint8_t>& idMap)
{
    const uint16_t w = lay.width();
    const uint16_t h = lay.height();

    for (uint16_t y = 0; y < h; ++y) {
        for (uint16_t x = 0; x < w; ++x) {
            tileaspect val = lay.at({x, y});
            if (val.x == undefAspect.x || val.y == undefAspect.y)
                continue;

            if (idMap.find(val.chipId) == idMap.end())
                continue;

            uint8_t chipLocalIdx = idMap.at(val.chipId);
            sf::Color tileIndex(val.x, val.y, chipLocalIdx);
            img.setPixel(x, y, tileIndex);
        }
    }
}

} // namespace DummyPlayer
