#include <cstdlib>
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
    cout << "Reading graph" << endl;
    graph g(fs);
    cout << "Computing sccs" << endl;
    unsigned max_scc_idx = g.compute_scc();
    cout << "Number of sccs: " << g.scc_size.size() << " with "
         << g.scc_size[max_scc_idx] << " maximum nodes" << endl;

    for (unsigned i = 0; i < 10; ++i) {
      // Only use nodes within the giant scc
      node_t id = static_cast<unsigned>(rand()) % g.nodes.size();
      while (g.scc[id] != max_scc_idx)
          id = static_cast<unsigned>(rand()) % g.nodes.size();
      cout << g.eccentricity(id) << endl;
    }
    return 0;
}
