#pragma once
#include <deque>
#include <thread>
#include <shared_mutex>

using namespace std;

template <typename T>
class SafeQueue
{
private:
    deque<T> _queue;
    mutable shared_mutex _mtx;
    size_t _max_size;

public:
    SafeQueue(size_t max_size = 0)
    {
        _max_size = max_size;
    }

    void push(const T& value)
    {
        unique_lock<shared_mutex> lock(_mtx);
        if (_max_size > 0)
        {
            if (_queue.size() >= _max_size)
                _queue.pop_front();
        }
        _queue.push_back(value);
    }

    T& pop()
    {
        unique_lock<shared_mutex> lock(_mtx);
        if (_queue.empty()) {
            throw runtime_error("Tentativa de pop em uma fila vazia");
        }
        T& value = _queue.front();
        _queue.pop_front();
        return value;
    }

    bool empty() const
    {
        shared_lock<shared_mutex> lock(_mtx);
        return _queue.empty();
    }
};