//
// Created by Rares Bozga on 28.03.2023.
//

#pragma once

#include "../graph/graph.h"

class ui {

private:
    Graph *graph;

    static void print_all_commands();
    static void parse_args(const std::string& raw_command, std::string into_where[100]);

    std::string read_command(std::string args[100]);
    std::string write_command(std::string args[100]);
    std::string random_command(std::string args[100]);
    std::string modify_command(std::string args[100]);
    std::string peek_command(std::string args[100]);
    std::string print_command();
    std::string search_command(std::string args[100]);

public:
    ui();
    void run();

};
