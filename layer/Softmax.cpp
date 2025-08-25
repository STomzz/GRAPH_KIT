#include "Softmax.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

Softmax::Softmax() : BaseNode() {}

std::vector<int> Softmax::run(const std::vector<int>& input) {
    // 为了防止数值溢出，先减去最大值
    int maxVal = *std::max_element(input.begin(), input.end());

    // 计算指数
    std::vector<double> expValues(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        expValues[i] = std::exp(input[i] - maxVal);
    }

    // 计算总和
    double sumExp = std::accumulate(expValues.begin(), expValues.end(), 0.0);

    // 计算softmax并转换回int（这里简化处理，实际应用中可能需要浮点数）
    std::vector<int> result(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        result[i] = static_cast<int>(expValues[i] / sumExp * 1000);  // 放大1000倍以便用int表示
    }

    return result;
}

void Softmax::execute() {
    const auto& inputNode = getInputs();
    if (inputNode.size() != 1) {
        throw std::runtime_error("Conv2d must have exactly one input node");
    }
    const auto& inputData = inputNode[0]->getOutput();
    if (inputData.empty()) {
        throw std::runtime_error("Softmax input cannot be empty");
    }

    auto result = run(inputData);

    std::lock_guard<std::mutex> lock(outputMutex_);
    output_ = std::move(result);
}