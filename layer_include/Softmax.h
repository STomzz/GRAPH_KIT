#pragma once
#include <vector>
#include "BaseNode.h"

class Softmax : public BaseNode {
   public:
    Softmax();

    void execute() override;

   private:
    // 辅助函数：执行softmax操作
    std::vector<int> run(const std::vector<int>& input);
};