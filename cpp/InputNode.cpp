#include "InputNode.h"

InputNode::InputNode(std::vector<int>& data) : BaseNode({}), data_(data){}

void InputNode::execute(){
    std::lock_guard<std::mutex> lock(outputMutex_);
    output_ = data_;
}


