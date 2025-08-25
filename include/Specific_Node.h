
#include "BaseNode.h"

#pragma once
class MultiplyBy2Node : public BaseNode{
public:
    MultiplyBy2Node();
    void execute() override;
};

class AddNode : public BaseNode{
public:
    AddNode();
    void execute() override;
};