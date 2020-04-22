#ifndef DUMMYRPG_DIALOGRENDER_HPP
#define DUMMYRPG_DIALOGRENDER_HPP

#include <SFML/Graphics.hpp>

#include "dummyrpg/dialog.hpp"

namespace DummyPlayer {

///////////////////////////////////////////////////////////////////////////////

/// \brief MapRenderError: some error related to the MapRendering
class DialogRenderError : public std::runtime_error
{
public:
    explicit DialogRenderError(const std::string& msg);
};

///////////////////////////////////////////////////////////////////////////////

class DialogRender
{
public:
    explicit DialogRender();

    void showText(const Dummy::DialogSentence&);
    void showChoice(const Dummy::DialogChoice&);
    void setChoice(uint8_t choiceIdx);
    void hide();

    void render(sf::RenderWindow&);

private:
    void wordWrap(sf::Text&, int width);

    sf::Font m_font;
    sf::Text m_textSpeaker;
    std::string m_textSentence;
    sf::Text m_textSentenceWrapped;

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    sf::Texture m_choiceTexture;
    sf::Sprite m_choiceSprite;
    int m_choiceFrame = 0;

    bool m_show = false;
};

} // namespace DummyPlayer

#endif // DUMMYRPG_DIALOGRENDER_HPP
