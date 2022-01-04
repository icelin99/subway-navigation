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



using Matrix = vector<vector<Tuple>>;  //连接矩阵
using Matrix_int = vector<vector<uint>>; // 最基础的Dijkstra是int型
using SVex = vector<tuple<string, uint, string, uint>>;  //已经获得的最短路径的点的集合， 元素有：顶点名，到source最短距离，最短距离中的上一个点
using UVex = list<tuple<string, uint, string, uint>>;  //未遍历的集合点
using EVex = tuple<string, uint, string, uint>; // 每个结点包含顶点名，到source最短距离，最短距离的上一个顶点的名字， 与那个顶点之间是在几号线上


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
 * 从未遍历的顶点中找下一个距离最短的 EVex
 * @param uVex 未遍历的顶点
 * @return  下一个距离最短的点
 */
EVex findShortest(const UVex &uVex) {
    uint minDistance = UINT_MAX;
    EVex shortest;
    int count = 0;
    for (const auto &node: uVex) {
        //cout<<count++<<" "<<get<1>(node)<<endl;
        if (get<1>(node) != UINT_MAX && get<1>(node) <= minDistance) {  // 未遍历点中，tuple的第一个元素 小于最小距离
            minDistance = get<1>(node);
            shortest = node;
            //cout<<get<0>(node)<<endl;
        }
    }
    return shortest;
}

/**
 * Dijkstra算法找到起始点的最短距离
 * @param graph 连接矩阵
 * @param source 起始点
 * @return 返回每个点到起始点的最短距离的数组
 */
SVex Dijkstra(const Matrix& G, int source_number) {
    int Vsize = G.size(); //顶点个数
    SVex sVex;
    UVex uVex;
    //initialize sVex and uVex
    for(int i = 0; i < G[0].size(); i++) {
        if (get<0>(G[0][i]) == get<0>(G[0][source_number]))
            sVex.emplace_back(get<0>(G[0][i]), 0, get<0>(G[0][source_number]),0); //如果i就是起始点，距离source为0。上一个顶点也为source
        else
            uVex.emplace_back(get<0>(G[0][i]), get<1>(G[source_number][i]), get<0>(G[0][source_number]),get<2>(G[source_number][i]));
    }

    while (!uVex.empty()) {
        auto nextVex = findShortest(uVex); //找到位遍历点中距离起始点距离最小的点
        //cout<<get<0>(nextVex)<<" next"<<endl;
        uVex.erase(find(uVex.begin(), uVex.end(), nextVex)); //从U中删除
        sVex.emplace_back(nextVex);
        int next_order = find_order_in_graph(G,get<0>(nextVex));
        // 更新U
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
    stack<string> tracks;  //从尾部出发，使用stack将每个顶点的最短路径中的前一个顶点入栈，然后出栈的顺序就是最短路径顺序
    // 第一个元素是起始点，从第二个元素进行打印输出
    for (auto it = ++sv.begin(); it != sv.end(); ++it) {
        // 打印头部信息
        cout<<get<0>(sv[0])<<" -> "<<get<0>(*it)<<"\t Length: "<<get<1>(*it)<<"\t Paths: "<<get<0>(sv[0]);

        auto pointer = *it;
        // 如果当前指针pointer指向的节点有中途节点（判断的条件是最短路径中的前一个节点不是起始点）
        while (get<2>(pointer) != get<0>(sv[0])) {
            tracks.push(get<0>(pointer));
            // Lambda表达式，使用find_if函数把当前顶点的前一个顶点从paths中找出来继续进行循环直到前一个节点就是起始点
            auto condition = [pointer](EVex x)
                    { return get<0>(x) == get<2>(pointer); };
            pointer = *find_if(sv.begin(), sv.end(), condition);
        }
        tracks.push(get<0>(pointer));

        // 以出栈的顺序进行打印输出
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
    cout<<"累计换乘"<<line_count<<"次"<<endl;
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
    cout<<"累计换乘"<<line_count<<"次"<<endl;
}
void print(const SVex& sv, string destination) {
    vector<Pair> answer; // 输出倒叙，按照从后往前把站存入
    int destination_order = find_order_in_SVex(sv, destination);
    auto pointer = sv[destination_order];
    answer.emplace_back(Pair(get<0>(pointer),get<3>(pointer)));
    while(get<2>(pointer) != get<0>(sv[0])) {
        auto condition = [pointer](tuple<string, uint, string, uint> x)
        { return get<0>(x) == get<2>(pointer); };
        pointer = *find_if(sv.begin(), sv.end(), condition);
        answer.emplace_back(Pair(get<0>(pointer),get<3>(pointer)));
    }
    // 以倒叙的顺序进行打印输出
    while(1) {
        cout << "选择输出格式： 1： 全部输入； 2：不输出中间站点；  0：退出" << endl;
        int cd;
        cin >> cd;
        if (cd == 1) {
            cout<<get<0>(sv[0])<<" -> "<<get<0>(sv[destination_order])<<"\t 预计通行时间: "<<get<1>(sv[destination_order])<<"分钟"<<endl;
            cout << "Path: " << get<0>(sv[0]);
            all_print(answer);
        }
        else if (cd == 2) {
            cout<<get<0>(sv[0])<<" -> "<<get<0>(sv[destination_order])<<"\t 预计通行时间: "<<get<1>(sv[destination_order])<<"分钟"<<endl;
            cout << "Path: " << get<0>(sv[0]);
            simple_print(answer);
        }
        else if(cd == 0)
            break;
        else
            cout << "重输"<<endl;
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
    };  // 图对应的连接矩阵

//    auto results = Dijkstra(graph, uint('D' - 65));          // 选取顶点C（大写字母A的ASCII编码是65）
//    print(results);     // 打印输出结果

}

void graph_dijkstra(const Graph& g,const string & source_name, const string & destination) {
    int source_order = find_order_in_graph(g.GRAPH, source_name);
    auto results = Dijkstra(g.GRAPH, source_order);
    // print(results);  全部输出从source到每个点
    print(results, destination);
}



