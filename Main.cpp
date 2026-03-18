#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <string>
using namespace std;

int main() {
    // Read one line at a time; each line must contain exactly 3 integers.
    string line;
    vector<array<int, 3>> trikits;
    // Get one array (3 integers) per line until EOF (ctrl+Z + Enter)
    while (getline(cin, line)) {
        istringstream iss(line);
        array<int, 3> trikit;

        if ((iss >> trikit[0] >> trikit[1] >> trikit[2]) && iss.eof()) {
            trikits.push_back(trikit);
        } else {
            continue; // Skip lines that don't contain exactly 3 integers
        }
    }

}