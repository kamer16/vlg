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
public:
    // 2 Lower bound algorithm
    std::pair<unsigned, node_t> eccentricity(node_t start);
    unsigned double_sweep_lower_bound(node_t start);
    // 2 upper bound algorithm
    unsigned trivial_upper_bound(node_t start);
    // TODO max_deg upper_bound

    // returns distance from start to all other nodes
    void distances(node_t start, std::vector<unsigned>& seen);
    // computes the strongly connected componetns
    std::vector<node_t> sort_by_deg();
    unsigned compute_scc();
    graph(std::fstream& fs);
    void print();

    // Access attributes
    size_t scc_count() { return scc_size.size(); }
    unsigned scc_id_of(node_t node) { return scc[node]; }
    size_t scc_nb_nodes(unsigned component) { return scc_size[component]; }
    size_t nb_nodes() const { return nodes.size(); }

private:
    transitions_t transitions;
    nodes_t nodes;

    // Gives the scc id for each node
    std::vector<unsigned> scc;
    // Gives the number of nodes in each scc
    std::vector<unsigned> scc_size;
    // Stored a spannig tree computed using a bfs
    // Each nodes stores its father or itself for the root and -1 for the
    // remaining nodes.
    std::vector<node_t> spanning_tree;
    // Degree for each node of spanning_tree
    std::vector<unsigned> tree_deg;

    // Helper functions
    void add_transition(std::vector<unsigned short>& offset, node_t src,
                        node_t dst);
    void scc_bfs(node_t start);
    // Computes spanning_tree and associated degrees of each node of tree
    void bfs_spanning_tree(node_t start);

};
