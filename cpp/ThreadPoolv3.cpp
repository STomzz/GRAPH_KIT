#include "ThreadPoolV3.h"
#include <stdexcept>

ThreadPoolV3::ThreadPoolV3(size_t threadCount)
{
    if (threadCount == 0)
        threadCount = 1; // 至少1个线程
    startWorkers(threadCount);
}

ThreadPoolV3::~ThreadPoolV3()
{
    stopWorkers();
}

void ThreadPoolV3::startWorkers(size_t threadCount)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers_.emplace_back([this]
                              {
            while (!stopFlag_) {
                // 从RingBuffer获取任务
                auto task = tasks_.pop();
                
                // 执行任务
                if (task) {
                    task();
                }
            } });
    }
}

void ThreadPoolV3::stopWorkers()
{
    stopFlag_ = true;

    // 为了确保所有等待的线程都能退出，我们需要唤醒它们
    // 这里简单地添加足够多的空任务来唤醒所有工作线程
    for (size_t i = 0; i < workers_.size(); ++i)
    {
        tasks_.push([] {});
    }

    for (auto &worker : workers_)
    {
        if (worker.joinable())
            worker.join();
    }
}

// 模板方法的实现
template <typename F, typename... Args>
auto ThreadPoolV3::enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
{
    // 1.定义返回类型
    using return_type = typename std::invoke_result<F, Args...>::type;

    // 2.创建一个共享的任务对象
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    // 3.获取任务的结果(future)对象
    auto res = task->get_future();

    // 检查线程池是否已停止
    if (stopFlag_)
        throw std::runtime_error("enqueue on stopped ThreadPoolV3");

    // 4.将任务添加到RingBuffer任务队列
    tasks_.push([task]()
                { (*task)(); });

    // 5.返回future对象
    return res;
}

// 显式实例化模板，避免链接错误
// 为void返回类型显式实例化
template std::future<void>
ThreadPoolV3::enqueue<std::function<void()>>(std::function<void()> &&);
// 为int返回类型显式实例化
template std::future<int> ThreadPoolV3::enqueue<std::function<int()>>(std::function<int()> &&);