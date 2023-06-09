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

class LineGraph: public Graph {
    public:
        LineGraph(int _n): Graph(_n) {
            p = vector<float> (n, 0);
            p_out = vector<float> (n, 0);
            in_degree = vector<int> (n, 0);
            out_degree = vector<int> (n, 0);
        }

        void add_edge(int from, int to) {
            edges.push_back(Edge(from, to, head[from]));
            head[from] = edges.size() - 1;
            in_degree[to] += 1;
		    out_degree[from] += 1;
        }

        int get_max_p_index() {
            int node_num = head.size()-1;
            float max_p = 0;
            int index = 0;
            for(int i = 1; i <= node_num; i++) {
                if(max_p < p[i]) {
                    max_p = p[i];
                    index = i;
                }
            }
            return index;
        }

        // Node's Indegree
    	vector<int> in_degree;
        // Node's Outdegree
        vector<int> out_degree;
        // Node's Page Rank Score
        vector<float> p;
        // Node's Page Rank Score Out
        vector<float> p_out;
};

class GreedyGraph: public Graph {
    public:
        GreedyGraph(int _n): Graph(_n) {
            out_degree = vector<int> (n+1, 0);
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

    //cout << "===begin update_degree====" << endl;

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

vector<int> find_source(GreedyGraph* g, vector<int> state) {

    //cout << "===begin find_source====" << endl;

    vector<int> source_index;
    for (int i = 0; i < g->out_degree.size(); i++) {

        if (state[i] != 0) continue;

        if (g->out_degree[i] == 0) source_index.push_back(i);
    }

    return source_index;
}

vector<int> find_sink(GreedyGraph* g, vector<int> state) {

    //cout << "===begin find_sink====" << endl;

    vector<int> sink_index;
    for (int i = 0; i < g->out_degree.size(); i++) {

        if (state[i] != 0) continue;

        if (g->out_degree[i] == 0) sink_index.insert(sink_index.begin(), i);
    }

    return sink_index;
}

int find_max(GreedyGraph* for_g, GreedyGraph* back_g, vector<int> state) {

    //cout << "===begin find_max====" << endl;

    int theta = -1, theta_temp = 0, max_index = -1;
    for (int i = 0; i < for_g->out_degree.size(); i++) {
        if (state[i] != 0) continue;
        theta_temp = for_g->out_degree[i] - back_g->out_degree[i];
        if (theta < theta_temp) {
            theta = theta_temp;
            max_index = i;
        }
    }

    return max_index;
}


vector<int> GreedyFAS(GreedyGraph* for_g, GreedyGraph* back_g, int n) {

    // compute out_degree - in_degree
    vector<int> delta;
    // store the source node's index
    vector<int> s1;
    // store the sink node's index
    vector<int> s2;
    // store the index after GreedyFAS
    vector<int> s;
    // whether node i has been stored in s1/s2, 1 means yes
    vector<int> state (n + 1, 0);

    cout << "===begin greedyfas====" << endl;

    while (accumulate(state.begin(), state.end(), 0) != (n+1))
    {
        vector<int> temps1 = find_source(back_g, state);
        while(temps1.size() != 0){
            for (int i = 0; i < temps1.size(); i++) state[temps1[i]] = 1;
            update_degree(for_g, back_g, temps1);
            s1.insert(s1.end(), temps1.begin(), temps1.end());
            temps1 = find_source(back_g, state);
        }

        vector<int> temps2 = find_sink(for_g, state);
        while(temps2.size() != 0){
            for (int i = 0; i < temps2.size(); i++)
                state[temps2[i]] = 1;
            update_degree(for_g, back_g, temps2);
            s2.insert(s2.begin(), temps2.begin(), temps2.end());
            temps2 = find_sink(for_g, state);
        }

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

void readGraph(Graph* g, string dataset_path, int for_or_back) {

    ifstream file(dataset_path.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
    }

	string line;
	if (for_or_back == 0) {
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

    int n = 69244;

    GreedyGraph forward_g(n);

    GreedyGraph backward_g(n);

    string file_path = "D:\\Code\\graph_datasets\\enron.txt";

    //"D:\\Code\\data\\dataset.txt","D:\\Code\\data\\dataset_2.txt","D:\\Code\\graph_datasets\\enron.txt"

    readGraph(&forward_g, file_path, 0);

    readGraph(&backward_g, file_path, 1);

    vector<int> s = GreedyFAS(&forward_g, &backward_g, n);

    //for(int i=0;i<=s.size()-1;i++) cout<<s[i]<<endl;
    vector<int> state (n + 1, 0);

    int back_src = 0;

    cout << "GreedyFAS is:\n";
    for(auto x: s) {
        state[x] = 1;
        for(int i = forward_g.head[x]; i != -1; i = forward_g.edges[i].next) {
            if(state[forward_g.edges[i].to]) {
                //cout << x << ' ' << forward_g.edges[i].to << endl;
                back_src +=1;
            }
        }
    }

    cout << "back_src = " << back_src << endl;

    return 0;
}
