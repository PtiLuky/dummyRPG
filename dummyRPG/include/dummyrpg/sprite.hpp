#ifndef DUMMYRPG_SPRITE_HPP
#define DUMMYRPG_SPRITE_HPP

#include <string>

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

/// \brief An Animated Sprite is a Sprite that can contains up to 4 animations
/// The images of the animation shall be horizontally aligned.
///
struct AnimatedSprite
{
    std::string imgPath; ///< path of the sprite image

    uint16_t width  = 0; ///< width (px) of the sprite
    uint16_t height = 0; ///< height (px) of the sprite

    // Animation 1 (uses m_x and m_y)
    uint16_t x = 0; ///< pos x (in px) of top-left corner
    uint16_t y = 0; ///< pos y (in px) of top-left corner
    uint8_t nbFrames = 1;

    // Animation 2
    uint16_t x2       = 0; ///< pos x (in px) of top-left corner of the 2nd block
    uint16_t y2       = 0; ///< pos y (in px) of top-left corner of the 2dn block
    uint8_t nbFrames2 = 0;

    // Animation 3
    uint16_t x3       = 0; ///< pos x (in px) of top-left corner of the 3rd block
    uint16_t y3       = 0; ///< pos y (in px) of top-left corner of the 3rd block
    uint8_t nbFrames3 = 0;

    // Animation 4
    uint16_t x4       = 0; ///< pos x (in px) of top-left corner of the 4th block
    uint16_t y4       = 0; ///< pos y (in px) of top-left corner of the 4th block
    uint8_t nbFrames4 = 0;
};

} // namespace Dummy

#endif // DUMMYRPG_SPRITE_HPP
