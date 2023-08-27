#ifndef SQL_CONNECT_RAII_H
#define SQL_CONNECT_RAII_H

#include "sqlconnectionpool.h"
#include <mysql/mysql.h>
#include <vector>
#include <string>

class SqlConnRAII {
public:
    // 构造函数，在构造对象时获取数据库连接
    SqlConnRAII(SqlConnPool* connpool) {
        assert(connpool);
        _sql = connpool->GetConn();
        _connpool = connpool;
    }

    // 析构函数，在对象析构时释放数据库连接
    ~SqlConnRAII() {
        if (_sql) {
            _connpool->FreeConn(_sql);  // 释放连接
        }
    }

    // 执行 SQL 语句
    bool Execute(const std::string& query) {
        if (!_sql) {
            return false;  // 连接不存在，无法执行
        }

        if (mysql_query(_sql, query.c_str()) != 0) {
            return false;  // 执行 SQL 语句失败
        }

        return true;  // 执行成功
    }

    // 执行 SQL 查询，并返回结果集
    std::vector<std::vector<std::string>> Query(const std::string& query) {
        std::vector<std::vector<std::string>> result;

        if (!_sql) {
            return result;  // 连接不存在，返回空结果集
        }

        if (mysql_query(_sql, query.c_str()) != 0) {
            return result;  // 执行 SQL 查询失败，返回空结果集
        }

        MYSQL_RES* res = mysql_store_result(_sql);
        if (!res) {
            return result;  // 无查询结果，返回空结果集
        }

        int num_fields = mysql_num_fields(res);

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            std::vector<std::string> row_data;
            for (int i = 0; i < num_fields; ++i) {
                row_data.push_back(row[i] ? row[i] : "NULL");
            }
            result.push_back(row_data);
        }

        mysql_free_result(res);
        return result;
    }

private:
    MYSQL* _sql;                 // 数据库连接对象
    SqlConnPool* _connpool;      // 数据库连接池对象
};

#endif

