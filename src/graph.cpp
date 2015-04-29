#include <cassert>
#include <iostream>

#include "graph.hpp"

using namespace std;

graph::graph(fstream& fs) {
    unsigned nb_states;
    unsigned nb_tr = 0;
    fs >> nb_states;
    nodes = nodes_t(nb_states);
    assert(nodes.size() == nb_states);

    for (unsigned i = 0; i < nb_states; ++i) {
        unsigned idx;
        unsigned deg;
        fs >> idx >> deg;
        nodes[idx].deg = deg;
        nb_tr += deg;
    }
    transitions.reserve(1 + nb_tr);
    // Push sentinell transition which will act as nullptr
    transitions.emplace_back(0, 0);
    unsigned src, dst;
    fs >> src >> dst;
    while (!fs.eof()) {
        add_transition(src, dst);
        add_transition(dst, src);
        fs >> src >> dst;
    }
    assert(transitions.size() == 1 + nb_tr);
}

void graph::print() {
    for (unsigned i = 0; i < nodes.size(); ++i) {
        cout << "node: " << i << endl;
        const node& n = nodes[i];
        transition_t t = n.first;
        while (t) {
            cout << transitions[t].dst << endl;
            t = transitions[t].next;
        }
    }
}

void graph::add_transition(transition_t src, transition_t dst) {
    transition_t succ = nodes[dst].first;
    transitions.emplace_back(succ, src);
    nodes[dst].first = static_cast<unsigned>(transitions.size() - 1);
}
