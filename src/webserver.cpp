#include "../include/webserver.h" 


using namespace std;

WebServer::WebServer() {
  ConfigParser* config = ConfigParser::Instance();
  
  _load_conf_file_ok =  config->Parse();

  _port = config->GetInt("server", "server_port", 8080);
  _open_linger = config->GetString("server", "open_linger", "off") == "on" ? true : false;
  std::string root_dir = config->GetString("server", "root_dir", "./");
  _src_root_dir = new char[root_dir.size() + 1]();
  strncpy(_src_root_dir, root_dir.c_str(), root_dir.size());
  _timeout_MS = config->GetInt("server", "timeout_Ms", 60000);
  _max_fd = config->GetInt("server", "_max_fd", 1024);

  int thread_pool_size = config->GetInt("pool", "thread_pool_size", std::thread::hardware_concurrency()); // 没有配置的话默认系统核心数
  
  _timer = new HeapTimer();
  _thread_pool = new ThreadPool(thread_pool_size);
  _epoller = new Epoller();

  int obj_pool_init_capacity = config->GetInt("pool", "_init_capacity", 1024);
  int obj_pool_increment = config->GetInt("pool", "_increment", 512); 
  bool obj_pool_is_lock = config->GetString("pool", "_is_lock", "off") == "on" ? true : false;
  _obj_pool = new ObjectPool<HttpConn>(obj_pool_init_capacity, obj_pool_increment, obj_pool_is_lock);

  HttpConn::_user_count = 0;
  HttpConn::_src_dir = _src_root_dir;

  int mysql_connection_pool_size = config->GetInt("pool", "mysql_connection_pool_size", 8);
  std::string mysql_host = config->GetString("mysql", "mysql_host", "127.0.0.1");
  int mysql_port = config->GetInt("mysql", "mysql_port", 3306);
  std::string mysql_user = config->GetString("mysql", "mysql_user", "root");
  std::string mysql_passwd = config->GetString("mysql", "mysql_passwd", "passwd");
  std::string mysql_databases = config->GetString("mysql", "mysql_databases", "databases");

  SqlConnPool::Instance()->Init(mysql_host.c_str(), mysql_port, mysql_user.c_str(), mysql_passwd.c_str(), mysql_databases.c_str(), mysql_connection_pool_size);
  
  int tirg_mode = config->GetInt("server", "trig_mode", 3);
  _is_close = false;
  InitEventMode(tirg_mode);  // 初始化事件模式
   
  if (!InitSocket()) { _is_close = true; }  // 初始化套接字失败则关闭服务器
  
  bool open_log = config->GetString("log", "log", "on") == "on" ? true : false;
  if (open_log) {
    std::string log_path = config->GetString("log", "log_path", "./log");
    std::string log_suffix = config->GetString("log", "log_suffix", ".log");
    int log_que_size = config->GetInt("log", "log_que_size", 1024);
    int log_level = config->GetInt("log", "log_level", 1);
    int log_max_lines = config->GetInt("log", "log_max_lines", 40278);
    Log::Instance()->init(log_level, log_path.c_str(), log_suffix.c_str(), log_que_size, log_max_lines);
    if (_is_close) {
      LOG_ERROR("========== 服务器初始化错误！==========");
    }
    else {
      LOG_INFO("========== 服务器初始化 ==========");
      if(!_load_conf_file_ok) LOG_WARN("配置文件加载失败!使用默认配置");
      LOG_INFO("端口：%d，开启Linger：%s", _port, _open_linger ? "true" : "false");
      
      LOG_INFO("监听模式：%s，连接模式：%s",
          (_listen_event & EPOLLET ? "ET" : "LT"),
          (_conn_event & EPOLLET ? "ET" : "LT"));
      LOG_INFO("日志等级：%d", log_level);
      LOG_INFO("资源路径：%s", HttpConn::_src_dir);
      LOG_INFO("Sql连接池数量：%d，线程池数量：%d", mysql_connection_pool_size, thread_pool_size);
      LOG_INFO("对象连接池初始数量：%d，起始扩容数量：%d，访问加锁：%s",  obj_pool_init_capacity,  obj_pool_increment,  obj_pool_is_lock ? "true" : "false");
    }
  }
}


