#include <iostream>
#include <vector>

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
    vector<int> find_source() {
        vector<int> temp;
        for (int i = 1; i <= outsum.size()-1; i++) {
            if(outsum[i] == 0) {
                temp.insert(temp.begin(), i);
                cout<<"source node is "<<i<<endl;
            }
        }
        cout<<endl;
        return temp;
    }
    // 输出每个点的出度
    void output_o() {
        for (int i = 1; i <= outsum.size()-1; i++) {
            cout<<i<<" outsum is "<<outsum[i]<<endl;
        }
        cout<<endl;
    }
    // 输出每个点的入度
    void output_i() {
        for (int i = 1; i <= outsum.size()-1; i++) {
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

    int n = 5;
    Graph g(n),G(n);
    g.addedge(1, 2);
    g.addedge(2, 1);
    g.addedge(2, 3);
    g.addedge(3, 4);
    g.addedge(4, 2);
    g.addedge(4, 1);
    g.addedge(5, 3);
    g.addedge(5, 4);
    G.addedge_op(1, 2);
    G.addedge_op(2, 1);
    G.addedge_op(2, 3);
    G.addedge_op(3, 4);
    G.addedge_op(4, 2);
    G.addedge_op(4, 1);
    G.addedge_op(5, 3);
    G.addedge_op(5, 4);

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
    vector<int> temp;
    g.output_o();
    G.output_i();
    temp = G.find_source();//查源头点
    //g.find_source(temp);//查汇集点
    //把s1中的点删除重新计算出度和入度
    cout<<"temp sum is "<<temp.size()<<endl;
    for (int i = 0; i < temp.size(); i++) {
        //重新计算出度
        for (int j = G.head[temp[i]];j != -1; j = G.edges[j].next) {
            g.outsum[G.edges[j].to]--;
        }
        //重新计算入度
        for (int j = g.head[temp[i]];j != -1; j = g.edges[j].next) {
            G.outsum[g.edges[j].to]--;
        }
    }
    s1.insert(s1.begin(), temp.begin(), temp.end());
    g.output_o();
    G.output_i();
    return 0;
}
