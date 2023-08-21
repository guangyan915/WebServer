#include "../include/epoller.h"


Epoller::Epoller(int maxEvent) : _epoll_fd(epoll_create(512)), _events(maxEvent) {
    assert(_epoll_fd >= 0 && _events.size() > 0);
}

Epoller::~Epoller() {
    close(_epoll_fd);
}

bool Epoller::AddFd(int fd, uint32_t events) {
    if (fd < 0) return false;
    epoll_event ev = { 0 };
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev);  // 添加事件到 epoll
}

bool Epoller::ModFd(int fd, uint32_t events) {
    if (fd < 0) return false;
    epoll_event ev = { 0 };
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev);  // 修改事件在 epoll 中的关注类型
}

bool Epoller::DelFd(int fd) {
    if (fd < 0) return false;
    epoll_event ev = { 0 };
    return 0 == epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &ev);  // 从 epoll 中删除事件
}

int Epoller::Wait(int timeoutMs) {
    return epoll_wait(_epoll_fd, &_events[0], static_cast<int>(_events.size()), timeoutMs);  // 等待事件发生
}

int Epoller::GetEventFd(size_t i) const {
    assert(i < _events.size() && i >= 0);
    return _events[i].data.fd;  // 获取第 i 个事件的文件描述符
}

uint32_t Epoller::GetEvents(size_t i) const {
    assert(i < _events.size() && i >= 0);
    return _events[i].events;  // 获取第 i 个事件的事件类型
}

