//
// Created by Rares Bozga on 15.03.2023.
//

#include <vector>
#include <iostream>
#include <fstream>
#include "graph.h"

using namespace std;

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
    vector<int> safeCopyIn = vertexIn[who]; // avoid concurrency issues
    vector<int> safeCopyOut = vertexOut[who];
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

int GraphIterator::getCurrent() {
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
        cout << "Vertex " << vertex << ";" << endl;
        //
        cout << "   IN: ";
        for (int in : graph.getVerticesIn(vertex)) {
            cout << in << " ";
        } cout << endl;
        //
        cout << "   OUT: ";
        for (int out : graph.getVerticesOut(vertex)) {
            cout << out << " ";
        } cout << endl;
        iter.next();
    } cout << endl;
    //
    iter.first();
    cout << "Edges: "<< endl;
    while (iter.valid()) {
        int vertex = iter.getCurrent();
        for (int out : graph.getVerticesOut(vertex)) {
            cout << vertex << "->" << out << " (cost: " << graph.getCost(vertex, out) << ")" << endl;
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
        cout << "Vertex " << vertex << ";" << endl;
        //
        cout << "   IN: ";
        for (int in : graph.getVerticesIn(vertex)) {
            cout << in << " ";
        } cout << endl;
        //
        cout << "   OUT: ";
        for (int out : graph.getVerticesOut(vertex)) {
            cout << out << " ";
        } cout << endl;
        iter.next();
    } cout << endl;
    //
    iter.first();
    cout << "Edges: "<< endl;
    while (iter.valid()) {
        int vertex = iter.getCurrent();
        for (int out : graph.getVerticesOut(vertex)) {
            cout << vertex << "->" << out << " (cost: " << graph.getCost(vertex, out) << ")" << endl;
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

    std::cout << "Adding vertices..." << endl;
    for (int i = 0; i < n; i++) graph.addVertex(i);
    std::cout << "Done adding vertices!" << endl;

    for (int i = 0; i < m; i++) {
        int from, to, cost;
        fin >> from >> to >> cost;
        graph.addEdge(from, to, cost);
        std::cout << "Adding edge: " << from << " " << to << endl;
    }

    std::cout << "Loaded graph in memory." << endl << endl;
    std::cout << "Elapsed time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    //testPrintGraph(&graph);
}
