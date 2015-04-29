#pragma once

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

using nodes_t = vector<node>;
using transitions_t = vector<transition>;
