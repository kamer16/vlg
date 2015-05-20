#include <algorithm>
#include <cassert>
#include <iostream>
#include <deque>

#include "graph.hpp"

using namespace std;

vector<node_t> graph::sort_by_deg() {
    vector<node_t> res;
    res.reserve(nb_nodes());
    for (unsigned i = 0; i < nb_nodes(); ++i) {
        res.push_back(i);
    }
    sort(res.begin(), res.end(),
         [&](node_t l, node_t r) -> bool {
             return nodes[l].deg > nodes[r].deg;
         });
    return res;
}

void graph::reorder(std::ofstream& os) {
    // We want to start on node with lowest degree
    auto degs = sort_by_deg();
    deque<unsigned> todo;
    size_t start = nodes.size() - 1;
    while (scc_id_of(degs[start]) != max_scc_idx)
      --start;
    todo.emplace_back(degs[start]);
    vector<node_t> map(nodes.size(), -1U);
    node_t m = 1;
    // New value of node at degs[start] is 0
    map[degs[start]] = 0;
    while (!todo.empty()) {
        node_t id = todo.front();
        todo.pop_front();
        const node& src = nodes[id];
        for (unsigned t = 0; t < src.deg; ++t) {
            node_t dst = transitions[t + src.first].dst;
            if (map[dst] == -1U) {
                todo.emplace_back(dst);
                map[dst] = m++;
          }
        }
    }
    assert(m == scc_size[max_scc_idx]);
    os << m << endl;
    for (unsigned i = 0; i < map.size(); ++i) {
        if (map[i] == -1U)
          continue;
        os << map[i] << " " << nodes[i].deg << endl;
    }
    for (unsigned i = 0; i < map.size(); ++i) {
        if (map[i] == -1U)
          continue;
        const node& n = nodes[i];
        // Make sure to not print twice transition so mark each node
        for (unsigned t = 0; t < n.deg; ++t) {
            node_t dst = transitions[t + n.first].dst;
            node_t src = i;
            if (map[dst] != -1U) {
                os << map[src] << " " << map[dst] << endl;
            }
        }
        map[i] = -1U;
    }
}

void graph::bfs_spanning_tree(node_t start) {
    deque<unsigned> todo;
    todo.emplace_back(start);
    spanning_tree.assign(nodes.size(), -1U);
    tree_deg.assign(nodes.size(), -1U);
    spanning_tree[start] = start;
    tree_deg[start] = 0;
    while (!todo.empty()) {
        node_t id = todo.front();
        todo.pop_front();
        const node& src = nodes[id];
        for (unsigned t = 0; t < src.deg; ++t) {
            node_t dst = transitions[t + src.first].dst;
            if (spanning_tree[dst] == -1U) {
                todo.emplace_back(dst);
                spanning_tree[dst] = id;
                tree_deg[dst] = 0;
                tree_deg[id]++;
          }
        }
    }
}

unsigned graph::trivial_upper_bound(node_t start) {
    bfs_spanning_tree(start);
    deque<unsigned> todo;
    for (unsigned idx = 0; idx < tree_deg.size(); ++idx) {
        if (tree_deg[idx] == 0) // Push Leaves
            todo.push_back(idx);
    }
    node_t id = todo.front();
    todo.pop_front();

    // Instead of recursive function, use iterative version and use multiple
    // vectors wich replace our stack.
    vector<unsigned> heights(nodes.size(), 0);
    vector<unsigned> subtree_max1_height(nodes.size(), 0);
    vector<unsigned> subtree_max2_height(nodes.size(), 0);
    vector<unsigned> subtree_max_dist(nodes.size(), 0);
    const auto& t = spanning_tree;
    while (id != start) {
        heights[t[id]] = max(heights[t[id]], heights[id] + 1);
        if (heights[id] + 1 > subtree_max1_height[t[id]]) {
            subtree_max2_height[t[id]] = subtree_max1_height[t[id]];
            subtree_max1_height[t[id]] = heights[id] + 1;
        } else if (heights[id] + 1 > subtree_max2_height[t[id]]) {
            subtree_max2_height[t[id]] = heights[id] + 1;
        }
        subtree_max_dist[t[id]] = max(subtree_max_dist[t[id]],
                                      subtree_max_dist[id]);
        subtree_max_dist[t[id]] = max(subtree_max_dist[t[id]],
                                      subtree_max1_height[t[id]] +
                                      subtree_max2_height[t[id]]);
        tree_deg[t[id]]--;
        if (tree_deg[t[id]] == 0) {
            todo.push_back(t[id]);
        }
        id = todo.front();
        todo.pop_front();
    }
    return subtree_max_dist[start];
}

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
    max_scc_idx = res;
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

unsigned graph::double_sweep_lower_bound(node_t start) {
    auto p = eccentricity(start);
    return eccentricity(p.second).first;
}

pair<unsigned, node_t> graph::eccentricity(node_t start) {
    unsigned height = 0;
    node_t res = start;
    vector<bool> seen(nodes.size(), false);
    deque<unsigned> todo;
    todo.emplace_back(start);
    while (!todo.empty()) {
        ++height;
        res = todo.front();
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
    return { height - 1, res };
}

graph::graph(fstream& fs) {
    unsigned nb_states;
    unsigned nb_tr = 0;
    fs >> nb_states;
    nodes = nodes_t(nb_states, node(0, 0));
    //nodes.reserve(nb_states);

    for (unsigned i = 0; i < nb_states; ++i) {
        unsigned idx;
        unsigned deg;
        fs >> idx >> deg;
        nodes[idx] = node(deg, nb_tr);
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
