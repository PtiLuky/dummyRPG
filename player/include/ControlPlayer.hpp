#ifndef DUMMYRPG_CONTROLPLAYER_HPP
#define DUMMYRPG_CONTROLPLAYER_HPP

#include "dummyrpg/character.hpp"
#include "dummyrpg/floor.hpp"

#include "Keymap.hpp"

namespace DummyPlayer {
namespace PlayerControl {
void ApplyMovement(Dummy::PlayerInstance& player, const Keymap&, const Dummy::Floor&);
};
} // namespace DummyPlayer

#endif // DUMMYRPG_CONTROLPLAYER_HPP
