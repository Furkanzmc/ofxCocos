#include "Node.h"
#include "ActionManager/ActionManager.h"
// openFrameworks
#include "ofMain.h"
USING_NS_OFXCOCOS
USING_NS_OFXCOCOS_ACTIONS

int Node::m_GlobalOrderOfArrival = 1;

Node::Node()
    : m_Position(0, 0, 0)
    , m_Scale(1, 1, 1)
    , m_Rotation(0, 0, 0)
    , m_Color(ofColor::white)
    , m_IsVisible(true)
    , m_IsDrawContentLimit(false)
    , m_IsCascadeOpacityEnabled()
    , m_AnchorPercent(0, 0)
    , m_AnchorPoint(0, 0)
    , m_ContentSize(0, 0)
    , m_Name("")
    , m_LocalZOrder(0)
    , m_OrderOfArrival(0)
    , m_DisplayedOpacity(255)
    , m_RealOpacity(255)
    , m_Parent(nullptr)
    , m_Skew(0, 0)
{

}

Node::~Node()
{
    if (m_IsAutoUpdating) {
        ofRemoveListener(ofEvents().update, this, &Node::autoUpdate);
    }
}

Node *Node::create()
{
    Node *nd = new(std::nothrow) Node();
    if (nd && nd->setup()) {
        nd->autorelease();
    }
    else {
        SAFE_RELEASE(nd);
    }

    return nd;
}

bool Node::setup()
{
    return true;
}

void Node::customDraw() const
{

}

void Node::updateColor()
{
    m_Color.a = m_DisplayedOpacity;
}

void Node::update(const float &delta)
{

}

void Node::autoUpdate(ofEventArgs &eventArgs)
{
    (void)eventArgs;
    if (m_IsAutoUpdating) {
        update(ofGetLastFrameTime());
    }
}

void Node::draw() const
{
    if (m_IsVisible == false) {
        return;
    }

    ofPushMatrix();
    {
        ofTranslate(this->getPosition().x - m_AnchorPoint.x * this->getScale().x, this->getPosition().y - m_AnchorPoint.y * this->getScale().y, this->getPosition().z);
        ofRotateX(m_Rotation.x);
        ofRotateY(m_Rotation.y);
        ofRotateZ(m_Rotation.z);
        ofScale(this->getScale());

        if (m_Skew.x > 0.f || m_Skew.y > 0.f) {
            skewView(m_Skew.x, m_Skew.y);
        }

        ofPushStyle();
        {
            ofSetColor(this->getColor());

            customDraw();
            for (const Node *child : m_Children) {
                child->draw();
            }
#ifdef _DEBUG
            drawContentLimits();
#endif //_DEBUG
        }
        ofPopStyle();
    }
    ofPopMatrix();
}

const ofVec3f &Node::getPosition() const
{
    return m_Position;
}

float Node::getPositionX() const
{
    return m_Position.x;
}

float Node::getPositionY() const
{
    return m_Position.y;
}

float Node::getPositionZ() const
{
    return m_Position.z;
}

void Node::setPosition(const ofVec3f &positon)
{
    m_Position = positon;
    if (onPositionChanged) {
        onPositionChanged(m_Position);
    }
}

void Node::setPosition(float x, float y, float z)
{
    setPosition(ofVec3f(x, y, z));
}

const ofVec3f &Node::getScale() const
{
    return m_Scale;
}

void Node::setScale(const ofVec3f &scale)
{
    m_Scale = scale;
    if (onScaleChanged) {
        onScaleChanged(m_Scale);
    }
}

void Node::setScale(float scale)
{
    m_Scale = ofVec3f(scale, scale, scale);
    if (onScaleChanged) {
        onScaleChanged(m_Scale);
    }
}

void Node::setScale(float scalex, float sclaey, float scalez)
{
    m_Scale = ofVec3f(scalex, sclaey, scalez);
    if (onScaleChanged) {
        onScaleChanged(m_Scale);
    }
}

const ofVec3f &Node::getRotation() const
{
    return m_Rotation;
}

void Node::setRotation(const ofVec3f &rotation)
{
    m_Rotation = rotation;
    if (onRotationChanged) {
        onRotationChanged(m_Rotation);
    }
}

