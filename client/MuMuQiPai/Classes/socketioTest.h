
#ifndef __SocketIo_H__
#define __SocketIo_H__

#include "lin.h"
#include "network/SocketIO.h"
#include "network/HttpClient.h"

using namespace cocos2d::network;
class SocketTest :public SocketIO::SIODelegate
{
public:
    /** Return the shared instance **/
    static SocketTest *getInstance();
    
    virtual void onConnect(SIOClient* client);
    
    virtual void onMessage(SIOClient* client, const std::string& data);
    
    virtual void onClose(SIOClient* client);
    virtual void onError(SIOClient* client, const std::string& data);

    void  sendEvent();
    void  closeSocket();
    void  connect();

    SocketTest();
    ~SocketTest();
    
    SIOClient *_socklient;
    
    
};

#endif // __HELLOWORLD_SCENE_H__
