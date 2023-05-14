//
// Created by Rares Bozga on 08.04.2023.
//

#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <array>
#include "../graph/graph.h"

const int COST_INFINITY = 100000;
typedef std::array<std::array<int, 20>, 20> matrix;

class GraphTraverser {

private:
    Graph graph;

    matrix apspExtend(matrix D, matrix W, std::vector<std::vector<int>> &paths);
    std::vector<int> apspGetPath(std::vector<std::vector<int>>& paths, int x, int y);

public:
    explicit GraphTraverser(Graph graph);

    std::tuple<std::map<int, int>, std::map<int, std::vector<int>>, std::map<int, int>>
        breadthFirstSearch(int startNode);

    std::pair<matrix, std::vector<int>> allPairsShortestPath(bool printVerbose, int x, int y);

    std::vector<int> topologicalSortPredecessor();
};
