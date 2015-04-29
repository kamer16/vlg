#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;
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

int main(int argc, char* argv[]) {
    std::fstream fs;
    fs.open (argv[1], std::fstream::in);
    unsigned nb_states;
    unsigned nb_tr = 0;
    fs >> nb_states;
    vector<node> nodes(nb_states);
    assert(nodes.size() == nb_states);

    for (unsigned i = 0; i < nb_states; ++i) {
        unsigned idx;
        unsigned deg;
        fs >> idx >> deg;
        nodes[idx].deg = deg;
        nb_tr += deg;
    }
    assert(nb_tr % 2 == 0);
    vector<transition> transitions;
    transitions.reserve(1 + nb_tr);
    // Push sentinell transition
    transitions.emplace_back(0, 0);
    unsigned src;
    unsigned dst;
    fs >> src >> dst;
    while (!fs.eof()) {
        {
            transitions.emplace_back(0, dst);
            transition_t prev = nodes[src].last;
            nodes[src].last = transitions.size() - 1;
            if (!nodes[src].first) {
                nodes[src].first = nodes[src].last;
            } else if (prev == nodes[src].first) {
                // Handles when first == last and first != last
                transitions[prev].next = nodes[src].last;
            }
        }
        {
            transitions.emplace_back(0, src);
            transition_t prev = nodes[dst].last;
            nodes[dst].last = transitions.size() - 1;
            if (!nodes[dst].first) {
                nodes[dst].first = nodes[dst].last;
            } else { //if (prev == nodes[dst].first) {
                // Handles when first == last and first != last
                transitions[prev].next = nodes[dst].last;
            }
        }
        fs >> src >> dst;
    }
    assert(transitions.size() == 1 + nb_tr);


    for (unsigned i = 0; i < nodes.size(); ++i) {
        cout << "node: " << i << endl;
        struct node& n = nodes[i];
        transition_t t = n.first;
        while (t) {
            cout << transitions[t].dst << endl;
            t = transitions[t].next;
        }
    }
}
