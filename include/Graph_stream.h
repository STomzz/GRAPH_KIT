#include "BaseNode.h"
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <memory>
#include "ThreadPool.h"

#pragma once

class GraphStream
{
public:
    //单例模式
    GraphStream(const GraphStream &) = delete;//删除拷贝构造
    GraphStream &operator=(const GraphStream &) = delete;//删除拷贝赋值
    // ~GraphStream();
    void addNode(BaseNode *node);
    void addDependency(BaseNode *current_node, BaseNode *dependent_input_node);
    void execute();
    void waitForCompletion();
    static GraphStream& getInstance();

private:
    GraphStream() = default; // 构造函数
    ~GraphStream();
    std::unordered_set<BaseNode *> nodes_;
    std::unordered_map<BaseNode *, std::vector<BaseNode *>> node_successors_;
    std::unordered_map<BaseNode *, int> inDegree_;
    int completed_node_count = 0;
    // 线程安全
    std::mutex nodes_mutex;
    std::mutex successor_mutex;
    std::mutex completed_mutex;
    std::condition_variable cv;
    ThreadPool threadPool_;

    // 线程启动
    void startNodeExecution(BaseNode *node);
};