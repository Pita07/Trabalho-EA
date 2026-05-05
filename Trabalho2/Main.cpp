#include <iostream>
#include <vector>
#include <stack>    
#include <algorithm>
#include <queue>

using namespace std;

class Element {
public:
    int num;
    int low;
    int dfs;
    bool visited;
    bool onStack;
    vector<int> adj;
    vector<int> weights;
    int scc_id;
    
    Element(int val) : num(val), low(-1), dfs(-1), visited(false), onStack(false), scc_id(-1), weights(), adj() {}
};

// Tarjan algorithm
void tarjan(int v, vector<Element>& elements, int& time, stack<int>& st, vector<vector<int>>& sccs) {
    elements[v].dfs = elements[v].low = time++;
    elements[v].visited = true;
    elements[v].onStack = true;
    st.push(v);

    for (int w : elements[v].adj) {
        if (!elements[w].visited) { // dfs has no value
            tarjan(w, elements, time, st, sccs);
            elements[v].low = min(elements[v].low, elements[w].low);
        } else if (elements[w].onStack) {
            elements[v].low = min(elements[v].low, elements[w].dfs);
        }
    }

    // v is the root of an SCC
    if (elements[v].low == elements[v].dfs) {
        vector<int> scc;
        while (true) {
            int w = st.top(); st.pop();
            elements[w].onStack = false;
            elements[w].scc_id = sccs.size(); // tag each node with its SCC id
            scc.push_back(elements[w].num); 
            if (w == v) break;
        }
        sort(scc.begin(), scc.end()); // Sort the SCC like in the example output
        sccs.push_back(scc);
    }
}
 
// Use a modified Bellman-Ford algorithm with a queue to detect negative cycles in the SCC (SPFA)
bool has_Positive_cycle(const vector<int>& scc, vector<Element>& elements, int id) {
    int n = scc.size();

    if (n == 1) {
        int u = scc[0];
        for (int i = 0; i < (int)elements[u].adj.size(); i++)
            if (elements[u].adj[i] == u && elements[u].weights[i] < 0) // negative weight = fuel gain
                return true;
        return false;
    }

    // Map global index to local SCC index
    vector<int> localIdx(elements.size(), -1);
    for (int i = 0; i < n; i++) localIdx[scc[i]] = i;

    vector<long long> dist(n, 0);
    vector<int> cnt(n, 0);
    vector<bool> inQueue(n, true);
    queue<int> q;
    for (int u : scc) q.push(u);

    bool result = false;
    while (!q.empty() && !result) {
        int u = q.front(); q.pop();
        inQueue[localIdx[u]] = false;

        for (int j = 0; j < (int)elements[u].adj.size(); j++) {
            int v = elements[u].adj[j];
            int cost = elements[u].weights[j];
            if (elements[v].scc_id != id) continue; // skip edges outside this SCC

            if (dist[localIdx[v]] > dist[localIdx[u]] + cost) {
                dist[localIdx[v]] = dist[localIdx[u]] + cost;
                cnt[localIdx[v]]++;
                if (cnt[localIdx[v]] >= n) { result = true; break; }
                if (!inQueue[localIdx[v]]) {
                    inQueue[localIdx[v]] = true;
                    q.push(v);
                }
            }
        }
    }

    return result;
}


int main() {
    int P, W; // number of vertices and connections
    cin >> P >> W;
    if (P < 5 || P > 60000 || W < 5 || W > 100000) return 0; // Constraints check
    vector<Element> elements;
    for (int i = 0; i < P; i++) {
        elements.push_back(Element(i));
    }
    for (int i = 0; i < W; i++) {
        int num, adjacent, weight;
        cin >> num >> adjacent >> weight;
        num -= 1; // Adjusting for 0-based index
        adjacent -= 1; // Adjusting for 0-based index
        elements[num].adj.push_back(adjacent);
        elements[num].weights.push_back(weight);       
    }

    // Perform Tarjan's algorithm to find strongly connected components
    stack<int> st;
    vector<vector<int>> sccs;
    int time = 0;
    for (auto& element : elements) {
        if (!element.visited) {
            tarjan(element.num, elements, time, st, sccs);
        }
    }

    // Check each strongly connected component for positive cycles
    vector<vector<int>> positiveSCCs;
    for (int i = 0; i < (int)sccs.size(); i++) {
        if (has_Positive_cycle(sccs[i], elements, i))
            positiveSCCs.push_back(sccs[i]);
    }

    cout << positiveSCCs.size() << "\n";
    for (auto& scc : positiveSCCs) {
        for (int i = 0; i < (int)scc.size(); i++) {
            if (i) cout << " ";
            cout << scc[i] + 1; // Adjusting back to 1-based index for output
        }
        cout << "\n";
    }

    return 0;

    
}