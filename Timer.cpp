#include "Timer.h"
// openFrameworks
#include "ofMain.h"
USING_NS_OFXCOCOS

Timer::Timer()
    : m_Duration(0.f)
    , m_RemainingDuration(0.f)
    , m_PauseDuration(0.f)
    , m_IsLooped(false)
    , m_IsRunning(false)
    , m_IsStarted(false)
{

}

Timer::~Timer()
{
    if (m_IsStarted) {
        ofRemoveListener(ofEvents().update, this, &Timer::autoUpdate);
    }
}

Timer *Timer::create()
{
    Timer *timer = new(std::nothrow) Timer();
    if (timer) {
        timer->autorelease();
        timer->retain();
    }
    else {
        SAFE_RELEASE(timer);
    }

    return timer;
}

void Timer::slingShot(const float &duration, std::function<void ()> callback)
{
    Timer *timer = Timer::create();
    if (timer == nullptr) {
        return;
    }

    auto endFunc = [](Ref * ref, std::function<void ()> customCallback) {
        if (customCallback) {
            customCallback();
        }
        ref->release();
    };

    timer->setDuration(duration);
    timer->onTimerEnded = std::bind(endFunc, timer, callback);
    timer->startTimer();
}

void Timer::autoUpdate(ofEventArgs &event)
{
    if (m_IsRunning == false) {
        return;
    }

    if (m_PauseDuration > 0) {
        m_PauseDuration--;
        return;
    }

    m_RemainingDuration -= ofGetLastFrameTime();
    if (m_RemainingDuration <= 0) {

        if (m_IsLooped) {
            m_RemainingDuration = m_Duration;
            if (onTimerLooped) {
                onTimerLooped();
            }
        }
        else {
            m_IsRunning = false;
            m_RemainingDuration = 0;

            if (onTimerEnded) {
                ofRemoveListener(ofEvents().update, this, &Timer::autoUpdate);
                onTimerEnded();
            }
        }
    }
}

float Timer::getRemainingTime() const
{
    return m_RemainingDuration;
}

float Timer::getDuration() const
{
    return m_Duration;
}

void Timer::setDuration(float duration, bool forceUpdate)
{
    if (m_IsRunning == false || forceUpdate) {
        m_Duration = duration;
        m_RemainingDuration = m_Duration;
    }
}

bool Timer::IsLooped() const
{
    return m_IsLooped;
}

void Timer::setLooped(bool isLooped)
{
    m_IsLooped = isLooped;
}

bool Timer::isRunning() const
{
    return m_IsRunning;
}

void Timer::setRunning(bool isRunning)
{
    m_IsRunning = isRunning;
    if (m_IsStarted == false) {
        m_IsStarted = true;

        if (onTimerStarted) {
            ofAddListener(ofEvents().update, this, &Timer::autoUpdate);
            onTimerStarted();
        }
    }
}

void Timer::startTimer()
{
    setRunning(true);
}

void Timer::pauseTimer(float pauseDuration)
{
    m_PauseDuration = pauseDuration;
    setRunning(false);
}

void Timer::resetTimer()
{
    setRunning(false);
    m_IsStarted = false;
    m_RemainingDuration = m_Duration;

    if (onTimerEnded) {
        ofRemoveListener(ofEvents().update, this, &Timer::autoUpdate);
        onTimerEnded();
    }
}
