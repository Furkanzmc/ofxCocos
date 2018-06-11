#ifndef SPRITE_H
#define SPRITE_H
#include "Node.h"
// openFrameworks
#include "ofRectangle.h"
class ofTexture;

OFXCOCOS_NS_BEGIN

class Sprite : public ofxCocos::Node
{
public:
    static Sprite *create(const std::string &filePath = "");

    ofTexture *getTexture();
    void setTexture(const std::string &filePath);
    void setTexture(std::shared_ptr<ofTexture> texture);

    const ofRectangle &getTextureRect() const;
    void setTextureRect(const ofRectangle &textureRect);

    bool isAllocated() const;
    ofVec2f getTextureSize() const;

    /**
     * @brief Clears the content of the texture if the texture is unique and valid.
     */
    void clearTexture();

protected:
    void customDraw() const override;

private:
    std::shared_ptr<ofTexture> m_Texture;
    // This is set to true if the texture size and texture rect does not equal to each other
    bool m_IsTextureRectSet;

private:
    ofRectangle m_TextureRect;

private:
    Sprite();
    bool setup(const std::string &filePath);

    /**
     * @brief If isUseTextureRect is true uses texture rect to set the content size, otherwise uses the texture size
     * @param isUseTextureRect
     */
    void updateContentSize(bool isUseTextureRect = false);
};

OFXCOCOS_NS_END

#endif // SPRITE_H
