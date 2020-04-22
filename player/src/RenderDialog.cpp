#include "RenderDialog.hpp"

// static const char* DEFAULT_FONT             = "Resources/EuropeanTeletext.ttf";
static const char* DEFAULT_FONT             = "Resources/PixelOperator8.ttf";
static const unsigned int DEFAULT_FONT_SIZE = 16;  // in px
static const int DEFAULT_INTERLINE          = 8;   // in px
static const int DIALOG_MARGIN              = 10;  // in px
static const int DIALOG_HEIGHT              = 150; // in px

static const float CHOICE_SPRITE_ZOOM = 2.f;

namespace DummyPlayer {

DialogRenderError::DialogRenderError(const std::string& msg)
    : std::runtime_error(msg)
{}

DialogRender::DialogRender()
{
    if (! m_font.loadFromFile(DEFAULT_FONT)) {
        std::string errorMsg = "Could not load font ";
        errorMsg += DEFAULT_FONT;
        throw DialogRenderError(errorMsg);
    }

    if (! m_backgroundTexture.loadFromFile("Resources/bluePx.png"))
        throw DialogRenderError("Could not load a sprite texture: Resources/bluePx.png");

    // TODO slicing sprite shader for text boxes
    m_backgroundTexture.setRepeated(true);
    m_backgroundSprite.setTexture(m_backgroundTexture);

    // TODO better choice icon ?
    if (! m_choiceTexture.loadFromFile("Resources/ClassicRPG_Sheet.png"))
        throw DialogRenderError("Could not load a sprite texture: Resources/ClassicRPG_Sheet.png");

    m_choiceSprite.setTexture(m_choiceTexture);
    m_choiceSprite.setScale(CHOICE_SPRITE_ZOOM, CHOICE_SPRITE_ZOOM);
    m_choiceSprite.setTextureRect({144, 112, 16, 16});

    m_textSpeaker.setFont(m_font);
    m_textSentence.setFont(m_font);

    m_textSpeaker.setCharacterSize(DEFAULT_FONT_SIZE);
    m_textSentence.setCharacterSize(DEFAULT_FONT_SIZE);

    m_textSpeaker.setFillColor(sf::Color::Black);
    m_textSentence.setFillColor(sf::Color::Black);

    m_textSpeaker.setStyle(sf::Text::Underlined);
}

void DialogRender::showText(const Dummy::DialogSentence& dialog)
{
    m_show = true;

    m_textSpeaker.setString(dialog.speaker());
    m_textSentence.setString(dialog.sentence());
}

void DialogRender::showChoice(const Dummy::DialogChoice& choice)
{
    m_show = true;
    m_textSpeaker.setString(choice.question());
}

void DialogRender::setChoice(uint8_t choiceIdx) {}

void DialogRender::hide()
{
    m_show = false;
}

void DialogRender::render(sf::RenderWindow& window)
{
    if (! m_show)
        return;

    const auto winSize = window.getSize();
    const sf::IntRect textRect(0, 0, static_cast<int>(winSize.x), DIALOG_HEIGHT);
    int textY = static_cast<int>(winSize.y) - DIALOG_HEIGHT;

    m_backgroundSprite.setPosition(0, textY);
    m_backgroundSprite.setTextureRect(textRect);
    window.draw(m_backgroundSprite);

    textY += DIALOG_MARGIN;
    m_textSpeaker.setPosition(DIALOG_MARGIN * 2, textY);
    window.draw(m_textSpeaker);

    textY += DEFAULT_INTERLINE;
    textY += DEFAULT_FONT_SIZE;
    m_textSentence.setPosition(DIALOG_MARGIN, textY);
    window.draw(m_textSentence);

    textY += DEFAULT_INTERLINE;
    textY += DEFAULT_FONT_SIZE;
    auto& rect = m_choiceSprite.getTextureRect();
    m_choiceSprite.setPosition(static_cast<int>(winSize.x) - rect.width * CHOICE_SPRITE_ZOOM,
                               static_cast<int>(winSize.y) - rect.height * CHOICE_SPRITE_ZOOM
                                   + m_choiceFrame * 4);
    m_choiceFrame = (m_choiceFrame + 1) % 2;
    window.draw(m_choiceSprite);
}

} // namespace DummyPlayer
