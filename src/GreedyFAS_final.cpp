#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <numeric>
#include <vector>
#include <cstring>

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

        virtual void add_edge(int from, int to) {
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

class GreedyGraph: public Graph {
    public:
        GreedyGraph(int _n): Graph(_n) {
            out_degree = vector<int> (n, 0);
        }
        void add_edge(int from, int to) {
            edges.push_back(Edge(from, to, head[from]));
            head[from] = edges.size() - 1;
		    out_degree[from] += 1;
        }
        // Node's Outdegree
        vector<int> out_degree;
};

void update_degree(GreedyGraph* for_g, GreedyGraph* back_g, vector<int> remove_node) {
    for (int i = 0; i < remove_node.size(); i++) {
        //update out_degree
        for (int j = back_g->head[remove_node[i]];j != -1; j = back_g->edges[j].next) {
            for_g->out_degree[back_g->edges[j].to]--;
        }
        //updata in_degree
        for (int j = for_g->head[remove_node[i]];j != -1; j = for_g->edges[j].next) {
            back_g->out_degree[for_g->edges[j].to]--;
        }
    }

}

vector<int> find_source(GreedyGraph* back_g, vector<int> &state) {
    vector<int> source_index;
    for (int i = 0; i < back_g->out_degree.size(); i++) {
        // If this node have been processed, then continue
        if (state[i] != 0) continue;
        // Insert the source node at the end of the source_index and update this node's state
        if (back_g->out_degree[i] == 0) {
            source_index.push_back(i);
            state[i] = 1;
        }
    }

    return source_index;
}

vector<int> find_sink(GreedyGraph* for_g, vector<int> &state) {
    vector<int> sink_index;
    for (int i = 0; i < for_g->out_degree.size(); i++) {
        // If this node have been processed, then continue
        if (state[i] != 0) continue;
        // Insert the source node at the begninning of the sink_index and update this node's state
        if (for_g->out_degree[i] == 0) {
            sink_index.insert(sink_index.begin(), i);
            state[i] = 1;
        }
    }

    return sink_index;
}

int find_max(GreedyGraph* for_g, GreedyGraph* back_g, vector<int> state) {
    int delta = -1, delta_temp = 0, max_index = -1;
    for (int i = 0; i < for_g->out_degree.size(); i++) {
        // If this node have been processed, then continue
        if (state[i] != 0) continue;
        // Find the biggest delta
        delta_temp = for_g->out_degree[i] - back_g->out_degree[i];
        if (delta < delta_temp) {
            delta = delta_temp;
            max_index = i;
        }
    }

    return max_index;
}


vector<int> GreedyFAS(GreedyGraph* for_g, GreedyGraph* back_g, int n) {
    // store the source node's index
    vector<int> s1;
    // store the sink node's index
    vector<int> s2;
    // store the index after GreedyFAS
    vector<int> s;
    // whether node i has been stored in s1/s2, 1 means yes
    vector<int> state (n, 0);

    while (accumulate(state.begin(), state.end(), 0) < (n)) {
        // Find source node and insert it in the end of s1
        vector<int> temps1 = find_source(back_g, state);
        while(temps1.size() != 0){
            update_degree(for_g, back_g, temps1);
            s1.insert(s1.end(), temps1.begin(), temps1.end());
            temps1 = find_source(back_g, state);
        }
        // Find sink node and insert it in the begin of s2
        vector<int> temps2 = find_sink(for_g, state);
        while(temps2.size() != 0){
            update_degree(for_g, back_g, temps2);
            s2.insert(s2.begin(), temps2.begin(), temps2.end());
            temps2 = find_sink(for_g, state);
        }
        // Find the node with the most value of out_degree - in_degree
        vector<int> max_index (1, -1);
        max_index[0] = find_max(for_g, back_g, state);
        if (max_index[0] == -1) break;
        s1.push_back(max_index[0]);
        state[max_index[0]] = 1;
        update_degree(for_g, back_g, max_index);
    }
    s = s1;
    s.insert(s.end(), s2.begin(), s2.end());

    return s;
}

void readGraph(Graph* g, string dataset_path, int for_or_reve) {
    ifstream file(dataset_path.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
    }

	string line;
	// 0 represents storing forward graph, 1 represents storing reverse graph
	if (for_or_reve == 0) {
        while (getline(file, line)) {
            stringstream ss(line);
            int num1, num2;
            char comma;
            if (ss >> num1 >> comma >> num2) {
                g->add_edge(num1, num2);
            }
        }
	}
	else {
            while (getline(file, line)) {
            stringstream ss(line);
            int num1, num2;
            char comma;
            if (ss >> num1 >> comma >> num2) {
                g->add_edge(num2, num1);
            }
        }
	}

    file.close();
}

int main() {
    int n = enron_node;
    GreedyGraph forward_g(n);
    GreedyGraph reverse_g(n);

    string file_path = enron_path;
    readGraph(&forward_g, file_path, 0);
    readGraph(&reverse_g, file_path, 1);

    vector<int> s = GreedyFAS(&forward_g, &reverse_g, n);
    vector<int> state (n, 0);
    float back_src = 0;
    float greedy_fas = 0;
    // Print the back_src
    for(auto x: s) {
        state[x] = 1;
        for(int i = forward_g.head[x]; i != -1; i = forward_g.edges[i].next) {
            if(state[forward_g.edges[i].to]) back_src +=1;
        }
    }
    cout << "back_src = " << back_src << endl;
    greedy_fas = back_src/forward_g.edges.size();
    cout << "greedy_fas = " << greedy_fas << endl;
    return 0;
}
