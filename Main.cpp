#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <string>
using namespace std;

int main() {
    // Read one line at a time, max 20
    int n = 0;
    string line;
    vector<array<int, 3>> trikits;
    // Get one array (3 integers) per line until EOF (ctrl+Z + Enter)
    while (getline(cin, line) && n < 20) {
        istringstream iss(line);
        array<int, 3> trikit;

        if ((iss >> trikit[0] >> trikit[1] >> trikit[2]) && iss.eof()) {
            trikits.push_back(trikit);
            n++;
        } else {
            continue; // Skip lines that don't contain exactly 3 integers
        }
    }

}