//#ifndef HTTP_REQUEST_H
//#define HTTP_REQUEST_H



#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>     
#include <mysql/mysql.h>

#include "buff.h"
#include "log.h"
#include "sqlconnectionpool.h"
#include "sqlconnectionRAII.h"

 // HTTP请求类，用于解析和处理HTTP请求
class HttpRequest {
public:
    enum PARSE_STATE {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };

    enum HTTP_CODE {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };

    HttpRequest() { Init(); }
    ~HttpRequest() = default;

    // 初始化请求对象
    void Init();

    // 解析HTTP请求
    bool parse(Buffer& buff);

    // 获取请求路径
    std::string path() const;
    std::string& path();

    // 获取HTTP方法
    std::string method() const;

    // 获取HTTP版本
    std::string version() const;

    // 获取POST请求参数
    std::string GetPost(const std::string& key) const;
    std::string GetPost(const char* key) const;

    // 检查是否保持连接
    bool IsKeepAlive() const;

    // TODO: 添加其他解析方法，如FormData和Json
    // void HttpConn::ParseFormData() {}
    // void HttpConn::ParseJson() {}

private:
    // 解析请求行
    bool ParseRequestLine(const std::string& line);

    // 解析请求头部
    void ParseHeader(const std::string& line);

    // 解析请求体
    void ParseBody(const std::string& line);

    // 解析路径
    void ParsePath();

    // 解析POST请求参数
    void ParsePost();

    // 解析Url编码的POST参数
    void ParseFromUrlencoded();

    // 用户验证
    static bool UserVerify(const std::string& name, const std::string& pwd, bool isLogin);

    PARSE_STATE _state;                                     // 解析状态
    std::string _method, _path, _version, _body;            // 请求方法、路径、版本和请求体
    std::unordered_map<std::string, std::string> _header;   // 请求头部
    std::unordered_map<std::string, std::string> _post;     // POST请求参数

    // 默认的HTML资源路径集合和标签映射
    static const std::unordered_set<std::string> DEFAULT_HTML;
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG;

    //十六进制字符转换为对应的整数值
    static int ConverHex(char ch);
};

//#endif //HTTP__requestH

