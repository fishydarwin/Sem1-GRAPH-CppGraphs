//
// Created by Rares Bozga on 28.03.2023.
//

#include "ui.h"
#include "../graphworker/graphtraverser.h"
#include "../graph/undirected/undirectedgraph.h"
#include <iostream>
#include <vector>
#include <set>

ui::ui() {
    this->graph = new Graph();
}

void ui::print_all_commands() {
    std::cout << std::endl << "Graphs" << std::endl;
    std::cout << "read (filename) [undirected?] - Stores in memory the graph from a saved file "
                 "(as generated by 'write' command). Optionally, load in undirected mode." << std::endl;
    std::cout << "write (filename) - Writes graph to a file "
                 "(CAREFUL: will overwrite the file!)" << std::endl;
    std::cout << "random (vertices) (edges) - Randomize a graph-> (NOTE: might take a while)" << std::endl;
    std::cout << "modify addV/remV (index) || addE (from) (to) (cost) || remE (from) (to) || modE (from) (to) (cost)"
                 "- Modifies graph->"
    << std::endl;
    std::cout << "peek isV (index) || isE (from) (to) || costOf (from) (to) || in || out || edgeCost "
                 "|| vIn (to) || vOut (from) || all || degVIn (to) || degVOut (from) - "
                 "Peeks (safely) into graph data."
    << std::endl;
    std::cout << "print - Print the entire parsed graph (NOTE: might take a while)" << std::endl;
    std::cout << "search (bfs connComps/mtxMul lowCostWalk) - Search for properties in "
                 "a graph using a certain algorithm." << std::endl;
    std::cout << std::endl;
    std::cout << "exit - See you later!" << std::endl;
}

void ui::parse_args(const std::string& raw_command, std::string *into_where) {
    int index = 0;

    std::string delimiter = " ";
    unsigned long start, end = -1 * delimiter.size();
    do {
        start = end + delimiter.size();
        end = raw_command.find(delimiter, start);
        std::string token = raw_command.substr(start, end - start);
        if (token.length() > 0) into_where[index++] = token;
    } while (end != -1);
}

// COMMAND IMPLEMENTATION

std::string ui::read_command(std::string *args) {
    const clock_t begin_time = clock(); // track time
    if (args[2] == "undirected")
        graph = new UndirectedGraph();
    if (graph->fromFile(args[1])) { // load

        float end_time = float(clock() - begin_time) /  CLOCKS_PER_SEC;
        return "Successfully opened file in " + std::to_string(end_time) + "s.";

    } else {
        return "Failed to open file. Did you specify a valid file?";
    }
}

std::string ui::write_command(std::string *args) {
    if (graph->toFile(args[1])) {
        return "Successfully wrote to file.";
    } else {
        return "Failed to write to file. Is this file protected?";
    }
}

std::string ui::random_command(std::string *args) {
    int n = stoi(args[1]);
    int m = stoi(args[2]);

    for (int i = 0; i < n; i++) graph->addVertex(i);

    unsigned long max_possible = n * n;

    int edges = 0;
    while (edges < m && edges < max_possible) {
        int from = (int) (random() % n);
        int to = (int) (random() % n);
        int cost = (int) (random() % 1001);
        if (!graph->isEdge(from, to)) {
            graph->addEdge(from, to, cost);
            edges++;
        }
    }

    if (edges < m) std::cout << "WARN: Could not fit " << m << " edges in a graph with " << n << " vertices!"
    << std::endl;
    return "Generated random graph->";
}


std::string ui::modify_command(std::string *args) {
    if (args[1] == "addV") {
        int v = stoi(args[2]);
        if (graph->addVertex(v)) {
            return "Added vertex!";
        } else {
            return "This vertex already exists!";
        }
    } else if (args[1] == "remV") {
        int v = stoi(args[2]);
        if (graph->removeVertex(v)) {
            return "Removed vertex!";
        } else {
            return "This vertex does not yet exist!";
        }
    } else if (args[1] == "addE") {
        int from = stoi(args[2]);
        int to = stoi(args[3]);
        int cost = stoi(args[4]);
        if (graph->addEdge(from, to, cost)) {
            return "Added edge successfully!";
        } else {
            return "This edge already exists!";
        }
    } else if (args[1] == "remE") {
        int from = stoi(args[2]);
        int to = stoi(args[3]);
        if (graph->removeEdge(from, to)) {
            return "Removed edge successfully!";
        } else {
            return "This edge does not yet exist!";
        }
    } else if (args[1] == "modE") {
        int from = stoi(args[2]);
        int to = stoi(args[3]);
        int cost = stoi(args[4]);
        if (graph->isEdge(from, to)) {
            graph->removeEdge(from, to);
            graph->addEdge(from, to, cost);
            return "Modified edge successfully!";
        } else {
            return "This edge does not yet exist!";
        }
    }
    return "Invalid use. Please try again";
}

