#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <sstream>
#include <string>
using namespace std;


int match_scores(const array<pair<int, int>, 3>& trikit1, const array<pair<int, int>, 3>& trikit2) {
    int best = 0;
    for (const auto& side1 : trikit1) {
        for (const auto& side2 : trikit2) {

            if ((side1.first == side2.first && side1.second == side2.second) ||
                (side1.first == side2.second && side1.second == side2.first)) {

                int score = side1.first + side1.second;
                if (score > best) {
                    best = score;
                }
            }
        }
    }
    return best;
}

int main() {
    int n = 0;
    string line;
    vector<array<int, 3>> trikits;

    while (getline(cin, line) && n < 20) {
        istringstream iss(line);
        array<int, 3> trikit;

        if ((iss >> trikit[0] >> trikit[1] >> trikit[2]) && iss.eof()) {
            trikits.push_back(trikit);
            n++;
        }
    }

    vector<array<pair<int, int>, 3>> sided_pairs;
    for (const auto& trikit : trikits) {
        array<pair<int, int>, 3> pairs;
        pairs[0] = make_pair(trikit[0], trikit[1]);
        pairs[1] = make_pair(trikit[1], trikit[2]);
        pairs[2] = make_pair(trikit[0], trikit[2]);
        sided_pairs.push_back(pairs);
    }
        
    vector<vector<int>> matches(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matches[i][j] = match_scores(sided_pairs[i], sided_pairs[j]);
            }
        }
    }

    int total = 1 << n;
    vector<int> dp(total, -1);

    dp[0] = 0;

    for (int mask = 0; mask < total; mask++) {
        if (dp[mask] == -1) continue;

        for (int j = 0; j < n; j++) {
            if (!(mask & (1 << j))) {

                if (mask == 0) {
                    dp[1 << j] = max(dp[1 << j], 0);
                } else {

int best_match = 0;

for (int i = 0; i < n; i++) {
    if (mask & (1 << i)) {
        best_match = max(best_match, matches[i][j]);
    }
}

if (best_match > 0) {
    int new_mask = mask | (1 << j);
    dp[new_mask] = max(dp[new_mask], dp[mask] + best_match);
}
                }
            }
        }
    }

    int ans = 0;
    for (int mask = 0; mask < total; mask++) {
        ans = max(ans, dp[mask]);
    }

    cout << ans << endl;
}