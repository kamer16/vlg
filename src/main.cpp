#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <string.h>

#include "graph.hpp"

using namespace std;

static const char* str[] = { "Trivial lower bound", "Double sweep lower bound",
  "Spanning tree upper bound", "Max degree spanning tree upper bound",
  "Trivial upper bound"};


int main(int argc, char* argv[]) {
    fstream fs;
    if (argc >= 2) {
        fs.open(argv[1], fstream::in);
        if (!fs.good()) {
            cerr << "Bad input file: " << argv[1] << endl;
            return 1;
        }
    } else {
        cerr << "Pass graph path to prgram" << endl;
        cerr << "./main graph [OPTION] [Nb-iteration]" << endl;
        cerr << "OPTIONS:" << endl;
        cerr << "\t-tlb" << endl;
        cerr << "\t-dslb" << endl;
        cerr << "\t-tub" << endl;
        cerr << "\t-rtub" << endl;
        cerr << "\t-hdtub" << endl;
        return 1;
    }
    short opt = 0;
    if (argc >= 3) {
        if (!strncmp(argv[2], "-dslb", 4))
          opt = 1;
        else if (!strncmp(argv[2], "-rtub", 4))
          opt = 2;
        else if (!strncmp(argv[2], "-hdtub", 4))
          opt = 3;
        else if (!strncmp(argv[2], "-tub", 4))
          opt = 4;
        else if (!strncmp(argv[2], "-tlb", 4))
          opt = 9;
        else
          cerr << "Warning: unrecognized option: " << argv[2] << endl;
    }
    unsigned nb_iter = 1;
    if (argc >= 4) {
        nb_iter = static_cast<unsigned>(atol(argv[3]));
    }

    cout << "Reading graph" << endl;
    graph g(fs);
    cout << "Computing sccs" << endl;
    unsigned max_scc_idx = g.compute_scc();
    cout << "Number of sccs: " << g.scc_count() << " with "
         << g.scc_nb_nodes(max_scc_idx) << " maximum nodes" << endl;

    cout << "Executing " << str[opt] << endl;
    cout << "id:\t" << "value" << endl;
    vector<node_t> nodes;
    for (unsigned i = 0; i < nb_iter; ++i) {
        unsigned res = 0;
        // Only use nodes within the giant scc
        node_t id = static_cast<unsigned>(rand()) % g.nb_nodes();
        while (g.scc_id_of(id) != max_scc_idx)
            id = static_cast<unsigned>(rand()) % g.nb_nodes();
        if (opt == 0 || opt == 4) {
            res = g.eccentricity(id).first;
            res = opt == 4 ? res * 2 : res;
        } else if (opt == 1) {
            res = g.double_sweep_lower_bound(id);
        } else if (opt == 2) {
            res = g.trivial_upper_bound(id);
        } else if (opt == 3) {
            if (i == 0) {
                cout << "Sorting graph" << endl;
                nodes = g.sort_by_deg();
            }
            res = g.trivial_upper_bound(nodes[i]);
        }
        cout << i << ":\t" << res << endl;
    }
    return 0;
}
