#include "BaseNode.h"

BaseNode::BaseNode() {}

const std::vector<BaseNode *> &BaseNode::getInputs() const
{
    return inputs_;
}

void BaseNode::setRemainDependencies(int num)
{
    remaining_dependencies.store(num);
}

bool BaseNode::isReady()
{
    // 修改为原子操作，确保线程安全
    return remaining_dependencies.fetch_sub(1) == 1;
}

const std::vector<int> &BaseNode::getOutput() const
{
    std::lock_guard<std::mutex> lock(outputMutex_);
    return output_;
}

void BaseNode::addInput(BaseNode *input)
{
    if (input)
        inputs_.emplace_back(input);
}
