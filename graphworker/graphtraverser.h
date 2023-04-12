//
// Created by Rares Bozga on 08.04.2023.
//

#pragma once

#include <map>
#include <tuple>
#include <vector>
#include "../graph/graph.h"

class GraphTraverser {

private:
    Graph graph;

public:
    explicit GraphTraverser(Graph graph);

    std::tuple<std::map<int, int>, std::map<int, std::vector<int>>, std::map<int, int>>
        breadthFirstSearch(int startNode);
};
