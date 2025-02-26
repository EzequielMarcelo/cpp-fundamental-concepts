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
        SafeQueue(size_t max_size = 0);
        void push(const T& value);
        T pop();
        bool empty() const;
};
