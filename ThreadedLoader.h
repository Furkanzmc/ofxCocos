#ifndef THREADEDLOADER_H
#define THREADEDLOADER_H
// STD
#include <vector>
#include <functional>
// ofxCocos
#include "ofxCocosTypes.h"

class ofHttpResponse;

OFXCOCOS_NS_BEGIN

class Sprite;

class ThreadedLoader
{
public:
    ~ThreadedLoader();

    static ThreadedLoader *getInstance();
    static void destroyInstance();

    void loadImage(const std::string &url, Sprite *sp, std::function<void(Sprite *)> callback = nullptr);
    void downloadImage(const std::string &url, const std::string &filePath);

private:
    static ThreadedLoader *m_Instance;
    static int m_LastIndex;

    std::vector<std::tuple<std::string, unsigned int, Sprite *>> m_Sprites;
    std::vector<std::pair<unsigned int, std::function<void(Sprite *)>>> m_Callbacks;
    std::vector<int> m_IDs;

private:
    ThreadedLoader();

    void onNewResponse(ofHttpResponse &response);
    bool isRequestBelongsHere(int id);

    Sprite *getSpriteWithIndex(const unsigned int &index);

    std::string getSaveFilePath(const unsigned int &index) const;
    void saveFile(const char *data, const unsigned int &size, const std::string &filepath);

    void executeCallback(const unsigned int &index, Sprite *sp);
};

OFXCOCOS_NS_END

#endif // THREADEDLOADER_H
