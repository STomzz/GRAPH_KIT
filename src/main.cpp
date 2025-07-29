#include "Graph_stream.h"
#include <vector>
#include <iostream>
#include "InputNode.h"
#include "Specific_Node.h"

int main()
{
    try
    {
        std::vector<int> data = {1, 2, 3, 4, 5};
        auto *inputNode = new InputNode(data);
        auto *node1 = new MultiplyBy2Node({inputNode});
        auto *node2 = new AddNode({node1, inputNode});

        GraphStream &graph = GraphStream::getInstance();
        graph.addDependency(node1, inputNode);
        graph.addDependency(node2, inputNode);
        graph.addDependency(node2, node1);
        graph.execute();
        graph.waitForCompletion();

        std::vector<int> ans = node2->getOutput();
        std::cout << "the ans of node2:" << std::endl;
        for (int i = 0; i < ans.size(); i++)
        {
            std::cout << ans[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}