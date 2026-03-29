#include <bits/stdc++.h>
using namespace std;

struct Tile {
    int v[3];
};

struct Pos {
    int x, y;
    bool operator<(const Pos& o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
};

struct Placed {
    int c[3];
};

class Game {
private:
    static const int SIZE = 100;
    static const int CENTER = 50;

    Tile tiles[20];
    Placed board[SIZE][SIZE];

    bool occupied[SIZE][SIZE] = {};
    bool blocked[SIZE][SIZE] = {};
    bool used[20] = {};

    int N = 0;
    int max_score = 0;

    bool is_up(int x, int y) {
        return (x + y) % 2 == 0;
    }

    bool can_place(int x, int y, int c[3], int &gain) {
        gain = 0;
        int matches = 0;

        if (is_up(x, y)) {
            if (occupied[x + 1][y]) {
                if (c[0] == board[x + 1][y].c[1] && c[1] == board[x + 1][y].c[0]) {
                    gain += c[0] + c[1]; matches++;
                } else return false;
            }
            if (occupied[x][y + 1]) {
                if (c[1] == board[x][y + 1].c[2] && c[2] == board[x][y + 1].c[1]) {
                    gain += c[1] + c[2]; matches++;
                } else return false;
            }
            if (occupied[x - 1][y]) {
                if (c[2] == board[x - 1][y].c[0] && c[0] == board[x - 1][y].c[2]) {
                    gain += c[2] + c[0]; matches++;
                } else return false;
            }
        } else {
            if (occupied[x - 1][y]) {
                if (c[0] == board[x - 1][y].c[1] && c[1] == board[x - 1][y].c[0]) {
                    gain += c[0] + c[1]; matches++;
                } else return false;
            }
            if (occupied[x][y - 1]) {
                if (c[1] == board[x][y - 1].c[2] && c[2] == board[x][y - 1].c[1]) {
                    gain += c[1] + c[2]; matches++;
                } else return false;
            }
            if (occupied[x + 1][y]) {
                if (c[2] == board[x + 1][y].c[0] && c[0] == board[x + 1][y].c[2]) {
                    gain += c[2] + c[0]; matches++;
                } else return false;
            }
        }

        return matches > 0;
    }

    vector<Pos> get_neighbors(Pos p) {
        vector<Pos> res;
        if (is_up(p.x, p.y)) {
            res = {{p.x + 1, p.y}, {p.x - 1, p.y}, {p.x, p.y + 1}};
        } else {
            res = {{p.x - 1, p.y}, {p.x, p.y - 1}, {p.x + 1, p.y}};
        }
        return res;
    }

    void solve(int score, set<Pos>& open_slots, int start_i) {
        max_score = max(max_score, score);
        if (open_slots.empty()) return;

        Pos p = *open_slots.begin();
        open_slots.erase(open_slots.begin());

        // Option 1: block this position
        blocked[p.x][p.y] = true;
        solve(score, open_slots, start_i);
        blocked[p.x][p.y] = false;

        // Option 2: place tiles
        for (int i = start_i + 1; i < N; i++) {
            if (used[i]) continue;

            int configs[3][3] = {
                {tiles[i].v[0], tiles[i].v[1], tiles[i].v[2]},
                {tiles[i].v[1], tiles[i].v[2], tiles[i].v[0]},
                {tiles[i].v[2], tiles[i].v[0], tiles[i].v[1]}
            };

            for (int k = 0; k < 3; k++) {
                int gain;
                if (!can_place(p.x, p.y, configs[k], gain)) continue;

                used[i] = true;
                occupied[p.x][p.y] = true;
                memcpy(board[p.x][p.y].c, configs[k], sizeof(int) * 3);

                vector<Pos> added;
                for (auto nb : get_neighbors(p)) {
                    if (!occupied[nb.x][nb.y] && !blocked[nb.x][nb.y] &&
                        open_slots.find(nb) == open_slots.end()) {
                        open_slots.insert(nb);
                        added.push_back(nb);
                    }
                }

                solve(score + gain, open_slots, start_i);

                for (auto nb : added) open_slots.erase(nb);
                occupied[p.x][p.y] = false;
                used[i] = false;
            }
        }

        open_slots.insert(p);
    }

public:
    void run() {
        while (N < 20 && scanf("%d %d %d",
               &tiles[N].v[0], &tiles[N].v[1], &tiles[N].v[2]) == 3) {
            N++;
        }

        for (int i = 0; i < N; i++) {
            used[i] = true;
            occupied[CENTER][CENTER] = true;

            memcpy(board[CENTER][CENTER].c, tiles[i].v, sizeof(int) * 3);

            set<Pos> start = {
                {CENTER + 1, CENTER},
                {CENTER - 1, CENTER},
                {CENTER, CENTER + 1}
            };

            solve(0, start, i);

            occupied[CENTER][CENTER] = false;
            used[i] = false;
        }

        printf("%d\n", max_score);
    }
};

int main() {
    Game g;
    g.run();
    return 0;
}