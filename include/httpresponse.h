
#ifndef HTTP__RESPONSE_H
#define HTTP__RESPONSE_H

#include <unordered_map>
#include <fcntl.h>       
#include <unistd.h>      
#include <sys/stat.h>    
#include <sys/mman.h>    

#include "buff.h"
#include "log.h"

 // HTTP响应类，用于生成HTTP响应
class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    // 初始化响应对象
    void Init(const std::string& _src_dir, std::string& path, bool isKeepAlive = false, int code = -1);

    // 构建HTTP响应报文
    void MakeResponse(Buffer& buff);

    // 解除内存映射
    void UnmapFile();

    // 获取文件指针
    char* File();

    // 获取文件长度
    size_t FileLen() const;

    // 构建错误响应内容
    void ErrorContent(Buffer& buff, std::string message);

    // 获取HTTP响应状态码
    int Code() const { return _code; }

private:
    // 添加状态行
    void AddStateLine(Buffer& buff);

    // 添加响应头部
    void AddHeader(Buffer& buff);

    // 添加响应内容
    void AddContent(Buffer& buff);

    // 构建错误的HTML响应内容
    void ErrorHtml();

    // 获取文件类型
    std::string GetFileType_();

    int _code;              // 响应状态码
    bool _isKeepAlive;      // 是否保持连接

    std::string _path;      // 请求路径
    std::string _src_root_dir;    // 资源根目录目录路径

    char* _mm_file;           // 内存映射文件指针
    struct stat _mm_file_stat; // 文件状态信息


    // 文件后缀与类型映射
    static const std::unordered_map<std::string, std::string> _suffix_type;

    // HTTP状态码与描述映射
    static const std::unordered_map<int, std::string> _code_status;

    // HTTP状态码与文件路径映射
    static const std::unordered_map<int, std::string> _codePATH;
};

#endif

