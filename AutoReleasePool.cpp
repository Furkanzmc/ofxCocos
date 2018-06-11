/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
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

#include "AutoreleasePool.h"
// openFrameworks
#include "ofMain.h"
USING_NS_OFXCOCOS

AutoreleasePool::AutoreleasePool()
    : m_Name("")
#if defined(_DEBUG)
    , m_IsClearing(false)
#endif
{
    m_ManagedObjectArray.reserve(150);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::AutoreleasePool(const std::string &name)
    : m_Name(name)
#if defined(_DEBUG)
    , m_IsClearing(false)
#endif
{
    m_ManagedObjectArray.reserve(150);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::~AutoreleasePool()
{
    ofLog() << "deallocing AutoreleasePool: %p" << m_Name;
    clear();

    PoolManager::getInstance()->pop();
}

void AutoreleasePool::addObject(ofxCocos::Ref *object)
{
    m_ManagedObjectArray.push_back(object);
}

void AutoreleasePool::clear()
{
#if defined(_DEBUG)
    m_IsClearing = true;
#endif
    std::vector<Ref *> releasings;
    releasings.swap(m_ManagedObjectArray);

    for (const auto &obj : releasings) {
        if (obj->getReferenceCount() == 0) {
            obj->release();
        }
    }

#if defined(_DEBUG)
    m_IsClearing = false;
#endif
}

bool AutoreleasePool::contains(ofxCocos::Ref *object) const
{
    for (const auto &obj : m_ManagedObjectArray) {
        if (obj == object)
            return true;
    }
    return false;
}

void AutoreleasePool::dump()
{
    ofLog() << "autorelease pool: %s, number of managed object %d\n" << m_Name.c_str() << static_cast<int>(m_ManagedObjectArray.size());
    for (const auto &obj : m_ManagedObjectArray) {
        (void)obj;
    }
}


//--------------------------------------------------------------------
//
// PoolManager
//
//--------------------------------------------------------------------

PoolManager *PoolManager::m_SingleInstance = nullptr;

PoolManager::PoolManager()
{
    m_ReleasePoolStack.reserve(10);
    ofAddListener(ofEvents().update, this, &PoolManager::autoUpdate);
}

PoolManager::~PoolManager()
{
    ofLog() << "deallocing PoolManager: %p" << this;
    ofRemoveListener(ofEvents().update, this, &PoolManager::autoUpdate);

    while (!m_ReleasePoolStack.empty()) {
        AutoreleasePool *pool = m_ReleasePoolStack.back();
        delete pool;
    }
}

PoolManager *PoolManager::getInstance()
{
    if (m_SingleInstance == nullptr) {
        m_SingleInstance = new(std::nothrow) PoolManager();
        // Add the first auto release pool
        new AutoreleasePool("cocos2d autorelease pool");
    }

    return m_SingleInstance;
}

void PoolManager::destroyInstance()
{
    delete m_SingleInstance;
    m_SingleInstance = nullptr;
}

AutoreleasePool *PoolManager::getCurrentPool() const
{
    return m_ReleasePoolStack.back();
}

bool PoolManager::isObjectInPools(ofxCocos::Ref *obj) const
{
    for (const auto &pool : m_ReleasePoolStack) {
        if (pool->contains(obj))
            return true;
    }

    return false;
}

void PoolManager::push(AutoreleasePool *pool)
{
    m_ReleasePoolStack.push_back(pool);
}

void PoolManager::autoUpdate(ofEventArgs &eventArgs)
{
    (void)eventArgs;
    getCurrentPool()->clear();
}

void PoolManager::pop()
{
    if (m_ReleasePoolStack.empty() == false) {
        m_ReleasePoolStack.pop_back();
    }
}
