#include <iostream>
#include <cmath>
#include "graph.h"
#include "utils.h"

using namespace std;

void print_page_rank(LineGraph* g) {
	int node_num = g->head.size()-1;
	for (int i = 1; i <= node_num; i++)
        cout << "Vertex " << i << " page rank score = " << g->p[i] << endl;
	cout << endl;
}

void page_rank(LineGraph* g, int max_iteration) {
	
	int node_num = g->head.size()-1;
	float init_score = 1.0 / node_num;
	for(int i = 1; i <= node_num; i++)
		g->p[i] = init_score;
	
	print_page_rank(g);
	
	int iteration = 1;
	while(iteration < max_iteration) {
		
		for(int i = 1; i <= node_num; i++) {
			if(g->in_degree[i] != 0)
				g->p_out[i] = g->p[i] / g->in_degree[i];
		}
		
		for(int i = 1; i <= node_num; i++) {
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
	
	print_page_rank(g);
	
}

int main() {
    
    int n = 6;
    
    LineGraph lineGraph(n);
    
    string file_path = "./dataset.txt";

    readGraph(&lineGraph, file_path);
    
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

    int index = lineGraph.get_max_p_index();
    cout << "Max page rank score index = " << index << endl;

    return 0;
}
