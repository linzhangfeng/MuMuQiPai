#include "socketioTest.h"

using namespace cocostudio::timeline;

SocketTest *g_pSocketTestModel;
SocketTest* SocketTest::getInstance()
{
    if (g_pSocketTestModel == NULL) {
        g_pSocketTestModel = new SocketTest();
    }
    return g_pSocketTestModel;
}

SocketTest::SocketTest()
{

}


SocketTest::~SocketTest()
{
    
}
void SocketTest::connect()
{
    _socklient = SocketIO::connect("127.0.0.1:10000",*this);
    if (_socklient)
    {
        _socklient->setTag("init socket");
        _socklient->on("news", [=](SIOClient* c, const std::string& data)
                       {
                       });
        _socklient->on("event", [=](SIOClient* c, const std::string& data)
                       {
                       });
    }
    
//    sendEvent();  //发送事件testevent给服务器，参数是{\"name"}
}

void  SocketTest::sendEvent()
{
    if (_socklient)
        _socklient->emit("event", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");
}
void SocketTest::closeSocket()
{
//    if (_socklient)
//        _socklient->disconnect();
}
void SocketTest::onConnect(SIOClient* client)
{
    log("onConnect ");
    log("%s connect",client->getTag());
    
    
}

void SocketTest::onMessage(SIOClient* client, const std::string& data)
{
    log("onMessage");
    log("%s received content is :%s",client->getTag(),data.c_str());
    
    
    
}

void SocketTest::onClose(SIOClient* client)
{
    log("onClose");
    log("%s is close",client->getTag());
    
}
void SocketTest::onError(SIOClient* client, const std::string& data)
{
    log("onError");
    log("%s is Error %s",client->getTag(),data.c_str());
}