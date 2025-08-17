#include "ThreadPoolV1.h"

ThreadPoolV1::ThreadPoolV1(size_t threadCount)
{
    if (threadCount == 0)
        threadCount = 1; // 至少1个线程
    startWorkers(threadCount);
}

ThreadPoolV1::~ThreadPoolV1()
{
    stopWorkers();
}

void ThreadPoolV1::startWorkers(size_t threadCount)
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

void ThreadPoolV1::stopWorkers()
{
    stopFlag_ = true;
    condition_.notify_all(); // 唤醒所有worker线程
    for (auto &worker : workers_)
    {
        if (worker.joinable())
            worker.join();
    }
}