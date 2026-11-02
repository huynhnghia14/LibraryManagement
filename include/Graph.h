#ifndef GRAPH_H
#define GRAPH_H

#include "DynamicArray.h"

enum EdgeType { EDGE_SAME_CATEGORY = 0, EDGE_SAME_READER = 1 };

struct Edge {
    int to;
    int type;                                       // EdgeType
    int weight;
};

// Đồ thị danh sách kề – gợi ý sách cùng thể loại (cài đặt ở tuần 7)
class Graph {
    int n;                                          // số đỉnh = số sách
    DynamicArray<Edge>* adj;
    DynamicArray<char*> labels;                     // isbn của đỉnh i

public:
    Graph();
    ~Graph();

    int  addVertex(const char* isbn);
    void addEdge(int u, int v, int type, int w = 1);   // vô hướng, cộng dồn weight nếu đã có
    void removeVertex(int u);                          // đánh dấu xóa (lazy)
    DynamicArray<int> bfs(int s, int maxDepth, int type);
    template <typename F> void dfs(int s, F visit, int type);
    const DynamicArray<Edge>& neighbors(int u) const;
    int  components(int type);
};

#endif