const ofColor &Node::getColor() const
{
    return m_Color;
}

void Node::setColor(const ofColor &color)
{
    m_Color = color;
    if (onColorChanged) {
        onColorChanged(m_Color);
    }
}

GLubyte Node::getOpacity() const
{
    return m_Color.a;
}

void Node::setOpacity(GLubyte alpha)
{
    m_DisplayedOpacity = m_RealOpacity = alpha;
    updateCascadeOpacity();
    if (onOpacityChanged) {
        onOpacityChanged(m_DisplayedOpacity);
    }
}

void Node::runAction(Action *action)
{
    ActionManager::getInstance()->addAction(action, this, false);
}

Action *Node::getActionByTag(int tag) const
{
    return ActionManager::getInstance()->getActionByTag(tag, this);
}

std::size_t Node::getNumberOfRunningActions() const
{
    return ActionManager::getInstance()->getNumberOfRunningActionsInTarget(this);
}

void Node::stopAllActions()
{
    ActionManager::getInstance()->removeAllActionsFromTarget(this);
}

void Node::stopActionByTag(int tag)
{
    ActionManager::getInstance()->removeActionByTag(tag, this);
}

bool Node::isVisible() const
{
    return m_IsVisible;
}

void Node::setVisible(bool visible)
{
    m_IsVisible = visible;
    if (onVisibilityChanged) {
        onVisibilityChanged(m_IsVisible);
    }
}

ofVec2f Node::getAnchorPosition() const
{
    return m_AnchorPoint;
}

ofVec2f Node::getAnchorPercent() const
{
    return m_AnchorPercent;
}

void Node::setAnchorPercent(const ofVec2f &percent, const ofVec2f &contentSize)
{
    m_AnchorPercent.x = std::max(std::min(percent.x, 1.f), 0.f);
    m_AnchorPercent.y = std::max(std::min(percent.y, 1.f), 0.f);

    m_AnchorPoint.x = contentSize.x * m_AnchorPercent.x;
    m_AnchorPoint.y = contentSize.y * m_AnchorPercent.y;
}

void Node::setAnchorPercent(const ofVec2f &anchorPoint)
{
    setAnchorPercent(anchorPoint, m_ContentSize);
}

const ofVec2f &Node::getContentSize() const
{
    return m_ContentSize;
}

void Node::setContentSize(const ofVec2f &contentSize)
{
    m_ContentSize = contentSize;

    setAnchorPercent(m_AnchorPercent, contentSize);
}

bool Node::getIsDrawContentLimit() const
{
    return m_IsDrawContentLimit;
}

void Node::setDrawContentLimit(bool isDraw)
{
    m_IsDrawContentLimit = isDraw;
}

Node *Node::getParent()
{
    return m_Parent;
}

void Node::setParent(Node *parent)
{
    m_Parent = parent;
}

Node *Node::getChildByName(const string &name)
{
    const int index = getChildIndex(name);
    return index == -1 ? nullptr : m_Children.at(index);
}

void Node::addChild(Node *child, const int &localZOrder)
{
    if (child == nullptr) {
        return;
    }

    if (child->getParent()) {
        ofLog() << __FUNCTION__ << "Node already has a parent!";
        return;
    }

    child->retain();
    child->setParent(this);
    child->setLocalZOrder(localZOrder);
    child->setOrderOfArrival(m_GlobalOrderOfArrival++);
    m_Children.push_back(child);
    sortChildren();
}

void Node::addChild(Node *child)
{
    addChild(child, child->getLocalZOrder());
}

void Node::removeFromParent()
{
    m_Parent->removeChild(m_Name);
}

void Node::removeChild(const string &name)
{
    const int index = getChildIndex(name);
    if (index == -1) {
        return;
    }

    m_Children.at(index)->release();
    m_Children.erase(m_Children.begin() + index);
}

int Node::getLocalZOrder() const
{
    return m_LocalZOrder;
}

void Node::setLocalZOrder(int z)
{
    m_LocalZOrder = z;
}

int Node::getOrderOfArrival() const
{
    return m_OrderOfArrival;
}

void Node::setOrderOfArrival(int orderOfArrival)
{
    m_OrderOfArrival = orderOfArrival;
}

