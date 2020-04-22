#include "ControlGame.hpp"

#include <iostream>

namespace DummyPlayer {

GameControl::GameControl(const Dummy::GameStatic& game, Dummy::GameInstance& gameInst)
    : m_game(game)
    , m_gameInstance(gameInst)
{
    try {
        m_dialog_render = std::make_unique<DialogRender>();
    } catch (const DialogRenderError& e) {
        std::cerr << "Could not load Dialog Render" << e.what() << std::endl;
    }
}

void GameControl::registerKeyPressed(sf::Keyboard::Key keyCode, const Keymap& keymap)
{
    if (keyCode == keymap.action) {
        m_actionRequested = true;
    }
}

void GameControl::doAction()
{
    if (m_actionRequested) {
        if (m_gameInstance.eventBlocked()) {
            // Finalize event
            if (m_dialog_render)
                m_dialog_render->hide();
            m_gameInstance.blockEvents(false);
        } else {
            m_gameInstance.stopPlayer();
            m_gameInstance.interractInFrontOfPlayer();
        }

        m_actionRequested = false;
    }
}

void GameControl::applyPlayerMovement(const Keymap& km)
{
    if (m_gameInstance.eventBlocked())
        return;

    if (sf::Keyboard::isKeyPressed(km.moveUp)) {
        m_gameInstance.movePlayer(Dummy::Direction::Top);
    } else if (sf::Keyboard::isKeyPressed(km.moveLeft)) {
        m_gameInstance.movePlayer(Dummy::Direction::Left);
    } else if (sf::Keyboard::isKeyPressed(km.moveDown)) {
        m_gameInstance.movePlayer(Dummy::Direction::Bottom);
    } else if (sf::Keyboard::isKeyPressed(km.moveRight)) {
        m_gameInstance.movePlayer(Dummy::Direction::Right);
    } else {
        m_gameInstance.stopPlayer();
    }
}

void GameControl::executeEvent(Dummy::event_id eventId)
{
    const auto& eventList = m_game.events;
    if (eventId >= eventList.size()) // Undef event is -1 = uint32_t_max
        return;

    switch (eventList[eventId].type) {
    case Dummy::EventType::Dialog:
        executeDialog(m_game.dialogs[eventList[eventId].idxPerType]);
        break;
    case Dummy::EventType::Choice:
        executeChoice(m_game.dialogsChoices[eventList[eventId].idxPerType]);
        break;
    default:
        break;
    }
}

void GameControl::executeDialog(const Dummy::DialogSentence& dialog)
{
    m_gameInstance.blockEvents(true);

    if (m_dialog_render)
        m_dialog_render->showText(dialog);
    else
        std::cout << dialog.speaker() << " says : " << dialog.sentence() << std::endl;

    m_gameInstance.registerEvent(dialog.nextEvent());
}

void GameControl::executeChoice(const Dummy::DialogChoice& choice)
{
    m_gameInstance.blockEvents(true);

    if (m_dialog_render)
        m_dialog_render->showChoice(choice);
    else
        std::cout << "Question : " << choice.question() << std::endl;
}

void GameControl::renderOverlays(sf::RenderWindow& renderWindow)
{
    if (m_dialog_render)
        m_dialog_render->render(renderWindow);
}

} // namespace DummyPlayer
