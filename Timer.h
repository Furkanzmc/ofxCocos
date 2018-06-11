#ifndef TIMER_H
#define TIMER_H
#include "Ref.h"
// STD
#include <functional>
class ofEventArgs;

OFXCOCOS_NS_BEGIN

class Timer : public Ref
{
public:
    /**
     * @brief Called when timer starts for the first time
     */
    std::function<void()> onTimerStarted;

    /**
     * @brief Called when timer is not set to loop and it ends
     */
    std::function<void()> onTimerEnded;

    /**
     * @brief Called when timer loops
     */
    std::function<void()> onTimerLooped;

public:
    ~Timer();

    static Timer *create();

    /**
     * @brief Creates a Timer instance with the given duration and when it ends calls the callback and destroys the instance
     * @param duration
     * @param callback
     * @return
     */
    static void slingShot(const float &duration, std::function<void()> callback);

    float getRemainingTime() const;
    float getDuration() const;

    /**
     * @brief If the timer is running and forceUpdate is false duration is not uptaded.
     * @param duration in seconds
     * @param forceUpdate
     */
    void setDuration(float duration, bool forceUpdate = false);

    bool IsLooped() const;
    void setLooped(bool isLooped);

    bool isRunning() const;
    void setRunning(bool isRunning);

    void startTimer();

    /**
     * @brief Pause the timer for a specified time or until it is started again
     * @param pauseDuration in seconds
     */
    void pauseTimer(float pauseDuration = 0.f);

    /**
     * @brief Resets the timer and calls onTimerEnded
     */
    void resetTimer();

private:
    float m_Duration,
          m_RemainingDuration,
          m_PauseDuration;

    bool m_IsLooped,
         m_IsRunning,
         m_IsStarted;

private:
    Timer();

    void autoUpdate(ofEventArgs &event);
};

OFXCOCOS_NS_END

#endif // TIMER_H
