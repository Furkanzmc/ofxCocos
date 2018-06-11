#include "Sprite.h"
#include "TextureCache.h"
// openFrameworks
#include "ofMain.h"
USING_NS_OFXCOCOS

Sprite::Sprite()
    : m_Texture(nullptr)
    , m_IsTextureRectSet(false)
{

}

Sprite *Sprite::create(const string &filePath)
{
    Sprite *sp = new(std::nothrow) Sprite();
    if (sp && sp->setup(filePath)) {
        sp->autorelease();
    }
    else {
        SAFE_RELEASE(sp);
    }

    return sp;
}

bool Sprite::setup(const string &filePath)
{
    if (!Node::setup()) {
        return false;
    }

    if (filePath.size() > 0) {
        m_Texture = TextureCache::getInstance()->addTexture(filePath);
    }

    updateContentSize();

    return true;
}

void Sprite::customDraw() const
{
    if (m_Texture && m_Texture->isAllocated()) {
        if (m_IsTextureRectSet == false) {
            m_Texture->draw(0, 0, 0);
        }
        else {
            m_Texture->drawSubsection(0, 0, 0, m_TextureRect.width, m_TextureRect.height,
                                      m_TextureRect.x, m_TextureRect.y, m_TextureRect.width, m_TextureRect.height);
        }
    }
}

ofTexture *Sprite::getTexture()
{
    return m_Texture.get();
}

void Sprite::setTexture(const string &filePath)
{
    if (m_Texture && m_Texture.unique()) {
        m_Texture->clear();
    }

    m_Texture.reset();
    m_Texture = TextureCache::getInstance()->addTexture(filePath);
    updateContentSize();
}

void Sprite::setTexture(std::shared_ptr<ofTexture> texture)
{
    if (texture == nullptr) {
        ofLogError() << __FUNCTION__ << ": Texture cannot be nullptr!";
        return;
    }

    if (m_Texture && m_Texture.unique()) {
        m_Texture->clear();
    }

    m_Texture.reset();
    m_Texture = std::move(texture);
    updateContentSize();
}

const ofRectangle &Sprite::getTextureRect() const
{
    return m_TextureRect;
}

void Sprite::setTextureRect(const ofRectangle &textureRect)
{
    m_TextureRect.x = std::max(std::min(textureRect.x, m_ContentSize.x), 0.f);
    m_TextureRect.y = std::max(std::min(textureRect.y, m_ContentSize.y), 0.f);

    m_TextureRect.width = std::max(std::min(textureRect.width, m_ContentSize.x), 0.f);
    m_TextureRect.height = std::max(std::min(textureRect.height, m_ContentSize.y), 0.f);

    m_IsTextureRectSet = !(m_TextureRect.width == m_Texture->getWidth() && m_TextureRect.height == m_Texture->getHeight()
                           && m_TextureRect.x == 0 && m_TextureRect.y == 0);

    updateContentSize(true);
}

bool Sprite::isAllocated() const
{
    return m_Texture && m_Texture->isAllocated();
}

ofVec2f Sprite::getTextureSize() const
{
    return ofVec2f(m_Texture->getWidth(), m_Texture->getHeight());
}

void Sprite::clearTexture()
{
    if (!m_Texture) {
        return;
    }

    if (m_Texture.unique()) {
        m_Texture->clear();
    }
    else {
        ofLog(OF_LOG_NOTICE, "%s Texture use count is %d", __FUNCTION__, m_Texture.use_count());
    }
}

void Sprite::updateContentSize(bool isUseTextureRect)
{
    if (m_Texture != nullptr && m_Texture->isAllocated()) {
        if (isUseTextureRect == false) {
            this->setContentSize(ofVec2f(m_Texture->getWidth(), m_Texture->getHeight()));
            setTextureRect(ofRectangle(0, 0, m_ContentSize.x, m_ContentSize.y));
        }
        else {
            this->setContentSize(ofVec2f(m_TextureRect.width, m_TextureRect.height));
        }
    }
}
