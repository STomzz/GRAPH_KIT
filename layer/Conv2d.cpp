#include "Conv2d.h"
#include <cmath>
#include <stdexcept>

Conv2d::Conv2d(int kernelSize,
               const std::vector<std::vector<int>>& kernel,
               int stride,
               int padding)
    : BaseNode(), kernelSize_(kernelSize), stride_(stride), padding_(padding), kernel_(kernel) {
    // if (inputNode.size() != 1) {
    //     throw std::invalid_argument("Conv2d must have exactly one input");
    // }

    if (kernel.size() != kernelSize_ || kernel[0].size() != kernelSize_) {
        throw std::invalid_argument("Kernel dimensions do not match kernelSize");
    }
    // 验证卷积核是方阵
    for (const auto& row : kernel) {
        if (row.size() != kernelSize_) {
            throw std::invalid_argument("Kernel must be a square matrix");
        }
    }
}

std::vector<int> Conv2d::run(const std::vector<int>& input, int inputWidth, int inputHeight) {
    // 计算填充后的尺寸
    int paddedWidth = inputWidth + 2 * padding_;
    int paddedHeight = inputHeight + 2 * padding_;

    // 创建填充后的输入
    std::vector<std::vector<int>> paddedInput(paddedHeight, std::vector<int>(paddedWidth, 0));
    for (int i = 0; i < inputHeight; ++i) {
        for (int j = 0; j < inputWidth; ++j) {
            paddedInput[i + padding_][j + padding_] = input[i * inputWidth + j];
        }
    }

    // 计算输出尺寸
    int outputWidth = (paddedWidth - kernelSize_) / stride_ + 1;
    int outputHeight = (paddedHeight - kernelSize_) / stride_ + 1;
    if (outputWidth <= 0 || outputHeight <= 0) {
        throw std::runtime_error("Invalid output dimensions after convolution");
    }
    std::vector<int> output(outputWidth * outputHeight, 0);

    // 执行卷积
    for (int i = 0; i < outputHeight; ++i) {
        for (int j = 0; j < outputWidth; ++j) {
            int sum = 0;
            for (int k = 0; k < kernelSize_; ++k) {
                for (int l = 0; l < kernelSize_; ++l) {
                    int inputX = j * stride_ + l;
                    int inputY = i * stride_ + k;
                    sum += paddedInput[inputY][inputX] * kernel_[k][l];
                }
            }
            output[i * outputWidth + j] = sum;
        }
    }
    return output;
}

void Conv2d::execute() {
    const auto& inputNode = getInputs();

    // 验证输入节点数量
    if (inputNode.size() != 1) {
        throw std::runtime_error("Conv2d must have exactly one input node");
    }
    const auto& inputData = inputNode[0]->getOutput();

    if (inputData.empty()) {
        throw std::runtime_error("Conv2d input data cannot be empty");
    }

    // 假设输入是方形图像，这里简化处理
    int inputSize = static_cast<int>(std::sqrt(inputData.size()));
    if (inputSize * inputSize != static_cast<int>(inputData.size())) {
        throw std::runtime_error("Conv2d input must be a square image");
    }

    auto result = run(inputData, inputSize, inputSize);

    std::lock_guard<std::mutex> lock(outputMutex_);
    output_ = std::move(result);
}