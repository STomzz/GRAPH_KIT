#include "BaseNode.h"

#pragma once
class InputNode : public BaseNode{
public:
    explicit InputNode(std::vector<int>& data);
    void execute() override;

private:
    std::vector<int> data_;
};
