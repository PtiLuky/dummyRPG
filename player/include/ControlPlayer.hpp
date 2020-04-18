#ifndef DUMMYRPG_CONTROLPLAYER_HPP
#define DUMMYRPG_CONTROLPLAYER_HPP

#include "dummyrpg/game.hpp"

#include "Keymap.hpp"

namespace DummyPlayer {
namespace PlayerControl {
void ApplyMovement(Dummy::GameInstance& player, const Keymap&);
};
} // namespace DummyPlayer

#endif // DUMMYRPG_CONTROLPLAYER_HPP
