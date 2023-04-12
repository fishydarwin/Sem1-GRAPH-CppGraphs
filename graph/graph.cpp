//
// Created by Rares Bozga on 15.03.2023.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include "graph.h"

Graph::Graph() {
    this->vertexIn = std::map<int, std::vector<int>>();
    this->vertexOut = std::map<int, std::vector<int>>();
    this->edgeCost = std::map<std::pair<int, int>, int>();
}

// GRAPH
bool Graph::isVertex(int who) {
    return vertexIn.find(who) != vertexIn.end();
}

bool Graph::addVertex(int who) {
    if (isVertex(who)) return false;
    vertexIn[who] = std::vector<int>();
    vertexOut[who] = std::vector<int>();
    return true;
}

bool Graph::isEdge(int from, int to) {
    return edgeCost.find(std::pair<int, int>(from, to)) != edgeCost.end();
}

bool Graph::addEdge(int from, int to, int cost) {
    if (isEdge(from, to)) return false;
    vertexIn[to].push_back(from);
    vertexOut[from].push_back(to);
    edgeCost[std::pair<int, int>(from, to)] = cost;
    return true;
}

int Graph::getCost(int from, int to) {
    if (!isEdge(from, to)) return 0;
    return edgeCost[std::pair<int, int>(from, to)];
}

bool Graph::removeEdge(int from, int to) {
    if (!isEdge(from, to)) return false;
    vertexIn[to].erase(std::find(vertexIn[to].begin(), vertexIn[to].end(), from));
    vertexOut[from].erase(std::find(vertexOut[from].begin(), vertexOut[from].end(), to));
    edgeCost.erase(std::pair<int, int>(from, to));
    return true;
}

bool Graph::removeVertex(int who) {
    if (!isVertex(who)) return false;
    std::vector<int> safeCopyIn = vertexIn[who]; // avoid concurrency issues
    std::vector<int> safeCopyOut = vertexOut[who];
    for (int i : safeCopyIn) {
        removeEdge(i, who);
    }
    for (int i : safeCopyOut)  {
        removeEdge(who, i);
    }
    // clear from map
    vertexIn.erase(who);
    vertexOut.erase(who);
    return true;
}

std::vector<int> Graph::getVerticesOut(int from) {
    if (!isVertex(from)) return {};
    return vertexOut[from];
}

std::vector<int> Graph::getVerticesIn(int to) {
    if (!isVertex(to)) return {};
    return vertexIn[to];
}

GraphIterator Graph::iterator() const {
    return GraphIterator(*this);
}

// FILE INTEROP

bool Graph::fromFile(const std::string &filename) {
    std::ifstream fin(filename);
    if (fin.fail()) return false;

    int n, m;
    fin >> n >> m;

    int vertices = 0;

    while (!fin.eof()) {
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

        this->addEdge(from, to, cost); // add edge
    }

    if (vertices < n) { // cheap hack, too bad
        for (int i = 0; i < n; i++) {
            if (!isVertex(i)) {
                addVertex(i);
                vertices++;
            }
            if (vertices >= n) break;
        }
    }

    return true;
}

bool Graph::toFile(const std::string &filename) {
    std::ofstream fout(filename);

    int totalNodes = (int) vertexIn.size();
    fout << totalNodes << " " << this->edgeCost.size() << std::endl;

    bool hasDiscontinuities = false;
    int foundLostNodes = 0;
    std::set<int> notLost = std::set<int>();

    for (const auto &vertexOutPair : vertexOut) {
        auto vertex = vertexOutPair.first;
        auto outVertices = vertexOutPair.second;

        if (vertex >= totalNodes) hasDiscontinuities = true;

        if (vertex >= totalNodes) { // we ignore if less than n-1 because cheap hack saves us
            if (outVertices.empty() && vertexIn[vertex].empty()) {
                fout << vertex << " " << -1 << std::endl; // edge case - no vIn and vOut
                notLost.insert(vertex);
            }
        } else if (hasDiscontinuities) {
            if (outVertices.empty() && vertexIn[vertex].empty())
                foundLostNodes++;
        }

        // add all associated out edges
        for (int outVertex : outVertices) {
            fout << vertex << " " << outVertex << " " << edgeCost[std::pair<int, int>(vertex, outVertex)]
            << std::endl;
            notLost.insert(vertex);
            notLost.insert(outVertex);
        }
    }

    if (hasDiscontinuities) {
        std::cout << foundLostNodes << std::endl;
        int i = 0;
        bool maybeLucky = true;
        while (notLost.size() + foundLostNodes < totalNodes) {
            if (maybeLucky && isVertex(i)) { i++; foundLostNodes++; continue; }
            maybeLucky = false;
            if (isVertex(i)) {
                fout << i << " " << -1 << std::endl; // recover lost nodes
                foundLostNodes++;
            }
            i++;
        }
    }

    return true;
}

