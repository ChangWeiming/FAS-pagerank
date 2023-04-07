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

    // The last iteration's position order
    vector<int> posi_order;
    for (int i = 0; i <= n; i++) posi_order.push_back(i);

    // 对n个元素进行重排序
    for (int i = 0; i <= n; i++) {

        // Find node i's index
        auto i_index = find(posi_order.begin(), posi_order.end(), i);
        int index = distance(posi_order.begin(), i_index);
        cout << "node " << i << " is now in " << index <<endl;
        int min_posi = 0;

        // 有n种重排序的方法
        int now_backarc = 0, last_backarc = 0, backarc_without_nodei = 0;
        int now_val = 0, min_val = 0;
        min_posi = 0;

        // compute the number of back_src without node i
        vector<int> order_without_nodei = posi_order;
        vector<int> state (n + 1, 0);
        order_without_nodei.erase(order_without_nodei.begin()+index);
        for(int i = 0; i <= order_without_nodei.size()-1; i++) {
            //cout << "===compute the number of back_src without node i===" <<endl;
            int x = order_without_nodei[i];
            state[x] = 1;
            for(int k = g->head[x]; k != -1; k = g->edges[k].next) {
                if(state[g->edges[k].to]) backarc_without_nodei += 1;
            }
            //cout << "backarc_without_nodei is " << backarc_without_nodei << endl;
        }

        for (int j = 0; j <= n; j++) {
            // Insert node i into the last linear arrangement for each position
            vector<int> temp_order = order_without_nodei;
            temp_order.insert(temp_order.begin() + j, i);
            //for(int i = 0; i <= temp_order.size()-1; i++) cout << temp_order[i] << " ";
            //cout << endl;
            // compute the number of back_src from node i
            vector<int> state (n + 1, 0);
            now_backarc = backarc_without_nodei;
            //cout << "backarc_without_nodei is " << backarc_without_nodei << endl;
            for (int i = 0; i <= j; i++) state[temp_order[i]] = 1;
            for(int k = g->head[i]; k != -1; k = g->edges[k].next) {
                //cout << "===compute the number of back_src from node i===" <<endl;
                if(state[g->edges[k].to]) now_backarc += 1;
            }
            //cout << "now_backarc is " << now_backarc << endl;
            // compute the number of back_src to node i
            for (int y = j + 1; y <= n; y++) {
                int x = temp_order[y];
                //cout << "y is " << y << endl;
                //cout << "x is " << x << endl;
                //cout << "===compute the number of back_src to node i===" <<endl;
                for(int k = g->head[x]; k != -1; k = g->edges[k].next) {
                    if(g->edges[k].to == i) now_backarc += 1;
                }
            }
            //cout << "===init===" <<endl;
            if(j == 0) {
                last_backarc = now_backarc;
                now_val = 0;
                min_val = 0;
                min_posi = 0;
                continue;
            }

            now_val += now_backarc - last_backarc;

            last_backarc = now_backarc;
            if (now_val < min_val) {
                min_val = now_val;
                min_posi = j;
            }
        }

        posi_order.erase(i_index);
        posi_order.insert(posi_order.begin() + min_posi, i);
    }

    return posi_order;
}

int main() {
    int n = 69244 - 1;

    Graph g(n);

    cout << "========= " << endl;

    string file_path = "C:\\Users\\苏铄淼\\Desktop\\codes\\graph_datasets\\enron.txt";
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
            //cout << "num1: " << num1 << ", num2: " << num2 << '\n';
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
                //cout << x << ' ' << forward_g.edges[i].to << endl;
                back_src +=1;
            }
        }
    }

    cout << "back_src = " << back_src << endl;

    return 0;
}

