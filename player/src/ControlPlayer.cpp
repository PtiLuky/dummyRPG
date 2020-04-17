#include "ControlPlayer.hpp"

namespace DummyPlayer {
namespace PlayerControl {
void ApplyMovement(Dummy::PlayerInstance& player, const Keymap& km, const Dummy::Floor& floor)
{
    if (player.state == Dummy::CharState::Walking)
        player.state = Dummy::CharState::Idle;

    auto newPos = player.pos;

    if (sf::Keyboard::isKeyPressed(km.moveUp)) {
        if (newPos.y > 0)
            --newPos.y;
        player.dir   = Dummy::Direction::Top;
        player.state = Dummy::CharState::Walking;

    } else if (sf::Keyboard::isKeyPressed(km.moveLeft)) {
        if (newPos.x > 0)
            --newPos.x;
        player.dir   = Dummy::Direction::Left;
        player.state = Dummy::CharState::Walking;

    } else if (sf::Keyboard::isKeyPressed(km.moveDown)) {
        if (newPos.y < floor.height() - 1)
            ++newPos.y;
        player.dir   = Dummy::Direction::Bottom;
        player.state = Dummy::CharState::Walking;

    } else if (sf::Keyboard::isKeyPressed(km.moveRight)) {
        if (newPos.x < floor.width() - 1)
            ++newPos.x;
        player.dir   = Dummy::Direction::Right;
        player.state = Dummy::CharState::Walking;
    }

    if (floor.blockingLayer().at(newPos) == false)
        player.pos = newPos;
}
} // namespace PlayerControl
} // namespace DummyPlayer
