//
// Created by Rares Bozga on 15.03.2023.
//

#pragma once

#include <map>
#include "../graph.h"

class UndirectedGraph : public Graph {
    friend class GraphIterator;

    public:
    bool addEdge(int from, int to, int cost) override;
    bool removeEdge(int from, int to) override;

    bool toFile(const std::string& filename) override;
    bool fromFile(const std::string& filename) override;
};