WebServer::~WebServer() {
    close(_listen_fd);
    _is_close = true;
    delete _src_root_dir;
    delete _timer;
    delete _thread_pool;
    delete _epoller;
    SqlConnPool::Instance()->ClosePool();
}

// 初始化事件模式
void WebServer::InitEventMode(int trigMode) {
    _listen_event = EPOLLRDHUP;
    _conn_event = EPOLLONESHOT | EPOLLRDHUP;
    switch (trigMode) {
    case 0:
        break;
    case 1:
        _conn_event |= EPOLLET;
        break;
    case 2:
        _listen_event |= EPOLLET;
        break;
    case 3:
        _listen_event |= EPOLLET;
        _conn_event |= EPOLLET;
        break;
    default:
        _listen_event |= EPOLLET;
        _conn_event |= EPOLLET;
        break;
    }
    HttpConn::_is_ET = (_conn_event & EPOLLET);
}

// 启动服务器
void WebServer::Start() {
    int timeMS = -1;  // epoll等待超时时间，-1表示无事件将一直阻塞
    if (!_is_close) { LOG_INFO("========== 服务器启动 =========="); }
    while (!_is_close) {
        if (_timeout_MS > 0) {
            timeMS = _timer->GetNextTick();
        }

        int eventCnt = _epoller->Wait(timeMS);  // 等待事件发生
        for (int i = 0; i < eventCnt; i++) {
            int fd = _epoller->GetEventFd(i);  // 获取事件的文件描述符
            uint32_t events = _epoller->GetEvents(i);  // 获取事件类型
            if (fd == _listen_fd) {
                DealListen();  // 处理监听事件
            }
            else if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                // 处理套接字关闭、挂起或错误的情况
                assert(_users.count(fd) > 0);
                CloseConn(_users[fd]);  // 关闭连接
            }
            else if (events & EPOLLIN) {
                assert(_users.count(fd) > 0);
                DealRead(_users[fd]);  // 处理读事件
            }
            else if (events & EPOLLOUT) {
                assert(_users.count(fd) > 0);
                DealWrite(_users[fd]);  // 处理写事件
            }
            else {
                LOG_ERROR("未知的事件类型");
            }
        }
    }
}

// 发送错误信息给客户端
void WebServer::SendError(int fd, const char* info) {
    assert(fd > 0);
    int ret = send(fd, info, strlen(info), 0);
    if (ret < 0) {
        LOG_WARN("向客户端[%d]发送错误信息失败！", fd);
    }
    close(fd);
}

// 关闭连接
void WebServer::CloseConn(HttpConn* client) {
    assert(client);
    LOG_INFO("客户端[%d]断开连接！", client->GetFd());
    _epoller->DelFd(client->GetFd());  // 从epoll中移除
    client->Close();  // 关闭连接
    _obj_pool->Delete(client);
    auto it = _users.find(client->GetFd());

    //bug _users.erase会再次析构client,Delete已经回收过了
    //if(it != _users.end()) _users.erase(it);
    
    if(it != _users.end()) {
      _users[client->GetFd()] = nullptr;
      _users.erase(it);
    }
}

// 添加客户端连接
void WebServer::AddClient(int fd, sockaddr_in addr) {
    assert(fd > 0);
    _users[fd] = _obj_pool->New();
    _users[fd]->init(fd, addr);
    if (_timeout_MS > 0) {
        _timer->add(fd, _timeout_MS, std::bind(&WebServer::CloseConn, this, _users[fd]));
    }
    _epoller->AddFd(fd, EPOLLIN | _conn_event);  // 将客户端加入epoll监听
    SetFdNonblock(fd);  // 设置文件描述符为非阻塞模式
    LOG_INFO("客户端[%d]连接！", _users[fd]->GetFd());
}

// 处理监听事件
void WebServer::DealListen() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    do {
        int fd = accept(_listen_fd, (struct sockaddr*)&addr, &len);  // 接受连接请求
        if (fd <= 0) { return; }
        else if (HttpConn::_user_count >= _max_fd) {
            SendError(fd, "服务器繁忙！");
            LOG_WARN("客户端已满！");
            return;
        }
        AddClient(fd, addr);  // 添加新客户端连接
    } while (_listen_event & EPOLLET);  // 是否启用边缘触发模式
}

