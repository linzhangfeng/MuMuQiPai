#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "Network.h"
#include "TcpClientTest.h"



static void *requestThread(void *data)
{
    MsgRequest * request = NULL;
    TcpClientTest* self = (TcpClientTest*)data;
    while (1) {
        self->s_requestQueue->get_msg(request);
        if(request->cmd == REQ_CONNECT)
        {
            self->resetResponeQueue();
            self->connectReq(request);
        }
        else if(request->cmd == REQ_DISCONNECT)
        {
            self->disconnectReq(request);
        }
        else if(request->cmd == REQ_DATA)
        {
            self->dataReq(request);
        }
        delete request;
    }
    pthread_exit(NULL);          
}

static void *responseThread(void *data)
{
    TcpClientTest* self = (TcpClientTest*)data;
    while (1) {
        self->readRes();
    }
    pthread_exit(NULL);
}

std::map<int, TcpClientTest*> g_tcpClients;

TcpClientTest* TcpClientTest::getInstance(int id)
{
    TcpClientTest* tcpClient = g_tcpClients[id];
    if(!tcpClient){
        tcpClient = new TcpClientTest();
        tcpClient->init();
        g_tcpClients[id] = tcpClient;
    }
    return tcpClient;
}

TcpClientTest::TcpClientTest()
{
    
}

TcpClientTest::~TcpClientTest()
{
    
}

void TcpClientTest::init()
{
    s_requestQueue = new Queue<MsgRequest*>();
    s_responseQueue = new Queue<MsgResponse*>();
    
    pthread_create(&s_requestThread, NULL, requestThread, this);
    pthread_detach(s_requestThread);
    
    pthread_create(&s_responseThread, NULL, responseThread, this);
    pthread_detach(s_responseThread);
    
    m_isConnected = false;
    m_isClose = true;
    m_fd = -1;
}

MsgResponse* TcpClientTest::get()
{
    MsgResponse* response = new MsgResponse();
    s_responseQueue->get_msg(response);
    return response;
}

void TcpClientTest::put(MsgRequest *request)
{
    s_requestQueue->put_msg(request);
}

void TcpClientTest::startConnect()
{
    resetResponeQueue();
    resetConnect();
    createRequre(REQ_CONNECT, "REQ_CONNECT");
}

void TcpClientTest::resetConnect()
{
    if(m_isConnected)
    {
        close(m_fd);
        m_fd = -1;
        m_isConnected = false;
        m_isClose = true;
    }
    else
    {
        m_fd = -1;
        m_isConnected = false;
        m_isClose = true;
    }

}

void TcpClientTest::disconnect()
{
    createRequre(REQ_DISCONNECT, "REQ_DISCONNECT");
}

void TcpClientTest::connectReq(MsgRequest *request)
{
    m_fd = Network::tcp_connect(m_host.c_str(), m_port, 3, 3);
    if(m_fd < 0)
    {
        createRespone(RES_CONNECT_ERROR, "RES_CONNECT_ERROR");
    }else
    {
        createRespone(RES_CONNECT_OK, "RES_CONNECT_OK");
    }
}

void TcpClientTest::disconnectReq(MsgRequest *request)
{
    resetConnect();
}

void TcpClientTest::dataReq(MsgRequest *request)
{
    int ret = writeData(request);
    if(ret < 0)
    {
        resetConnect();
        createRespone(REQ_DISCONNECT, "REQ_DISCONNECT");
    }else
    {
        createRespone(RES_SEND_DATA_OK, "RES_SEND_DATA_OK");
    }
}

int TcpClientTest::packHeader(MsgJsonHeader *pHeader, char *pBuff)
{
    char* buf = pBuff;
    memcpy(buf, &(pHeader->length), sizeof(pHeader->length));
    return 0;
}

int TcpClientTest::writeData(MsgRequest* request)
{
    MsgJsonHeader tmHeader;
    tmHeader.length = request->data.length();
    
    int allLen = sizeof(tmHeader) + tmHeader.length;
    cout << "tmHeader to size: " << sizeof(tmHeader)  << endl;
    cout << "tmHeader to length: " << tmHeader.length  << endl;
    char allBuff[65536] = {0};
    char* pall = allBuff;
    packHeader(&tmHeader, pall);
    pall += sizeof(tmHeader);
    memcpy(pall, request->data.c_str(), tmHeader.length);
    
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = allBuff;
    nleft = allLen;
    
    while (nleft > 0) {
        nwritten = write(m_fd,ptr,nleft);
        if(nwritten <= 0)
        {
            return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return 0;
}

void TcpClientTest::sendData(std::string data)
{
    cout << "sendData:" << data  << endl;
    createRequre(REQ_DATA, data);
}

void TcpClientTest::readRes()
{
    int res = 0;
    int ret;
    int cur_head_len = 0;
    int cur_body_len = 0;
    char recv_buf[65536];
    int state = PARSE_STATE_HEADER;
    fd_set fds;
    struct timeval tv;
    m_isClose = false;
    while (1) {
        if(m_isClose)break;
        FD_ZERO(&fds);
        //cout << "m_fd to connect: " << m_fd  << endl;
        if(m_fd != -1)
        {
            FD_SET(m_fd,&fds);
        }
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if(m_fd != -1)res = select(m_fd+1, &fds, NULL, NULL, &tv);
        if(res > 0)
        {
            if(state == PARSE_STATE_HEADER){
                ret = read(m_fd, &header[cur_head_len], sizeof(struct Header_Packet) - cur_head_len);
                if(ret <= 0)
                {
                    if(errno == EINTR)continue;
                    //读取失败
                    break;
                }
                cur_head_len = cur_head_len + ret;
                
                if(cur_head_len == sizeof(header))
                {
                    state = PARSE_STATE_BODY;
                    cur_head_len = 0;
                    cur_body_len = 0;
                    body.clear();
                }
            }
            else if(state == PARSE_STATE_BODY)
            {
                ret = read(m_fd, &recv_buf, header_p->length - cur_body_len);
                if(ret <= 0)
                {
                    if(errno == EINTR)continue;
                    //读取失败
                    break;
                }
                
                cur_body_len += ret;
                recv_buf[ret] = '\0';
                body.append(recv_buf,ret);
                
                if(cur_body_len == header_p->length)
                {
                    state = PARSE_STATE_HEADER;
                    cur_body_len = 0;
                    cur_body_len = 0;
                    createRespone(RES_RECV_DATA_OK, body);
                }
            }
        }
    }
}

void TcpClientTest::resetResponeQueue()
{
    s_requestQueue->reset_msg();
}

long long TcpClientTest::getCurrentTime()
{
    return getCurrentSecend();
}

long long TcpClientTest::getCurrentSecend()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ms;
}

long long TcpClientTest::createRequre(int cmd,std::string data)
{
    MsgRequest *requre = new MsgRequest();
    requre->cmd = cmd;
    requre->data = data;
    requre->seqNo = 123;
    s_requestQueue->put_msg(requre);
    return requre->seqNo;
}

void TcpClientTest::createRespone(int cmd,std::string data)
{
    MsgResponse* reponse = new MsgResponse();
    reponse->cmd = cmd;
    reponse->data = data;
    reponse->seqNo = 123;
    reponse->time = getCurrentTime();
}


void TcpClientTest::setHost(std::string host)
{
    m_host = host;
}

void TcpClientTest::setPost(int port)
{
    m_port = port;
}