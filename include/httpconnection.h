//#ifndef HTTP_CONNECTION_H
//#define HTTP_CONNECTION_H


#pragma once

#include <sys/types.h>
#include <sys/uio.h>     
#include <arpa/inet.h>   
#include <stdlib.h>     
#include <errno.h>      

#include "log.h"
#include "sqlconnectionRAII.h"
#include "buff.h"
#include "httprequest.h"
#include "httpresponse.h"

 // HTTP连接类，处理HTTP请求和响应
class HttpConn {
public:
    HttpConn();

    ~HttpConn();

    // 初始化连接
    void init(int sockFd, const sockaddr_in& addr);

    // 从套接字读取数据
    ssize_t read(int* saveErrno);

    // 向套接字写入数据
    ssize_t write(int* saveErrno);

    // 关闭连接
    void Close();

    // 获取套接字文件描述符
    int GetFd() const;

    // 获取端口号
    int GetPort() const;

    // 获取IP地址
    const char* GetIP() const;

    // 获取套接字地址
    sockaddr_in GetAddr() const;

    // 处理HTTP请求
    bool process();

    // 获取待写入的字节数
    int ToWriteBytes() {
        return _iov[0].iov_len + _iov[1].iov_len;
    }

    // 检查是否保持连接
    bool IsKeepAlive() const {
        return _request.IsKeepAlive();
    }

    // 是否为边沿触发模式
    static bool _is_ET;

    // 资源目录路径
    static const char* _src_dir;

    // 当前连接用户数
    static std::atomic<int> _user_count;

private:

    int _fd;                        // 套接字文件描述符
    struct sockaddr_in _addr;       // 套接字地址

    bool _is_close;                 // 连接是否关闭

    int _iov_cnt;                   // iov 数量
    struct iovec _iov[2];           // 用于readv/writev

    Buffer read_buff;               // 读缓冲区
    Buffer write_buff;              // 写缓冲区

    HttpRequest _request;           // HTTP请求
    HttpResponse _response;         // HTTP响应
};


//#endif