std::string ui::peek_command(std::string *args) {
    if (args[1] == "isV") {
        int v = stoi(args[2]);
        return graph->isVertex(v) ? "True" : "False";
    } else if (args[1] == "isE") {
        int from = stoi(args[2]);
        int to = stoi(args[3]);
        return graph->isEdge(from, to) ? "True" : "False";
    } else if (args[1] == "costOf") {
        int from = stoi(args[2]);
        int to = stoi(args[3]);
        return graph->isEdge(from, to) ? "Cost: " + std::to_string(graph->getCost(from, to)) : "Not an edge.";
    } else if (args[1] == "vIn") {
        int to = stoi(args[2]);
        std::cout << "Inbound of " << to << std::endl;
        for (int vertex : graph->getVerticesIn(to)) {
            std::cout << vertex << " -> " << to << " " << graph->getCost(vertex, to) << std::endl;
        }
        return "Printed inbound data for a vertex.";
    } else if (args[1] == "vOut") {
        int from = stoi(args[2]);
        std::cout << "Outbound of " << from << std::endl;
        for (int vertex : graph->getVerticesOut(from)) {
            std::cout << from << " -> " << vertex << " " << graph->getCost(from, vertex) << std::endl;
        }
        return "Printed inbound data for a vertex.";
    } else if (args[1] == "in") {
        GraphIterator iter = graph->iterator();
        iter.first();
        while (iter.valid()) {
            int vertex = iter.getCurrent();
            std::cout << vertex << ": [ ";
            for (int in: graph->getVerticesIn(vertex)) {
                std::cout << in << " ";
            }
            std::cout << "] " << std::endl;
            iter.next();
        }
        return "Printed vertex inbound data.";
    } else if (args[1] == "out") {
        GraphIterator iter = graph->iterator();
        iter.first();
        while (iter.valid()) {
            int vertex = iter.getCurrent();
            std::cout << vertex << ": [ ";
            for (int out: graph->getVerticesOut(vertex)) {
                std::cout << out << " ";
            }
            std::cout << "] " << std::endl;
            iter.next();
        }
        return "Printed vertex outbound data.";
    } else if (args[1] == "edgeCost") {
        GraphIterator iter = graph->iterator();
        iter.first();
        while (iter.valid()) {
            int vertex = iter.getCurrent();
            for (int out: graph->getVerticesOut(vertex)) {
                std::cout << "<" << vertex << ", " << out << "> " << graph->getCost(vertex, out);
                std::cout << std::endl;
            }
            iter.next();
        }
        return "Printed edge cost data.";
    } else if (args[1] == "all") {
        GraphIterator iter = graph->iterator();
        // in
        std::cout << "vertexIn:" << std::endl;
        iter.first();
        while (iter.valid()) {
            int vertex = iter.getCurrent();
            std::cout << vertex << ": [ ";
            for (int in: graph->getVerticesIn(vertex)) {
                std::cout << in << " ";
            }
            std::cout << "] " << std::endl;
            iter.next();
        }
        // out
        iter.first();
        std::cout << std::endl << "vertexOut:" << std::endl;
        while (iter.valid()) {
            int vertex = iter.getCurrent();
            std::cout << vertex << ": [ ";
            for (int out: graph->getVerticesOut(vertex)) {
                std::cout << out << " ";
            }
            std::cout << "] " << std::endl;
            iter.next();
        }
        // out
        std::cout << std::endl << "edgeCost:" << std::endl;
        iter.first();
        while (iter.valid()) {
            int vertex = iter.getCurrent();
            for (int out: graph->getVerticesOut(vertex)) {
                std::cout << "<" << vertex << ", " << out << "> " << graph->getCost(vertex, out);
                std::cout << std::endl;
            }
            iter.next();
        }
        return "Printed all data.";
    } else if (args[1] == "degVIn") {
        int to = stoi(args[2]);
        return "Degree In: " + std::to_string(graph->getVerticesIn(to).size());
    } else if (args[1] == "degVOut") {
        int from = stoi(args[2]);
        return "Degree Out: " + std::to_string(graph->getVerticesOut(from).size());
    }
    return "Invalid use. Please try again";
}

