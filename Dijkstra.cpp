// c++14
#include <vector>
#include <list>
#include <stack>
#include <tuple>
#include <string>
#include "graph.h"


//using std::string;
//using std::vector;
//using std::cin;
//using std::cout;
//using std::endl;
//using std::tuple;
//using std::get;
//using std::make_tuple;
//using std::list;
//using std::stack;
//using std::make_pair;



using Matrix = vector<vector<Tuple>>;  //���Ӿ���
using Matrix_int = vector<vector<uint>>; // �������Dijkstra��int��
using SVex = vector<tuple<string, uint, string, uint>>;  //�Ѿ���õ����·���ĵ�ļ��ϣ� Ԫ���У�����������source��̾��룬��̾����е���һ����
using UVex = list<tuple<string, uint, string, uint>>;  //δ�����ļ��ϵ�
using EVex = tuple<string, uint, string, uint>; // ÿ������������������source��̾��룬��̾������һ����������֣� ���Ǹ�����֮�����ڼ�������


void run_dijkstra();

int find_order_in_graph(const Matrix& gh, const string& stationName) {
    int order = 0;
    while (get<0>(gh[0][order]) != stationName)
        order++;
    return order;
}
int find_order_in_SVex(const SVex& sv, const string& name) {
    int order = 0;
    while(get<0>(sv[order]) != name)
        order++;
    return order;
}


/**
 * ��δ�����Ķ���������һ��������̵� EVex
 * @param uVex δ�����Ķ���
 * @return  ��һ��������̵ĵ�
 */
EVex findShortest(const UVex &uVex) {
    uint minDistance = UINT_MAX;
    EVex shortest;
    int count = 0;
    for (const auto &node: uVex) {
        //cout<<count++<<" "<<get<1>(node)<<endl;
        if (get<1>(node) != UINT_MAX && get<1>(node) <= minDistance) {  // δ�������У�tuple�ĵ�һ��Ԫ�� С����С����
            minDistance = get<1>(node);
            shortest = node;
            //cout<<get<0>(node)<<endl;
        }
    }
    return shortest;
}

/**
 * Dijkstra�㷨�ҵ���ʼ�����̾���
 * @param graph ���Ӿ���
 * @param source ��ʼ��
 * @return ����ÿ���㵽��ʼ�����̾��������
 */
SVex Dijkstra(const Matrix& G, int source_number) {
    int Vsize = G.size(); //�������
    SVex sVex;
    UVex uVex;
    //initialize sVex and uVex
    for(int i = 0; i < G[0].size(); i++) {
        if (get<0>(G[0][i]) == get<0>(G[0][source_number]))
            sVex.emplace_back(get<0>(G[0][i]), 0, get<0>(G[0][source_number]),0); //���i������ʼ�㣬����sourceΪ0����һ������ҲΪsource
        else
            uVex.emplace_back(get<0>(G[0][i]), get<1>(G[source_number][i]), get<0>(G[0][source_number]),get<2>(G[source_number][i]));
    }

    while (!uVex.empty()) {
        auto nextVex = findShortest(uVex); //�ҵ�λ�������о�����ʼ�������С�ĵ�
        //cout<<get<0>(nextVex)<<" next"<<endl;
        uVex.erase(find(uVex.begin(), uVex.end(), nextVex)); //��U��ɾ��
        sVex.emplace_back(nextVex);
        int next_order = find_order_in_graph(G,get<0>(nextVex));
        // ����U
        int k = 0;
        for (auto &node: uVex) {
            // distance[source->nextVex] + distance[nextVex->node]  <? distance[source->node]
            //  get<1>node = distance[source->node]
            int node_order = find_order_in_graph(G,get<0>(node));
            if (get<1>(G[next_order][node_order]) != UINT_MAX &&
                    get<1>(nextVex) + get<1>(G[next_order][node_order]) < get<1>(node))
            {
                get<1>(node) = get<1>(nextVex) + get<1>(G[next_order][node_order]);
                get<2>(node) = get<0>(nextVex); //update the node's last node to nextVex
                get<3>(node) = get<2>(G[next_order][node_order]);
                //cout<<get<0>(node)<<" -> "<<get<2>(node)<<get<1>(node)<<endl;
            }
            k++;
        }
    }
    return sVex;
}

