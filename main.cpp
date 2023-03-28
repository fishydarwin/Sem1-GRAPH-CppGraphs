#include <iostream>
#include <vector>
#include "graph/graph.h"
#include "ui/ui.h"

int main() {

    //testGraph();
    //testGraphFile("../graph1k.txt");
    //testGraphFile("../graph10k.txt");
    //testGraphFile("../graph100k.txt");
    //testGraphFile("../graph1m.txt");

    ui UI = ui();
    UI.run();

    return 0;
}
