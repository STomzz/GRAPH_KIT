#pragma once
#include "BaseNode.h"
#include <vector>

class Conv2d : public BaseNode
{
public:
    Conv2d(int kernelSize,
           const std::vector<std::vector<int>> &kernel,
           int stride = 1,
           int padding = 0);

    void execute() override;

private:
    int kernelSize_;
    int stride_;
    int padding_;
    std::vector<std::vector<int>> kernel_;

    // 辅助函数：执行卷积操作
    std::vector<int> run(const std::vector<int> &input, int inputWidth, int inputHeight);
};