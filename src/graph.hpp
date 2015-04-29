#pragma once

#include <vector>
#include <fstream>

using transition_t = unsigned;

struct transition {
    unsigned next;
    unsigned dst;
    transition(unsigned n, unsigned d)
        : next(n), dst(d)
    {}
};

struct node {
    unsigned deg;
    transition_t first;
};

using nodes_t = std::vector<node>;
using transitions_t = std::vector<transition>;

struct graph {
    graph(std::fstream& fs);
    void print();

private:
    void add_transition(transition_t src, transition_t dst);
    nodes_t nodes;
    transitions_t transitions;
};

