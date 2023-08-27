#include "../include/httpconnection.h"
using namespace std;

const char* HttpConn::_src_dir;
std::atomic<int> HttpConn::_user_count;
bool HttpConn::_is_ET;

HttpConn::HttpConn() { 
    _fd = -1;
    _addr = { 0 };
    _is_close = true;
};

HttpConn::~HttpConn() { 
    Close(); 
};

void HttpConn::init(int fd, const sockaddr_in& addr) {
    assert(fd > 0);
    _user_count++;
    _addr = addr;
    _fd = fd;
    write_buff.RetrieveAll();
    read_buff.RetrieveAll();
    _is_close = false;
    LOG_INFO("Client[%d](%s:%d) in, _user_count:%d", _fd, GetIP(), GetPort(), (int)_user_count);
}

void HttpConn::Close() {
    _response.UnmapFile();
    if(_is_close == false){
        _is_close = true; 
        _user_count--;
        close(_fd);
        LOG_INFO("Client[%d](%s:%d) quit, UserCount:%d", _fd, GetIP(), GetPort(), (int)_user_count);
    }
}

int HttpConn::GetFd() const {
    return _fd;
};

struct sockaddr_in HttpConn::GetAddr() const {
    return _addr;
}

const char* HttpConn::GetIP() const {
    return inet_ntoa(_addr.sin_addr);
}

int HttpConn::GetPort() const {
    return _addr.sin_port;
}

ssize_t HttpConn::read(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = read_buff.ReadFd(_fd, saveErrno);
        if (len <= 0) {
            break;
        }
    } while (_is_ET);
    return len;
}

ssize_t HttpConn::write(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = writev(_fd, _iov, _iov_cnt);
        // 在每次迭代中打印 _iov 中的数据内容
        //printf("Sent data: %.*s\n", static_cast<int>(len), static_cast<char*>(_iov[0].iov_base));
        if(len <= 0) {
            *saveErrno = errno;
            break;
        }
        if(_iov[0].iov_len + _iov[1].iov_len  == 0) { break; } // 传输结束
        else if(static_cast<size_t>(len) > _iov[0].iov_len) {
            _iov[1].iov_base = (uint8_t*) _iov[1].iov_base + (len - _iov[0].iov_len);
            _iov[1].iov_len -= (len - _iov[0].iov_len);
            if(_iov[0].iov_len) {
                write_buff.RetrieveAll();
                _iov[0].iov_len = 0;
            }
        }
        else {
            _iov[0].iov_base = (uint8_t*)_iov[0].iov_base + len; 
            _iov[0].iov_len -= len; 
            write_buff.Retrieve(len);
        }
    } while(_is_ET || ToWriteBytes() > 10240);
    return len;
}

bool HttpConn::process() {
    _request.Init();
    if(read_buff.ReadableBytes() <= 0) {
        return false;
    }
    // 解析http请求
    else if(_request.parse(read_buff)) {
        LOG_DEBUG("%s", _request.path().c_str());
        auto _post = _request.GetPost();
        _response.Init(_src_dir, _request.path(), _request.IsKeepAlive(), 200, _post);

    } else {
        auto _post = _request.GetPost();
        _response.Init(_src_dir, _request.path(), false, 400, _post);
        return false;
    }
    
    // 制作响应
    _response.MakeResponse(write_buff);
    // 响应头
    _iov[0].iov_base = const_cast<char*>(write_buff.Peek());
    _iov[0].iov_len = write_buff.ReadableBytes();
    _iov_cnt = 1;
    
    //std::string res(write_buff.Peek(), write_buff.BeginWriteConst());
    //LOG_DEBUG("响应: %s", res.c_str());
    
    
    // 文件
    if(_response.FileLen() > 0  && _response.File()) {
        _iov[1].iov_base = _response.File();
        _iov[1].iov_len = _response.FileLen();
        _iov_cnt = 2;
    }
    
    LOG_DEBUG("filesize:%d, %d  to %d", _response.FileLen() , _iov_cnt, ToWriteBytes());
    return true;
  }
