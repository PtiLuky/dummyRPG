#include "ControlPlayer.hpp"

namespace DummyPlayer {
namespace PlayerControl {
void ApplyMovement(Dummy::GameInstance& gi, const Keymap& km)
{
    if (sf::Keyboard::isKeyPressed(km.moveUp)) {
        gi.MovePlayer(Dummy::Direction::Top);
    } else if (sf::Keyboard::isKeyPressed(km.moveLeft)) {
        gi.MovePlayer(Dummy::Direction::Left);
    } else if (sf::Keyboard::isKeyPressed(km.moveDown)) {
        gi.MovePlayer(Dummy::Direction::Bottom);
    } else if (sf::Keyboard::isKeyPressed(km.moveRight)) {
        gi.MovePlayer(Dummy::Direction::Right);
    } else {
        gi.StopPlayer();
    }
}
} // namespace PlayerControl
} // namespace DummyPlayer
