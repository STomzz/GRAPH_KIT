#include <iostream>
#include <vector>
#include "Conv2d.h"
#include "Graph_stream.h"
#include "InputNode.h"
#include "Softmax.h"
#include "Specific_Node.h"

int main() {
    //===============test1==================
    // try
    // {
    //     std::vector<int> data = {1, 2, 3, 4, 5};
    //     auto *inputNode = new InputNode(data);
    //     auto *node1 = new MultiplyBy2Node();
    //     auto *node2 = new AddNode();

    //     GraphStream &graph = GraphStream::getInstance();
    //     graph.addDependency(node1, inputNode);
    //     graph.addDependency(node2, inputNode);
    //     graph.addDependency(node2, node1);
    //     graph.execute();
    //     graph.waitForCompletion();

    //     std::vector<int> ans = node2->getOutput();
    //     std::cout << "the ans of node2:" << std::endl;
    //     for (auto &i : ans)
    //     {
    //         std::cout << i << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // catch (const std::exception &e)
    // {
    //     std::cout << e.what() << std::endl;
    //     return 1;
    // }
    // return 0;

    //===============test2==================
    try {
        // 创建输入数据（示例使用1D数据，实际应根据Conv2D需求调整）4x4
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        auto* inputNode = new InputNode(data);

        // 创建Conv2D节点（参数示例，需根据实际实现调整）
        int kernelSize = 3;
        std::vector<std::vector<int>> kernel = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};  // 3x3卷积核
        auto* conv2dNode = new Conv2d(kernelSize, kernel, 1, 0);

        // 创建softmax节点
        auto* softmaxNode = new Softmax();

        GraphStream& graph = GraphStream::getInstance();
        graph.addDependency(conv2dNode, inputNode);
        graph.addDependency(softmaxNode, conv2dNode);

        graph.execute();
        graph.waitForCompletion();

        // 获取并输出前5个结果
        std::vector<int> result = softmaxNode->getOutput();
        std::cout << "Conv2D前5个输出：\n";
        for (int i = 0; i < 4 && i < result.size(); ++i) {
            std::cout << result[i] << " ";
        }
        std::cout << std::endl;

        // 清理内存
        // delete inputNode;
        // delete conv2dNode;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}