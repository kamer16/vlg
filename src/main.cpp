#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "graph.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    fstream fs;
    if (argc >= 2) {
        fs.open(argv[1], fstream::in);
    } else {
        cerr << "Pass graph path to prgram" << endl;
        return 1;
    }
    graph g(fs);
    for (unsigned i = 0; i < 10; ++i) {
      cout << g.eccentricity(i * 100) << endl;
    }
    vector<unsigned> dists;
    dists.reserve(g.nodes.size());
    for (unsigned i = 0; i < 10; ++i) {
      dists.assign(g.nodes.size(), 0);
      g.distances(i * 100, dists);
      cout << *max_element(dists.begin(), dists.end()) << endl;
    }
    return 0;
}
