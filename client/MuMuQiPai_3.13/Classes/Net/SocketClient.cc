#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include "SocketClient.h"

static void *requestSocketThread(void *data)
{
    SocketRequest *request = NULL;
    SocketClient* self = (SocketClient*)data;
    
    while (1)
    {
        self->getRequestQueue()->get_msg(request);
//        printf("requestThread cmd %d data %s\n", request->cmd, request->data.c_str());
        bool sd = self->isConnect();
        if (CONNECT_REQ == request->cmd)
        {
//            self->reset_response_queue();
//            self->connect_req(request);
        }
        else if (self->isConnect() && DISCONNECT_REQ == request->cmd)
        {
            self->disconnect_req(request);
        }
        else if (self->isConnect() && DATA_REQ == request->cmd)
        {
            self->data_req(request);
        }
        delete request;
    }
    pthread_exit(NULL);
}

static void *responseSocketThread(void *data)
{
    SocketClient* self = (SocketClient*)data;
    while (1)
    {
        if (self->isConnect() == false)
        {
            sleep(1);
            continue;
        }
        CCLOG("lct:begin to read phase in responseThread");
        
        if (self->isConnect())
        {
//            if (self->protoType == ProtoType::JSON)
//            {
//                self->read_res();
//            }
//            else if (self->protoType == ProtoType::PROTOBUF_WANBEI)
//            {
//                self->read_res_pb();
//            }
//            else if (self->protoType == ProtoType::PROTOBUF_CJJ || self->protoType == ProtoType::PROTOBUF_CJJ2)
//            {
//                self->read_res_pb2();
//            }
//            else if (self->protoType == ProtoType::PROTOBUF_CJJ3)
//            {
//                self->read_res_pb3();
//            }
        }
        else
        {
            CCLOG("lct:no connect");
            continue;
        }
    }
    
    pthread_exit(NULL);
}
SocketClient::SocketClient()
{
    
}

SocketClient::~SocketClient()
{
    
}

std::map<std::string, SocketClient *> g_socketClients;
SocketClient* SocketClient::getInstance(std::string id)
{
    SocketClient* socketClient = g_socketClients[id];
    if (socketClient == NULL)
    {
        socketClient = new SocketClient();
        socketClient->init();
        g_socketClients[id] = socketClient;
    }
    return socketClient;
}

void SocketClient::onClose(SIOClient* client)
{
    log("onClose");
    log("%s is close",client->getTag());
    m_pSocklient = NULL;
    setConnectState(false);
    g_socketClients[getCurRoomId()] = NULL;
    
    //连接关闭
    Json::Value data;
    create_response(DISCONNECT_RES, data.toStyledString());
}
void SocketClient::onError(SIOClient* client, const std::string& data)
{
    log("onError");
    log("%s is Error %s",client->getTag(),data.c_str());
}

void SocketClient::fireEventToScript(SIOClient* client, const std::string& eventName, const std::string& data) {
    CC_UNUSED_PARAM(client);
    
    if(eventName == "disconnect"){
        m_pSocklient = NULL;
        setConnectState(false);
        g_socketClients[getCurRoomId()] = NULL;
        
        //连接关闭
        Json::Value data;
        create_response(DISCONNECT_RES, data.toStyledString());
    }
    CCLOG("SIODelegate event '%s' fired with data: %s", eventName.c_str(), data.c_str());
};

void SocketClient::init()
{
    m_pRequestQueue = new Queue<SocketRequest*>();
    m_pResponseQueue = new Queue<SocketResponse*>();
    
    pthread_create(&m_requestThread, NULL, requestSocketThread, this);
    pthread_detach(m_requestThread);
    
//    pthread_create(&m_responseThread, NULL, responseSocketThread, this);
//    pthread_detach(m_responseThread);
}

SocketResponse* SocketClient::get()
{
    SocketResponse *response = NULL;
    int ret = m_pResponseQueue->pop_msg(response);
    if (ret == 0)
    {
        return response;
    }
    else
    {
        return NULL;
    }
}

void SocketClient::put(SocketRequest *request)
{
    m_pRequestQueue->put_msg(request);
}

void SocketClient::data_req(SocketRequest *request)
{
    std::string jsonStr = getJsonStr(request);
    if (m_pSocklient){
//        log("data_req: %s",jsonStr.c_str());
        m_pSocklient->emit("data", jsonStr);
    }
}

void SocketClient::send_Data(std::string sendData, int id)
{
    SocketRequest* request = new SocketRequest();
    request->cmd = DATA_REQ;
    request->data = sendData;
    request->id = id;
    request->seqNo = -1;
    put(request);
}

void SocketClient::connect_req()
{
    m_pSocklient = SocketIO::connect("127.0.0.1:10000",*this);
    if (m_pSocklient)
    {
        m_pSocklient->setTag("initsocket");
        m_pSocklient->on("data", [=](SIOClient* c, const std::string& data)
                       {
                           //存入消息队列
                           Json::Reader reader;
                           Json::Value  recvVal;
                           recvVal.clear();
                           reader.parse(data,recvVal);
                           create_response(RECV_DATA_OK_RES,recvVal["data"].toStyledString(),recvVal["id"].asInt());
                           if(recvVal["id"].asInt() == 4003){//连接成功
                               this->setConnectState(true);
                           }
                       });
    }
}

void SocketClient::disconnect_req(SocketRequest *request)
{
    
}

void SocketClient::create_response(int cmd, string data, int id, int seqNo, int svrid)
{
    SocketResponse* response = new SocketResponse();
    response->cmd = cmd;
    response->data = data;
    response->id = id;
    response->seqNo = seqNo;
    response->svrid = svrid;
    response->time = Utils::getCurrentTime();
    m_pResponseQueue->put_msg(response);
}

std::string SocketClient::getJsonStr(SocketRequest *request){
    std::string repStr;
    Json::Value repJson;
    repJson["cmd"] = request->cmd;
    repJson["data"] = request->data;
    repJson["id"] = request->id;
    repJson["seqNo"] = request->seqNo;
    return repJson.toStyledString();
}

void SocketClient::reset_response_queue()
{
    SocketResponse *response = NULL;
    while (true) {
        response = get();
        if (!response)
            break;
        delete response;
    }
}

