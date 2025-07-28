#include "Graph_stream.h"

GraphStream &GraphStream::getInstance()
{
    static GraphStream instance;
    return instance;
}

GraphStream::~GraphStream()
{
    waitForCompletion();
    for (auto &node : nodes_)
    {
        delete node;
    }
    nodes_.clear();

    std::lock_guard<std::mutex> lock(nodes_mutex);
    node_successors_.clear();
    inDegree_.clear();
}

void GraphStream::addNode(BaseNode *node)
{
    if (!node)
    {
        throw std::invalid_argument("Node cannot be null");
    }
    std::lock_guard<std::mutex> lock(nodes_mutex);
    if (nodes_.count(node) == 0)
    {
        nodes_.insert(node);
        inDegree_[node] = 0;
    }
}

void GraphStream::addDependency(BaseNode *current_node, BaseNode *dependent_input_node)
{
    if (!current_node || !dependent_input_node)
    {
        throw std::invalid_argument("Nodes cannot be null");
    }
    addNode(current_node);
    addNode(dependent_input_node);
    {
        std::lock_guard<std::mutex> lock(successor_mutex);
        node_successors_[dependent_input_node].push_back(current_node);
        inDegree_[current_node]++;
    }
}

void GraphStream::startNodeExecution(BaseNode *node)
{
    std::thread t([this, node]()
                  {
        try{
            node->execute();
        }
        catch(const std::exception & e){
            std::cerr<<"[ERROR]this node startNodeExecution failed:"<<e.what()<<std::endl;
            //强制结束等待
            std::lock_guard<std::mutex> lock(completed_mutex);
            completed_node_count++;
            cv.notify_all();
        }
        //通知后继节点
        {
            std::lock_guard<std::mutex> lock_successor(successor_mutex);
            if(node_successors_.count(node)){
                for(auto successor : node_successors_[node]){
                    //若后继的节点满足条件，则启动线程
                    if(successor->isReady()){
                        startNodeExecution(successor);
                    }
                }
            }
        }
        std::lock_guard<std::mutex> lock(completed_mutex);
        //计数已完成的node数量
        completed_node_count++;
        cv.notify_all(); });
    std::lock_guard<std::mutex> lock(threads_mutex);
    threads_.push_back(std::move(t));
}

void GraphStream::execute()
{
    std::lock_guard<std::mutex> lock(nodes_mutex);
    for (const auto &[node, degree] : inDegree_)
    {
        node->setRemainDependencies(degree);
    }
    for (const auto &[node, degree] : inDegree_)
    {
        if (degree == 0)
        {
            startNodeExecution(node);
        }
    }
}

void GraphStream::waitForCompletion()
{
    std::unique_lock<std::mutex> lock(completed_mutex);
    cv.wait(lock, [this]()
            {
        std::lock_guard<std::mutex> nodes_lock(nodes_mutex);
        return completed_node_count == nodes_.size(); });
    // 等待所有线程结束
    std::lock_guard<std::mutex> threads_lock(threads_mutex);
    for (auto &t : threads_)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}
