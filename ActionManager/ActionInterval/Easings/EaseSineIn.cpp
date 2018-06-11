#include "EaseSineIn.h"
#include "EaseSineOut.h"

OFXCOCOS_NS_BEGIN
ACTIONS_NS_BEGIN

EaseSineIn::EaseSineIn()
{

}

EaseSineIn *EaseSineIn::create(ActionInterval *action)
{
    EaseSineIn *ret = new(std::nothrow) EaseSineIn();
    if (ret) {
        if (ret->initWithAction(action) == false) {
            SAFE_RELEASE(ret);
        }
    }

    return ret;
}

EaseSineIn *EaseSineIn::clone() const
{
    // no copy constructor
    auto a = new(std::nothrow) EaseSineIn();
    a->initWithAction(m_Inner->clone());
    return a;
}

void EaseSineIn::update(float time)
{
    m_Inner->update(tweenfunc::sineEaseIn(time));
}

ActionEase *EaseSineIn::reverse() const
{
    return EaseSineOut::create(m_Inner->reverse());
}

ACTIONS_NS_END
OFXCOCOS_NS_END
