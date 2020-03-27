#ifndef _TCP_CLIENT_TEST_H_
#define _TCP_CLIENT_TEST_H_

#include <string>
#include <semaphore.h>
#include "JPacket.h"
#include "Queue.h"

enum RequestCmd
{
    REQ_CONNECT = 0,
    REQ_DISCONNECT,
    REQ_DATA,
};

enum ResponseRmd
{
    RES_CONNECT_OK = 0,
    RES_CONNECT_ERROR,
    RES_SEND_DATA_OK,
    RES_SEND_DATA_ERROR,
    RES_RECV_DATA_OK,
    RES_RECV_DATA_ERROR,
    RES_DISCONNECT,
};

enum ParseState
{
    PARSE_STATE_HEADER    	= 0,
    PARSE_STATE_BODY      	= 1,
    PARSE_STATE_ERR       	= 2,
    PARSE_STATE_CROSS_DOMAIN	= 3,
};

struct MsgJsonHeader {
    unsigned int length;
};

void test();

class MsgRequest
{
public:
    int cmd;
    std::string data;
    int id;
    long long seqNo;
};

class MsgResponse
{
public:
    int cmd;
    std::string data;
    int id;
    long long seqNo;
    int svrid;
    long long time;
    void release(){delete this;}
    std::string parseDatta(){return JPacket::decodeStr(data);}
};

class TcpClientTest
{
public:
    /** Return the shared instance **/
    static TcpClientTest *getInstance(int vid = -1);
    
    TcpClientTest();
    ~TcpClientTest();
    
public:
    void init();
    void startConnect();
    void resetConnect();
    void disconnect();
    
    void connectReq(MsgRequest* request);
    void disconnectReq(MsgRequest* request);
    void dataReq(MsgRequest* request);
    void readRes();
    int writeData(MsgRequest* request);
    int packHeader(MsgJsonHeader* pHeader,char* pBuff);
    void createRespone(int cmd,std::string data);
    long long createRequre(int cmd ,std::string data);
    
    void put(MsgRequest* request);
    MsgResponse *get();
    
    void sendData(std::string data);
    void resetResponeQueue();
    
    long long getCurrentTime();
    long long getCurrentSecend();
    
    void setHost(std::string host);
    void setPost(int port);
public:
    Queue<MsgRequest*> *s_requestQueue;
    Queue<MsgResponse*> *s_responseQueue;
    
    pthread_t       s_requestThread;
    pthread_t       s_responseThread;
    
    int                 m_fd;
    
    char                header[sizeof(struct MsgJsonHeader)];
    struct MsgJsonHeader    *header_p;
    std::string         body;
    bool                m_isConnected;
    bool                m_isClose;
    std::string         m_host;
    int                 m_port;
};

#endif //_TCP_CLIENT_TEST_H_
