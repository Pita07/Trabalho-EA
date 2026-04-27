#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <numeric>
#include <numeric>

using namespace std;

// Represents a trikit with numbers at its corners in clockwise order [cite: 3, 4]
struct Trikit {
    int corners[3]; 
};

// Coordinate system for the triangular grid
// Up-pointing: (r + c) is even; Down-pointing: (r + c) is odd
struct Position {
    int row, col;
    bool operator<(const Position& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

int N;
vector<Trikit> pieces;
map<Position, vector<int>> board;
set<Position> frontier;
int best_score = 0;

/**
 * Validates side-to-side corner matches and calculates points [cite: 12, 44]
 * Corner Mapping (Up): 0:Top, 1:Bottom-Right, 2:Bottom-Left
 * Corner Mapping (Down): 0:Bottom, 1:Top-Left, 2:Top-Right
 */
pair<int, int> get_placement_results(Position position, const vector<int>& piece_values) {
    bool is_up = ((position.row + position.col) % 2 == 0);
    int points = 0;
    int matches = 0;

    auto check = [&](Position neighbor, int my_corner1, int neighbor_corner1, int my_corner2, int neighbor_corner2) {
        if (board.count(neighbor)) {
            const auto& neighbor_values = board[neighbor];
            // Match happens when two tiles share a side with the same two numbers [cite: 9]
            if (piece_values[my_corner1] == neighbor_values[neighbor_corner1] && piece_values[my_corner2] == neighbor_values[neighbor_corner2]) {
                points += piece_values[my_corner1] + piece_values[my_corner2]; // Points are the sum of matching numbers [cite: 12]
                matches++;
                return true;
            }
            return false; // Physical mismatch [cite: 15]
        }
        return true; 
    };

    if (is_up) {
        if (!check({position.row, position.col + 1}, 0, 1, 1, 0)) return {-1, 0}; // Right (Down nb)
        if (!check({position.row, position.col - 1}, 0, 2, 2, 0)) return {-1, 0}; // Left (Down nb)
        if (!check({position.row - 1, position.col}, 1, 2, 2, 1)) return {-1, 0}; // Bottom (Down nb)
    } else {
        if (!check({position.row, position.col - 1}, 0, 1, 1, 0)) return {-1, 0}; // Left (Up nb)
        if (!check({position.row, position.col + 1}, 0, 2, 2, 0)) return {-1, 0}; // Right (Up nb)
        if (!check({position.row + 1, position.col}, 1, 2, 2, 1)) return {-1, 0}; // Top (Up nb)
    }
    return {points, matches};
}

/**
 * Explores possible placements using a frontier of adjacent empty slots [cite: 19]
 */
void search(int used_mask, int current_score) {
    best_score = max(best_score, current_score);

    // Prune: stop if frontier gets too large
    if (frontier.size() > 10) return;

    for (int piece_index = 0; piece_index < N; ++piece_index) {
        if (!(used_mask & (1 << piece_index))) {
            vector<Position> current_frontier(frontier.begin(), frontier.end());
            for (Position pos : current_frontier) {
                for (int rotation = 0; rotation < 3; ++rotation) {
                    vector<int> rotated_piece = {pieces[piece_index].corners[rotation], pieces[piece_index].corners[(rotation + 1) % 3], pieces[piece_index].corners[(rotation + 2) % 3]};
                    auto placement_result = get_placement_results(pos, rotated_piece);
                    
                    // Piece must match at least one side already on table [cite: 8]
                    if (placement_result.second > 0) {
                        board[pos] = rotated_piece;
                        frontier.erase(pos);
                        
                        bool is_up = ((pos.row + pos.col) % 2 == 0);
                        vector<Position> neighbors;
                        if (is_up) neighbors = {{pos.row, pos.col + 1}, {pos.row, pos.col - 1}, {pos.row - 1, pos.col}};
                        else neighbors = {{pos.row, pos.col - 1}, {pos.row, pos.col + 1}, {pos.row + 1, pos.col}};
                        
                        set<Position> newly_added;
                        for (auto& neighbor : neighbors) {
                            if (!board.count(neighbor) && !frontier.count(neighbor)) {
                                frontier.insert(neighbor);
                                newly_added.insert(neighbor);
                            }
                        }

                        search(used_mask | (1 << piece_index), current_score + placement_result.first);
                        
                        // Backtrack
                        for (auto& neighbor : newly_added) frontier.erase(neighbor);
                        frontier.insert(pos);
                        board.erase(pos);
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
        Trikit piece;
        if (line.find(' ') == string::npos && line.length() >= 3) {
            for(int j = 0; j < 3; ++j) piece.corners[j] = line[j] - '0';
        } else {
            stringstream ss(line);
            ss >> piece.corners[0] >> piece.corners[1] >> piece.corners[2];
        }
        pieces.push_back(piece);
    }
    
    N = pieces.size();
    for (int starting_piece = 0; starting_piece < N; ++starting_piece) {
        // Place the first trikit freely [cite: 7, 18]
        board[{0, 0}] = {pieces[starting_piece].corners[0], pieces[starting_piece].corners[1], pieces[starting_piece].corners[2]};
        frontier = {{0, 1}, {0, -1}, {-1, 0}};
        search(1 << starting_piece, 0);
        board.clear();
        frontier.clear();
    }
    cout << best_score << endl;
    return 0;
}