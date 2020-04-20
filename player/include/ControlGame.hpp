#ifndef DUMMYRPG_CONTROLGAME_HPP
#define DUMMYRPG_CONTROLGAME_HPP

#include "dummyrpg/dialog.hpp"
#include "dummyrpg/dummy_types.hpp"
#include "dummyrpg/game.hpp"

#include "Keymap.hpp"

namespace DummyPlayer {

class GameControl
{
public:
    GameControl(const Dummy::GameStatic&, Dummy::GameInstance&);

    void registerKeyPressed(sf::Keyboard::Key, const Keymap&);
    void doAction();

    void applyPlayerMovement(const Keymap&);

    void executeEvent(Dummy::event_id);
    void executeDialog(const Dummy::DialogSentence&);
    void executeChoice(const Dummy::DialogChoice&);

private:
    const Dummy::GameStatic& m_game;
    Dummy::GameInstance& m_gameInstance;

    bool m_actionRequested = false;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_CONTROLGAME_HPP
