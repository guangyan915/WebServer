#include "../include/buff.h"

Buffer::Buffer(int initBuffSize) : _buffer(initBuffSize), _read_pos(0), _write_pos(0) {}

size_t Buffer::ReadableBytes() const {
    return _write_pos - _read_pos;
}

size_t Buffer::WritableBytes() const {
    return _buffer.size() - _write_pos;
}

size_t Buffer::PrependableBytes() const {
    return _read_pos;
}

const char* Buffer::Peek() const {
    return BeginPtr_() + _read_pos;
}

void Buffer::Retrieve(size_t len) {
    assert(len <= ReadableBytes());
    _read_pos += len;
}

void Buffer::RetrieveUntil(const char* end) {
    assert(Peek() <= end);
    Retrieve(end - Peek());
}

void Buffer::RetrieveAll() {
    bzero(&_buffer[0], _buffer.size());
    _read_pos = 0;
    _write_pos = 0;
}

std::string Buffer::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char* Buffer::BeginWriteConst() const {
    return BeginPtr_() + _write_pos;
}

char* Buffer::BeginWrite() {
    return BeginPtr_() + _write_pos;
}

void Buffer::HasWritten(size_t len) {
    _write_pos += len;
}

void Buffer::Append(const std::string& str) {
    Append(str.data(), str.length());
}

void Buffer::Append(const void* data, size_t len) {
    assert(data);
    Append(static_cast<const char*>(data), len);
}

void Buffer::Append(const char* str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}

void Buffer::Append(const Buffer& buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

void Buffer::EnsureWriteable(size_t len) {
    if (WritableBytes() < len) {
        MakeSpace_(len);
    }
    assert(WritableBytes() >= len);
}

ssize_t Buffer::ReadFd(int fd, int* saveErrno) {
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    /* 分散读， 保证数据全部读完 */
    iov[0].iov_base = BeginPtr_() + _write_pos;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);  // 使用readv进行分散读取
    if (len < 0) {
        *saveErrno = errno;
    }
    // 缓冲区未满
    else if (static_cast<size_t>(len) <= writable) {
        _write_pos += len;
    }
    // 缓冲区满，将buff数据移动数据到缓冲区
    else {
        _write_pos = _buffer.size();
        Append(buff, len - writable);
    }
    return len;
}

ssize_t Buffer::WriteFd(int fd, int* saveErrno) {
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);  // 使用write写入数据到文件描述符
    if (len < 0) {
        *saveErrno = errno;
        return len;
    }
    _read_pos += len;
    return len;
}

char* Buffer::BeginPtr_() {
    return &*_buffer.begin();
}

const char* Buffer::BeginPtr_() const {
    return &*_buffer.begin();
}

void Buffer::MakeSpace_(size_t len) {
    // 缓冲区容量不足直接扩容
    if (WritableBytes() + PrependableBytes() < len) {
        _buffer.resize(_write_pos + len + 1);
    }
    // 缓冲区容量足够，将数据前移
    else {
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + _read_pos, BeginPtr_() + _write_pos, BeginPtr_());
        _read_pos = 0;
        _write_pos = _read_pos + readable;
        assert(readable == ReadableBytes());
    }
}

