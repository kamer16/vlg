#pragma once

#include <vector>
#include <fstream>
#include <utility>

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

class graph {
    graph(std::fstream& fs);
    void print();
    std::pair<unsigned, node_t> eccentricity(node_t start);
    unsigned double_sweep_lower_bound(node_t start);
    // returns distance from start to all other nodes
    void distances(node_t start, std::vector<unsigned>& seen);
    unsigned compute_scc();
    void add_transition(std::vector<unsigned short>& offset, node_t src,
                        node_t dst);
    void scc_bfs(node_t start);

    size_t scc_count() { return scc_size.size(); }
    unsigned scc_id_of(node_t node) { return scc[node]; }
    size_t scc_nb_nodes(unsigned component) { return scc_size[component]; }
    size_t nb_nodes() { return nodes.size(); }

private:
    transitions_t transitions;
    nodes_t nodes;

    // Gives the scc id for each node
    std::vector<unsigned> scc;
    // Gives the number of nodes in each scc
    std::vector<unsigned> scc_size;
    // Stored a spannig tree computed using a bfs
    std::vector<unsigned> spanning_tree;
};
