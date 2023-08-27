#include "../include/httprequest.h"
using namespace std;


const unordered_set<string> HttpRequest::_default_html{
            "/index", "/register", "/login",
             "/welcome", "/video", "/picture", };

const unordered_map<string, int> HttpRequest::DEFAULT_HTML_TAG {
            {"/register.html", 0}, {"/login.html", 1},  };

void HttpRequest::Init() {
    _method = _path = _version = _body = "";
    _state = REQUEST_LINE;
    _header.clear();
    _post.clear();
}

bool HttpRequest::IsKeepAlive() const {
    if(_header.count("Connection") == 1) {
        return _header.find("Connection")->second == "keep-alive" && _version == "1.1";
    }
    return false;
}

bool HttpRequest::parse(Buffer& buff) {
    const char CRLF[] = "\r\n";
    if (buff.ReadableBytes() <= 0) {
        return false;  // 缓冲区为空，无法解析
    }
    while (buff.ReadableBytes() && _state != FINISH) {
        // 在缓冲区中查找换行符（CRLF，即"\r\n"）的位置，从而确定一行内容的结束位置
        const char* lineEnd = search(buff.Peek(), buff.BeginWriteConst(), CRLF, CRLF + 2);
        std::string line(buff.Peek(), lineEnd);  // 从缓冲区中读取一行内容
        LOG_DEBUG("解析: %s", line.c_str());
        switch (_state) {
        case REQUEST_LINE:
            if (!ParseRequestLine(line)) {
                return false;   // 解析请求行失败，返回 false
            }
            ParsePath();        // 解析请求路径
            break;
        case HEADERS:
            ParseHeader(line);  // 解析请求头部
            if (buff.ReadableBytes() <= 2) {
                _state = FINISH;  // 头部解析完毕，进入 FINISH 状态
            }
            break;
        case BODY:
            ParseBody(line);  // 解析请求主体
            break;
        default:
            break;
        }
        if (lineEnd == buff.BeginWrite()) {
            break;  // 已处理完缓冲区中的所有内容，退出循环
        }
        buff.RetrieveUntil(lineEnd + 2);  // 从缓冲区中移除已解析的内容
    }
    // 在调试日志中打印解析得到的请求方法、路径和版本
    LOG_DEBUG("[%s], [%s], [%s]", _method.c_str(), _path.c_str(), _version.c_str());
    return true;
}

void HttpRequest::ParsePath() {
    // 如果请求路径为根路径，则将路径设置为默认的首页路径
    if (_path == "/") {
        _path = "/index.html";
    }
    else {
        // 遍历默认的静态资源路径数组
      /*  
      for (auto& item : _default_html) {
            // 如果请求路径在默认静态资源路径中找到匹配项，则将路径末尾添加 .html 扩展名
            if (item == _path) {
                _path += ".html";
                break;
            }
        }
        */
    }
}