bool Node::isCascadeOpacityEnabled() const
{
    return m_IsCascadeOpacityEnabled;
}

void Node::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_IsCascadeOpacityEnabled = cascadeOpacityEnabled;
}

std::string Node::getName() const
{
    return m_Name;
}

void Node::setName(const std::string &name)
{
    m_Name = name;
}

ofRectangle Node::getBoundingBox() const
{
    return ofRectangle(this->getPosition(), this->getContentSize().x * this->getScale().x, this->getContentSize().y * this->getScale().y);
}

void Node::startAutoUpdate()
{
    if (m_IsAutoUpdating == false) {
        m_IsAutoUpdating = true;
        ofAddListener(ofEvents().update, this, &Node::autoUpdate);
    }
}

void Node::stopAutoUpdate()
{
    if (m_IsAutoUpdating) {
        m_IsAutoUpdating = false;
        ofRemoveListener(ofEvents().update, this, &Node::autoUpdate);
    }
}

GLubyte Node::getDisplayedOpacity() const
{
    return m_DisplayedOpacity;
}

GLubyte Node::getRealOpacity() const
{
    return m_RealOpacity;
}

float Node::getSkewX() const
{
    return m_Skew.x;
}

void Node::setSkewX(float skewX)
{
    m_Skew.x = skewX;
}

float Node::getSkewY() const
{
    return m_Skew.y;
}

void Node::setSkewY(float skewY)
{
    m_Skew.y = skewY;
}

ofVec2f Node::getSkew() const
{
    return m_Skew;
}

void Node::setSkew(float skewX, float skewY)
{
    m_Skew.x = skewX;
    m_Skew.y = skewY;
}

void Node::setSkew(const ofVec2f &skew)
{
    m_Skew = skew;
}

void Node::skewView(const float &skewX, const float &skewY)
{
    float skewMatArray[16] = {
        1, tanf(ofDegToRad(skewX)), 0, 0,
        tanf(ofDegToRad(skewY)), 1, 0, 0,
        0,  0,  1, 0,
        0,  0,  0, 1
    };
    ofMatrix4x4 mat(skewMatArray);
    const ofMatrix4x4 pre(ofGetCurrentRenderer()->getCurrentMatrix(ofMatrixMode::OF_MATRIX_MODELVIEW));
    ofGetCurrentRenderer()->loadMatrix(pre * mat);
}

void Node::sortChildren()
{
    auto sortFunc = [](Node * n1, Node * n2) {
        return (n1->getLocalZOrder() < n2->getLocalZOrder() ||
                (n1->getLocalZOrder() == n2->getLocalZOrder() && n1->getOrderOfArrival() < n2->getOrderOfArrival()));
    };

    std::sort(std::begin(m_Children), std::end(m_Children), sortFunc);
}

int Node::getChildIndex(const string &name)
{
    auto foundIt = std::find_if(m_Children.begin(), m_Children.end(), [this, name](const Node * child) {
        return child->getName() == name;
    });

    if (foundIt == m_Children.end()) {
        return -1;
    }

    return std::distance(m_Children.begin(), foundIt);
}

void Node::drawContentLimits() const
{
    if (m_IsDrawContentLimit) {
        ofPushStyle();
        {
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(ofColor::red);
            ofDrawRectangle(ofPoint::zero(), m_ContentSize.x, m_ContentSize.y);
            ofSetLineWidth(1);
            ofFill();
            ofSetColor(ofColor::green);
            ofCircle(ofPoint(m_AnchorPoint.x, m_AnchorPoint.y), 5);
        }
        ofPopStyle();
    }
}

void Node::updateCascadeOpacity()
{
    GLubyte parentOpacity = 255;

    if (m_Parent != nullptr && m_Parent->isCascadeOpacityEnabled()) {
        parentOpacity = m_Parent->getDisplayedOpacity();
    }

    updateDisplayedOpacity(parentOpacity);
}

void Node::updateDisplayedOpacity(GLubyte parentOpacity)
{
    m_DisplayedOpacity = m_RealOpacity * parentOpacity / 255;
    updateColor();

    if (isCascadeOpacityEnabled()) {
        for (const auto &child : m_Children) {
            child->updateDisplayedOpacity(m_DisplayedOpacity);
        }
    }
}
