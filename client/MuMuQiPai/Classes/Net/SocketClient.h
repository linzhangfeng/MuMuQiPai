#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include <string>
#include <semaphore.h>
#include "cocos2d.h"

#include "../lin.h"
#include "JPacket.h"
#include "Queue.h"
enum request_cmd
{
    CONNECT_REQ = 0,
    DISCONNECT_REQ,
    DATA_REQ,
};

enum response_cmd
{
    CONNECT_OK_RES = 0,
    CONNECT_ERROR_RES,
    SEND_DATA_OK_RES,
    SEND_DATA_ERROR_RES,
    RECV_DATA_OK_RES,
    RECV_DATA_ERROR_RES,
    DISCONNECT_RES,
};

class SocketRequest
{
public:
    int cmd;
    std::string data;
    int id;
    int seqNo;
};

class SocketResponse
{
public:
    int cmd;
    std::string data;
    int id;
    int seqNo;
    int svrid;
    int time;
    void release(){delete this;}
    std::string parseDatta(){return JPacket::decodeStr(data);}
};

class SocketClient :public Ref,SocketIO::SIODelegate
{
public:
    virtual void onClose(SIOClient* client);
    virtual void onError(SIOClient* client, const std::string& data);
    
public:
    /** Return the shared instance **/
    static SocketClient *getInstance(std::string vid = "");
    /** Relase the shared instance **/
//    static void destroyInstance();
    SocketClient();
    virtual ~SocketClient();
    void init();
    
    void put(SocketRequest* request);
    SocketResponse *get();
    void connect_req();
    void data_req(SocketRequest *request);
    void disconnect_req(SocketRequest *request);
//    void read_res_pd();
    void reset_response_queue();
    void create_response(int cmd, std::string data, int id = -1,int seqNo = 0, int svrid = 0);
    std::string getJsonStr(SocketRequest *request);
    void send_Data(std::string sendData,int cmd);
    void setConnectState(bool isConnet){
        m_bConnect = isConnet;
    }
    void seCurRoomId(std::string curRoomId){
        m_curRoomId = curRoomId;
    }
    std::string getCurRoomId(){
        return m_curRoomId;
    }
private:
    Queue<SocketRequest*> *m_pRequestQueue;
    Queue<SocketResponse*> *m_pResponseQueue;
    bool m_bConnect;
    pthread_t       m_requestThread;
    pthread_t       m_responseThread;
    SIOClient       *m_pSocklient;
    std::string     m_curRoomId;

public:
    static std::string m_url;
    
    Queue<SocketRequest*> *getRequestQueue(){
        return m_pRequestQueue;
    }
    Queue<SocketResponse*> *getResponseQueue(){
        return m_pResponseQueue;
    }
    bool isConnect(){
        return m_bConnect;
    };
};

#endif //_TCP_CLIENT_H_
