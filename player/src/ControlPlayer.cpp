#include "ControlPlayer.hpp"

namespace DummyPlayer {
namespace PlayerControl {
void applyMovement(Dummy::GameInstance& gi, const Keymap& km)
{
    if (sf::Keyboard::isKeyPressed(km.moveUp)) {
        gi.movePlayer(Dummy::Direction::Top);
    } else if (sf::Keyboard::isKeyPressed(km.moveLeft)) {
        gi.movePlayer(Dummy::Direction::Left);
    } else if (sf::Keyboard::isKeyPressed(km.moveDown)) {
        gi.movePlayer(Dummy::Direction::Bottom);
    } else if (sf::Keyboard::isKeyPressed(km.moveRight)) {
        gi.movePlayer(Dummy::Direction::Right);
    } else {
        gi.stopPlayer();
    }

    if (sf::Keyboard::isKeyPressed(km.action)) {
        gi.interractInFrontOfPlayer();
    }
}
} // namespace PlayerControl
} // namespace DummyPlayer
