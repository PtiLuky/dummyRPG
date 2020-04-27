#ifndef DUMMYRPG_CONTROLGAME_HPP
#define DUMMYRPG_CONTROLGAME_HPP

#include <SFML/Graphics.hpp>

#include "dummyrpg/dialog.hpp"
#include "dummyrpg/dummy_types.hpp"
#include "dummyrpg/game.hpp"

#include "Keymap.hpp"
#include "RenderDialog.hpp"

namespace DummyPlayer {

class GameControl
{
public:
    GameControl(const Dummy::GameStatic&, Dummy::GameInstance&);
    int run();

    void registerKeyPressed(sf::Keyboard::Key, const Keymap&);
    void doAction();

    void applyPlayerMovement(const Keymap&);

    void executeEvent(Dummy::event_id);
    void executeDialog(const Dummy::DialogSentence&);
    void executeChoice(const Dummy::DialogChoice&);

    void renderOverlays(sf::RenderWindow&);

private:
    const Dummy::GameStatic& m_game;
    Dummy::GameInstance& m_gameInstance;

    bool m_actionRequested = false;

    std::unique_ptr<DialogRender> m_dialogRender;

    Dummy::DialogChoice m_currChoice{"undef", Dummy::undefEvent};
    int m_currChoiceIdx;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_CONTROLGAME_HPP
