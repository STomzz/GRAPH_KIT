#include "ThreadPoolv2.h"
#include <stdexcept>
#include <iostream>
ThreadPool::ThreadPool(size_t threadCount)
{
    std::cout << "ThreadPoolv2 constructor called with threadCount: " << threadCount << std::endl;
    if (threadCount == 0)
        threadCount = 1; // 至少1个线程
    startWorkers(threadCount);
}

ThreadPool::~ThreadPool()
{
    stopWorkers();
}

void ThreadPool::startWorkers(size_t threadCount)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers_.emplace_back([this]
                              {
            while (!stopFlag_) {
                std::function<void()> task;
                // 从队列获取任务（加锁）
                {
                    std::unique_lock<std::mutex> lock(queueMutex_);
                    condition_.wait(lock, [this] { 
                        return stopFlag_ || !tasks_.empty(); 
                    });
                    if (stopFlag_ && tasks_.empty()) break; // 停止且无任务，退出线程
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task(); // 执行任务
            } });
    }
}

void ThreadPool::stopWorkers()
{
    stopFlag_ = true;
    condition_.notify_all(); // 唤醒所有worker线程
    for (auto &worker : workers_)
    {
        if (worker.joinable())
            worker.join();
    }
}

template <typename F, typename... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
{
    // 1.定义返回类型
    using return_type = typename std::invoke_result<F, Args...>::type;
    // 2.创建一个共享的任务对象
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    // 3.获取任务的结果(future)对象
    auto res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        if (stopFlag_)
            throw std::runtime_error("enqueue on stopped ThreadPoolv2");
        // 4.将任务添加到任务队列
        //  注意：这里使用std::function<void()>来存储任务
        tasks_.emplace([task]()
                       { (*task)(); });
    }
    condition_.notify_one();
    return res;
}

// 显式实例化模板，避免链接错误
// 注意：在实际使用中，可能需要根据具体的函数类型添加更多的实例化
// 为void返回类型显式实例化
template std::future<void> ThreadPool::enqueue<std::function<void()>>(std::function<void()> &&);
// 为int返回类型显式实例化
template std::future<int> ThreadPool::enqueue<std::function<int()>>(std::function<int()> &&);