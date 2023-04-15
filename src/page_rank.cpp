#include <iostream>
#include <cmath>
#include <stack>
#include <cassert>

#include "graph.h"
#include "utils.h"

using namespace std;

void print_page_rank(LineGraph* g) {
	int node_num = g->n;
	for (int i = 0; i < node_num; i++)
        cout << "Vertex " << i << " page rank score = " << g->p[i] << endl;
	cout << endl;
}

void page_rank(LineGraph* g, int max_iteration) {
	
	int node_num = g->n;
	float init_score = 1.0 / node_num;
	for(int i = 0; i < node_num; i++)
		g->p[i] = init_score;
	
	//print_page_rank(g);
	
	int iteration = 1;
	while(iteration < max_iteration) {
		
		for(int i = 0; i < node_num; i++) {
			if(g->in_degree[i] != 0)
				g->p_out[i] = g->p[i] / g->in_degree[i];
		}
		
		for(int i = 0; i < node_num; i++) {
			float p_sum = 0;
			for(int j = g->head[i]; j != -1; j = g->edges[j].next) {
				int to = g->edges[j].to;
				p_sum += g->p_out[to];
			}
			g->p[i] = p_sum;
		}
		// print_page_rank(g);
		iteration++;
	}
	
	//print_page_rank(g);
	
}

class PageRankFAS{	
public:
	Graph* g;
	// dfn & low for timestamp of the node
	vector<int> dfn, low, in_stack, scc;
	vector< vector<int> > scc_nodes;
	vector<int> lg_node_to_g_edge, g_edge_to_lg_node, vis;
	vector<int> is_in_FAS;
	stack<int> s;
	int timestamp, sc_num;
	PageRankFAS(Graph* _g) {
		g = _g;
		int n = g->n;
		dfn = vector<int>(n, 0);
		low = vector<int> (n, 0); 
		in_stack = vector<int>(n, 0);
		scc = vector<int>(n, 0);
		is_in_FAS = vector<int> (g->edges.size(), 0);
		timestamp = 0; sc_num = 0;

		//1 index array
		scc_nodes.push_back(vector<int>());

		//mappings 
		g_edge_to_lg_node = vector<int>(g->edges.size(), -1);
		lg_node_to_g_edge = vector<int>(g->edges.size(), -1);

		//used for generate line graph
		vis = vector<int>(g->n, 0);
	}

	//used for SCC
	void tarjan(int u) {
		low[u] = dfn[u] = ++timestamp;
		s.push(u);
		in_stack[u] = 1;
		for(int i = g->head[u]; ~i; i = g->edges[i].next) {
			if(is_in_FAS[i]) continue;

			int v = g->edges[i].to;
			if(!dfn[v]) {
				tarjan(v);
				low[u] = min(low[u], low[v]);
			} else if(in_stack[v]) {
				low[u] = min(low[u], dfn[v]);

			}
		}

		if(dfn[u] == low[u]) {
			++sc_num;	
			vector<int> scc_nodes_tmp;

			while(s.top() != u) {
				int s_top = s.top();s.pop();
				scc[s_top] = sc_num;
				in_stack[s_top] = 0;
				scc_nodes_tmp.push_back(s_top);
			}
			int s_top = s.top();s.pop();
			scc[s_top] = sc_num;
			in_stack[s_top] = 0;
			scc_nodes_tmp.push_back(s_top);
			scc_nodes.push_back(scc_nodes_tmp);
		}
	}

	void scc_to_line_graph(int u, int scc_num, int prev, int &now_lg_node_id, LineGraph* lg) {
		vis[u] = 1;
		//u for from, v for to
		for(int i = g->head[u]; ~i; i = g->edges[i].next) {
			if(is_in_FAS[i]) continue;

			int v = g->edges[i].to;
			//not in the same scc
			if(scc[u] != scc[v]) continue;

			int &lg_node_id = g_edge_to_lg_node[i];
			if(g_edge_to_lg_node[i] < 0) {
				g_edge_to_lg_node[i] = ++now_lg_node_id;
				lg_node_to_g_edge[now_lg_node_id] = i;
			}
			if(prev >= 0) {
				//add reverse edge
				lg->add_edge(prev, lg_node_id);
			}
			if(!vis[v]) {
				scc_to_line_graph(v, scc_num, lg_node_id, now_lg_node_id, lg);
			} else {
				for(int j = g->head[v]; ~j; j = g->edges[j].next) {
					lg->add_edge(lg_node_id, g_edge_to_lg_node[j]);
				}
				
			}
		}
	}

	int get_scc_edge_cnt(int u, int scc_num,vector<int> &vis_scc) {
		vis_scc[u] = 1;
		int cnt = 0;
		for(int i = g->head[u]; ~i; i = g->edges[i].next) {
			if(is_in_FAS[i]) continue;

			int v = g->edges[i].to;
			if(scc[v] != scc_num) continue;

			cnt++;
			if(!vis_scc[v]) cnt += get_scc_edge_cnt(v, scc_num, vis_scc);
		}
		return cnt;

	}

