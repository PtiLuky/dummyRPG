#include "ControlGame.hpp"

#include <iostream>

#include "Keymap.hpp"
#include "RenderGame.hpp"
#include "RenderMap.hpp"

static const int WIN_WIDTH  = 600;
static const int WIN_HEIGHT = 600;
static const int WIN_FPS    = 6;

namespace DummyPlayer {

GameControl::GameControl(const Dummy::GameStatic& game, Dummy::GameInstance& gameInst)
    : m_game(game)
    , m_gameInstance(gameInst)
{
    try {
        m_dialogRender = std::make_unique<DialogRender>();
    } catch (const DialogRenderError& e) {
        std::cerr << "Could not load Dialog Render" << e.what() << std::endl;
    }
}

int GameControl::run()
{
    DummyPlayer::Keymap keymap;
    bool gameHasFocus = true;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), m_game.name());
    window.setFramerateLimit(WIN_FPS);

    DummyPlayer::GameRender renderer(m_game, m_gameInstance);
    auto* map = m_gameInstance.currentMap();
    if (map == nullptr)
        return 1; // corrupted data ???
    renderer.setMap(*map);

    while (window.isOpen()) {
        sf::Event event;

        // Process input events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.F, 0.F, static_cast<float>(event.size.width),
                                          static_cast<float>(event.size.height));
                window.setView(sf::View(visibleArea));
            } else if (event.type == sf::Event::GainedFocus) {
                gameHasFocus = true;
            } else if (event.type == sf::Event::LostFocus) {
                gameHasFocus = false;
            } else if (event.type == sf::Event::KeyPressed) {
                // split register and doing action to avoid multiple events
                registerKeyPressed(event.key.code, keymap);
            }
        }

        // Do actions registered in previous loop
        doAction();

        // Process game-sent events
        Dummy::event_id eventId = m_gameInstance.dequeEvent();
        while (eventId != Dummy::undefEvent) {
            executeEvent(eventId);
            eventId = m_gameInstance.dequeEvent();
        }

        // Process pressed-states (events will only give the pressed moment)
        if (gameHasFocus) {
            applyPlayerMovement(keymap);
        }

        // Render game
        window.clear();
        renderer.render(window);
        renderOverlays(window);
        window.display();
    }

    return 0;
}

void GameControl::registerKeyPressed(sf::Keyboard::Key keyCode, const Keymap& keymap)
{
    if (keyCode == keymap.action) {
        m_actionRequested = true;
    }

    if (m_currChoice.id() != Dummy::undefEvent) {
        if (keyCode == keymap.moveUp)
            --m_currChoiceIdx;
        if (keyCode == keymap.moveDown)
            ++m_currChoiceIdx;

        m_currChoiceIdx += m_currChoice.nbOptions();
        m_currChoiceIdx %= m_currChoice.nbOptions();

        if (m_dialogRender)
            m_dialogRender->setChoice(static_cast<uint8_t>(m_currChoiceIdx));
    }
}

void GameControl::doAction()
{
    if (m_actionRequested) {
        if (m_gameInstance.eventBlocked()) {
            // Finalize event
            if (m_dialogRender)
                m_dialogRender->hide();

            if (m_currChoice.id() != Dummy::undefEvent) {
                auto option = m_currChoice.optionAt(static_cast<uint8_t>(m_currChoiceIdx));
                m_gameInstance.registerEvent(option.nextEvent);
                m_currChoice = Dummy::DialogChoice("undef", Dummy::undefEvent);
            }

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
    const auto* event = m_game.event(eventId);
    if (event == nullptr)
        return;

    if (event->type == Dummy::EventType::Dialog) {
        const auto* dlg = m_game.dialog(event->idxPerType);
        if (dlg != nullptr)
            executeDialog(*dlg);

    } else if (event->type == Dummy::EventType::Choice) {
        const auto* dlg = m_game.choice(event->idxPerType);
        if (dlg != nullptr)
            executeChoice(*dlg);
    }
}

void GameControl::executeDialog(const Dummy::DialogSentence& dialog)
{
    m_gameInstance.blockEvents(true);

    if (m_dialogRender)
        m_dialogRender->showText(dialog);

    m_gameInstance.registerEvent(dialog.nextEvent());
}

void GameControl::executeChoice(const Dummy::DialogChoice& choice)
{
    m_gameInstance.blockEvents(true);

    m_currChoice    = choice;
    m_currChoiceIdx = 0;
    if (m_dialogRender)
        m_dialogRender->showChoice(choice);
}

void GameControl::renderOverlays(sf::RenderWindow& renderWindow)
{
    if (m_dialogRender)
        m_dialogRender->render(renderWindow);
}

} // namespace DummyPlayer
