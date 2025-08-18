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
                try {
                    // 从RingBuffer获取任务（阻塞等待）
                    auto task = tasks_.pop();
                
                // 执行任务
                    if (task) {
                        task();  // 执行任务
                    }
                } catch (const std::exception& e) {
                    // 处理可能的异常，主要是中断异常
                    if (stopFlag_) break;
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