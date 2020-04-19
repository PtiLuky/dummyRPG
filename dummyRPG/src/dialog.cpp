#include "dummyrpg/dialog.hpp"

namespace Dummy {

DialogChoice::DialogChoice(const std::string& question, event_id id)
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

DialogOption DialogChoice::option(uint8_t idx) const
{
    if (idx >= m_options.size())
        return DialogOption();

    return m_options[idx];
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

DialogSentence::DialogSentence(const std::string& speaker, const std::string& sentence, event_id id)
    : m_id(id)
    , m_speaker(speaker)
    , m_sentence(sentence)
{}

event_id DialogSentence::id() const
{
    return m_id;
}

void DialogSentence::setNextEvent(event_id nextEventId)
{
    m_nextEvent = nextEventId;
}

} // namespace Dummy
