//#ifndef HEAP_TIMER_H
//#define HEAP_TIMER_H

#pragma once


#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h> 
#include <functional> 
#include <assert.h> 
#include <chrono>
#include "log.h"

 // 超时回调函数类型
typedef std::function<void()> TimeoutCallBack;
// Clock 为 std::chrono 中的高分辨率时钟类型，用于测量时间间隔
typedef std::chrono::high_resolution_clock Clock;
// MS 为毫秒的时间间隔，用于表示一段时间
typedef std::chrono::milliseconds MS;
// TimeStamp 为 Clock::time_point 类型，用于表示时间点
typedef Clock::time_point TimeStamp;

// 定义定时器节点结构体
struct TimerNode {
    int id;
    TimeStamp expires;                      // 定时器到期时间点
    TimeoutCallBack cb;                     // 定时器回调函数
    bool operator < (const TimerNode& t) {
        return expires < t.expires;
    }
    bool operator > (const TimerNode& t) {
        return expires > t.expires;
    }
};

// 定时器类
class HeapTimer {
public:
    HeapTimer() { _heap.reserve(64); }

    ~HeapTimer() { clear(); }

    void adjust(int id, int newExpires);                        // 调整定时器到期时间

    void add(int id, int timeOut, const TimeoutCallBack& cb);   // 添加定时器

    void doWork(int id);                                        // 执行定时器回调函数

    void clear();                                               // 清空定时器

    void tick();                                                // 执行到期的定时器回调函数

    void pop();                                                 // 移除堆顶定时器

    int GetNextTick();                                          // 获取下一个定时器到期的时间

private:
    void del(size_t i);                                        // 移除指定位置的定时器

    void siftup(size_t i);                                     // 将节点上浮到合适位置

    bool siftdown(size_t index, size_t n);                     // 将节点下沉到合适位置

    void SwapNode(size_t i, size_t j);                         // 交换两个节点的位置

    std::vector<TimerNode> _heap;                               // 存储定时器节点的堆

    std::unordered_map<int, size_t> _ref;                       // 存储定时器节点id和在堆中的位置映射
};

//#endif 

