
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



using Matrix = vector<vector<Tuple>>;  //���Ӿ���
using N = tuple<int,int,int, int>; // �ߣ���������ǰ�����ţ�ǰһ����ţ�Ȩ�أ�������line�ϣ� ��get<1> ָ��get<0>
using EDGE = vector<N>;  //�Ѿ���õ����·���ĵ�ļ���

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

