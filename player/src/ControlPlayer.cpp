#include "ControlPlayer.hpp"

namespace DummyPlayer {
namespace PlayerControl {
void ApplyMovement(Dummy::PlayerInstance& player, const Keymap& km, const Dummy::Floor& floor)
{
    player.isMoving = false;

    auto newPos = player.pos;
    if (sf::Keyboard::isKeyPressed(km.moveUp)) {
        if (newPos.y > 0)
            --newPos.y;
        player.dir      = Dummy::Direction::Top;
        player.isMoving = true;
    } else if (sf::Keyboard::isKeyPressed(km.moveLeft)) {
        if (newPos.x > 0)
            --newPos.x;
        player.dir      = Dummy::Direction::Left;
        player.isMoving = true;
    } else if (sf::Keyboard::isKeyPressed(km.moveDown)) {
        if (newPos.y <= floor.height())
            ++newPos.y;
        player.dir      = Dummy::Direction::Bottom;
        player.isMoving = true;
    } else if (sf::Keyboard::isKeyPressed(km.moveRight)) {
        if (newPos.x <= floor.width())
            ++newPos.x;
        player.dir      = Dummy::Direction::Right;
        player.isMoving = true;
    }

    if (floor.blockingLayer().at(newPos) == false)
        player.pos = newPos;
}
} // namespace PlayerControl
} // namespace DummyPlayer
