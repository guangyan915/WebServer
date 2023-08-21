#include "../include/heaptimer.h"

void HeapTimer::siftup(size_t i) {
    assert(i >= 0 && i < _heap.size());
    size_t j = (i - 1) / 2;
    while (j >= 0) {
        if (_heap[j] < _heap[i]) { break; } // 上浮直到父节点小于等于子节点
        SwapNode(i, j); // 交换节点
        i = j;
        j = (i - 1) / 2;
    }
}

void HeapTimer::SwapNode(size_t i, size_t j) {
    assert(i >= 0 && i < _heap.size());
    assert(j >= 0 && j < _heap.size());
    std::swap(_heap[i], _heap[j]); // 交换节点在堆中的位置
    _ref[_heap[i].id] = i; // 更新交换后的位置映射
    _ref[_heap[j].id] = j;
}

bool HeapTimer::siftdown(size_t index, size_t n) {
    assert(index >= 0 && index < _heap.size());
    assert(n >= 0 && n <= _heap.size());
    size_t i = index;
    size_t j = i * 2 + 1;
    while (j < n) {
        if (j + 1 < n && _heap[j + 1] < _heap[j]) j++; // 选择左右子节点中较小的一个
        if (_heap[i] < _heap[j]) break; // 如果父节点小于等于子节点，不需要再调整
        SwapNode(i, j); // 交换节点
        i = j;
        j = i * 2 + 1;
    }
    return i > index;
}

void HeapTimer::add(int id, int timeout, const TimeoutCallBack& cb) {
    assert(id >= 0);
    size_t i;
    if (_ref.count(id) == 0) {
        // 新节点：堆尾插入，调整堆
        i = _heap.size();
        _ref[id] = i;
        _heap.push_back({ id, Clock::now() + MS(timeout), cb }); // 创建新的定时器节点
        siftup(i); // 节点上浮
    }
    else {
        // 已有结点：调整堆
        i = _ref[id];
        _heap[i].expires = Clock::now() + MS(timeout); // 更新已有定时器的到期时间
        _heap[i].cb = cb;
        if (!siftdown(i, _heap.size())) {
            siftup(i);
        }
    }
}

void HeapTimer::doWork(int id) {
    // 删除指定id结点，并触发回调函数
    if (_heap.empty() || _ref.count(id) == 0) {
        return;
    }
    size_t i = _ref[id];
    TimerNode node = _heap[i];
    node.cb(); // 执行回调函数
    del(i); // 移除定时器
}

void HeapTimer::del(size_t index) {
    // 删除指定位置的结点
    assert(!_heap.empty() && index >= 0 && index < _heap.size());
    // 将要删除的结点换到队尾，然后调整堆 
    size_t i = index;
    size_t n = _heap.size() - 1;
    assert(i <= n);
    if (i < n) {
        SwapNode(i, n);
        if (!siftdown(i, n)) {
            siftup(i);
        }
    }
    // 队尾元素删除
    _ref.erase(_heap.back().id);
    _heap.pop_back();
}

void HeapTimer::adjust(int id, int timeout) {
    // 调整指定id的结点
    assert(!_heap.empty() && _ref.count(id) > 0);
    _heap[_ref[id]].expires = Clock::now() + MS(timeout);
    siftdown(_ref[id], _heap.size());
}

void HeapTimer::tick() {
    // 清除超时结点
    if (_heap.empty()) {
        return;
    }
    while (!_heap.empty()) {
        TimerNode node = _heap.front();
        if (std::chrono::duration_cast<MS>(node.expires - Clock::now()).count() > 0) {
            break;
        }
        node.cb();
        pop();
    }
}

void HeapTimer::pop() {
    assert(!_heap.empty());
    del(0);
}

void HeapTimer::clear() {
    _ref.clear();
    _heap.clear();
}

int HeapTimer::GetNextTick() {
    tick();
    size_t res = -1;
    if (!_heap.empty()) {
        res = std::chrono::duration_cast<MS>(_heap.front().expires - Clock::now()).count();
        if (res < 0) res = 0; 
    }
    return res;
}

