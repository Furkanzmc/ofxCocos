#include "EaseIn.h"

OFXCOCOS_NS_BEGIN
ACTIONS_NS_BEGIN

EaseIn::EaseIn()
{

}

EaseIn *EaseIn::create(ActionInterval *action, float rate)
{
    EaseIn *easeIn = new(std::nothrow) EaseIn();
    if (easeIn) {
        if (easeIn->initWithAction(action, rate) == false) {
            SAFE_RELEASE(easeIn);
        }
    }

    return easeIn;
}

EaseIn *EaseIn::clone() const
{
    // no copy constructor
    auto a = new(std::nothrow) EaseIn();
    a->initWithAction(m_Inner->clone(), m_Rate);
    return a;
}

void EaseIn::update(float time)
{
    m_Inner->update(tweenfunc::easeIn(time, m_Rate));
}

EaseIn *EaseIn::reverse() const
{
    return EaseIn::create(m_Inner->reverse(), 1 / m_Rate);
}

ACTIONS_NS_END
OFXCOCOS_NS_END
