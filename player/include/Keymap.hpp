#ifndef DUMMYRPG_KEYMAP_HPP
#define DUMMYRPG_KEYMAP_HPP

#include <SFML/Window/Keyboard.hpp>

namespace DummyPlayer {
struct Keymap
{
    sf::Keyboard::Key moveLeft  = sf::Keyboard::Left;
    sf::Keyboard::Key moveRight = sf::Keyboard::Right;
    sf::Keyboard::Key moveUp    = sf::Keyboard::Up;
    sf::Keyboard::Key moveDown  = sf::Keyboard::Down;

    sf::Keyboard::Key action = sf::Keyboard::Space;
};
} // namespace DummyPlayer
#endif // DUMMYRPG_KEYMAP_HPP
