
#ifndef PJ2_BELLMAN_FORD_H
#define PJ2_BELLMAN_FORD_H

#include "graph.h"

//using std::cout;
//using std::endl;
//using std::vector;
//using std::string;
//using std::tuple;
//using std::list;
//using std::get;
//using std::make_tuple;



using Matrix = vector<vector<Tuple>>;  //连接矩阵
using N = tuple<int,int,int, int>; // 边，包含：当前起点序号，前一点序号，权重，在哪条line上， 从get<1> 指向get<0>
using EDGE = vector<N>;  //已经获得的最短路径的点的集合

class Bellman {
private:
    int vertex;
    EDGE Edge;

public:
    explicit Bellman(int v) : vertex(v) {
    }

    void insert_edge(const Matrix &G);

    void bellman_ford(const Matrix &G, const string &source_name, const string &destination, const vector<string> &station);

};

void graph_bellman(const Graph& g,const string & source_name, const string & destination) {
    Bellman bm(g.GRAPH.size());
    bm.insert_edge(g.GRAPH);
    bm.bellman_ford(g.GRAPH,source_name,destination, g.station);
}

#endif //PJ2_BELLMAN_FORD_H

