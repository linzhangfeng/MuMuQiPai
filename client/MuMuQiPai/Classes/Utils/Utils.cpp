#include "Utils.h"
USING_NS_CC;
#include "scripting/js-bindings/manual/ScriptingCore.h"

Utils *m_Utils;

Utils::Utils()
{
    
}

Utils::~Utils()
{
    
}

Utils* Utils::getInstance()
{
    if(m_Utils == NULL){
        m_Utils = new Utils();
    }
    return m_Utils;
}

Node* Utils::findNode(Node* node,const string& name)
{
    if(!node)
    {
        return NULL;
    }
    if(node->getName() ==name)
    {
        return node;
    }
    auto& cs = node->getChildren();
    for(auto i : cs)
    {
        auto ret = Utils::findNode(i,name);
        if(ret)
        {
            return ret;
        }
    }
    return NULL;
}

long long Utils::getCurrentTime()
{
    return Utils::getCurrentSecend();
}
long long Utils::getCurrentSecend()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ms;
}