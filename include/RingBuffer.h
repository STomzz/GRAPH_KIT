#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

// 简单的RingBuffer实现
template <typename T, size_t Capacity>
class RingBuffer
{
private:
    std::vector<T> buffer_;
    size_t read_index_ = 0;
    size_t write_index_ = 0;
    std::atomic<size_t> count_ = 0;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    RingBuffer() : buffer_(Capacity) {}

    // 生产者调用，向缓冲区添加元素
    void push(T &&item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this]
                       { return count_ < Capacity; });

        buffer_[write_index_] = std::move(item);
        write_index_ = (write_index_ + 1) % Capacity;
        ++count_;

        lock.unlock();
        not_empty_.notify_one();
    }

    // 消费者调用，从缓冲区取出元素
    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this]
                        { return count_ > 0; });

        T item = std::move(buffer_[read_index_]);
        read_index_ = (read_index_ + 1) % Capacity;
        --count_;

        lock.unlock();
        not_full_.notify_one();

        return item;
    }

    // 检查缓冲区是否为空
    bool empty() const
    {
        return count_ == 0;
    }

    // 检查缓冲区是否已满
    bool full() const
    {
        return count_ == Capacity;
    }

    // 获取缓冲区当前元素数量
    size_t size() const
    {
        return count_;
    }
};