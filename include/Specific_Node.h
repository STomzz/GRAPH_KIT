
#include "BaseNode.h"

#pragma once
class MultiplyBy2Node : public BaseNode{
public:
    MultiplyBy2Node(std::initializer_list<BaseNode *> inputs);
    void execute() override;
};

class AddNode : public BaseNode{
public:
    AddNode(std::initializer_list<BaseNode*> inputs);
    void execute() override;
};