#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <atomic>

class ThreadPool {
public:
    // 构造函数：创建指定数量的worker线程（默认使用硬件并发数）
    explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency()) {
        if (threadCount == 0) threadCount = 1; // 至少1个线程
        startWorkers(threadCount);
    }

    // 析构函数：停止所有worker线程并等待完成
    ~ThreadPool() {
        stopWorkers();
    }

    // 禁止拷贝构造和赋值
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // 提交任务到线程池
    template <typename F>
    void enqueue(F&& task) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        tasks_.emplace(std::forward<F>(task));
        condition_.notify_one(); // 唤醒一个worker线程
    }

private:
    std::vector<std::thread> workers_;       // worker线程列表
    std::queue<std::function<void()>> tasks_; // 任务队列
    std::mutex queueMutex_;                  // 保护任务队列的互斥锁
    std::condition_variable condition_;      // 用于唤醒worker线程的条件变量
    std::atomic<bool> stopFlag_{false};      // 线程池停止标志

    // 启动worker线程
    void startWorkers(size_t threadCount) {
        for (size_t i = 0; i < threadCount; ++i) {
            workers_.emplace_back([this] {
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
                }
            });
        }
    }

    // 停止所有worker线程
    void stopWorkers() {
        stopFlag_ = true;
        condition_.notify_all(); // 唤醒所有worker线程
        for (auto& worker : workers_) {
            if (worker.joinable()) worker.join();
        }
    }
};