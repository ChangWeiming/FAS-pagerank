#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

const int easy_node = 7;
const int enron_node = 69244;
const int word_node = 10617;
const string easy_path = "D:\\Code\\graph_datasets\\easygraph.txt";
const string enron_path = "D:\\Code\\graph_datasets\\enron.txt";
const string word_path = "D:\\Code\\graph_datasets\\wordassociation-2011.txt";

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
    posi_order.push_back(0);

    for (int i = 1; i < n; i++) {
        int val = 0, min = 0, loc = i;
        // Update the val and find the smallest location
        for (int j = i - 1; j >= 0; j--) {
            // If the arc from vi to vj exists, the val sub 1;
            for(int k = g->head[i]; k != -1; k = g->edges[k].next) {
                if(g->edges[k].to == posi_order[j]) {
                    val -= 1;
                    break;
                }
            }
            // If the arc from vj to vi exists, the val add 1;
            for(int k = g->head[posi_order[j]]; k != -1; k = g->edges[k].next) {
                if(g->edges[k].to == i) {
                    val += 1;
                    break;
                }
            }
            // Update the min and find the leftmost position
            if (val <= min) {
                min = val;
                loc = j;
            }
        }
        posi_order.insert(posi_order.begin() + loc, i);
    }

    return posi_order;
}

void readGraph(Graph* g, string dataset_path) {
    ifstream file(dataset_path.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
    }

	string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int num1, num2;
        char comma;
        if (ss >> num1 >> comma >> num2) {
            g->add_edge(num1, num2);
        }
    }

    file.close();
}

int main(int argc, char* argv[]) {
    int n = enron_node;
    string file_path = enron_path;

	if (argc == 3) {
		sscanf(argv[1], "%d", &n);
		file_path = string(argv[2]);
	}

    Graph g(n);
    readGraph(&g, file_path);

    vector<int> s = SortFAS(&g);
    vector<int> state (n, 0);
    float back_src = 0;
    float sort_fas = 0;
    for(auto x: s) {
        state[x] = 1;
        for(int i = g.head[x]; i != -1; i = g.edges[i].next) {
            if(state[g.edges[i].to]) back_src +=1;
        }
    }
    sort_fas = back_src/g.edges.size();
    cout << "back_src = " << back_src << endl;
    cout << "sortfas = " << sort_fas << endl;
    return 0;
}

