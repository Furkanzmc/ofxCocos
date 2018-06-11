#ifndef OFXCOCOSTYPES_H
#define OFXCOCOSTYPES_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete; \
    TypeName &operator =(const TypeName &) = delete

#define SAFE_RELEASE(p) do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_DELETE(p) do { if(p) { delete (p); (p) = nullptr; } } while(0)

#define OFXCOCOS_NS_BEGIN namespace ofxCocos {
#define OFXCOCOS_NS_END }
#define USING_NS_OFXCOCOS using namespace ofxCocos;

#define ACTIONS_NS_BEGIN namespace Actions {
#define ACTIONS_NS_END }
#define USING_NS_OFXCOCOS_ACTIONS using namespace ofxCocos::Actions;

#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->setup()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

#endif // OFXCOCOSTYPES_H
