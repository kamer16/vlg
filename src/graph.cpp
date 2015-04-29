#include <cassert>
#include <iostream>
#include <deque>

#include "graph.hpp"

using namespace std;

void graph::scc_bfs(node_t start) {
    deque<unsigned> todo;
    todo.emplace_back(start);
    scc_size.push_back(1);
    scc[start] = static_cast<unsigned>(scc_size.size() - 1);
    while (!todo.empty()) {
        node_t id = todo.front();
        todo.pop_front();
        const node& src = nodes[id];
        for (unsigned t = 0; t < src.deg; ++t) {
          node_t dst = transitions[t + src.first].dst;
          if (scc[dst] == -1U) {
            todo.emplace_back(dst);
            scc[dst] = static_cast<unsigned>(scc_size.size() - 1);
            scc_size[scc_size.size() - 1]++;
          }
        }
    }
}

unsigned graph::compute_scc() {
    scc.assign(nodes.size(), -1U);
    unsigned res = 0;
    for (unsigned i = 0; i < nodes.size(); ++i) {
        if (scc[i] == -1U) {
            scc_bfs(i);
            unsigned idx = static_cast<unsigned>(scc_size.size() - 1);
            res = scc_size[idx] < scc_size[res] ? res : idx;
        }
    }
    return res;
}

void graph::distances(node_t start, vector<unsigned>& seen) {
    deque<unsigned> todo;
    todo.emplace_back(start);
    while (!todo.empty()) {
        // Iterate from 0 to todo.size(), romev those elements and add new
        // ones at back withouth iterating on them
        node_t id = todo.front();
        todo.pop_front();
        const node& src = nodes[id];
        for (unsigned t = 0; t < src.deg; ++t) {
          node_t dst = transitions[t + src.first].dst;
          if (!seen[dst]) {
            todo.emplace_back(dst);
            seen[dst] = seen[id] + 1;
          }
        }
    }
}

unsigned graph::eccentricity(node_t start) {
    unsigned height = 0;
    vector<bool> seen(nodes.size(), false);
    deque<unsigned> todo;
    todo.emplace_back(start);
    while (!todo.empty()) {
        ++height;
        size_t len = todo.size();
        // Iterate from 0 to todo.size(), romev those elements and add new
        // ones at back withouth iterating on them
        for (unsigned i = 0; i < len; ++i) {
            node_t id = todo.front();
            todo.pop_front();
            const node& src = nodes[id];
            for (unsigned t = 0; t < src.deg; ++t) {
                node_t dst = transitions[t + src.first].dst;
                if (!seen[dst]) {
                    todo.emplace_back(dst);
                    seen[dst] = true;
                }
            }
        }
    }
    return height - 1;
}

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
