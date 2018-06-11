#ifndef SKEWEACTION_H
#define SKEWACTION_H
#include "../ActionInterval.h"

OFXCOCOS_NS_BEGIN
ACTIONS_NS_BEGIN

/** @class SkewTo
 * @brief Rotates a ActionTarget object to a certain angle by modifying it's rotation attribute.
 The direction will be decided by the shortest angle.
*/
class SkewTo : public ActionInterval
{
public:
    /**
     * Creates the action with separate rotation angles.
     *
     * @param duration Duration time, in seconds.
     * @param dstAngleX In degreesCW.
     * @param dstAngleY In degreesCW.
     * @return An autoreleased SkewTo object.
     */
    static SkewTo *create(float duration, float dstAngleX, float dstAngleY);

    /**
     * Creates the action.
     *
     * @param duration Duration time, in seconds.
     * @param dstAngle In degreesCW.
     * @return An autoreleased SkewTo object.
     */
    static SkewTo *create(float duration, float dstAngle);

    /**
     * Creates the action with 3D rotation angles.
     * @param duration Duration time, in seconds.
     * @param dstAngle3D A ofVec2f skew.
     * @return An autoreleased SkewTo object.
     */
    static SkewTo *create(float duration, const ofVec2f &dstSkew);

    //
    // Overrides
    //
    virtual SkewTo *clone() const override;
    virtual SkewTo *reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;

public:
    SkewTo();
    virtual ~SkewTo() {}

    /**
     * initializes the action
     * @param duration in seconds
     * @param dstAngleX in degreesCW
     * @param dstAngleY in degreesCW
     */
    bool initWithDuration(float duration, float dstSkewX, float dstSkewY);
    /**
     * initializes the action
     * @param duration in seconds
     */
    bool initWithDuration(float duration, const ofVec2f &dstSkew);

    /**
     * calculates the start and diff angles
     * @param dstAngle in degreesCW
     */
    void calculateSkews(float &startAngle, float &diffAngle, float dstAngle);

    const ofVec2f &getDestinationSkew() const;

protected:
    ofVec2f m_DstSkew;
    ofVec2f m_StartSkew;
    ofVec2f m_DiffSkew;

private:
    DISALLOW_COPY_AND_ASSIGN(SkewTo);
};

/** @class SkewBy
 * @brief Rotates a ActionTarget object clockwise a number of degrees by modifying it's rotation attribute.
*/
class SkewBy : public ActionInterval
{
public:
    /** Creates the action with 3D rotation angles.
     *
     * @param duration Duration time, in seconds.
     * @param deltaAngle3D A ofVec2f skew.
     * @return An autoreleased SkewBy object.
     */
    static SkewBy *create(float duration, const ofVec2f &deltaSkew);

    //
    // Override
    //
    virtual SkewBy *clone() const override;
    virtual SkewBy *reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;

public:
    SkewBy();
    virtual ~SkewBy() {}

    bool initWithDuration(float duration, const ofVec2f &deltaSkew);

    const ofVec2f &getDeltaSkew() const;

protected:
    ofVec2f m_DeltaSkew;
    ofVec2f m_StartSkew;

private:
    DISALLOW_COPY_AND_ASSIGN(SkewBy);
};

ACTIONS_NS_END
OFXCOCOS_NS_END

#endif // ROTATEACTION_H
