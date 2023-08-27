#include "../include/mysqlopt.h"
#include "../include/log.h"
#include "../include/QString.hpp"


// 注册功能
int MysqlOpt::Register(const std::string& name, const std::string& phone, const std::string& passwd) {
    SqlConnRAII connRAII(SqlConnPool::Instance());
    std::string query = QString("SELECT * FROM users WHERE phone_number='%0'").arg(phone).toStdString();


    std::vector<std::vector<std::string>> queryResult = connRAII.Query(query);

    if (!queryResult.empty()) {
        LOG_DEBUG("手机号已存在,注册失败");
        return -2;  // 手机号已存在，注册失败
    }

    query = QString("INSERT INTO users (name, phone_number, passwd) VALUES ('%0', '%1', '%2')").arg(name).arg(phone).arg(passwd).toStdString();

    if (!connRAII.Execute(query)) {
        LOG_WARN("%s 执行失败", query.c_str());
        return -1;  // 注册失败
    }
    LOG_DEBUG("name:%s,passwd:%s,phone:%s注册成功！",name.c_str(), passwd.c_str(), phone.c_str());
    return 0;  // 注册成功
}

 
// 登录功能
    
int MysqlOpt::Login(const std::string& name, const std::string& passwd) {
    SqlConnRAII connRAII(SqlConnPool::Instance());
    std::string query = QString("SELECT * FROM users WHERE (name='%0' OR phone_number='%1') AND passwd='%2'").arg(name).arg(name).arg(passwd).toStdString();
    std::vector<std::vector<std::string>> queryResult = connRAII.Query(query);
    if (queryResult.empty()) {
      LOG_DEBUG("%s登录失败",query.c_str());
      return -1;  // 登录失败
    }
    LOG_DEBUG("登录成功");
    return 0;  // 登录成功
}
