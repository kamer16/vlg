#include <cassert>
#include <iostream>

#include "graph.hpp"

using namespace std;

graph::graph(fstream& fs) {
    unsigned nb_states;
    unsigned nb_tr = 0;
    fs >> nb_states;
    nodes = nodes_t();
    nodes.reserve(nb_states);

    for (unsigned i = 0; i < nb_states; ++i) {
        unsigned idx;
        unsigned deg;
        fs >> idx >> deg;
        nodes.emplace_back(deg, nb_tr);
        // nb_tr also corresponds to the idx in our transitions_t
        nb_tr += deg;
    }

    // An offset table (init to 0) needed when inserting a new transition
    // We assume that a node has at most 2^16-1 degree
    vector<unsigned short> offset(nb_states);
    // Initialize all transitions to 0
    transitions = transitions_t(nb_tr);
    node_t src, dst;
    fs >> src >> dst;
    while (!fs.eof()) {
        add_transition(offset, src, dst);
        add_transition(offset, dst, src);
        fs >> src >> dst;
    }
    assert(transitions.size() == nb_tr);
}

void graph::print() {
    cout << "node: " << endl;
    for (unsigned i = 0; i < nodes.size(); ++i) {
        cout << "node: " << i << endl;
        const node& n = nodes[i];
        for (unsigned t = 0; t < n.deg; ++t) {
            cout << transitions[t + n.first].dst << endl;
        }
    }
}

void graph::add_transition(vector<unsigned short>& offset, node_t src,
                           node_t dst) {
    transition_t idx = offset[src] + nodes[src].first;
    transitions[idx] = dst;
    offset[src]++;
}
