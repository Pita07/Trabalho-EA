#include <iostream>
#include <vector>
#include <stack>    
#include <algorithm>

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

    Element(int val) : num(val), low(-1), dfs(-1), visited(false), onStack(false), weights(), adj() {}
};

// Tarjan alghoritm
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
            scc.push_back(elements[w].num); 
            if (w == v) break;
        }
        sort(scc.begin(), scc.end()); // Sort the SCC like in the example output
        sccs.push_back(scc);
    }
}
 
// Use Bellman-Ford algorithm to check for positive cycles in the strongly connected components
bool has_Positive_cycle(const vector<int>& scc, const vector<Element>& elements) {
    int n = scc.size();

    // Single node: only qualifies if it has a positive self-loop
    if (n == 1) {
        int u = scc[0];
        for (int i = 0; i < (int)elements[u].adj.size(); i++) {
            if (elements[u].adj[i] == u && elements[u].weights[i] > 0)
                return true;
        }
        return false;
    }

    // Initialize distances to 0 (instead of infinity, since we just need cycle detection)
    vector<int> d(n, 0);

    // Map global index to local SCC index
    vector<int> localIdx(elements.size(), -1);
    for (int i = 0; i < n; i++)
        localIdx[scc[i]] = i;

    // Relax |V| - 1 times
    for (int i = 0; i < n - 1; i++) {
        for (int u : scc) {
            int lu = localIdx[u];
            for (int j = 0; j < (int)elements[u].adj.size(); j++) {
                int v = elements[u].adj[j];
                if (localIdx[v] == -1) continue; // outside this SCC
                int lv = localIdx[v];
                int w = elements[u].weights[j];
                if (d[lu] + w < d[lv])
                    d[lv] = d[lu] + w;
            }
        }
    }

    // Check for negative cycle (fuel profit cycle)
    for (int u : scc) {
        int lu = localIdx[u];
        for (int j = 0; j < (int)elements[u].adj.size(); j++) {
            int v = elements[u].adj[j];
            if (localIdx[v] == -1) continue;
            int lv = localIdx[v];
            int w = elements[u].weights[j];
            if (d[lu] + w < d[lv])
                return true;
        }
    }
    return false;
}

int main() {
    int P, W; // number of vertices and connections
    cin >> P >> W;
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

    // Perform Trojan's algorithm to find strongly connected components
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
    for (const auto& scc : sccs) {
        if (has_Positive_cycle(scc, elements)) {
            positiveSCCs.push_back(scc);
        }
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