void print_test(const SVex& sv) {
    stack<string> tracks;  //��β��������ʹ��stack��ÿ����������·���е�ǰһ��������ջ��Ȼ���ջ��˳��������·��˳��
    // ��һ��Ԫ������ʼ�㣬�ӵڶ���Ԫ�ؽ��д�ӡ���
    for (auto it = ++sv.begin(); it != sv.end(); ++it) {
        // ��ӡͷ����Ϣ
        cout<<get<0>(sv[0])<<" -> "<<get<0>(*it)<<"\t Length: "<<get<1>(*it)<<"\t Paths: "<<get<0>(sv[0]);

        auto pointer = *it;
        // �����ǰָ��pointerָ��Ľڵ�����;�ڵ㣨�жϵ����������·���е�ǰһ���ڵ㲻����ʼ�㣩
        while (get<2>(pointer) != get<0>(sv[0])) {
            tracks.push(get<0>(pointer));
            // Lambda���ʽ��ʹ��find_if�����ѵ�ǰ�����ǰһ�������paths���ҳ�����������ѭ��ֱ��ǰһ���ڵ������ʼ��
            auto condition = [pointer](EVex x)
                    { return get<0>(x) == get<2>(pointer); };
            pointer = *find_if(sv.begin(), sv.end(), condition);
        }
        tracks.push(get<0>(pointer));

        // �Գ�ջ��˳����д�ӡ���
        while (!tracks.empty()) {
            cout<<" -> "<<tracks.top();
            tracks.pop();
        }
        cout<<endl;
    }
}

void all_print(vector<Pair> answer) {
    int line_count = 0, line = answer[answer.size()-1].second;
    for(int i = (int)answer.size()-1; i>=0; i--){
        if(answer[i].second !=line){
            line = answer[i].second;
            line_count++;
        }
    }
    for(int i = (int)answer.size()-1; i>=0; i--) {
        cout<<" - line "<<answer[i].second<<" - "<<answer[i].first;
    }
    cout<<endl;
    cout<<"�ۼƻ���"<<line_count<<"��"<<endl;
}
void simple_print(vector<Pair> answer) {
    int line = answer[answer.size()-1].second;
    int line_count = 0;
    string name = answer[answer.size()-1].first;
    for(int i = (int)answer.size()-1; i>=0; i--){
        if(answer[i].second !=line){
            cout<<" - line "<<line<<" - "<<name;
            line = answer[i].second;
            line_count++;
        }
        name = answer[i].first;
        if(i == 0)
            cout<<" - line "<<line<<" - "<<name<<endl;
    }
    cout<<"�ۼƻ���"<<line_count<<"��"<<endl;
}
void print(const SVex& sv, string destination) {
    vector<Pair> answer; // ������𣬰��մӺ���ǰ��վ����
    int destination_order = find_order_in_SVex(sv, destination);
    auto pointer = sv[destination_order];
    answer.emplace_back(Pair(get<0>(pointer),get<3>(pointer)));
    while(get<2>(pointer) != get<0>(sv[0])) {
        auto condition = [pointer](tuple<string, uint, string, uint> x)
        { return get<0>(x) == get<2>(pointer); };
        pointer = *find_if(sv.begin(), sv.end(), condition);
        answer.emplace_back(Pair(get<0>(pointer),get<3>(pointer)));
    }
    // �Ե����˳����д�ӡ���
    while(1) {
        cout << "ѡ�������ʽ�� 1�� ȫ�����룻 2��������м�վ�㣻  0���˳�" << endl;
        int cd;
        cin >> cd;
        if (cd == 1) {
            cout<<get<0>(sv[0])<<" -> "<<get<0>(sv[destination_order])<<"\t Ԥ��ͨ��ʱ��: "<<get<1>(sv[destination_order])<<"����"<<endl;
            cout << "Path: " << get<0>(sv[0]);
            all_print(answer);
        }
        else if (cd == 2) {
            cout<<get<0>(sv[0])<<" -> "<<get<0>(sv[destination_order])<<"\t Ԥ��ͨ��ʱ��: "<<get<1>(sv[destination_order])<<"����"<<endl;
            cout << "Path: " << get<0>(sv[0]);
            simple_print(answer);
        }
        else if(cd == 0)
            break;
        else
            cout << "����"<<endl;
    }

}


void run_dijkstra() { //test
    Matrix_int graph = {
            {0,        12,       UINT_MAX, UINT_MAX, UINT_MAX, 16, 14},
            {12,       0,        10,       UINT_MAX, UINT_MAX, 7, UINT_MAX},
            {UINT_MAX, 10,       0, 3,               5,        6, UINT_MAX},
            {UINT_MAX, UINT_MAX, 3, 0,               4, UINT_MAX, UINT_MAX},
            {UINT_MAX, UINT_MAX, 5, 4,               0,        2,  8},
            {16,       7,        6,        UINT_MAX, 2,        9,  9},
            {14,       UINT_MAX, UINT_MAX, UINT_MAX, 8,        9,  0}
    };  // ͼ��Ӧ�����Ӿ���

//    auto results = Dijkstra(graph, uint('D' - 65));          // ѡȡ����C����д��ĸA��ASCII������65��
//    print(results);     // ��ӡ������

}

void graph_dijkstra(const Graph& g,const string & source_name, const string & destination) {
    int source_order = find_order_in_graph(g.GRAPH, source_name);
    auto results = Dijkstra(g.GRAPH, source_order);
    // print(results);  ȫ�������source��ÿ����
    print(results, destination);
}



