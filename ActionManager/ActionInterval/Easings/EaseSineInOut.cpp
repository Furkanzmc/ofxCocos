#include "EaseSineInOut.h"

OFXCOCOS_NS_BEGIN
ACTIONS_NS_BEGIN

EaseSineInOut *EaseSineInOut::create(ActionInterval *action)
{
    EaseSineInOut *ret = new(std::nothrow) EaseSineInOut();
    if (ret) {
        if (ret->initWithAction(action) == false) {
            SAFE_RELEASE(ret);
        }
    }

    return ret;
}

EaseSineInOut *EaseSineInOut::clone() const
{
    // no copy constructor
    auto a = new(std::nothrow) EaseSineInOut();
    a->initWithAction(m_Inner->clone());
    return a;
}

void EaseSineInOut::update(float time)
{
    m_Inner->update(tweenfunc::sineEaseInOut(time));
}

EaseSineInOut *EaseSineInOut::reverse() const
{
    return EaseSineInOut::create(m_Inner->reverse());
}

ACTIONS_NS_END
OFXCOCOS_NS_END
