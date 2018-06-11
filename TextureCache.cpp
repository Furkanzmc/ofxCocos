#include "TextureCache.h"
// openFrameworks
#include "ofMain.h"
USING_NS_OFXCOCOS

TextureCache *TextureCache::m_Instance = nullptr;

TextureCache::TextureCache()
{

}

TextureCache::~TextureCache()
{
    for (auto &item : m_Textures) {
        if (item.second) {
            item.second->clear();
        }

        item.second.reset();
    }
}

TextureCache *TextureCache::getInstance()
{
    if (m_Instance == nullptr) {
        m_Instance = new TextureCache();
    }

    return m_Instance;
}

void TextureCache::destroyInstance()
{
    if (!m_Instance) {
        return;
    }

    delete m_Instance;
    m_Instance = 0;
}

void TextureCache::printUseCounts()
{
    for (auto &item : m_Textures) {
        std::cerr << item.first << " -> " << item.second.use_count() << std::endl;
    }
}

std::shared_ptr<ofTexture> TextureCache::getTexture(const std::string &texturePath)
{
    if (m_Textures[texturePath] == nullptr) {
        return addTexture(texturePath);
    }

    return m_Textures[texturePath];
}

std::shared_ptr<ofTexture> TextureCache::addTexture(const std::string &texturePath)
{
    if (m_Textures.find(texturePath) == m_Textures.end()) {
        auto tex = std::shared_ptr<ofTexture>(new ofTexture());
        if (ofLoadImage(*tex.get(), texturePath)) {
            m_Textures[texturePath] = std::move(tex);
            return m_Textures[texturePath];
        }
        else {
            ofLogError("ofxCocos::addTexture", "File (" + texturePath + ") cannot be found.");
            tex.reset();
            return nullptr;
        }
    }

    return m_Textures[texturePath];
}

void TextureCache::clean()
{
    auto it = m_Textures.begin();
    while (it != m_Textures.end()) {
        if ((*it).second.unique()) {
            (*it).second->clear();
            (*it).second.reset();
            it = m_Textures.erase(it);
        }
        else {
            it++;
        }
    }
}

void TextureCache::autoUpdate(ofEventArgs &eventArgs)
{
    (void)eventArgs;
    clean();
}
