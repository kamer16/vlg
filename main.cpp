#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include "graph.hpp"

using namespace std;

void iterate_graph(const nodes_t& nodes, const transitions_t& transitions) {
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

int main(int argc, char* argv[]) {
    std::fstream fs;
    fs.open (argv[1], std::fstream::in);
    unsigned nb_states;
    unsigned nb_tr = 0;
    fs >> nb_states;
    nodes_t nodes(nb_states);
    assert(nodes.size() == nb_states);

    for (unsigned i = 0; i < nb_states; ++i) {
        unsigned idx;
        unsigned deg;
        fs >> idx >> deg;
        nodes[idx].deg = deg;
        nb_tr += deg;
    }
    assert(nb_tr % 2 == 0);
    transitions_t transitions;
    transitions.reserve(1 + nb_tr);
    // Push sentinell transition
    transitions.emplace_back(0, 0);
    unsigned src;
    unsigned dst;
    fs >> src >> dst;
    while (!fs.eof()) {
        {
            transition_t succ = nodes[src].first;
            transitions.emplace_back(succ, dst);
            nodes[src].first = transitions.size() - 1;
        }
        {
            transition_t succ = nodes[dst].first;
            transitions.emplace_back(succ, src);
            nodes[dst].first = transitions.size() - 1;
        }
        fs >> src >> dst;
    }
    assert(transitions.size() == 1 + nb_tr);
}
