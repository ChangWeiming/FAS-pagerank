#include <cmath>
#include <numeric>
#include "graph.h"
#include "utils.h"

using namespace std;

vector<int> update_degree(GreedyGraph* for_g, GreedyGraph* back_g, vector<int> remove_node) {

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

    vector<int> source_index;
    for (int i = 1; i <= g->out_degree.size()-1; i++) {

        if (state[i] != 0) continue;

        if (g->out_degree[i] == 0) source_index.push_back(i);
    }

    return source_index;
}

vector<int> find_sink(GreedyGraph* g, vector<int> state) {

    vector<int> sink_index;
    for (int i = 1; i <= g->out_degree.size()-1; i++) {

        if (state[i] != 0) continue;

        if (g->out_degree[i] == 0) sink_index.insert(sink_index.begin(), i);
    }

    return sink_index;
}

int find_max(GreedyGraph* for_g, GreedyGraph* back_g, vector<int> state) {

    int theta = -1, theta_temp = 0, max_index = -1;
    for (int i = 1; i <= for_g->out_degree.size()-1; i++) {
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

    while (accumulate(state.begin(), state.end(), 0) != n)
    {

        vector<int> temps1 = find_source(back_g, state);
        while(temps1.size() != 0){
            for (int i = 0; i < temps1.size(); i++)
                state[temps1[i]] = 1;
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

int main() {

    int n = 6;

    GreedyGraph forward_g(n);

    GreedyGraph backward_g(n);

    string file_path = "./dataset.txt";

    readGraph(&forward_g, file_path, 0);

    readGraph(&backward_g, file_path, 0);

    for (int i = 1; i <= forward_g.head.size()-1; i++) {
        cout << "Vertex " << i << " is connected to: ";
        for (int j = forward_g.head[i]; j != -1; j = forward_g.edges[j].next) {
            cout << forward_g.edges[j].to << " ";
        }
        cout << endl;
        cout << "Vertex " << i << " in degree = " << backward_g.out_degree[i] << endl;
        cout << "Vertex " << i << " out degree = " << forward_g.out_degree[i] << endl;
    }

    vector<int> s = GreedyFAS(&forward_g, &backward_g, n);

    vector<int> state (n + 1, 0);

    cout << "FAS is:\n";
    for(auto x: s) {
        state[x] = 1;
        for(int i = forward_g.head[x]; i != -1; i = forward_g.edges[i].next) {
            if(state[forward_g.edges[i].to]) cout << x << ' ' << forward_g.edges[i].to << endl;
        }
    }

    return 0;
}
