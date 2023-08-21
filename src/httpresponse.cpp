#include "../include/httpresponse.h"

using namespace std;

const unordered_map<string, string> HttpResponse::_suffix_type = {
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".word",  "application/nsword" },
    { ".png",   "image/png" },
    { ".gif",   "image/gif" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".au",    "audio/basic" },
    { ".mpeg",  "video/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".avi",   "video/x-msvideo" },
    { ".gz",    "application/x-gzip" },
    { ".tar",   "application/x-tar" },
    { ".css",   "text/css "},
    { ".js",    "text/javascript "},
};

const unordered_map<int, string> HttpResponse::_code_status = {
    { 200, "OK" },
    { 400, "Bad Request" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
};

const unordered_map<int, string> HttpResponse::_codePATH = {
    { 400, "/400.html" },
    { 403, "/403.html" },
    { 404, "/404.html" },
};

HttpResponse::HttpResponse() {
    _code = -1;
    _path = _src_root_dir = "";
    _isKeepAlive = false;
    _mm_file = nullptr; 
    _mm_file_stat = { 0 };
};

HttpResponse::~HttpResponse() {
    UnmapFile();
}

void HttpResponse::Init(const string& _src_dir, string& path, bool isKeepAlive, int code){
    assert(_src_dir != "");
    if(_mm_file) { UnmapFile(); }
    _code = code;
    _isKeepAlive = isKeepAlive;
    _path = path;
    _src_root_dir = _src_dir;
    _mm_file = nullptr; 
    _mm_file_stat = { 0 };
}

void HttpResponse::MakeResponse(Buffer& buff) {
    // 判断请求的资源文件是否存在或者是目录
    if (stat((_src_root_dir + _path).data(), &_mm_file_stat) < 0 || S_ISDIR(_mm_file_stat.st_mode)) {
        _code = 404;  // 请求的资源不存在，设置状态码为 404
    }
    else if (!(_mm_file_stat.st_mode & S_IROTH)) {
        _code = 403;  // 请求的资源没有读权限，设置状态码为 403
    }
    else if (_code == -1) {
        _code = 200;  // 如果状态码没有被设置过，默认设置为 200（成功）
    }

    ErrorHtml();   // 生成错误页面内容（如果状态码不是 200）

    AddStateLine(buff);  // 添加响应状态行，包括协议版本和状态码
    AddHeader(buff);      // 添加响应头部信息
    AddContent(buff);    // 添加响应内容，可能是文件内容或错误页面内容
}


char* HttpResponse::File() {
    return _mm_file;
}

size_t HttpResponse::FileLen() const {
    return _mm_file_stat.st_size;
}

void HttpResponse::ErrorHtml() {
    if(_codePATH.count(_code) == 1) {
        _path = _codePATH.find(_code)->second;
        stat((_src_root_dir + _path).data(), &_mm_file_stat);
    }
}

void HttpResponse::AddStateLine(Buffer& buff) {
    string status;
    if(_code_status.count(_code) == 1) {
        status = _code_status.find(_code)->second;
    }
    else {
        _code = 400;
        status = _code_status.find(400)->second;
    }
    buff.Append("HTTP/1.1 " + to_string(_code) + " " + status + "\r\n");
}

void HttpResponse::AddHeader(Buffer& buff) {
    buff.Append("Connection: ");
    if(_isKeepAlive) {
        buff.Append("keep-alive\r\n");
        buff.Append("keep-alive: max=6, timeout=120\r\n");
    } else{
        buff.Append("close\r\n");
    }
    buff.Append("Content-type: " + GetFileType_() + "\r\n");
}

void HttpResponse::AddContent(Buffer& buff) {
    int srcFd = open((_src_root_dir + _path).data(), O_RDONLY);
    if(srcFd < 0) { 
        ErrorContent(buff, "File NotFound!");
        return; 
    }

    //将文件映射到内存提高文件的访问速度 MAP_PRIVATE 建立一个写入时拷贝的私有映射
    LOG_DEBUG("file path %s", (_src_root_dir + _path).data());
    int* mmRet = (int*)mmap(0, _mm_file_stat.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
    if(*mmRet == -1) {
        ErrorContent(buff, "File NotFound!");
        return; 
    }
    _mm_file = (char*)mmRet;
    close(srcFd);
    buff.Append("Content-length: " + to_string(_mm_file_stat.st_size) + "\r\n\r\n");
}

void HttpResponse::UnmapFile() {
    if(_mm_file) {
        munmap(_mm_file, _mm_file_stat.st_size);
        _mm_file = nullptr;
    }
}

string HttpResponse::GetFileType_() {
    /* 判断文件类型 */
    string::size_type idx = _path.find_last_of('.');
    if(idx == string::npos) {
        return "text/plain";
    }
    string suffix = _path.substr(idx);
    if(_suffix_type.count(suffix) == 1) {
        return _suffix_type.find(suffix)->second;
    }
    return "text/plain";
}

void HttpResponse::ErrorContent(Buffer& buff, string message) 
{
    string body;
    string status;
    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";
    if(_code_status.count(_code) == 1) {
        status = _code_status.find(_code)->second;
    } else {
        status = "Bad Request";
    }
    body += to_string(_code) + " : " + status  + "\n";
    body += "<p>" + message + "</p>";
    body += "<hr><em>TinyWebServer</em></body></html>";

    buff.Append("Content-length: " + to_string(body.size()) + "\r\n\r\n");
    buff.Append(body);
}

