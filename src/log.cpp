#include "../include/log.h" 

using namespace std;

Log::Log() {
    _line_count = 0;
    _is_async = false;
    _write_thread = nullptr;
    _deque = nullptr;
    _to_day = 0;
    _log_file_fp = nullptr;
}

Log::~Log() {
    // 释放资源和线程
    if (_write_thread && _write_thread->joinable()) {
        while (!_deque->empty()) {
            _deque->flush();
        };
        _deque->Close();
        _write_thread->join();
    }
    if (_log_file_fp) {
        lock_guard<mutex> locker(_mtx);
        flush();
        fclose(_log_file_fp);
    }
}

int Log::GetLevel() {
    // 获取日志级别
    lock_guard<mutex> locker(_mtx);
    return _level;
}

void Log::SetLevel(int level) {
    // 设置日志级别
    lock_guard<mutex> locker(_mtx);
    _level = level;
}

void Log::init(int level, const char* path, const char* suffix,
    int maxQueueSize) {
    // 初始化日志
    _is_open = true; // 标记日志系统为开启状态
    _level = level; // 设置日志级别

    if (maxQueueSize > 0) {
        _is_async = true; // 启用异步写入模式

        if (!_deque) {
            // 如果异步队列为空，创建一个新的异步队列
            unique_ptr<BlockDeque<std::string>> newDeque(new BlockDeque<std::string>);
            _deque = move(newDeque);

            // 启动异步线程去刷新缓冲区数据到文件
            std::unique_ptr<std::thread> NewThread(new thread(FlushLogThread));
            _write_thread = move(NewThread);
        }
    }
    else {
        _is_async = false; // 关闭异步写入模式
    }

    _line_count = 0; // 初始化行计数器

    time_t timer = time(nullptr); // 获取当前时间戳
    struct tm* sysTime = localtime(&timer); // 转换为本地时间结构
    struct tm t = *sysTime;

    _path = path;
    _suffix = suffix;

    char fileName[LOG_NAME_LEN] = { 0 };
    // 使用格式化字符串生成日志文件名
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s",
        _path, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, _suffix);
    _to_day = t.tm_mday; 

    {
        lock_guard<mutex> locker(_mtx);
        _buff.RetrieveAll(); // 清空缓冲区
        if (_log_file_fp) {
            flush(); // 刷新缓冲区到文件
            fclose(_log_file_fp); // 关闭之前的日志文件
        }

        // 创建新的日志文件并以追加模式打开
        _log_file_fp = fopen(fileName, "a");
        if (_log_file_fp == nullptr) {
            mkdir(_path, 0777); // 如果目录不存在，创建目录
            _log_file_fp = fopen(fileName, "a"); // 再次尝试打开文件
        }
        assert(_log_file_fp != nullptr);
    }
}


void Log::write(int level, const char* format, ...) {
    struct timeval now = { 0, 0 };
    // 获取当前时间戳和微秒
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    // 将时间戳转换为本地时间结构
    struct tm* sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list vaList;

    // 是否需要切换日志文件
    if (_to_day != t.tm_mday || (_line_count && (_line_count % MAX_LINES == 0)))
    {
        unique_lock<mutex> locker(_mtx);
        locker.unlock();

        char newFile[LOG_NAME_LEN];
        char tail[36] = { 0 };
        // 将年、月、日信息格式化成字符串写入到tail里"YYYY_MM_DD
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);

        // 将存储路径、日期字符串、后缀名拼接在一起，生成一个完整的文件路径字符串，用于创建日志文件
        if (_to_day != t.tm_mday)
        {
            
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", _path, tail, _suffix);
            _to_day = t.tm_mday;
            _line_count = 0;
        }
        else {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", _path, tail, (_line_count / MAX_LINES), _suffix);
        }

        locker.lock();
        flush();
        // 关闭写满的文件
        fclose(_log_file_fp);
        // 追加模式打开创建新文件
        _log_file_fp = fopen(newFile, "a");
        assert(_log_file_fp != nullptr);
    }

    {
        unique_lock<mutex> locker(_mtx);
        _line_count++;
        // 将日期和时间信息格式化为字符串，放入缓冲区
        int n = snprintf(_buff.BeginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
            t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);

        _buff.HasWritten(n);
        // 根据日志等级，在缓冲区中添加相应的日志等级标题
        AppendLogLevelTitle_(level);

        va_start(vaList, format);
        // 使用格式化字符串和可变参数将日志内容添加到缓冲区
        int m = vsnprintf(_buff.BeginWrite(), _buff.WritableBytes(), format, vaList);
        va_end(vaList);

        _buff.HasWritten(m);
        _buff.Append("\n\0", 2);

        // 如果是异步模式并且阻塞队列未满，将缓冲区中的内容添加到阻塞队列
        if (_is_async && _deque && !_deque->full()) {
            _deque->push_back(_buff.RetrieveAllToStr());
        }
        else {
            // 否则将缓冲区中的内容写入日志文件
            fputs(_buff.Peek(), _log_file_fp);
        }
        _buff.RetrieveAll();
    }
}

void Log::AppendLogLevelTitle_(int level) {
    // 添加日志级别标签
    switch (level) {
    case 0:
        _buff.Append("[debug]: ", 9);
        break;
    case 1:
        _buff.Append("[info] : ", 9);
        break;
    case 2:
        _buff.Append("[warn] : ", 9);
        break;
    case 3:
        _buff.Append("[error]: ", 9);
        break;
    default:
        _buff.Append("[info] : ", 9);
        break;
    }
}

void Log::flush() {
    // 刷新日志缓冲
    if (_is_async) {
        _deque->flush();
    }
    // 将缓冲区中的数据写入到日志文件中
    fflush(_log_file_fp);
}

void Log::AsyncWrite() {
    // 异步写日志
    string str = "";
    while (_deque->pop(str)) {
        lock_guard<mutex> locker(_mtx);
        fputs(str.c_str(), _log_file_fp);
    }
}

Log* Log::Instance() {
    // 单例
    static Log inst;
    return &inst;
}

void Log::FlushLogThread() {
    // 异步写日志线程函数
    Log::Instance()->AsyncWrite();
}

