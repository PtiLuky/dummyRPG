#ifndef DUMMYRPG_DIALOG_HPP
#define DUMMYRPG_DIALOG_HPP

#include <string>
#include <vector>

#include "dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////

namespace Dummy {

///////////////////////////////////////////////////////////////////////////////

struct DialogOption
{
    std::string option;
    event_id nextEvent = undefEvent;
};

class DialogChoice
{
    friend class Serializer;

public:
    DialogChoice(const std::string& question, event_id);

    event_id id() const;
    uint8_t nbOptions() const;
    DialogOption option(uint8_t idx) const;
    const std::string& question() const;

    bool setOption(const DialogOption&, uint8_t idx);
    bool addOption(const DialogOption&);
    bool removeOption(uint8_t idx);

    static const uint8_t NB_OPTIONS_MAX = 4;

private:
    event_id m_id;
    std::string m_question;
    std::vector<DialogOption> m_options;
};

///////////////////////////////////////////////////////////////////////////////

class DialogSentence
{
    friend class Serializer;

public:
    DialogSentence(const std::string& speaker, const std::string& sentence, event_id);

    event_id id() const;
    event_id nextEvent() const;
    const std::string& speaker() const;
    const std::string& sentence() const;

    void setNextEvent(event_id);

private:
    event_id m_id;
    std::string m_speaker;
    std::string m_sentence;

    event_id m_nextEvent = undefEvent;
};

} // namespace Dummy


#endif // DUMMYRPG_DIALOG_HPP
