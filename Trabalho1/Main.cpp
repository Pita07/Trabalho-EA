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
    // Tabuleiro grande para expandir a partir do centro.
    static const int SIZE = 100;
    static const int CENTER = 50;

    Tile tiles[20];
    Placed board[SIZE][SIZE];

    bool occupied[SIZE][SIZE] = {};
    bool blocked[SIZE][SIZE] = {};
    bool used[20] = {};

    int N = 0;
    int max_score = 0;

    bool can_place(int x, int y, int c[3], int &gain) {
        gain = 0;
        int matches = 0;

        // Verifica se a pode ser colocada em (x, y).
        // Se tiver em vizinhos ocupados, todos esses lados teem de coincidir.
        // Sempre que ha coincidencia, soma-se ao valor final
        if ((x + y) % 2 == 0) {
            // Triangulo virado para cima: compara com direita, baixo e esquerda.
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
            // Triangulo virado para baixo: compara com esquerda, cima e direita.
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

        // A jogada so e valida se a peca encaixar em pelo menos um vizinho ja colocado.
        return matches > 0;
    }

    vector<Pos> get_neighbors(Pos p) {
        vector<Pos> res;
        // Cada triangulo tem exatamente tres posicoes adjacentes.
        if ((p.x + p.y) % 2 == 0) {
            res = {{p.x + 1, p.y}, {p.x - 1, p.y}, {p.x, p.y + 1}};
        } else {
            res = {{p.x - 1, p.y}, {p.x, p.y - 1}, {p.x + 1, p.y}};
        }
        return res;
    }

    void solve(int score, set<Pos>& open_slots, int start_i) {
        max_score = max(max_score, score);
        if (open_slots.empty()) return;

        Pos current_slot = *open_slots.begin();
        open_slots.erase(open_slots.begin());

        // Opcao 1: desistir desta posicao e continuar a explorar as restantes.
        blocked[current_slot.x][current_slot.y] = true;
        solve(score, open_slots, start_i);
        blocked[current_slot.x][current_slot.y] = false;

        // Opcao 2: tentar colocar uma peca ainda nao usada nesta posicao.
        for (int i = start_i + 1; i < N; i++) {
            if (used[i]) continue;

            int rotations[3][3] = {
                {tiles[i].v[0], tiles[i].v[1], tiles[i].v[2]},
                {tiles[i].v[1], tiles[i].v[2], tiles[i].v[0]},
                {tiles[i].v[2], tiles[i].v[0], tiles[i].v[1]}
            };

            for (int k = 0; k < 3; k++) {
                int gain = 0;
                if (!can_place(current_slot.x, current_slot.y, rotations[k], gain)) {
                    continue;
                }

                used[i] = true;
                occupied[current_slot.x][current_slot.y] = true;
                memcpy(board[current_slot.x][current_slot.y].c, rotations[k], sizeof(int) * 3);

                vector<Pos> new_open_slots;
                // Cada vizinho vazio passa a ser uma nova posicao candidata.
                for (const Pos& neighbor : get_neighbors(current_slot)) {
                    if (occupied[neighbor.x][neighbor.y]) continue;
                    if (blocked[neighbor.x][neighbor.y]) continue;
                    if (open_slots.find(neighbor) != open_slots.end()) continue;

                    open_slots.insert(neighbor);
                    new_open_slots.push_back(neighbor);
                }

                solve(score + gain, open_slots, start_i);

                for (const Pos& neighbor : new_open_slots) {
                    open_slots.erase(neighbor);
                }
                occupied[current_slot.x][current_slot.y] = false;
                used[i] = false;
            }
        }

        open_slots.insert(current_slot);
    }

public:
    void run() {
        while (N < 20 && scanf("%d %d %d",
               &tiles[N].v[0], &tiles[N].v[1], &tiles[N].v[2]) == 3) {
            N++;
        }

        // Tenta cada peca como peca inicial colocada no centro do tabuleiro.
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