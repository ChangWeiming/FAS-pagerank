#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>

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
        p = vector<float> (n+1, 0);
		p_out = vector<float> (n+1, 0);
        in_degree = vector<int> (n+1, 0);
        out_degree = vector<int> (n+1, 0);
    }
    void addedge(int from, int to) {
        edges.push_back(Edge(from, to, head[from]));
        head[from] = edges.size() - 1;
        in_degree[to] += 1;
		out_degree[from] += 1;
    }
    //图中的点的个数
    int n;
    //head数组
    vector<int> head;
	//入度数组
	vector<int> in_degree;
	//出度数组
	vector<int> out_degree;  
    //边集合数组
    vector<Edge> edges;
    //Page Rank Score
    vector<float> p;
    //Page Rank Score Out
    vector<float> p_out;
};

void print_page_rank(Graph* g) {
	int node_num = g->head.size()-1;
	for (int i = 1; i <= node_num; i++)
        cout << "Vertex " << i << " page rank score = " << g->p[i] << endl;
	cout << endl;
}

void page_rank(Graph* g, int max_iteration) {
	
	int node_num = g->head.size()-1;
	float init_score = 1.0 / node_num;
	for(int i = 1; i <= node_num; i++)
		g->p[i] = init_score;
	
	print_page_rank(g);
	
	float diff = 1.0;
	int iteration = 1;
	while(iteration < max_iteration) {
		
		for(int i = 1; i <= node_num; i++) {
			if(g->in_degree[i] != 0)
				g->p_out[i] = g->p[i] / g->in_degree[i];
		}
		
		diff = 0;
		for(int i = 1; i <= node_num; i++) {
			float p_sum = 0;
			for(int j = g->head[i]; j != -1; j = g->edges[j].next) {
				int to = g->edges[j].to;
				p_sum += g->p_out[to];
			}
			diff += fabs(g->p[i] - p_sum);
			g->p[i] = p_sum;
		}
		// print_page_rank(g);
		iteration++;
	}
	
	print_page_rank(g);
	
}

int main() {
    
    int n = 6;
    
    Graph lineGraph(n);
    
    string file_path = "./dataset.txt";
    ifstream file(file_path.c_str());
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
            lineGraph.addedge(num2, num1);
        }
    }

    file.close();

    for (int i = 1; i <= lineGraph.head.size()-1; i++) {
        cout << "Vertex " << i << " is connected to: ";
        for (int j = lineGraph.head[i]; j != -1; j = lineGraph.edges[j].next) {
            cout << lineGraph.edges[j].to << " ";
        }
        cout << endl;
        cout << "Vertex " << i << " in degree = " << lineGraph.in_degree[i] << endl;
        cout << "Vertex " << i << " out degree = " << lineGraph.out_degree[i] << endl;
    }
    
    page_rank(&lineGraph, 20);
    
    

    // cout << "FAS is:\n";
    // for(int i = lineGraph.head[x]; i != -1; i = g.edges[i].next) {
    //     cout << x << ' ' << g.edges[i].to << endl;
    // }

    return 0;
}
