#include "ThreadPoolv2.h"
#include <stdexcept>
#include <iostream>
ThreadPoolV2::ThreadPoolV2(size_t threadCount)
{
    std::cout << "ThreadPoolV2 constructor called with threadCount: " << threadCount << std::endl;
    if (threadCount == 0)
        threadCount = 1; // 至少1个线程
    startWorkers(threadCount);
}

ThreadPoolV2::~ThreadPoolV2()
{
    stopWorkers();
}

void ThreadPoolV2::startWorkers(size_t threadCount)
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

void ThreadPoolV2::stopWorkers()
{
    stopFlag_ = true;
    condition_.notify_all(); // 唤醒所有worker线程
    for (auto &worker : workers_)
    {
        if (worker.joinable())
            worker.join();
    }
}

// template <typename F, typename... Args>
// auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
// {
//     // 1.定义返回类型
//     using return_type = typename std::invoke_result<F, Args...>::type;
//     // 2.创建一个共享的任务对象
//     auto task = std::make_shared<std::packaged_task<return_type()>>(
//         std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//     // 3.获取任务的结果(future)对象
//     auto res = task->get_future();
//     {
//         std::unique_lock<std::mutex> lock(queueMutex_);
//         if (stopFlag_)
//             throw std::runtime_error("enqueue on stopped ThreadPoolv2");
//         // 4.将任务添加到任务队列
//         //  注意：这里使用std::function<void()>来存储任务
//         tasks_.emplace([task]()
//                        { (*task)(); });
//     }
//     condition_.notify_one();
//     return res;
// }

// 模板编译机制：C++ 模板在编译时才会生成具体的代码。当编译器遇到模板定义时，它并不会立即生成代码，而是等到模板被具体实例化时才会生成相应的代码。

// 分离编译问题：当模板定义在.cpp 文件中而不是头文件中时，会出现链接问题。因为编译器在编译其他使用该模板的.cpp 文件时，并不知道模板的具体实现，因此无法生成相应的代码。
// 解决方法：
// 1. 将模板定义放在头文件中：这样每个使用该模板的.cpp 文件都会包含模板的定义，从而避免链接错误。
// 2. 显式实例化模板：在.cpp 文件中显式实例化特定类型的模板，这样编译器就会生成相应的代码。

// 显式实例化模板，避免链接错误
// 为void返回类型显式实例化
template std::future<void>
ThreadPoolV2::enqueue<std::function<void()>>(std::function<void()> &&);
// 为int返回类型显式实例化
template std::future<int> ThreadPoolV2::enqueue<std::function<int()>>(std::function<int()> &&);