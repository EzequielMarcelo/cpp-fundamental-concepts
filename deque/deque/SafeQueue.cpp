#include "SafeQueue.h"

template <typename T>
SafeQueue<T>::SafeQueue(size_t max_size)
{
    _max_size = max_size;
}

template <typename T>
void SafeQueue<T>::push(const T& value)
{
    unique_lock<shared_mutex> lock(_mtx);
    if (_max_size > 0)
    {
        if (_queue.size() >= _max_size)
            _queue.pop_front();
    }
    _queue.push_back(value);
}

template <typename T>
T SafeQueue<T>::pop()
{
    unique_lock<shared_mutex> lock(_mtx);

    if (_queue.empty())
        throw runtime_error("Tentativa de pop em uma fila vazia");

    T value = _queue.front();
    _queue.pop_front();
    return value;
}

template <typename T>
bool SafeQueue<T>::empty() const
{
    shared_lock<shared_mutex> lock(_mtx);
    return _queue.empty();
}

template class SafeQueue<int>;