std::string ui::print_command() {
    graph->print();
    return "Done printing graph->";
}

std::string ui::search_command(std::string *args) {
    std::string algo = "none";
    if (args[1] == "bfs") algo = "bfs";
    else if (args[1] == "mtxMul") algo = "mtxMul";
    if (algo == "none") return "Unknown algorithm.";

    if (args[2] == "connComps") {
        GraphTraverser graphTraverser = GraphTraverser(*graph);

        bool save = false;
        std::string saveDir;

        std::cout << "Save resulting connected components (yes/no)? ";
        std::string answer;
        getline(std::cin, answer);
        if (answer == "yes") {
            save = true;
            std::cout << "Filenames structure to save at (e.g. 'components' => components1.txt etc.): ";
            getline(std::cin, saveDir);
        }

        if (algo == "bfs") {

            std::set<int> foundAll = std::set<int>();

            GraphIterator iter = graph->iterator();
            iter.first();
            int i = 1;
            while (iter.valid()) {
                int vertex = iter.getCurrent();
                if (foundAll.find(vertex) != foundAll.end()) { iter.next(); continue; } // skip found...

                auto found = graphTraverser.breadthFirstSearch(vertex); // grab via bfs
                Graph component; // required for correct storage, actually
                std::cout << "Component #" << i << " [";
                for (const auto &mapPair : std::get<1>(found)) {
                    auto who = mapPair.first;
                    foundAll.insert(who); // fast cuz we're printing too!
                    std::cout << " " << who;

                    // also generate the graph object in memory
                    component.addVertex(who);
                    for (int in : graph->getVerticesIn(who))
                        component.addEdge(in, who, graph->getCost(in, who));
                    for (int out : graph->getVerticesIn(who))
                        component.addEdge(who, out, graph->getCost(who, out));

                }
                std::cout << " ]" << std::endl;

                // save if asked
                if (save) component.toFile(saveDir + std::to_string(i) + ".txt");

                i++;
                iter.next();
            }
            std::cout << std::endl;

            return "Printed all connected components.";
        } else {
            return "Unimplemented operation.";
        }
    }
    else if (args[2] == "lowCostWalk") {

        GraphTraverser graphTraverser = GraphTraverser(*graph);

        int x, y;
        std::cout << "Please enter the start and target vertices:" << std::endl;
        std::cin >> x >> y;

        if (algo == "mtxMul") {

            auto result = graphTraverser.allPairsShortestPath(true, x, y);

            if (result.second.empty())
                std::cout << "No path was found from x = " << x << " to y = " << y << "." << std::endl;
            else {
                if (result.second[0] == -1) {
                    std::cout << "There is a negative cycle in this graph; a path could not be found." << std::endl;
                } else {
                    std::cout << "Found minimum cost path: " << std::endl;
                    for (int node : result.second) {
                        std::cout << node << " ";
                    }
                    std::cout << std::endl;
                    std::cout << "Total cost of path: " << result.first[x][y] << std::endl;
                }
            }

            std::cout << std::endl;
            return "Completed operation.";

        }

    }

    return "Invalid use. Please try again";
}

// MENU

void ui::run() {

    while (true) {

        print_all_commands();

        std::string command;
        getline(std::cin, command);

        std::string args[100];
        parse_args(command, args);

        if (args[0] == "read") {
            this->graph = new Graph();
            std::cout << read_command(args) << std::endl;
        }
        else if (args[0] == "write") {
            std::cout << write_command(args) << std::endl;
        }
        else if (args[0] == "random") {
            this->graph = new Graph();
            std::cout << random_command(args) << std::endl;
        }
        else if (args[0] == "modify") {
            std::cout << modify_command(args) << std::endl;
        }
        else if (args[0] == "peek") {
            std::cout << peek_command(args) << std::endl;
        }
        else if (args[0] == "print") {
            std::cout << print_command() << std::endl;
        }
        else if (args[0] == "search") {
            const clock_t begin_time = clock(); // track time
            std::cout << search_command(args) << std::endl;
            float end_time = float(clock() - begin_time) /  CLOCKS_PER_SEC;
            std::cout << "Operation done in " + std::to_string(end_time) + "s." << std::endl;
        }
        else if (args[0] == "exit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }

    }

}
