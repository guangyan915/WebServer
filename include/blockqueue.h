//#ifndef BLOCKQUEUE_H
//#define BLOCKQUEUE_H
#pragma once


#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>


template<class T>
class BlockDeque {
public:
    explicit BlockDeque(size_t MaxCapacity = 1000);
    ~BlockDeque();

    void clear();       // 清空队列
    bool empty();       // 判断队列是否为空
    bool full();        // 判断队列是否已满
    void Close();       // 关闭队列，清空队列，释放资源
    size_t size();      // 获取队列中元素个数
    size_t capacity();  // 获取队列容量
    T front();                          // 获取队首元素
    T back();                           // 获取队尾元素
    void push_back(const T& item);      // 将元素加入队尾
    void push_front(const T& item);     // 将元素加入队首
    bool pop(T& item);                  // 弹出队首元素
    bool pop(T& item, int timeout);     // 弹出队首元素，支持超时等待
    void flush();                       // 唤醒等待在队列上的消费者线程

private:
    std::deque<T> _deque;                   // 队列容器
    size_t _capacity;                       // 队列容量
    std::mutex _mtx;                        // 互斥锁，保护队列操作
    bool _is_close;                         // 队列是否关闭的标志
    std::condition_variable _cv_consumer;   // 消费者条件变量
    std::condition_variable _cv_producer;   // 生产者条件变量
};


template<class T>
BlockDeque<T>::BlockDeque(size_t MaxCapacity) :_capacity(MaxCapacity) {
    assert(MaxCapacity > 0);
    _is_close = false;
}

template<class T>
BlockDeque<T>::~BlockDeque() {
    Close();
};

template<class T>
void BlockDeque<T>::Close() {
    {   
        std::lock_guard<std::mutex> locker(_mtx);
        _deque.clear();
        _is_close = true;
    }
    _cv_producer.notify_all();
    _cv_consumer.notify_all();
};

template<class T>
void BlockDeque<T>::flush() {
    _cv_consumer.notify_one();
};

template<class T>
void BlockDeque<T>::clear() {
    std::lock_guard<std::mutex> locker(_mtx);
    _deque.clear();
}

template<class T>
T BlockDeque<T>::front() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deque.front();
}

template<class T>
T BlockDeque<T>::back() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deque.back();
}

template<class T>
size_t BlockDeque<T>::size() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deque.size();
}

template<class T>
size_t BlockDeque<T>::capacity() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _capacity;
}

template<class T>
void BlockDeque<T>::push_back(const T &item) {
    std::unique_lock<std::mutex> locker(_mtx);
    while(_deque.size() >= _capacity) {
        _cv_producer.wait(locker);
    }
    _deque.push_back(item);
    _cv_consumer.notify_one();
}

template<class T>
void BlockDeque<T>::push_front(const T &item) {
    std::unique_lock<std::mutex> locker(_mtx);
    while(_deque.size() >= _capacity) {
        _cv_producer.wait(locker);
    }
    _deque.push_front(item);
    _cv_consumer.notify_one();
}

template<class T>
bool BlockDeque<T>::empty() {
    std::lock_guard<std::mutex> locker(_mtx);
    return _deque.empty();
}

template<class T>
bool BlockDeque<T>::full(){
    std::lock_guard<std::mutex> locker(_mtx);
    return _deque.size() >= _capacity;
}

template<class T>
bool BlockDeque<T>::pop(T &item) {
    std::unique_lock<std::mutex> locker(_mtx);
    while(_deque.empty()){
        _cv_consumer.wait(locker);
        if(_is_close){
            return false;
        }
    }
    item = _deque.front();
    _deque.pop_front();
    _cv_producer.notify_one();
    return true;
}

template<class T>
bool BlockDeque<T>::pop(T &item, int timeout) {
    std::unique_lock<std::mutex> locker(_mtx);
    while(_deque.empty()){
        if(_cv_consumer.wait_for(locker, std::chrono::seconds(timeout)) 
                == std::cv_status::timeout){
            return false;
        }
        if(_is_close){
            return false;
        }
    }
    item = _deque.front();
    _deque.pop_front();
    _cv_producer.notify_one();
    return true;
}

//#endif // BLOCKQUEUE_H
