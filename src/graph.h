#ifndef __GRAPH_H__
#define __GRAPH_H__

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
            head = vector<int> (n+1, -1);
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
            p = vector<float> (n+1, 0);
            p_out = vector<float> (n+1, 0);
            in_degree = vector<int> (n+1, 0);
            out_degree = vector<int> (n+1, 0);
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

#endif
