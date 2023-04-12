//
// Created by Rares Bozga on 15.03.2023.
//

#include <vector>
#include <iostream>
#include <fstream>
#include "undirectedgraph.h"

bool UndirectedGraph::addEdge(int from, int to, int cost) {
    if (isEdge(from, to)) return false;
    //
    vertexIn[to].push_back(from);
    vertexOut[from].push_back(to);
    edgeCost[std::pair<int, int>(from, to)] = cost;
    //
    vertexIn[from].push_back(to);
    vertexOut[to].push_back(from);
    edgeCost[std::pair<int, int>(to, from)] = cost;
    //
    return true;
}

bool UndirectedGraph::removeEdge(int from, int to) {
    if (!isEdge(from, to)) return false;
    //
    vertexIn[to].erase(std::find(vertexIn[to].begin(), vertexIn[to].end(), from));
    vertexOut[from].erase(std::find(vertexOut[from].begin(), vertexOut[from].end(), to));
    edgeCost.erase(std::pair<int, int>(from, to));
    //

    vertexIn[from].erase(std::find(vertexIn[from].begin(), vertexIn[from].end(), to));
    vertexOut[to].erase(std::find(vertexOut[to].begin(), vertexOut[to].end(), from));
    edgeCost.erase(std::pair<int, int>(to, from));
    //
    return true;
}

bool UndirectedGraph::fromFile(const std::string &filename) {
    std::ifstream fin(filename);
    if (fin.fail()) return false;

    int n, m;
    fin >> n >> m;

    int vertices = 0;

    for (int i = 0; i < m; i++) {
        int from, to, cost;
        fin >> from >> to;

        if (!isVertex(from)) {
            addVertex(from); // register
            vertices++;
        }

        if (to < 0) continue; // edge case - no vIn and vOut
        if (!isVertex(to)) {
            addVertex(to); // else register
            vertices++;
        }

        fin >> cost;

        this->addEdge(from, to, cost); // add edge both ways
        this->addEdge(to, from, cost);
    }

    if (vertices < n) { // cheap hack... too bad!
        for (int i = 0; i < n; i++) {
            if (!isVertex(i)) addVertex(i);
        }
    }

    return true;
}
