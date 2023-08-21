#ifndef SQL_CONNECTION_POOL_H
#define SQL_CONNECTION_POOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "log.h"

class SqlConnPool {
public:
    static SqlConnPool* Instance();
    // 获取一个数据库连接
    MYSQL* GetConn();

    // 释放数据库连接
    void FreeConn(MYSQL* conn);

    // 获取空闲数据库连接数量
    int GetFreeConnCount();

    // 初始化数据库连接池
    void Init(const char* host, int port,
        const char* user, const char* pwd,
        const char* dbName, int connSize);

    // 关闭数据库连接池
    void ClosePool();

private:
    SqlConnPool();
    ~SqlConnPool();

    int _MAX_CON_COUNT;              // 最大连接数
    int _user_count;                 // 已使用连接数
    int _free_count;                 // 空闲连接数

    std::queue<MYSQL*> _conn_que;   // 存储连接的队列
    std::mutex _mtx;                // 互斥锁，用于保护对连接队列的操作
    sem_t _semId;                   // 信号量，用于控制连接的使用

};

#endif

