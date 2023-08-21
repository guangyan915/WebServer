//#ifndef EPOLLER_H
//#define EPOLLER_H


#pragma once

#include <sys/epoll.h> 
#include <fcntl.h>     
#include <unistd.h>    
#include <assert.h>   
#include <vector>
#include <errno.h>

class Epoller {
public:
    explicit Epoller(int maxEvent = 1024);          // 构造函数，初始化 epoll 句柄和事件数组
    ~Epoller();                                     // 析构函数，关闭 epoll 句柄
    bool AddFd(int fd, uint32_t events);            // 添加文件描述符和关注的事件到 epoll
    bool ModFd(int fd, uint32_t events);            // 修改文件描述符关注的事件
    bool DelFd(int fd);                             // 从 epoll 中删除文件描述符
    int Wait(int timeoutMs = -1);                   // 等待事件的发生，timeoutM 为等待超时时间
    int GetEventFd(size_t i) const;                 // 获取第 i 个事件的文件描述符
    uint32_t GetEvents(size_t i) const;             // 获取第 i 个事件的事件类型
private:
    int _epoll_fd;                                  // epoll 句柄
    std::vector<struct epoll_event> _events;        // 用于存储事件的数组
};


//#endif
