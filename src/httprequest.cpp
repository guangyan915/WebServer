#include "../include/httprequest.h"
using namespace std;

const unordered_set<string> HttpRequest::DEFAULT_HTML{
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
    return true;  // 解析成功，返回 true
}


void HttpRequest::ParsePath() {
    // 如果请求路径为根路径，则将路径设置为默认的首页路径（index.html）
    if (_path == "/") {
        _path = "/index.html";
    }
    else {
        // 遍历默认的静态资源路径数组
        for (auto& item : DEFAULT_HTML) {
            // 如果请求路径在默认静态资源路径中找到匹配项，则将路径末尾添加 .html 扩展名
            if (item == _path) {
                _path += ".html";
                break;
            }
        }
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

    // 使用正则表达式匹配当前行是否符合 HTTP 请求头部行的格式
    if (regex_match(line, subMatch, patten)) {
        // 如果匹配成功，将匹配到的内容添加到请求头部的映射中
        // 匹配到的 subMatch[1] 是请求头部字段，subMatch[2] 是字段值
        _header[subMatch[1]] = subMatch[2];
    }
    else {
        // 如果当前行不符合请求头部行格式，说明已经解析完请求头部，进入请求体解析状态
        _state = BODY;
    }
}


void HttpRequest::ParseBody(const string& line) {
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
    // 检查请求方法是否为 POST，并且 Content-Type 是否为 application/x-www-form-urlencoded
    if (_method == "POST" && _header["Content-Type"] == "application/x-www-form-urlencoded") {
        // 解析 POST 请求体中的表单数据
        ParseFromUrlencoded();

        // 检查是否有与路径对应的默认 HTML 页面标签
        if (DEFAULT_HTML_TAG.count(_path)) {
            // 获取路径对应的默认 HTML 页面标签
            int tag = DEFAULT_HTML_TAG.find(_path)->second;

            LOG_DEBUG("Tag:%d", tag);

            if (tag == 0 || tag == 1) {
                // 否是登录页面
                bool isLogin = (tag == 1);

                // 验证用户身份，根据验证结果决定跳转页面
                if (UserVerify(_post["username"], _post["password"], isLogin)) {
                    _path = "/welcome.html"; // 跳转到欢迎页面
                }
                else {
                    _path = "/error.html"; // 跳转到错误页面
                }
            }
        }
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


bool HttpRequest::UserVerify(const string& name, const string& pwd, bool isLogin) {
    if (name == "" || pwd == "") {
        return false;
    }
    LOG_INFO("Verify name:%s pwd:%s", name.c_str(), pwd.c_str());

    // 获取一个数据库连接
    MYSQL* sql;
    SqlConnRAII(&sql, SqlConnPool::Instance());
    assert(sql);

    bool flag = false;
    unsigned int j = 0;
    char order[256] = { 0 };
    MYSQL_FIELD* fields = nullptr;      // 查询结果集中的字段信息
    MYSQL_RES* res = nullptr;           // 查询返回的行数据以及与结果集相关的元数据信息

    // 若为注册行为，置标志位为 true，否则为 false
    if (!isLogin) {
        flag = true;
    }

    ///查询用户及密码 
    snprintf(order, 256, "SELECT username, password FROM user WHERE username='%s' LIMIT 1", name.c_str());MYSQL_FIELD
    LOG_DEBUG("%s", order);

    if (mysql_query(sql, order)) {
        mysql_free_result(res);
        return false;
    }
    res = mysql_store_result(sql);
    j = mysql_num_fields(res);
    fields = mysql_fetch_fields(res);

    while (MYSQL_ROW row = mysql_fetch_row(res)) {
        LOG_DEBUG("MYSQL ROW: %s %s", row[0], row[1]);
        string password(row[1]);
        /* 若为登录行为且密码匹配 */
        if (isLogin) {
            if (pwd == password) {
                flag = true;
            }
            else {
                flag = false;
                LOG_DEBUG("pwd error!");
            }
        }
        /* 若为注册行为但用户名已被使用 */
        else {
            flag = false;
            LOG_DEBUG("user used!");
        }
    }
    mysql_free_result(res);

    // 若为注册行为且用户名未被使用 
    if (!isLogin && flag == true) {
        LOG_DEBUG("register!");
        bzero(order, 256);
        snprintf(order, 256, "INSERT INTO user(username, password) VALUES('%s','%s')", name.c_str(), pwd.c_str());
        LOG_DEBUG("%s", order);
        if (mysql_query(sql, order)) {
            LOG_DEBUG("Insert error!");
            flag = false;
        }
        flag = true;
    }

    // 释放数据库连接
    SqlConnPool::Instance()->FreeConn(sql);
    LOG_DEBUG("UserVerify success!!");
    return flag;
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
