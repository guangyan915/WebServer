#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <mutex>


template <class T>
class ObjectPool
{
    // 获取自由链表下一个对象的指针
    void*& NextObj(void* ptr)
    {
        // 截取头32为：4,64位：8 的内容
        return (*(void**)ptr);
    }
void* SystemAlloc(size_t size) {
    void* _memory = nullptr;
#ifdef _WIN32
    _memory = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (_memory == nullptr)
    {
        throw std::bad_alloc();
    }
#else
    //if (_block_size <= 128 * 1024)
    //{
    //    _memory = sbrk(size);
    //    if (_memory == (char*)-1)
    //    {
    //        throw std::bad_alloc();
    //    }
    //}
    //else
    //{
    _memory = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (_memory == MAP_FAILED)
    {
        throw std::bad_alloc();
    }
#endif
    return _memory;
}

void SystemFree(void* memory, size_t size)
{
#ifdef _WIN32
    VirtualFree(memory, 0, MEM_RELEASE);
#else
    munmap(memory, size);
#endif
}

public:
    // 构造函数，接受预分配对象个数和每次扩容的增量
    ObjectPool(size_t init_capacity = 1024, size_t increment = 128, bool is_lock = false)
        : _init_capacity(init_capacity),
        _memory(nullptr),
        _remain_size(0),
        _free_list(nullptr),
        _is_lock(is_lock)
    {
        // 计算对象大小，确保能够存储地址
        _obj_size = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
        _block_size = increment * _obj_size;

        _memory = static_cast<char*>(SystemAlloc(_init_capacity * _obj_size));
        if (_memory == nullptr)
        {
            throw std::bad_alloc();
        }
        _remain_size = _init_capacity * _obj_size;
        _block_memory.push_back({ _memory, _remain_size });
    }

    // 析构函数，释放所有已分配的内存块
    ~ObjectPool()
    {
        //std::unique_lock<std::mutex> lock(_mtx);
        if (_is_lock) _mtx.lock();
        for (int i = 0; i < _block_memory.size(); i++) {
            SystemFree(_block_memory[i].first, _block_memory[i].second);
        }
        if (_is_lock) _mtx.unlock();
    }

    // 申请对象，支持构造函数参数
    template <typename... Args>
    T* New(Args&&... args)
    {
        //std::unique_lock<std::mutex> lock(_mtx);
        if (_is_lock) _mtx.lock();
        T* obj = nullptr;

        if (_free_list != nullptr)
        {
            
            // 从自由链表头获取一个对象
            obj = static_cast<T*>(_free_list);
            _free_list = NextObj(_free_list);
        }
        else
        {
            if (_remain_size < _obj_size)
            {
                // 分配新的内存块，大小为增量乘以对象大小
                _block_size *= 2;
                char* new_block = static_cast<char*>(SystemAlloc(_block_size));
                if (new_block == nullptr)
                {
                    throw std::bad_alloc();
                }

                _memory = new_block;
                _remain_size = _block_size;
                _block_memory.push_back({ _memory, _block_size });

            }
            obj = reinterpret_cast<T*>(_memory);
            if (obj == nullptr)
            {
                throw std::bad_alloc();
            }
            _memory += _obj_size;
            _remain_size -= _obj_size;
        }
        // 使用 placement new 在已分配内存上调用构造函数，传递参数
        new (obj) T(std::forward<Args>(args)...);

        if (_is_lock) _mtx.unlock();
        return obj;
    }

    void Delete(T* obj)
    {
        //std::unique_lock<std::mutex> lock(_mtx);
        if (_is_lock) _mtx.lock();
        obj->~T();
        // 将对象插入自由链表
        NextObj(obj) = _free_list;
        _free_list = obj;
        if (_is_lock) _mtx.unlock();
    }

private:
    size_t _init_capacity;
    size_t _block_size;                 // 分配新的内存块，大小为增量乘以对象大小
    size_t _obj_size;                   // 对象大小，最小保证能够存下地址
    char* _memory;                      // 指向分配的内存块
    size_t _remain_size;                // 当前内存块中剩余大小
    void* _free_list;                   // 指向自由链表的头指针
    std::vector<std::pair<char*, size_t>> _block_memory;   // 保存每次分配的新内存块
    std::mutex _mtx;
    bool _is_lock;
};

#endif
