/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef BEZIERBY_H
#define BEZIERBY_H
#include "../ActionInterval.h"

/** @struct Bezier configuration structure
 */
struct BezierConfig {
    //! end position of the bezier
    ofVec2f endPosition;
    //! Bezier control point 1
    ofVec2f controlPoint_1;
    //! Bezier control point 2
    ofVec2f controlPoint_2;
};

OFXCOCOS_NS_BEGIN
ACTIONS_NS_BEGIN

/** @class BezierBy
 * @brief An action that moves the target with a cubic Bezier curve by a certain distance.
 */
class BezierBy : public ActionInterval
{
public:
    /** Creates the action with a duration and a bezier configuration.
     * @param t Duration time, in seconds.
     * @param c Bezier config.
     * @return A BezierBy object.
     */
    static BezierBy *create(float t, const BezierConfig &c);

    //
    // Overrides
    //
    virtual BezierBy *clone() const override;
    virtual BezierBy *reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void update(float time) override;

public:
    BezierBy();
    virtual ~BezierBy() {}

    /**
     * initializes the action with a duration and a bezier configuration
     * @param t in seconds
     */
    bool initWithDuration(float t, const BezierConfig &c);

protected:
    BezierConfig m_Config;
    ofVec2f m_StartPosition;
    ofVec2f m_PreviousPosition;

private:
    DISALLOW_COPY_AND_ASSIGN(BezierBy);
    float bezierat(float a, float b, float c, float d, float t);
};


// BezierTo

/** @class BezierTo
 * @brief An action that moves the target with a cubic Bezier curve to a destination point.
 */
class BezierTo : public BezierBy
{
public:
    /** Creates the action with a duration and a bezier configuration.
     * @param t Duration time, in seconds.
     * @param c Bezier config.
     * @return An autoreleased BezierTo object.
     */
    static BezierTo *create(float t, const BezierConfig &c);

    //
    // Overrides
    //
    virtual void startWithTarget(Node *target) override;
    virtual BezierTo *clone() const override;
    virtual BezierTo *reverse(void) const override;

public:
    BezierTo();
    virtual ~BezierTo() {}

    /**
     * @param t In seconds.
     */
    bool initWithDuration(float t, const BezierConfig &c);

protected:
    BezierConfig m_ToConfig;

private:
    DISALLOW_COPY_AND_ASSIGN(BezierTo);
};

ACTIONS_NS_END
OFXCOCOS_NS_END

#endif // BEZIERBY_H