bool HttpRequest::ParseRequestLine(const string& line) {
    // 定义正则表达式模式，匹配请求行的格式
    regex pattern("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

    // 存储匹配结果的对象
    smatch subMatch;
    // 使用正则表达式匹配请求行内容
    if (regex_match(line, subMatch, pattern)) {
        // 获取匹配到的子串，分别表示请求方法、请求路径、HTTP版本
        _method = subMatch[1];
        _path = subMatch[2];
        _version = subMatch[3];

        // 将状态设置为解析头部信息
        _state = HEADERS;

        // 解析成功，返回 true
        return true;
    }

    // 若匹配失败，输出错误日志并返回 false
    LOG_ERROR("RequestLine Error");
    return false;
}


void HttpRequest::ParseHeader(const string& line) {
    // 定义正则表达式模式，用于匹配 HTTP 请求头部行的格式
    regex patten("^([^:]*): ?(.*)$");
    smatch subMatch;

    if (regex_match(line, subMatch, patten)) {
        // 如果匹配成功，将匹配到的内容添加到请求头部的映射中
        // 匹配到的 subMatch[1] 是请求头部字段，subMatch[2] 是字段值
        _header[subMatch[1]] = subMatch[2];
    }
    else {
        // 如果当前行不符合请求头部行格式，说明已经解析完请求头部，进入请求体解析状态, 即匹配到空行了
        _state = BODY;
    }
}



void HttpRequest::ParseBody(const string& line) {
    if(_method == "GET") return;
    _body = line;                                               // 将请求体内容存储在 _body 变量中
    ParsePost();                                                // 解析请求体中的表单数据
    _state = FINISH;                                            // 将请求状态设置为 FINISH，表示解析完成
    LOG_DEBUG("Body:%s, len:%d", line.c_str(), line.size());    // 输出日志，记录解析后的请求体内容和长度
}


int HttpRequest::ConverHex(char ch) {
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return ch;
}

void HttpRequest::ParsePost() {
  if(_method == "POST") {
    if (_header["Content-Type"] == "application/json") {
      ParseJson();
    }
    else if(_header["Content-Type"] == "application/x-www-form-urlencoded") {
      ParseFromUrlencoded();
      cout << "ParseFromUrlencoded\n";
    }
    else if(_header["Content-Type"] == "multipart/form-data") {
      ParseFromData();
      cout << "ParseFromData\n";
    }
    else {
      LOG_ERROR("Content-Type: %s;暂不支持处理", _header["Content-Type"].c_str());
    }
  }
}

void HttpRequest::ParseJson() {
  if(_body.size() == 0) return;
  // 解析 JSON 请求体数据
  Json::CharReaderBuilder readerBuilder;
  Json::Value jsonData;
  std::istringstream iss(_body);
  std::string errs;
  if (Json::parseFromStream(readerBuilder, iss, &jsonData, &errs)) {
    for (const auto& key : jsonData.getMemberNames()) {
      if(jsonData[key].isString()) {
        _post[key] = jsonData[key].asString();
        LOG_DEBUG("JSON解析:%s : %s", key.c_str(), _post[key].c_str());
      }
    }
  } else {
    // 解析失败
    LOG_ERROR("JSON 解析错误: %s", errs.c_str());
  }
}

void HttpRequest::ParseFromUrlencoded() {
    // 如果请求体为空，直接返回
    if (_body.size() == 0) {
        return;
    }

    string key, value;
    int num = 0;
    int n = _body.size();
    int i = 0, j = 0;

    for (; i < n; i++) {
        char ch = _body[i];
        switch (ch) {
        case '=':
            // 解析表单数据的键
            key = _body.substr(j, i - j);
            j = i + 1;
            break;
        case '+':
            _body[i] = ' '; // 将 '+' 替换为空格
            break;
        case '%':
            // 解析 URL 编码的字符
            num = ConverHex(_body[i + 1]) * 16 + ConverHex(_body[i + 2]);
            _body[i + 2] = num % 10 + '0';
            _body[i + 1] = num / 10 + '0';
            i += 2;
            break;
        case '&':
            // 解析表单数据的值
            value = _body.substr(j, i - j);
            j = i + 1;
            _post[key] = value; // 将键值对添加到 _post 中
            LOG_DEBUG("%s = %s", key.c_str(), value.c_str()); // 输出日志
            break;
        default:
            break;
        }
    }

    assert(j <= i);
    // 检查是否有未处理的键值对
    if (_post.count(key) == 0 && j < i) {
        value = _body.substr(j, i - j);
        _post[key] = value;
    }
}

void HttpRequest::ParseFromData() {

}

std::string HttpRequest::path() const{
    return _path;
}

std::string& HttpRequest::path(){
    return _path;
}
std::string HttpRequest::method() const {
    return _method;
}

std::string HttpRequest::version() const {
    return _version;
}

const std::unordered_map<std::string, std::string>& HttpRequest::Handler() {
  return _header;
}

size_t HttpRequest::ContentLenth() {
  if(_method == "POST") return stoi(_header["Content-Length"]);
  return 0;
}

size_t HttpRequest::BodyLenth() {
  return _body.size();
}

std::string HttpRequest::GetPost(const std::string& key) const {
    assert(key != "");
    if(_post.count(key) == 1) {
        return _post.find(key)->second;
    }
    return "";
}

std::string HttpRequest::GetPost(const char* key) const {
    assert(key != nullptr);
    if(_post.count(key) == 1) {
        return _post.find(key)->second;
    }
    return "";
}

const std::unordered_map<std::string, std::string>& HttpRequest::GetPost()
{
    return _post;
}
