#include <fstream>
#include <iostream>

#include "graph.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    std::fstream fs;
    if (argc >= 2) {
        fs.open(argv[1], std::fstream::in);
    } else {
        cerr << "Pass graph path to prgram" << endl;
        return 1;
    }
    graph g(fs);
}
