#ifndef SQL_CONNECT_RAII_H
#define SQL_CONNECT_RAII_H
#include "sqlconnectionpool.h"

 /* 资源在对象构造初始化 资源在对象析构时释放*/
class SqlConnRAII {
public:
    // 构造函数，在构造对象时获取数据库连接
    SqlConnRAII(MYSQL** sql, SqlConnPool* connpool) {
        assert(connpool);
        *sql = connpool->GetConn();
        _sql = *sql;
        _connpool = connpool;
    }

    // 析构函数，在对象析构时释放数据库连接
    ~SqlConnRAII() {
        if (_sql) {
            _connpool->FreeConn(_sql);  // 释放连接
        }
    }

private:
    MYSQL* _sql;                 // 数据库连接对象
    SqlConnPool* _connpool;      // 数据库连接池对象
};

#endif

