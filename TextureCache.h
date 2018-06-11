#pragma once
#include <memory>
#include <map>
#include <string>
#include "ofxCocosTypes.h"

class ofTexture;
class ofEventArgs;

OFXCOCOS_NS_BEGIN

class TextureCache
{
public:
    ~TextureCache();

    static TextureCache *getInstance();
    static void destroyInstance();

    void printUseCounts();

    std::shared_ptr<ofTexture> getTexture(const std::string &texturePath);
    std::shared_ptr<ofTexture> addTexture(const std::string &texturePath);

    void clean();

private:
    static TextureCache *m_Instance;
    std::map<std::string, std::shared_ptr<ofTexture>> m_Textures;

private:
    TextureCache();

    void autoUpdate(ofEventArgs &eventArgs);
};

OFXCOCOS_NS_END
