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
    cout << "Number of sccs: " << g.scc_count() << " with "
         << g.scc_nb_nodes(max_scc_idx) << " maximum nodes" << endl;

    for (unsigned i = 0; i < 10; ++i) {
      // Only use nodes within the giant scc
      node_t id = static_cast<unsigned>(rand()) % g.nb_nodes();
      while (g.scc_id_of(id) != max_scc_idx)
          id = static_cast<unsigned>(rand()) % g.nb_nodes();
      //cout << g.eccentricity(id).first << endl;
      cout << g.double_sweep_lower_bound(id) << endl;
    }
    return 0;
}
