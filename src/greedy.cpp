#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Edge {
    int from, to, next;
    Edge(int f = 0, int t = 0, int n = -1) {
        from = f; to = t; next = n;
    }
};

class Graph {
public:
    Graph(int _n) {
        n = _n;
        head = vector<int>(n + 1, -1);
        p = vector<double>(n + 1, 0);
        outsum = vector<int>(n + 1, 0);
    }
    void addedge(int from, int to) {
        edges.push_back(Edge(from, to, head[from]));
        head[from] = edges.size() - 1;
        outsum[from]++;
    }
    void addedge_op(int to, int from) {
        edges.push_back(Edge(from, to, head[from]));
        head[from] = edges.size() - 1;
        outsum[from]++;
    }
    // 遍历得到入度为0的源点
    vector<int> find_source(vector<int> state) {
        vector<int> temp;
        for (int i = 1; i <= outsum.size()-1; i++) {
            if (state[i] != 0) continue;
            if (outsum[i] == 0) {
                temp.push_back(i);
                cout<<"source node is "<<i<<endl;
            }
        }
        cout<<endl;
        return temp;
    }
    // 遍历得到出度为0的汇集点
    vector<int> find_sink(vector<int> state) {
        vector<int> temp;
        for (int i = 1; i <= outsum.size()-1; i++) {
            if (state[i] != 0) continue;
            if (outsum[i] == 0) {
                temp.insert(temp.begin(), i);
                cout<<"sink node is "<<i<<endl;
            }
        }
        for (auto i : temp) {
            cout << i << " "<<endl;
        }
        cout << "temp.size is " << temp.size();
        cout<<endl;
        return temp;
    }
    // 输出每个点的出度
    void output_o(vector<int> state) {
        for (int i = 1; i <= outsum.size()-1; i++) {
            if (state[i] != 0) continue;
            cout<<i<<" outsum is "<<outsum[i]<<endl;
        }
        cout<<endl;
    }
    // 输出每个点的入度
    void output_i(vector<int> state) {
        for (int i = 1; i <= outsum.size()-1; i++) {
            if (state[i] != 0) continue;
            cout<<i<<" insum is "<<outsum[i]<<endl;
        }
        cout<<endl;
    }
    // 图中的点的个数
    int n;
    // head数组
    vector<int> head;
    // 边集合数组
    vector<Edge> edges;
    // pagerank概率
    vector<double> p;
    // 计算出度
    vector<int> outsum;
};


int main() {
    vector<int> diata;
    // 存源头点
    vector<int> s1;
    // 存汇集点
    vector<int> s2;
    // 存汇集点
    vector<int> s;
    int n = 6;
    // 是否被s1s2收录
    vector<int> state (n + 1, 0);
    Graph g(n),G(n);
    // 读取txt文件
    string file_path = "C:\\dataset.txt";
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
            cout << "num1: " << num1 << ", num2: " << num2 << '\n';
            g.addedge(num1, num2);
            G.addedge_op(num1, num2);
        }
    }

    file.close();

    cout << g.head.size() << endl;
    for (int i = 1; i <= g.head.size()-1; i++) {
        cout << "Vertex " << i << " is connected to: ";
        for (int j = g.head[i]; j != -1; j = g.edges[j].next) {
            if (g.edges[j].to != -1) {
                cout << g.edges[j].to << " ";
            }
        }
        cout << endl;
    }

    while (accumulate(state.begin(), state.end(), 0) != n)
    {
        g.output_o(state);
        G.output_i(state);
        //暂存s1
        vector<int> temps1;
        temps1 = G.find_source(state);//查源头点
        while(temps1.size() != 0){
            cout<<"temps1 sum is "<<temps1.size()<<endl<<endl;
            for (int i = 0; i < temps1.size(); i++) {
                state[temps1[i]] = 1;
                cout<<"state "<<temps1[i]<<" is changed to 1"<<endl;
            }
            //把s1中的点删除重新计算出度和入度
            for (int i = 0; i < temps1.size(); i++) {
                //重新计算出度
                for (int j = G.head[temps1[i]];j != -1; j = G.edges[j].next) {
                    g.outsum[G.edges[j].to]--;
                }
                //重新计算入度
                for (int j = g.head[temps1[i]];j != -1; j = g.edges[j].next) {
                    G.outsum[g.edges[j].to]--;
                }
            }
            s1.insert(s1.end(), temps1.begin(), temps1.end());
            g.output_o(state);
            G.output_i(state);
            temps1 = G.find_source(state);
        }

    //    g.output_o(state);
    //    G.output_i(state);
        //暂存s2
        vector<int> temps2;
        temps2 = g.find_sink(state);//查汇集点
        cout << "temps2 .size is " << temps2.size();
        while(temps2.size() != 0){
            cout<<"temps2 sum is "<<temps2.size()<<endl<<endl;
            for (int i = 0; i < temps2.size(); i++) {
                state[temps2[i]] = 1;
                cout<<"state "<<temps2[i]<<" is changed to 1"<<endl;
            }
            //g.find_source(temp);//查汇集点
            //把s1中的点删除重新计算出度和入度
            for (int i = 0; i < temps2.size(); i++) {
                //重新计算出度
                for (int j = G.head[temps2[i]];j != -1; j = G.edges[j].next) {
                    g.outsum[G.edges[j].to]--;
                }
                //重新计算入度
                for (int j = g.head[temps2[i]];j != -1; j = g.edges[j].next) {
                    G.outsum[g.edges[j].to]--;
                }
            }
            s2.insert(s2.begin(), temps2.begin(), temps2.end());
            g.output_o(state);
            G.output_i(state);
            temps2 = g.find_sink(state);
        }

        //if (accumulate(state.begin(), state.end(), 0) == n) break;
        // 𝛿=𝑑_𝑜𝑢𝑡−𝑑_𝑖𝑛，计算theta
        int theta = -1, theta_temp = 0, max_node = -1;
        for (int i = 1; i <= g.outsum.size()-1; i++) {
            if (state[i] != 0) continue;
            theta_temp = g.outsum[i] - G.outsum[i];
            if (theta < theta_temp) {
                theta = theta_temp;
                max_node = i;
            }
        }
        if (max_node == -1) break;
        cout<<"maxe node is "<<max_node<<endl;
        s1.push_back(max_node);
        state[max_node] = 1;
        cout<<"state "<<max_node<<" is changed to 1"<<endl;
        //重新计算出度
        for (int j = G.head[max_node];j != -1; j = G.edges[j].next) {
            g.outsum[G.edges[j].to]--;
        }
        //重新计算入度
        for (int j = g.head[max_node];j != -1; j = g.edges[j].next) {
            G.outsum[g.edges[j].to]--;
        }
        g.output_o(state);
        G.output_i(state);
        //temps2 = g.find_sink(state);
    }
    g.output_o(state);
    G.output_i(state);
    s = s1;
    s.insert(s.end(), s2.begin(), s2.end());
    cout << "s1s2: ";

    for(auto i = 0;i < state.size(); i++) state[i] = 0;

    cout << "FAS is:\n";
    for(auto x: s) {
        state[x] = 1;
        for(int i = g.head[x]; i != -1; i = g.edges[i].next) {
            if(state[g.edges[i].to]) cout << x << ' ' << g.edges[i].to << endl;
        }
    }

    return 0;
}
