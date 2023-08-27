#include "../include/httpresponse.h"

using namespace std;


const std::unordered_set<string> HttpResponse::_route{"/register", "/login"}; 

const std::unordered_map<std::string, std::string> HttpResponse::_suffix_type = {
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".doc",   "application/msword" },
    { ".docx",  "application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
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
    { ".css",   "text/css" },
    { ".js",    "application/javascript" },
    { ".json",  "application/json" },
    { ".woff2", "application/font-woff2" },
    { ".svg",   "image/svg+xml" },  
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

void HttpResponse::Init(const string& _src_dir, string& path, bool isKeepAlive, int code, const std::unordered_map<std::string, std::string>& post){
    assert(_src_dir != "");
    if(_mm_file) { UnmapFile(); }
    _code = code;
    _isKeepAlive = isKeepAlive;
    _path = path;
    _src_root_dir = _src_dir;
    _mm_file = nullptr; 
    _mm_file_stat = { 0 };
    _post = post;
}

void HttpResponse::MakeResponse(Buffer& buff) {
    // 找到路由
    if(_route.find(_path) != _route.end()) {
      if(_path == "/register") {
        // 注册请求
        HandlerRegister();
      }
      else if(_path == "/login") {
        // 登录请求
        HandlerLogin();
      }
      else {
        LOG_ERROR("路由:%s处理失败", _path.c_str());
        _code = 404;
      }
    }
    else if (stat((_src_root_dir + _path).data(), &_mm_file_stat) < 0) {
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
    if(_route.find(_path) != _route.end()) return;
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
    if(_route.find(_path) != _route.end()) buff.Append("HTTP/1.1 " + to_string(_code) + " " + _status + "\r\n");
    else buff.Append("HTTP/1.1 " + to_string(_code) + " " + _status + "\r\n");
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
    if(_path == "/register") {
      LOG_DEBUG("注册事件处理");
      Json::Value response;
      std::string jsonResponse;
      if(_code == 200) {
        response["success"] = true;
        response["message"] = "Register OK!";
        jsonResponse = response.toStyledString();
      }
      else {
        response["success"] = false;
        response["message"] = "Register Error";
        jsonResponse = response.toStyledString();
      }
      LOG_DEBUG("响应JSON:%s",jsonResponse.c_str());
      buff.Append("Content-Lenth: " + to_string(jsonResponse.size()) + "\r\n\r\n");
      buff.Append(jsonResponse); jsonResponse = response.toStyledString();
      return;
    }
    else if(_path == "/login") {
      std::string jsonResponse;
      LOG_DEBUG("登录事件处理");
      Json::Value response;
      if(_code == 200) {
        response["success"] = true;
        response["message"] = "Login OK";
        jsonResponse = response.toStyledString();
      }
      else {
        response["success"] = false;
        response["message"] = "Login Error";
        jsonResponse = response.toStyledString();
      } 
      LOG_DEBUG("响应JSON:%s",jsonResponse.c_str());
      buff.Append("Content-Lenth: " + to_string(jsonResponse.size()) + "\r\n\r\n");
      buff.Append(jsonResponse);
      return;
    }
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
    
    if(_route.find(_path) != _route.end()) {
      if((_path == "/register" || _path == "/login") && _suffix_type.count(".json") == 1) return _suffix_type.find(".json")->second;
      return "text/plain";
    }
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

void HttpResponse::HandlerRegister()
{
    if (_post.empty()) {
        _code = 400;
        _status = "Bad Request";
        LOG_DEBUG("处理登录请求，body为空");
        return;
    }

    // 检查是否存在所需字段
    if (_post.find("name") == _post.end() || _post.find("password") == _post.end() || _post.find("phone") == _post.end()) {
        _code = 400;
        _status = "Bad Request";
        LOG_DEBUG("处理登录请求，缺少所需字段");
        return;
    }

    const std::string name = _post.find("name")->second;
    const std::string phone = _post.find("phone")->second;
    const std::string password = _post.find("password")->second;

    int res = MysqlOpt::Register(name, phone, password);
    if (res == 0) {
        _code = 200; // 已创建
        _status = "registe OK";
        LOG_DEBUG("注册成功");
    } else if (res == -2) {
        _code = 409;
        _status = "phone_number exist";
        LOG_DEBUG("手机号已存在");
    } else if (res == -2) {

    } else {
        _code = 500; // 内部服务器错误
        _status = "server error";
        LOG_DEBUG("注册插入数据失败");
    }
}

void HttpResponse::HandlerLogin()
{
    if (_post.empty()) {
        _code = 400;
        _status = "Bad Request";
        LOG_DEBUG("body为空");
        return;
    }

    // 检查是否存在所需字段
    if (_post.find("name") == _post.end() || _post.find("password") == _post.end()) {
        _code = 400;
        _status = "Bad Request";
        LOG_DEBUG("缺少所需字段");
        return;
    }
    const std::string name = _post.find("name")->second;
    const std::string password = _post.find("password")->second;

    int res = MysqlOpt::Login(name, password);
    if (res == 0) {
        _code = 200; // 成功
        _status = "login OK";
        LOG_DEBUG("登录成功");
    } else {
        _code = 401; // 未授权
        _status = "login error";
        LOG_DEBUG("登录失败");
    }
}

