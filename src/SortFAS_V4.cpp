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
            head = vector<int> (n, -1);
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

    int n = g->head.size();

    vector<int> posi_order;
    for (int i = 0; i < n; i++) posi_order.push_back(i);
    int node_index = 0;

    for (int i = 0; i < n; i++) {
        // 找到当前点的位置，从当前位置往前面遍历查找
        auto item = find(posi_order.begin(), posi_order.end(), i);//返回在序列中的地址
        node_index = distance(posi_order.begin(),item);//返回在序列中的索引
        int now_val = 0, min_val = 0, min_loc = node_index;
        // 确认插入位置‘
        for (int j = node_index - 1; j >= 0; j--) {

            for(int k = g->head[i]; k != -1; k = g->edges[k].next) {
                if(g->edges[k].to == posi_order[j]) {
                    now_val -= 1;
                    break;
                }
            }

            for(int k = g->head[posi_order[j]]; k != -1; k = g->edges[k].next) {
                if(g->edges[k].to == i) {
                    now_val += 1;
                    break;
                }
            }

            if (now_val <= min_val) {
                min_val = now_val;
                min_loc = j;
            }
        }
//        cout << "min_posi now is " << min_posi << endl;
        posi_order.erase(item);
        posi_order.insert(posi_order.begin() + min_loc, i);
    }

    return posi_order;
}

int main() {
//    int n = 69244 - 1;
    int n = 10617;
    Graph g(n);

    string file_path = "D:\\Code\\graph_datasets\\wordassociation-2011.txt ";
    // "D:\\Code\\data\\dataset.txt","D:\\Code\\data\\dataset_2.txt","D:\\Code\\graph_datasets\\enron.txt" "D:\\Code\\graph_datasets\\wordassociation-2011.txt"
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
    vector<int> state (n, 0);
    float back_src = 0;

    cout << "SortFAS is:\n";
    for(auto x: s) {
        state[x] = 1;
        for(int i = g.head[x]; i != -1; i = g.edges[i].next) {
            if(state[g.edges[i].to]) {
                back_src +=1;
            }
        }
    }
    float word = 72172;
    float enron = 276143;
    float sortfas = back_src/word;
    cout << "back_src = " << back_src << endl;
    cout << "sortfas = " << sortfas << endl;
    return 0;
}

