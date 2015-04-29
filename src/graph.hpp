#pragma once

#include <vector>
#include <fstream>

// A transitions id
using transition_t = unsigned;
// A node's id
using node_t = unsigned;

// The content of a transition
struct transition {
    unsigned dst;
    transition() = default;
    transition(unsigned d)
        : dst(d)
    {}
};

struct node {
    unsigned deg;
    transition_t first;
    node(unsigned d, transition_t f)
        : deg(d), first(f)
    {}
};

using nodes_t = std::vector<node>;
using transitions_t = std::vector<transition>;

struct graph {
    graph(std::fstream& fs);
    void print();

private:
    void add_transition(std::vector<unsigned short>& offset, node_t src,
                        node_t dst);
    nodes_t nodes;
    transitions_t transitions;
};

