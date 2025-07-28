#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <atomic>
#pragma once

class BaseNode
{
public:
    // BaseNode(int id, int type, int x, int y);
    virtual ~BaseNode() = default;
    virtual void execute() = 0;

    //线程安全获取输出
    const std::vector<int>& getOutput() const;
    virtual const std::vector<BaseNode*>& getInputs() const;
    void setRemainDependencies(int num);
    bool isReady();
protected:
    std::vector<BaseNode*> inputs_;
    std::vector<int> output_;
    mutable std::mutex outputMutex_;//使用mutable确保const函数中可以修改该非const变量
    std::atomic<int> remaining_dependencies{0};//原子变量确保线程安全
    BaseNode(std::initializer_list<BaseNode*> inputs);//构造函数，初始化输入节点
};