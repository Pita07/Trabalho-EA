#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

// Represents a trikit with numbers at its corners in clockwise order [cite: 3, 4]
struct Trikit {
    int v[3]; 
};

// Coordinate system for the triangular grid
// Up-pointing: (r + c) is even; Down-pointing: (r + c) is odd
struct Pos {
    int r, c;
    bool operator<(const Pos& o) const {
        if (r != o.r) return r < o.r;
        return c < o.c;
    }
};

int N;
vector<Trikit> pieces;
map<Pos, vector<int>> board;
int max_glory = 0;

/**
 * Validates side-to-side corner matches and calculates points [cite: 12, 44]
 * Corner Mapping (Up): 0:Top, 1:Bottom-Right, 2:Bottom-Left
 * Corner Mapping (Down): 0:Bottom, 1:Top-Left, 2:Top-Right
 */
pair<int, int> get_placement_results(Pos p, const vector<int>& v) {
    bool is_up = ((p.r + p.c) % 2 == 0);
    int points = 0;
    int matches = 0;

    auto check = [&](Pos nb, int my1, int nb1, int my2, int nb2) {
        if (board.count(nb)) {
            const auto& nv = board[nb];
            // Match happens when two tiles share a side with the same two numbers [cite: 9]
            if (v[my1] == nv[nb1] && v[my2] == nv[nb2]) {
                points += v[my1] + v[my2]; // Points are the sum of matching numbers [cite: 12]
                matches++;
                return true;
            }
            return false; // Physical mismatch [cite: 15]
        }
        return true; 
    };

    if (is_up) {
        if (!check({p.r, p.c + 1}, 0, 1, 1, 0)) return {-1, 0}; // Right (Down nb)
        if (!check({p.r, p.c - 1}, 0, 2, 2, 0)) return {-1, 0}; // Left (Down nb)
        if (!check({p.r - 1, p.c}, 1, 2, 2, 1)) return {-1, 0}; // Bottom (Down nb)
    } else {
        if (!check({p.r, p.c - 1}, 0, 1, 1, 0)) return {-1, 0}; // Left (Up nb)
        if (!check({p.r, p.c + 1}, 0, 2, 2, 0)) return {-1, 0}; // Right (Up nb)
        if (!check({p.r + 1, p.c}, 1, 2, 2, 1)) return {-1, 0}; // Top (Up nb)
    }
    return {points, matches};
}

/**
 * Explores possible placements using a frontier of adjacent empty slots [cite: 19]
 */
void search(int mask, int current_score, set<Pos> frontier) {
    max_glory = max(max_glory, current_score);

    for (int i = 0; i < N; ++i) {
        if (!(mask & (1 << i))) {
            for (Pos p : frontier) {
                for (int rot = 0; rot < 3; ++rot) {
                    vector<int> v = {pieces[i].v[rot], pieces[i].v[(rot + 1) % 3], pieces[i].v[(rot + 2) % 3]};
                    auto res = get_placement_results(p, v);
                    
                    // Piece must match at least one side already on table [cite: 8]
                    if (res.second > 0) {
                        board[p] = v;
                        set<Pos> next_frontier = frontier;
                        next_frontier.erase(p);
                        
                        bool is_up = ((p.r + p.c) % 2 == 0);
                        vector<Pos> nbs;
                        if (is_up) nbs = {{p.r, p.c + 1}, {p.r, p.c - 1}, {p.r - 1, p.c}};
                        else nbs = {{p.r, p.c - 1}, {p.r, p.c + 1}, {p.r + 1, p.c}};
                        
                        for (auto& nb : nbs) if (!board.count(nb)) next_frontier.insert(nb);

                        search(mask | (1 << i), current_score + res.first, next_frontier);
                        board.erase(p);
                    }
                }
            }
        }
    }
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        Trikit t;
        if (line.find(' ') == string::npos && line.length() >= 3) {
            for(int j=0; j<3; ++j) t.v[j] = line[j] - '0';
        } else {
            stringstream ss(line);
            ss >> t.v[0] >> t.v[1] >> t.v[2];
        }
        pieces.push_back(t);
    }
    
    N = pieces.size();
    for (int i = 0; i < N; ++i) {
        // Place the first trikit freely [cite: 7, 18]
        board[{0, 0}] = {pieces[i].v[0], pieces[i].v[1], pieces[i].v[2]};
        set<Pos> initial_frontier = {{0, 1}, {0, -1}, {-1, 0}}; 
        search(1 << i, 0, initial_frontier);
        board.clear();
    }
    cout << max_glory << endl; // Output for example: 20
    return 0;
}