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
        p = vector<double> (n+1, 0);
    }
    void addedge(int from, int to) {
        edges.push_back(Edge(from, to, head[from]));
        head[from] = edges.size() - 1;
    }
    //图中的点的个数
    int n;
    //head数组
    vector<int> head;
    //边集合数组
    vector<Edge> edges;
    //pagerank概率
    vector<double> p;
};