	void get_reverse_line_graph(LineGraph* lg, LineGraph* rlg) {
		for(int u = 0;u < lg->n; u++) {
			for(int i = lg->head[u]; ~i; i = lg->edges[i].next) {
				rlg->add_edge(lg->edges[i].to, u);
			}
		}
	}

	void get_FAS_with_page_rank() {
		int cnt = 0;
		//for every connected component compute scc
		LineGraph *lg = new LineGraph(g->edges.size()); 
		LineGraph *rlg = new LineGraph(g->edges.size());
		int now_lg_node = -1;

		while(true) {
			fill(dfn.begin(), dfn.end(), 0);
			fill(low.begin(), low.end(), 0);
			fill(in_stack.begin(), in_stack.end(), 0);
			fill(scc.begin(), scc.end(), 0);
			fill(scc.begin(), scc.end(), 0);
			while(!s.empty()) s.pop();
			timestamp = 0; sc_num = 0;

			//used for generate line graph
			fill(vis.begin(), vis.end(), 0);
			fill(g_edge_to_lg_node.begin(), g_edge_to_lg_node.end(), -1);
			fill(lg_node_to_g_edge.begin(), lg_node_to_g_edge.end(), -1);

			//1 index array
			scc_nodes.clear();
			scc_nodes.push_back(vector<int>());

			for(int i = 0;i < g->n; i++) {
				if(!dfn[i]) tarjan(i);
			}

			if(sc_num == g->n) break;

			//compute line graph and reuse it

			//cout << "scc:\n";
			//for(int i = 0;i < g->n; i++) cout << scc[i] << ' ';
			//cout << endl;

			vector<bool> done_scc(scc_nodes.size(), false);

			for(int i = 0;i < g->n; i++) {
				if(done_scc[scc[i]] || scc_nodes[scc[i]].size() == 1) continue;

				//dbg info
				//cout << "origin graph:\n";
				//for(int i = 0;i < g->edges.size(); i++) if(is_in_FAS[i]) continue; else cout << g->edges[i].from << ' ' << g->edges[i].to << endl;
				//cout << "origin graph ends\n";

				//cout << "line graph:\n";
				scc_to_line_graph(i, scc[i], -1, now_lg_node, lg);
				//lg->print_graph();
				//cout << "line graph ends\n";
				//cout << "node to edge id mappings:"; for (int i = 0;i < now_lg_node + 1; i++) cout << i << "->" << lg_node_to_g_edge[i] << ';';
				//cout << endl;
				//cout << now_lg_node << endl;

				lg->n = rlg->n = now_lg_node + 1;
				get_reverse_line_graph(lg, rlg);

				done_scc[scc[i]] = true;
				page_rank(rlg, 20);
				int edge_index = lg_node_to_g_edge[rlg->get_max_p_index()];

				assert(is_in_FAS[edge_index] == 0);
				is_in_FAS[edge_index] = 1; cnt++;
				//cout << g->edges[edge_index].from << ' ' << g->edges[edge_index].to << ' ' << cnt << ' ' << sc_num << '\n';

				//clear
				for(int i = 0;i < rlg->n; i++) {
					g_edge_to_lg_node[lg_node_to_g_edge[i]] = -1;
					lg_node_to_g_edge[i] = -1;

				}
				lg->clear();
				rlg->clear();
				now_lg_node = -1;
			}
		}
		cout << cnt << endl;

	}

};


int page_rank_test() {
	cin.tie(0);
	cout.tie(0);

    int n = 6;
    
    LineGraph lineGraph(n);
    
    string file_path = "./dataset.txt";

    readGraph(&lineGraph, file_path);
    
    for (int i = 0; i < lineGraph.n; i++) {
        cout << "Vertex " << i << " is connected to: ";
        for (int j = lineGraph.head[i]; j != -1; j = lineGraph.edges[j].next) {
            cout << lineGraph.edges[j].to << " ";
        }
        cout << endl;
        cout << "Vertex " << i << " in degree = " << lineGraph.in_degree[i] << endl;
        cout << "Vertex " << i << " out degree = " << lineGraph.out_degree[i] << endl;
    }
    
    page_rank(&lineGraph, 20);

    int index = lineGraph.get_max_p_index();
    cout << "Max page rank score index = " << index << endl;

    return 0;
}


int main(int argc, char *argv[]) {
	cin.tie(0);
	cout.tie(0);

    int n = 69244;
    string file_path = "./enron.txt";

	if (argc == 3) {
		sscanf(argv[1], "%d", &n);
		file_path = string(argv[2]);
	}

    Graph g(n);
    readGraph(&g, file_path);
	PageRankFAS solver(&g);

	solver.get_FAS_with_page_rank();
	return 0;
}