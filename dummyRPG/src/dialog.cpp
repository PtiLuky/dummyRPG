#include "dummyrpg/dialog.hpp"

namespace Dummy {

DialogChoice::DialogChoice(event_id id, const std::string& question)
    : m_id(id)
    , m_question(question)
{}

event_id DialogChoice::id() const
{
    return m_id;
}

uint8_t DialogChoice::nbOptions() const
{
    return static_cast<uint8_t>(m_options.size());
}

DialogOption DialogChoice::optionAt(uint8_t idx) const
{
    if (idx >= m_options.size())
        return DialogOption();

    return m_options[idx];
}

const std::string& DialogChoice::question() const
{
    return m_question;
}

void DialogChoice::setQuestion(const std::string& question)
{
    m_question = question;
}

bool DialogChoice::setOption(const DialogOption& option, uint8_t idx)
{
    if (idx > m_options.size())
        return false;

    if (idx == m_options.size())
        return addOption(option);

    m_options[idx] = option;
    return true;
}

bool DialogChoice::addOption(const DialogOption& option)
{
    size_t nbOptions = m_options.size();
    if (nbOptions >= NB_OPTIONS_MAX)
        return false;

    m_options.resize(nbOptions + 1);
    m_options[nbOptions] = option;
    return true;
}

bool DialogChoice::removeOption(uint8_t idx)
{
    if (idx >= m_options.size())
        return false;

    m_options.erase(m_options.begin() + idx);
    return true;
}

///////////////////////////////////////////////////////////////////////////////

DialogSentence::DialogSentence(event_id id, char_id speaker, const std::string& sentence)
    : m_id(id)
    , m_speaker(speaker)
    , m_sentence(sentence)
{}

event_id DialogSentence::id() const
{
    return m_id;
}

event_id DialogSentence::nextEvent() const
{
    return m_nextEvent;
}

char_id DialogSentence::speakerId() const
{
    return m_speaker;
}

const std::string& DialogSentence::sentence() const
{
    return m_sentence;
}

void DialogSentence::setNextEvent(event_id nextEventId)
{
    m_nextEvent = nextEventId;
}

void DialogSentence::setSentence(const std::string& str)
{
    m_sentence = str;
}

} // namespace Dummy