// 处理读事件
void WebServer::DealRead(HttpConn* client) {
    assert(client);
    ExtenTime(client);  // 更新连接的超时时间
    _thread_pool->AddTask(std::bind(&WebServer::OnRead, this, client));  // 将读事件添加到线程池
}

// 处理写事件
void WebServer::DealWrite(HttpConn* client) {
    assert(client);
    ExtenTime(client);  // 更新连接的超时时间
    _thread_pool->AddTask(std::bind(&WebServer::OnWrite, this, client));  // 将写事件添加到线程池
}

// 延长连接的超时时间
void WebServer::ExtenTime(HttpConn* client) {
    assert(client);
    if (_timeout_MS > 0) { _timer->adjust(client->GetFd(), _timeout_MS); }
}

// 处理读事件
void WebServer::OnRead(HttpConn* client) {
    assert(client);
    int ret = -1;
    int readErrno = 0;
    ret = client->read(&readErrno);  // 读取数据
    if (ret <= 0 && readErrno != EAGAIN) {
        CloseConn(client);  // 关闭连接
        return;
    }
    OnProcess(client);  // 处理请求
}

// 处理请求
void WebServer::OnProcess(HttpConn* client) {
    if (client->process()) {
        _epoller->ModFd(client->GetFd(), _conn_event | EPOLLOUT);  // 修改事件类型为写事件
    }
    else {
        _epoller->ModFd(client->GetFd(), _conn_event | EPOLLIN);  // 修改事件类型为读事件
    }
}

// 处理写事件
void WebServer::OnWrite(HttpConn* client) {
    assert(client);
    int ret = -1;
    int writeErrno = 0;
    ret = client->write(&writeErrno);  // 写数据
    if (client->ToWriteBytes() == 0) {
        // 数据传输完成
        if (client->IsKeepAlive()) {
            OnProcess(client);  // 继续处理请求
            return;
        }
    }
    else if (ret < 0) {
        if (writeErrno == EAGAIN) {
            // 继续传输
            _epoller->ModFd(client->GetFd(), _conn_event | EPOLLOUT);  // 修改事件类型为写事件
            return;
        }
    }
    CloseConn(client);  // 关闭连接
}

// 初始化套接字
bool WebServer::InitSocket() {
    int ret;
    struct sockaddr_in addr;
    if (_port > 65535 || _port < 1024) {
        LOG_ERROR("端口：%d 错误！", _port);
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(_port);
    struct linger optLinger = { 0 };
    if (_open_linger) {
        // 优雅关闭: 等待剩余数据发送完毕或超时
        optLinger.l_onoff = 1;      // 非零表示启用 linger 选项
        optLinger.l_linger = 1;     // 延迟关闭的时间，以秒为单位
    }

    _listen_fd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字
    if (_listen_fd < 0) {
        LOG_ERROR("创建套接字失败！", _port);
        return false;
    }

    ret = setsockopt(_listen_fd, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));  // 设置SO_LINGER选项
    if (ret < 0) {
        close(_listen_fd);
        LOG_ERROR("初始化Linger失败！", _port);
        return false;
    }

    int optval = 1;
    ret = setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));  // 设置SO_REUSEADDR选项
    if (ret == -1) {
        LOG_ERROR("设置套接字选项SO_REUSEADDR失败！");
        close(_listen_fd);
        return false;
    }

    ret = bind(_listen_fd, (struct sockaddr*)&addr, sizeof(addr));  // 绑定地址
    if (ret < 0) {
        LOG_ERROR("绑定端口：%d 失败！", _port);
        close(_listen_fd);
        return false;
    }

    ret = listen(_listen_fd, 6);  // 开始监听，最大排队数为6
    if (ret < 0) {
        LOG_ERROR("监听端口：%d 失败！", _port);
        close(_listen_fd);
        return false;
    }
    ret = _epoller->AddFd(_listen_fd, _listen_event | EPOLLIN);  // 将监听套接字加入epoll监听
    if (ret == 0) {
        LOG_ERROR("添加监听失败！");
        close(_listen_fd);
        return false;
    }
    SetFdNonblock(_listen_fd);  // 设置文件描述符为非阻塞模式
    LOG_INFO("服务器端口：%d", _port);
    return true;
}

// 设置文件描述符为非阻塞模式
int WebServer::SetFdNonblock(int fd) {
    assert(fd > 0);
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

