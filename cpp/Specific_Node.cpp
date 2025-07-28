#include "Specific_Node.h"

MultiplyBy2Node::MultiplyBy2Node(std::initializer_list<BaseNode *> inputs) : BaseNode(inputs) {}

void MultiplyBy2Node::execute()
{
    size_t size = getInputs()[0]->getOutput().size();
    {
        std::lock_guard<std::mutex> lock(outputMutex_);
        output_.resize(size, 0);
        for (int i = 0; i < size; ++i)
        {
            output_[i] = getInputs()[0]->getOutput()[i] * 2;
        }
    }
}

AddNode::AddNode(std::initializer_list<BaseNode *> inputs) : BaseNode(inputs)
{
    if (inputs.size() == 0)
    {
        throw std::invalid_argument("AddNode must have at least one input");
    }
}

void AddNode::execute()
{
    size_t size = getInputs()[0]->getOutput().size();
    {
        std::lock_guard<std::mutex> lock(outputMutex_);
        output_.resize(size, 0);
        for (auto input : getInputs())
        {
            for (int i = 0; i < output_.size(); ++i)
            {
                output_[i] += input->getOutput()[i];
            }
        }
    }
}
