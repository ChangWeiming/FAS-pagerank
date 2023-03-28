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
    Graph(int _n) {
        n = _n;
        head = vector<int> (n+1, -1);
    }
    void addedge(int from, int to) {
        edges.push_back(Edge(from, to, head[from]));
        head[from] = edges.size() - 1;
    }
public:
    //图中的点的个数
    int n;
    //head数组
    vector<int> head;
    //边集合数组
    vector<Edge> edges;
};