#pragma once
#include <memory>
#include <functional>
#include <future>
#include "ThreadPool.h"

class ThreadPoolInterface
{
private:
    class Impl
    {
    public:
        explicit Impl(size_t threadCount) : threadPool_(std::make_unique<ThreadPool>(threadCount)) {}
        ~Impl() = default;

        // 禁止拷贝构造和赋值
        Impl(const Impl &) = delete;
        Impl &operator=(const Impl &) = delete;

        template <typename F, typename... Args>
        auto enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
        {
            return threadPool_->enqueue(std::forward<F>(f), std::forward<Args>(args)...);
        }

    private:
        std::unique_ptr<ThreadPool> threadPool_;
    };

public:
    explicit ThreadPoolInterface(size_t threadCount = std::thread::hardware_concurrency()) : pImpl(std::make_unique<Impl>(threadCount)) {}
    ~ThreadPoolInterface() = default;

    // 禁止拷贝构造和赋值
    ThreadPoolInterface(const ThreadPoolInterface &) = delete;
    ThreadPoolInterface &operator=(const ThreadPoolInterface &) = delete;

    // 通用的 enqueue 方法
    template <typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        return pImpl->enqueue(std::forward<F>(f), std::forward<Args>(args)...);
    }

private:
    std::unique_ptr<Impl> pImpl;
};