//#ifndef _bufferH
//#define _bufferH

#pragma once

#include <cstring>   // 包含C字符串操作函数的头文件，如bzero
#include <iostream>  // 输入输出流库头文件
#include <unistd.h>  // 包含Unix标准库的头文件，如write
#include <sys/uio.h> // 包含readv函数所需的头文件
#include <vector>    // 向量库头文件，用于存储数据
#include <atomic>    // 原子操作库头文件
#include <assert.h>  // 断言库头文件

class Buffer {
public:
    // 构造函数，初始化缓冲区大小，默认为1024字节
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;

    // 返回可写入的字节数
    size_t WritableBytes() const;

    // 返回可读取的字节数
    size_t ReadableBytes() const;

    // 返回已预留的字节数
    size_t PrependableBytes() const;

    // 返回缓冲区数据的起始指针
    const char* Peek() const;

    // 确保缓冲区可写入指定长度的数据
    void EnsureWriteable(size_t len);

    // 标记已写入指定长度的数据
    void HasWritten(size_t len);

    // 从缓冲区中读取指定长度的数据，移动读指针
    void Retrieve(size_t len);

    // 从缓冲区中读取数据直至指定位置，移动读指针
    void RetrieveUntil(const char* end);

    // 重置缓冲区，将所有数据清空
    void RetrieveAll();

    // 将缓冲区内的所有数据转为字符串并清空缓冲区
    std::string RetrieveAllToStr();

    // 返回写指针的常量指针
    const char* BeginWriteConst() const;

    // 返回写指针的指针
    char* BeginWrite();

    // 在缓冲区末尾追加字符串
    void Append(const std::string& str);

    // 在缓冲区末尾追加指定长度的字符串
    void Append(const char* str, size_t len);

    // 在缓冲区末尾追加指定长度的数据
    void Append(const void* data, size_t len);

    // 在缓冲区末尾追加另一个缓冲区
    void Append(const Buffer& buff);

    // 从文件描述符中读取数据到缓冲区
    ssize_t ReadFd(int fd, int* Errno);

    // 将缓冲区数据写入文件描述符
    ssize_t WriteFd(int fd, int* Errno);

private:
    // 返回缓冲区的起始指针
    char* BeginPtr_();

    // 返回缓冲区的起始指针（常量版本）
    const char* BeginPtr_() const;

    // 扩展缓冲区的大小以容纳指定长度的数据
    void MakeSpace_(size_t len);

    // 存储实际数据的向量
    std::vector<char> _buffer;

    // 读指针位置，原子类型，支持多线程并发操作
    std::atomic<std::size_t> _read_pos;

    // 写指针位置，原子类型，支持多线程并发操作
    std::atomic<std::size_t> _write_pos;
};

//#endif // _bufferH

