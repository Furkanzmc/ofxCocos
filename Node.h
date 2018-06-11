#ifndef ACTIONTARGET_H
#define ACTIONTARGET_H
#include "Ref.h"
#include "ofxCocosTypes.h"
// openFrameworks
#include "ofVec3f.h"
#include "ofColor.h"
#include "ofEvents.h"
#include "ofRectangle.h"

OFXCOCOS_NS_BEGIN

namespace Actions
{
class Action;
}

class Node : public Ref
{
public:
    std::function<void(const ofVec3f &)> onPositionChanged;
    std::function<void(const ofVec3f &)> onScaleChanged;
    std::function<void(const ofVec3f &)> onRotationChanged;

    std::function<void(const ofColor &)> onColorChanged;
    std::function<void(const GLubyte &)> onOpacityChanged;
    std::function<void(bool)> onVisibilityChanged;

public:
    static Node *create();

    virtual ~Node();

    virtual void update(const float &delta);
    void autoUpdate(ofEventArgs &eventArgs);

    /**
     * @brief This sets up the right position + orientation + scale then calls customDraw which you can override
     */
    void draw() const;

    const ofVec3f &getPosition() const;
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;
    void setPosition(const ofVec3f &positon);
    void setPosition(float x, float y, float z = 0);

    const ofVec3f &getScale() const;
    void setScale(const ofVec3f &scale);
    void setScale(float scale);
    void setScale(float scalex, float sclaey, float scalez = 1.f);

    const ofVec3f &getRotation() const;
    void setRotation(const ofVec3f &rotation);

    const ofColor &getColor() const;
    void setColor(const ofColor &color);

    GLubyte getOpacity() const;
    void setOpacity(GLubyte alpha);

    void runAction(Actions::Action *action);

    Actions::Action *getActionByTag(int tag) const;
    size_t getNumberOfRunningActions() const;

    void stopAllActions();
    void stopActionByTag(int tag);

    bool isVisible() const;
    void setVisible(bool visible);

    ofVec2f getAnchorPosition() const;
    ofVec2f getAnchorPercent() const;
    void setAnchorPercent(const ofVec2f &percent, const ofVec2f &contentSize);
    void setAnchorPercent(const ofVec2f &anchorPoint);

    const ofVec2f &getContentSize() const;
    void setContentSize(const ofVec2f &contentSize);

    bool getIsDrawContentLimit() const;
    void setDrawContentLimit(bool isDraw);

    Node *getParent();
    void setParent(Node *parent);

    Node *getChildByName(const std::string &name);

    void addChild(Node *child, const int &localZOrder);
    void addChild(Node *child);

    void removeFromParent();
    void removeChild(const std::string &name);

    int getLocalZOrder() const;
    void setLocalZOrder(int z);

    int getOrderOfArrival() const;
    void setOrderOfArrival(int orderOfArrival);

    bool isCascadeOpacityEnabled() const;
    void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);

    std::string getName() const;
    void setName(const std::string &name);

    ofRectangle getBoundingBox() const;

    void startAutoUpdate();
    void stopAutoUpdate();

    GLubyte getDisplayedOpacity() const;
    GLubyte getRealOpacity() const;

    float getSkewX() const;
    void setSkewX(float skewX);

    float getSkewY() const;
    void setSkewY(float skewY);

    ofVec2f getSkew() const;
    void setSkew(float skewX, float skewY);
    void setSkew(const ofVec2f &skew);

    /**
     * @brief This can be used to skew the drawing view wherever needed.
     * @param skewX
     * @param skewY
     */
    static void skewView(const float &skewX, const float &skewY);

protected:
    static int m_GlobalOrderOfArrival;

    ofVec3f m_Position;
    ofVec3f m_Scale;
    ofVec3f m_Rotation;
    ofColor m_Color;

    bool m_IsVisible,
         m_IsDrawContentLimit,
         m_IsCascadeOpacityEnabled,
         m_IsAutoUpdating;

    ofVec2f m_AnchorPercent,
            m_AnchorPoint,
            m_ContentSize;

    std::string m_Name;
    int m_LocalZOrder, m_OrderOfArrival;
    GLubyte m_DisplayedOpacity, m_RealOpacity;

    Node *m_Parent;
    ofVec2f m_Skew;

    std::vector<Node *> m_Children;

protected:
    Node();

    virtual bool setup();
    virtual void customDraw() const;

    virtual void updateColor();

private:
    void sortChildren();
    int getChildIndex(const std::string &name);

    void drawContentLimits() const;

    void updateCascadeOpacity();
    void updateDisplayedOpacity(GLubyte parentOpacity);
};

OFXCOCOS_NS_END

#endif // ACTIONTARGET_H
