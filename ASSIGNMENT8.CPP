// title- graph colouring for univercity exam
//name- Ankita Bharkade
// Prn- 123B1F008
#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

vector<vector<int>> adj;
int n;

// Greedy Coloring
vector<int> greedyColoring() {
    vector<int> result(n, -1);
    result[0] = 0;
    for (int u = 1; u < n; u++) {
        vector<bool> used(n, false);
        for (int v : adj[u]) if (result[v] != -1) used[result[v]] = true;
        int c; for (c = 0; c < n; c++) if (!used[c]) break;
        result[u] = c;
    }
    return result;
}

// Welsh-Powell Coloring
vector<int> welshPowellColoring() {
    vector<pair<int, int>> deg;
    for (int i = 0; i < n; i++) deg.push_back({adj[i].size(), i});
    sort(deg.rbegin(), deg.rend());
    vector<int> result(n, -1);
    int color = 0;
    for (auto [d, u] : deg) {
        if (result[u] == -1) {
            result[u] = color;
            for (auto [_, v] : deg)
                if (result[v] == -1) {
                    bool can = true;
                    for (int x : adj[v])
                        if (result[x] == color) can = false;
                    if (can) result[v] = color;
                }
            color++;
        }
    }
    return result;
}

// DSATUR Coloring
vector<int> dsaturColoring() {
    vector<int> result(n, -1), degree(n), sat(n, 0);
    for (int i = 0; i < n; i++) degree[i] = adj[i].size();

    for (int it = 0; it < n; it++) {
        int u = -1, maxSat = -1, maxDeg = -1;
        for (int i = 0; i < n; i++) if (result[i] == -1) {
            if (sat[i] > maxSat || (sat[i] == maxSat && degree[i] > maxDeg)) {
                u = i; maxSat = sat[i]; maxDeg = degree[i];
            }
        }
        vector<bool> used(n, false);
        for (int v : adj[u]) if (result[v] != -1) used[result[v]] = true;
        int c; for (c = 0; c < n; c++) if (!used[c]) break;
        result[u] = c;
        for (int v : adj[u]) if (result[v] == -1)
            sat[v] = unordered_set<int>(begin(adj[v]), end(adj[v])).size();
    }
    return result;
}

void printColoring(string name, vector<int> color) {
    cout << name << " Exam Slots: [";
    for (int i = 0; i < n; i++) {
        cout << color[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]\n";
}

void roomAllocation(vector<int> color) {
    unordered_map<int, vector<int>> slotCourses;
    for (int i = 0; i < n; i++) slotCourses[color[i]].push_back(i);

    cout << "\nRoom Allocation per Slot:\n";
    for (auto &[slot, courses] : slotCourses) {
        cout << "Exam Slot " << slot << ": Courses [";
        for (int i = 0; i < courses.size(); i++) {
            cout << courses[i];
            if (i < courses.size() - 1) cout << ", ";
        }
        cout << "]\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m; // edges
    cin >> n >> m;
    adj.assign(n, {});
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    auto start = high_resolution_clock::now();

    auto g = greedyColoring();
    auto w = welshPowellColoring();
    auto d = dsaturColoring();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    printColoring("Greedy", g);
    printColoring("Welsh-Powell", w);
    printColoring("DSATUR", d);

    roomAllocation(d);

    cout << "\nTime Taken: " << duration.count() << " milliseconds\n";
    return 0;

}