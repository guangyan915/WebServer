#ifndef LOG_H
#define LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>         
#include <assert.h>
#include <sys/stat.h>       
#include "blockqueue.h"       
#include "buff.h"

class Log {
public:
    void init(int level, const char* path = "./log",
        const char* suffix = ".log",
        int maxQueueCapacity = 1024,
        int log_max_size = 78402); // 初始化日志

    static Log* Instance();                         // 获取单例实例
    static void FlushLogThread();                   // 异步写日志线程

    void write(int level, const char* format, ...); // 写日志
    void flush();                                   // 刷新日志缓冲

    int GetLevel();                                 // 获取日志级别
    void SetLevel(int level);                       // 设置日志级别
    bool IsOpen() { return _is_open; }               // 判断日志是否开启

private:
    Log();
    void AppendLogLevelTitle(int level);           // 添加日志级别标签
    virtual ~Log();
    void AsyncWrite();                             // 异步写日志

private:
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    //static const int MAX_LINES = 5000;

    const char* _path;                              // 日志存储路径
    const char* _suffix;                            // 日志文件后缀

    int _log_max_lines;

    int _line_count;                                // 当前日志文件日志行数
    int _to_day;                                    // 现在是哪一天                                    

    bool _is_open;                                  // 日志系统是否打开

    Buffer _buff;                                   // 日志缓冲
    int _level;                                     // 日志级别
    bool _is_async;                                 // 是否异步写日志

    FILE* _log_file_fp;                                      // 日志文件指针
    std::unique_ptr<BlockDeque<std::string>> _deque;// 阻塞队列，用于异步写日志
    std::unique_ptr<std::thread> _write_thread;      // 异步写日志的线程
    std::mutex _mtx;                                // 互斥锁
};

#define LOG_BASE(level, format, ...) \
    do {\
        Log* log = Log::Instance();\
        if (log->IsOpen() && log->GetLevel() <= level) {\
            log->write(level, format, ##__VA_ARGS__); \
            log->flush();\
        }\
    } while(0);


#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);

#endif 