void Graph::print() {
    unsigned long n = this->vertexIn.size();
    std::cout << "Vertices: " << n << ", Edges: " << this->edgeCost.size() << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j : this->vertexOut[i]) {
            std::cout << i << " -> " << j << " " << this->edgeCost[std::pair<int, int>(i, j)] << std::endl;
        }
    }
}

// ITERATOR
GraphIterator::GraphIterator(const Graph &gf) : graph(gf)
{
    currentIndex = 0;
    auto it = graph.vertexIn.begin();
    std::advance(it, 0);
    currentVertex = it->first;
}

void GraphIterator::first() {
    currentIndex = 0;
    auto it = graph.vertexIn.begin();
    std::advance(it, 0);
    currentVertex = it->first;
}

void GraphIterator::next() {
    if (!valid()) throw std::exception();
    auto it = graph.vertexIn.begin();
    std::advance(it, ++currentIndex);
    currentVertex = it->first;
}

int GraphIterator::getCurrent() const {
    if (!valid()) throw std::exception();
    return currentVertex;
}

bool GraphIterator::valid() const {
    return this->currentIndex < graph.vertexIn.size();
}

// TESTS
void testPrintGraph(Graph* graphRef) {
    Graph graph = *graphRef;
    //
    GraphIterator iter = graph.iterator();
    iter.first();
    while (iter.valid()) {
        int vertex = iter.getCurrent();
        std::cout << "Vertex " << vertex << ";" << std::endl;
        //
        std::cout << "   IN: ";
        for (int in : graph.getVerticesIn(vertex)) {
            std::cout << in << " ";
        } std::cout << std::endl;
        //
        std::cout << "   OUT: ";
        for (int out : graph.getVerticesOut(vertex)) {
            std::cout << out << " ";
        } std::cout << std::endl;
        iter.next();
    } std::cout << std::endl;
    //
    iter.first();
    std::cout << "Edges: "<< std::endl;
    while (iter.valid()) {
        int vertex = iter.getCurrent();
        for (int out : graph.getVerticesOut(vertex)) {
            std::cout << vertex << "->" << out << " (cost: " << graph.getCost(vertex, out) << ")" << std::endl;
        }
        iter.next();
    }
}

void testGraph() {
    Graph graph;
    graph.addVertex(0);
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    assert(graph.addVertex(3) == false);
    //
    assert(graph.addEdge(1, 1, 10) == true);
    assert(graph.addEdge(1, 2, 12) == true);
    assert(graph.addEdge(1, 3, 12) == true);
    assert(graph.addEdge(2, 1, 10) == true);
    assert(graph.addEdge(2, 2, 12) == true);
    assert(graph.addEdge(3, 0, 50) == true);
    //
    assert(graph.removeVertex(1) == true);
    //
    GraphIterator iter = graph.iterator();
    iter.first();
    while (iter.valid()) {
        int vertex = iter.getCurrent();
       std::cout << "Vertex " << vertex << ";" << std::endl;
        //
       std::cout << "   IN: ";
        for (int in : graph.getVerticesIn(vertex)) {
           std::cout << in << " ";
        }std::cout << std::endl;
        //
       std::cout << "   OUT: ";
        for (int out : graph.getVerticesOut(vertex)) {
           std::cout << out << " ";
        }std::cout << std::endl;
        iter.next();
    }std::cout << std::endl;
    //
    iter.first();
   std::cout << "Edges: "<< std::endl;
    while (iter.valid()) {
        int vertex = iter.getCurrent();
        for (int out : graph.getVerticesOut(vertex)) {
           std::cout << vertex << "->" << out << " (cost: " << graph.getCost(vertex, out) << ")" << std::endl;
        }
        iter.next();
    }
}

void testGraphFile(const std::string& filename) {
    Graph graph;
    const clock_t begin_time = clock(); // track time

    std::ifstream fin(filename);
    int n, m;
    fin >> n >> m;

    std::cout << "Adding vertices..." << std::endl;
    for (int i = 0; i < n; i++) graph.addVertex(i);
    std::cout << "Done adding vertices!" << std::endl;

    for (int i = 0; i < m; i++) {
        int from, to, cost;
        fin >> from >> to >> cost;
        graph.addEdge(from, to, cost);
        std::cout << "Adding edge: " << from << " " << to << std::endl;
    }

    std::cout << "Loaded graph in memory." << std::endl << std::endl;
    std::cout << "Elapsed time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    //testPrintGraph(&graph);
}
