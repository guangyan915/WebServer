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

#include "configparser.h"
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
    ~WebServer();

    // 启动服务器
    void Start();

private:

    // 初始化服务器监听套接字
    bool InitSocket();

    // 根据触发模式初始化事件模式
    void InitEventMode(int trigMode);

    // 向 Epoller 添加客户端连接
    void AddClient(int fd, sockaddr_in addr);

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
    int _max_fd;

    bool _load_conf_file_ok;  // 配置文件是否加载成功

    int _port;
    bool _open_linger;        // 优雅关闭
    int _timeout_MS;          // 超时时间（毫秒）
    bool _is_close;           // 服务器是否关闭
    int _listen_fd;           
    char* _src_root_dir;

    uint32_t _listen_event;   // 监听事件模式
    uint32_t _conn_event;     // 新连接事件模式

    HeapTimer* _timer;
    ThreadPool* _thread_pool;
    Epoller* _epoller;
    std::unordered_map<int, HttpConn> _users;
};

#endif //WEBSERVER_H

