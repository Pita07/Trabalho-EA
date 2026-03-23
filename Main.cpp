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
            if (side1 == side2) { // Compare the numbers
                int score = side1.first + side1.second; // Add the two numbers for the score
                if (score > best) {
                    best = score;
                }
            }
        }
    }
    return best;
}

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

    // Make side pairs for each trikit
    vector<array<pair<int, int>, 3>> sided_pairs;
    for (const auto& trikit : trikits) {
        array<pair<int, int>, 3> pairs;
        pairs[0] = make_pair(trikit[0], trikit[1]);
        pairs[1] = make_pair(trikit[1], trikit[2]);
        pairs[2] = make_pair(trikit[0], trikit[2]);
        sided_pairs.push_back(pairs);
    }
        
    // Build match Matrix
    vector<vector<int>> matches(n, vector<int>(n, 0)); // n rows, n columns, initialized to 0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matches[i][j] = match_scores(sided_pairs[i], sided_pairs[j]);
            }
        }
    }

    // Solve the problem using a dp mask approach
    // 0 -> unused trikit, 1 -> used trikit, counting from most significant bit to least significant bit
    int total = 1 << n; // Total combinations of trikits used (2^n)

    vector<int> dp(total, -1);

    dp[0] = 0; // Base case: no trikits used, score is 0

    for (int mask = 0; mask < total; mask++) {
        if (dp[mask] == -1) continue;

        for (int j = 0; j < n; j++) {
            if (!(mask & (1 << j))) {

                if (mask == 0) {
                    dp[1 << j] = max(dp[1 << j], 0);
                } else {
                    for (int i = 0; i < n; i++) {
                        if (mask & (1 << i)) {
                            int score = matches[i][j];
                            

                            if (score > 0) {
                                int new_mask = mask | (1 << j);
                                dp[new_mask] = max(dp[new_mask], dp[mask] + score);
                            }
                        }
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
