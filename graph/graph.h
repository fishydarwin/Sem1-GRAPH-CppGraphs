//
// Created by Rares Bozga on 15.03.2023.
//

#pragma once

#include <map>

class GraphIterator;

class Graph {
    friend class GraphIterator;
    
    protected:
    std::map<int, std::vector<int>> vertexIn;
    std::map<int, std::vector<int>> vertexOut;
    std::map<std::pair<int, int>, int> edgeCost;

    public:
    Graph();
    bool isVertex(int who);
    bool addVertex(int who);
    bool isEdge(int from, int to);

    virtual bool addEdge(int from, int to, int cost);
    int getCost(int from, int to);

    virtual bool removeEdge(int from, int to);
    bool removeVertex(int who);
    std::vector<int> getVerticesOut(int from);
    std::vector<int> getVerticesIn(int to);
    [[nodiscard]] GraphIterator iterator() const;

    virtual bool fromFile(const std::string& filename);
    bool toFile(const std::string& filename);

    void print();
};

class GraphIterator {
    friend class Graph;

    private:
    const Graph& graph;
    int currentIndex = 0;
    int currentVertex = -1;
    explicit GraphIterator(const Graph& g);

    public:
    void first();
    void next();
    [[nodiscard]] int getCurrent() const;
    [[nodiscard]] bool valid() const;
};

// TESTS
void testPrintGraph();
void testGraph();
void testGraphFile(const std::string& filename);
