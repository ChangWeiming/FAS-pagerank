#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct Edge {
    int from, to, next;
    Edge (int f = 0, int t = 0, int n = -1) {
        from = f; to = t; next = n;
    }
};

class Graph {
    public:
        Graph(int _n) {
            n = _n;
            head = vector<int> (n+1, -1);
        }

        void add_edge(int from, int to) {
            edges.push_back(Edge(from, to, head[from]));
            head[from] = edges.size() - 1;
        }
        // Number of Node
        int n;
        // Node Set
        vector<int> head;
        // Edge Set
        vector<Edge> edges;
};

vector<int> SortFAS(Graph* g) {

    int n = g->head.size()-1;

    vector<int> posi_order;
    for (int i = 0; i <= n; i++) posi_order.push_back(i);


    for (int i = 0; i <= n; i++) {

        auto it = find(posi_order.begin(), posi_order.end(), i);
        posi_order.erase(it);
        int now_val = 0, min_val = 0, min_posi = 0;

        for (int j = 1; j <= n; j++) {

            for(int k = g->head[i]; k != -1; k = g->edges[k].next) {
                if(g->edges[k].to == posi_order[j - 1]) now_val += 1;
            }
            for(int k = g->head[posi_order[j - 1]]; k != -1; k = g->edges[k].next) {
                if(g->edges[k].to == i) now_val -= 1;
            }

            if (now_val < min_val) {
                min_val = now_val;
                min_posi = j;
            }
        }
        posi_order.insert(posi_order.begin() + min_posi, i);
    }

    return posi_order;
}

int main() {
    int n = 69244 - 1;

    Graph g(n);

    cout << "========= " << endl;

    string file_path = "D:\\Code\\graph_datasets\\enron.txt";
    // 数据集路径："D:\\Code\\data\\dataset.txt","D:\\Code\\data\\dataset_2.txt","D:\\Code\\graph_datasets\\enron.txt"
    // "C:\dataset_2.txt"
    // "C:\\Users\\苏铄淼\\Desktop\\codes\\graph_datasets\\enron.txt"
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int num1, num2;
        char comma;
        if (ss >> num1 >> comma >> num2) {
            g.add_edge(num1, num2);
        }
    }

    file.close();

    vector<int> s = SortFAS(&g);
    vector<int> state (n + 1, 0);
    int back_src = 0;

    cout << "SortFAS is:\n";
    for(auto x: s) {
        state[x] = 1;
        for(int i = g.head[x]; i != -1; i = g.edges[i].next) {
            if(state[g.edges[i].to]) {
                back_src +=1;
            }
        }
    }

    cout << "back_src = " << back_src << endl;

    return 0;
}

