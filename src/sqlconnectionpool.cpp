#include "../include/sqlconnectionpool.h"
using namespace std;

SqlConnPool::SqlConnPool() {
    _user_count = 0;
    _free_count = 0;
}

// 单例
SqlConnPool* SqlConnPool::Instance() {
    static SqlConnPool connPool;
    return &connPool;
}

// 初始化数据库连接池
void SqlConnPool::Init(const char* host, int port,
    const char* user, const char* pwd, const char* dbName,
    int connSize) {
    assert(connSize > 0);  // 连接数量必须大于0
    for (int i = 0; i < connSize; i++) {
        MYSQL* sql = nullptr;
        sql = mysql_init(sql);  // 初始化MySQL连接对象
        if (!sql) {
            LOG_ERROR("MySql 初始化失败！");
            assert(sql);
        }
        sql = mysql_real_connect(sql, host,
            user, pwd,
            dbName, port, nullptr, 0);  // 创建实际连接
        if (!sql) {
            LOG_ERROR("MySql 连接失败！");
        }
        _conn_que.push(sql);
    }
    _MAX_CON_COUNT = connSize;
    sem_init(&_semId, 0, _MAX_CON_COUNT);  // 初始化信号量
}

// 获取数据库连接
MYSQL* SqlConnPool::GetConn() {
    MYSQL* sql = nullptr;
    if (_conn_que.empty()) {  // 如果连接队列为空
        LOG_WARN("SqlConnPool 繁忙！");
        return nullptr;
    }
    sem_wait(&_semId);  // 等待信号量
    {
        lock_guard<mutex> locker(_mtx);
        sql = _conn_que.front();  // 获取队列首连接
        _conn_que.pop();  // 弹出队列首连接
    }
    return sql;
}

// 释放数据库连接
void SqlConnPool::FreeConn(MYSQL* sql) {
    assert(sql);
    lock_guard<mutex> locker(_mtx);
    _conn_que.push(sql);  // 将连接放回连接队列
    sem_post(&_semId);  // 增加信号量
}

// 关闭数据库连接池
void SqlConnPool::ClosePool() {
    lock_guard<mutex> locker(_mtx);
    while (!_conn_que.empty()) {
        auto item = _conn_que.front();
        _conn_que.pop();  // 弹出连接队列中的连接
        mysql_close(item);  // 关闭连接
    }
    mysql_library_end();  // 关闭MySQL库
}

// 获取空闲数据库连接数量
int SqlConnPool::GetFreeConnCount() {
    lock_guard<mutex> locker(_mtx);
    return _conn_que.size();  // 返回连接队列大小
}

// SqlConnPool类的析构函数
SqlConnPool::~SqlConnPool() {
    ClosePool();  // 关闭连接池
}

