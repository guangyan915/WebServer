//#ifndef HTTP_REQUEST_H
//#define HTTP_REQUEST_H
#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <regex>
#include <errno.h>     
#include <mysql/mysql.h>
#include <sys/stat.h>
#include <unistd.h>


#include <json/json.h>


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

    // 获取请求报头
    const std::unordered_map<std::string, std::string>& Handler();
    size_t ContentLenth();
    size_t BodyLenth();
    
    void ParseBody();
    
    // 获取POST请求参数
    std::string GetPost(const std::string& key) const;
    std::string GetPost(const char* key) const;
    const std::unordered_map<std::string, std::string>& GetPost();
    

    // 检查是否保持连接
    bool IsKeepAlive() const;

    void ParseJson();

    void ParseFromData();
    

    // 解析请求体
    //void ParseBody(Buffer& buff);

private:
    // 解析请求行
    bool ParseRequestLine(const std::string& line);

    // 解析请求头部
    void ParseHeader(const std::string& line);

    // 解析路径
    void ParsePath();
    
    void ParseBody(const std::string& line);
    
    // 解析POST请求参数
    void ParsePost();

    // 解析Url编码的POST参数
    void ParseFromUrlencoded();

    PARSE_STATE _state;                                     // 解析状态
    std::string _method, _path, _version, _body;            // 请求方法、路径、版本和请求体
    
    int _handle_body_state;                                 // 处理_body的结果
    

    std::unordered_map<std::string, std::string> _header;   // 请求头部
    std::unordered_map<std::string, std::string> _post;     // POST请求参数

    // 默认的标签映射
    static const std::unordered_set<std::string> _default_html;
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG;

    //十六进制字符转换为对应的整数值
    static int ConverHex(char ch);
};

//#endif //HTTP__requestH

