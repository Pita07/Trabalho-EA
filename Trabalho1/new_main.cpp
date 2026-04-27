#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

int main() {
    vector<vector<pair<int, int>>> tiles;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int a, b, c;
        ss >> a >> b >> c;
        vector<pair<int, int>> sides = {
            {min(a, b), max(a, b)},
            {min(b, c), max(b, c)},
            {min(c, a), max(c, a)}
        };
        tiles.push_back(sides);
    }

    int n = tiles.size();
    if (n == 0) {
        cout << 0 << endl;
        return 0;
    }

    // Build edge list
    vector<tuple<int, int, int>> edges; // {weight, u, v}
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int max_shared = 0;
            for (auto& side_i : tiles[i]) {
                for (auto& side_j : tiles[j]) {
                    if (side_i == side_j) {
                        max_shared = max(max_shared, side_i.first + side_i.second);
                    }
                }
            }
            if (max_shared > 0) {
                edges.push_back({max_shared, i, j});
            }
        }
    }

    // Sort by weight descending
    sort(edges.rbegin(), edges.rend());

    // Greedy maximum matching: each vertex can match at most once
    vector<bool> used(n, false);
    int max_score = 0;

    for (const auto& [w, u, v] : edges) {
        if (!used[u] && !used[v]) {
            used[u] = used[v] = true;
            max_score += w;
        }
    }

    cout << max_score << endl;
    return 0;
}