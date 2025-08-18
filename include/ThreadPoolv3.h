#pragma once
#include "RingBuffer.h"
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <atomic>
#include <future>

class ThreadPoolV3
{
public:
    // 构造函数：创建指定数量的worker线程（默认使用硬件并发数）
    explicit ThreadPoolV3(size_t threadCount = std::thread::hardware_concurrency());

    // 析构函数：停止所有worker线程并等待完成
    ~ThreadPoolV3();

    // 禁止拷贝构造和赋值
    ThreadPoolV3(const ThreadPoolV3 &) = delete;
    ThreadPoolV3 &operator=(const ThreadPoolV3 &) = delete;

    // 将任务添加到线程池
    template <typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
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

private:
    // 使用RingBuffer作为任务队列，容量为1024
    RingBuffer<std::function<void()>, 1024> tasks_;
    std::vector<std::thread> workers_;  // worker线程列表
    std::atomic<bool> stopFlag_{false}; // 线程池停止标志

    // 启动worker线程
    void startWorkers(size_t threadCount);

    // 停止所有worker线程
    void stopWorkers();
};