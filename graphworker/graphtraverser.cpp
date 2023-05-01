//
// Created by Rares Bozga on 08.04.2023.
//

#include "graphtraverser.h"

#include <utility>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

GraphTraverser::GraphTraverser(Graph graph) : graph(std::move(graph))
{}

/**
 * Grabs all vertices that can be reached from a start node using BFS.
 * @return A tuple of 3 maps: parent, children, and distances.
 * The first maps the first is a dictionary mapping each accessible vertex to its parent.
 * The second maps each accessible vertex to the list of its children.
 * The third maps mapping each accessible vertex to the distance from startNode to it.
 */
tuple<map<int, int>, map<int, vector<int>>, map<int, int>> GraphTraverser::breadthFirstSearch(int startNode) {
    map<int, int> parent = map<int, int>();
    map<int, vector<int>> children = map<int, vector<int>>();
    map<int, int> distances = map<int, int>();

    // parent of startNode is always null
    children[startNode] = vector<int>();
    distances[startNode] = 0;

    vector<int> queue = vector<int>(); // create a queue
    queue.push_back(startNode); // queue starting node
    map<int, int> visited = map<int, int>(); // visited nodes here
    visited[startNode] = 0; // we say that start node has been visited

    while (!queue.empty()) { // while there's stuff in the queue
        int current = queue[0]; // we pop the first element in the queue
        queue.erase(queue.begin());

        for (const auto &x : graph.getVerticesOut(current)) { // iterate through out vertices
            if (visited.find(x) == visited.end()) { // if not yet visited
                children[x] = vector<int>(); // initialize the children of this unvisited node
                parent[x] = current; // the parent of this node is the one we're iterating
                distances[x] = distances[current] + 1; // the distance from the start node is previous dist + 1

                visited[x] = 0; // mark as visited
                queue.push_back(x); // add this to the queue behind everyone else
                children[current].push_back(x); // for this node, say this is is one of the children of it
            }
        }
    }

    return { parent, children, distances }; // return the magic tuple
}

/**
 * Generates an n x n matrix containing all pairs shortest paths (APSP).
 * This is implemented as slowAPSP, meaning we don't use the cool squaring trick.
 * For small matrix sizes (n < 50) this will be fine (and as such, matrix obj is capped)
 *
 * To retrieve the path, we can define a second array, fk,x,y = the next vertex after x on the walk of cost wk,x,y.
 * When the minimum in the recurrence is reached for some intermediate vertex z, we set f2k,x,y = fk,x,z.
 */
std::pair<matrix, std::vector<int>> GraphTraverser::allPairsShortestPath(bool printVerbose, int x, int y) {
    // Note: D^0 = identity matrix
    int n = graph.nodeCount();
    matrix W = matrix();

    vector<vector<int>> paths;
    paths.resize(n, vector<int>(n, -1)); // initialize paths with -1 as the default value
    for (int i = 0; i < graph.nodeCount(); i++) {
        for (int j : graph.getVerticesOut(i)) {
            paths[i][j] = j;
        }
    }

    for (int i = 0; i < n; i++) // fill up W matrix, aka D^1
        for (int j = 0; j < n; j++) {
            if (i == j) {
                W[i][j] = 0;
                continue;
            }
            if (graph.isEdge(i, j)) {
                W[i][j] = graph.getCost(i, j);
            } else {
                W[i][j] = COST_INFINITY;
            }
        }

    auto D = matrix(W); // make a copy...

    for (int m = 2; m < n; m++) { // perform extending/matrix multiplication...

        if (printVerbose) {
            std::cout << "= = = = = = = = = = = = = =" << std::endl;
            std::cout << "D^" << m - 1 << std::endl;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++)
                    std::cout << D[i][j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
            std::cout << "Paths Matrix" << std::endl;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++)
                    std::cout << paths[i][j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << "= = = = = = = = = = = = = =" << std::endl;
        }

        D = apspExtend(D, W, paths); // D^m = extend(D^m-1, W)

    }

    if (printVerbose) {
        std::cout << "= = = = = = = = = = = = = =" << std::endl;
        std::cout << "D^" << n - 1 << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                std::cout << D[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
        std::cout << "Paths Matrix" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                std::cout << paths[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "= = = = = = = = = = = = = =" << std::endl;
    }

    std::cout << std::endl;
    vector<int> path = getPath(paths, x, y);
    return {D, path}; // return D^n-1
}

matrix GraphTraverser::apspExtend(matrix D, matrix W, vector<vector<int>> &paths) {
    matrix C = D;
    for (int i = 0; i < graph.nodeCount(); i++)
        for (int j = 0; j < graph.nodeCount(); j++) {
            if (i != j) {
                //C[i][j] = COST_INFINITY;
                for (int k = 0; k < graph.nodeCount(); k++) {
                    if (C[i][j] > D[i][k] + W[k][j]) {
                        C[i][j] = D[i][k] + W[k][j];
                        if (paths[i][k] != -1) {
                            paths[i][j] = paths[i][k];
                        }
                    }
                }
            }
        }
    return C;
}

vector<int> GraphTraverser::getPath(vector<vector<int>>& paths, int x, int y) {
    if (paths[x][y] == -1) {
        return {}; // No path exists from x to y
    } else {
        vector<int> path = {x};
        while (x != y) {
            x = paths[x][y];
            path.push_back(x);
        }
        return path;
    }
}