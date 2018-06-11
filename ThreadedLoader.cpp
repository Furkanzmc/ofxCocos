#include "ThreadedLoader.h"
#include "Sprite.h"
// openFrameworks
#include "ofMain.h"
USING_NS_OFXCOCOS

ThreadedLoader *ThreadedLoader::m_Instance = nullptr;
int ThreadedLoader::m_LastIndex = 0;

ThreadedLoader::ThreadedLoader()
{
    ofAddListener(ofURLResponseEvent(), this, &ThreadedLoader::onNewResponse);
}

ThreadedLoader::~ThreadedLoader()
{
    ofRemoveListener(ofURLResponseEvent(), this, &ThreadedLoader::onNewResponse);
}

ThreadedLoader *ThreadedLoader::getInstance()
{
    if (m_Instance == nullptr) {
        m_Instance = new ThreadedLoader();
    }

    return m_Instance;
}

void ThreadedLoader::destroyInstance()
{
    delete m_Instance;
    m_Instance = nullptr;
}

void ThreadedLoader::loadImage(const std::string &url, Sprite *sp, std::function<void(Sprite *)> callback)
{
    m_IDs.push_back(ofLoadURLAsync(url, std::to_string(m_LastIndex)));
    m_Sprites.push_back(std::make_tuple("", m_LastIndex, sp));
    if (callback) {
        m_Callbacks.push_back(std::make_pair(m_LastIndex, callback));
    }
    m_LastIndex++;
}

void ThreadedLoader::downloadImage(const std::string &url, const std::string &filePath)
{
    m_IDs.push_back(ofLoadURLAsync(url, std::to_string(m_LastIndex)));

    m_Sprites.push_back(std::make_tuple(filePath, m_LastIndex, nullptr));
    m_LastIndex++;
}

void ThreadedLoader::onNewResponse(ofHttpResponse &response)
{
    if (isRequestBelongsHere(response.request.getID()) == false) {
        return;
    }

    if (response.status != 200) {
        ofLogError() << __FUNCTION__ << ": Response failed. error buffer: " << response.error;
        return;
    }

    const unsigned int index = std::atoi(response.request.name.c_str());

    const std::string fileSavePath = getSaveFilePath(index);
    if (fileSavePath.size() > 0) {
        saveFile(response.data.getData(), response.data.size(), fileSavePath);
        return;
    }

    std::shared_ptr<ofTexture> texture = std::shared_ptr<ofTexture>(new ofTexture());
    if (texture && ofLoadImage(*texture.get(), response.data)) {
        //Now create Sprite from downloaded image
        Sprite *sprite = getSpriteWithIndex(index);
        if (sprite) {
            sprite->setTexture(texture);
            executeCallback(index, sprite);
        }
        else {
            ofLogWarning() << __FUNCTION__ << ": A sprite is not found!";
        }
    }
    else {
        ofLogWarning() << __FUNCTION__ << ": Sprite cannot be loaded! Request: " << response.request.name;
    }
}

bool ThreadedLoader::isRequestBelongsHere(int id)
{
    auto foundIt = std::find(m_IDs.begin(), m_IDs.end(), id);
    if (foundIt != m_IDs.end()) {
        m_IDs.erase(foundIt);
        return true;
    }

    return false;
}

Sprite *ThreadedLoader::getSpriteWithIndex(const unsigned int &index)
{
    auto foundIt = std::find_if(m_Sprites.begin(), m_Sprites.end(), [index](const std::tuple<std::string, unsigned int, Sprite *> &p) {
        return std::get<1>(p) == index;
    });

    if (foundIt == m_Sprites.end()) {
        return nullptr;
    }

    const int foundIndex = std::distance(m_Sprites.begin(), foundIt);

    Sprite *sp = foundIndex >= 0 ? std::get<2>(m_Sprites.at(foundIndex)) : nullptr;
    if (sp) {
        m_Sprites.erase(foundIt);
    }
    return sp;
}

std::string ThreadedLoader::getSaveFilePath(const unsigned int &index) const
{
    auto foundIt = std::find_if(m_Sprites.begin(), m_Sprites.end(), [index](const std::tuple<std::string, unsigned int, Sprite *> &p) {
        return std::get<1>(p) == index;
    });

    if (foundIt == m_Sprites.end()) {
        return "";
    }

    const int foundIndex = std::distance(m_Sprites.begin(), foundIt);
    return foundIndex >= 0 ? std::get<0>(m_Sprites.at(foundIndex)) : "";
}

void ThreadedLoader::saveFile(const char *data, const unsigned int &size, const std::string &filepath)
{
    std::ofstream out;
    out.open(filepath, std::ios::out | std::ios::binary);
    if (out.is_open()) {
        out.write(data, size);
        out.close();
    }
    else {
        ofLog() << __FUNCTION__ << ": Cannot open file = " << filepath;
    }
}

void ThreadedLoader::executeCallback(const unsigned int &index, Sprite *sp)
{
    auto foundIt = std::find_if(m_Callbacks.begin(), m_Callbacks.end(), [index](const std::pair<unsigned int, std::function<void(Sprite *)>> &p) {
        return p.first == index;
    });

    if (foundIt == m_Callbacks.end()) {
        return;
    }

    const int foundIndex = std::distance(m_Callbacks.begin(), foundIt);
    if (foundIndex >= 0 && m_Callbacks.at(foundIndex).second) {
        m_Callbacks.at(foundIndex).second(sp);
        m_Callbacks.erase(foundIt);
    }
}
