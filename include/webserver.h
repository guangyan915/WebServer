#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <unordered_map>
#include <fcntl.h>      
#include <unistd.h>     
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include  "configparser.h"
#include "epoller.h"      
#include "log.h"
#include "heaptimer.h"  
#include "sqlconnectionpool.h" 
#include "threadpool.h"
#include "sqlconnectionRAII.h"
#include "httpconnection.h"

class WebServer {
public:
    WebServer();
    WebServer(
        int port, int trigMode, int timeoutMS, bool OptLinger,
        int sqlPort, const char* sqlUser, const  char* sqlPwd,
        const char* dbName, int connPoolNum, int threadNum,
        bool openLog, int logLevel, int logQueSize);
    ~WebServer();

    // 启动服务器
    void Start();

private:

    // 初始化服务器监听套接字
    bool initSocket();

    // 根据触发模式初始化事件模式
    void InitEventMode_(int trigMode);

    // 向 Epoller 添加客户端连接
    void AddClient_(int fd, sockaddr_in addr);

    // 处理监听套接字事件
    void DealListen();

    // 处理写事件
    void DealWrite(HttpConn* client);

    // 处理读事件
    void DealRead(HttpConn* client);

    // 发送错误响应
    void SendError(int fd, const char* info);

    // 延长连接超时时间
    void ExtenTime(HttpConn* client);

    // 关闭连接
    void CloseConn(HttpConn* client);

    // 处理读事件回调
    void OnRead(HttpConn* client);

    // 处理写事件回调
    void OnWrite(HttpConn* client);

    // 处理请求响应
    void OnProcess(HttpConn* client);

    // 设置文件描述符为非阻塞模式
    static int SetFdNonblock(int fd);

    // 最大文件描述符数量
    static const int MAX_FD = 65536;

    bool _load_conf_file_ok;
    int _port;
    bool _openLinger;
    int _timeoutMS;  // 超时时间（毫秒）
    bool _is_close;
    int _listen_fd;
    char* _src_root_dir;

    uint32_t _listen_event;
    uint32_t _conn_event;

    HeapTimer* _timer;
    ThreadPool* _thread_pool;
    Epoller* _epoller;
    std::unordered_map<int, HttpConn> _users;
};

#endif //WEBSERVER_H

