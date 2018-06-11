#include "SkewAction.h"
#include "ofLog.h"

OFXCOCOS_NS_BEGIN
ACTIONS_NS_BEGIN

//
// SkewTo
//

SkewTo *SkewTo::create(float duration, float dstAngle)
{
    SkewTo *skewTo = new(std::nothrow) SkewTo();
    skewTo->initWithDuration(duration, dstAngle, dstAngle);

    return skewTo;
}

SkewTo *SkewTo::create(float duration, float dstAngleX, float dstAngleY)
{
    SkewTo *skewTo = new(std::nothrow) SkewTo();
    skewTo->initWithDuration(duration, dstAngleX, dstAngleY);

    return skewTo;
}

SkewTo *SkewTo::create(float duration, const ofVec2f &dstSkew)
{
    SkewTo *skewTo = new(std::nothrow) SkewTo();
    skewTo->initWithDuration(duration, dstSkew);

    return skewTo;
}

SkewTo::SkewTo()
{
}

bool SkewTo::initWithDuration(float duration, float dstAngleX, float dstSkewY)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_DstSkew.x = dstAngleX;
        m_DstSkew.y = dstSkewY;

        return true;
    }

    return false;
}

bool SkewTo::initWithDuration(float duration, const ofVec2f &dstSkew)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_DstSkew = dstSkew;

        return true;
    }

    return false;
}

SkewTo *SkewTo::clone(void) const
{
    // no copy constructor
    auto a = new(std::nothrow) SkewTo();
    a->initWithDuration(m_Duration, m_DstSkew);
    return a;
}

void SkewTo::calculateSkews(float &startAngle, float &diffAngle, float dstAngle)
{
    if (startAngle > 0) {
        startAngle = fmodf(startAngle, 360.0f);
    }
    else {
        startAngle = fmodf(startAngle, -360.0f);
    }

    diffAngle = dstAngle - startAngle;
    if (diffAngle > 180) {
        diffAngle -= 360;
    }
    if (diffAngle < -180) {
        diffAngle += 360;
    }
}

const ofVec2f &SkewTo::getDestinationSkew() const
{
    return m_DstSkew;
}

void SkewTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    m_StartSkew = m_Target->getSkew();

    calculateSkews(m_StartSkew.x, m_DiffSkew.x, m_DstSkew.x);
    calculateSkews(m_StartSkew.y, m_DiffSkew.y, m_DstSkew.y);
}

void SkewTo::update(float time)
{
    if (m_Target) {
        m_Target->setSkew(ofVec2f(
                              m_StartSkew.x + m_DiffSkew.x * time,
                              m_StartSkew.y + m_DiffSkew.y * time
                          ));
    }
}

SkewTo *SkewTo::reverse() const
{
    ofLogError("ofxActionManager") << __FUNCTION__ << ": SkewTo doesn't support the 'reverse' method";
    return nullptr;
}

//
// SkewBy
//

SkewBy *SkewBy::create(float duration, const ofVec2f &deltaSkew)
{
    SkewBy *skewBy = new(std::nothrow) SkewBy();
    skewBy->initWithDuration(duration, deltaSkew);

    return skewBy;
}

SkewBy::SkewBy()
{
}

bool SkewBy::initWithDuration(float duration, const ofVec2f &deltaSkew)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_DeltaSkew = deltaSkew;
        return true;
    }

    return false;
}

const ofVec2f &SkewBy::getDeltaSkew() const
{
    return m_DeltaSkew;
}

SkewBy *SkewBy::clone() const
{
    // no copy constructor
    auto a = new(std::nothrow) SkewBy();
    a->initWithDuration(m_Duration, m_DeltaSkew);
    return a;
}

void SkewBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_StartSkew = target->getSkew();
}

void SkewBy::update(float time)
{
    // FIXME: shall I add % 360
    if (m_Target) {
        ofVec2f skew;
        skew.x = m_StartSkew.x + m_DeltaSkew.x * time;
        skew.y = m_StartSkew.y + m_DeltaSkew.y * time;
        m_Target->setSkew(skew);
    }
}

SkewBy *SkewBy::reverse() const
{
    ofVec2f skew;
    skew.x = -m_DeltaSkew.x;
    skew.y = -m_DeltaSkew.y;
    return SkewBy::create(m_Duration, skew);
}

ACTIONS_NS_END
OFXCOCOS_NS_END
