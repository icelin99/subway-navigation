//
// Created by shirley ICE on 1/3/22.
//

#ifndef PJ2_GRAPH_H
#define PJ2_GRAPH_H

// 在debug时，如果要看结果，需要把txt文件编码改成utf8， 如果是在console输出，需要把txt编码改成gbk
// 此处，改成了GBK
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

const int MAX_NUM = 350;

typedef  struct Node{  // station
    string name; // 站点名称
    int line; // 在几号线
    int time2first; // 到首发站的时间
    Node *prev; // 前一个点
    int time2prev; //距离前一个点的时间（weight）
}Node;

typedef pair<string, int> Pair; // <名字，权重>, 考虑到两个数据不够, 更新为tuple
typedef tuple<string, uint, uint> Tuple; // <站名，与相邻站的权重，与相邻站在哪条地铁线上>







class Graph{
public:
    Node node[MAX_NUM];
    int lineNum; // 顶点数量，边数量
    vector<string> station; // 总共的地铁站个数
    vector<vector<Tuple>> GRAPH; // 地图数据
    vector<vector<Node>> list;

    /**
     * 初始化图,生成graph
     */
    void initGraph()
    {
        ifstream fin;
        fin.open("../subway.txt");
        if(!fin.is_open()) {
            cout << "file cannot open!" << endl;
            exit(0);
        }
        string s,a, station_name;
        int base_line = 0;
        int list_row = 0, list_col = 0;
        this->lineNum = 0;
        for(int i = 0; ; i++) {
            if(fin.peek() == EOF)
                break;
            getline(fin,s);
            stringstream ss(s);
            ss >> a;
            ss >> node[i].line; // line几号线
            ss >> station_name; // 车站名
            ss >> node[i].time2first; // 距离首发站的时长（分钟）
            node[i].name = station_name;

            if(base_line != node[i].line) { // 到下一条线了
                base_line = node[i].line;
                node[i].time2prev = 0; // 距离始发的weight=0
                node[i].prev = nullptr;
                //list
                this->lineNum++;
                list.emplace_back(vector<Node>());  // 新加一行

            }
            else { //在同一条线上
                // 处理10号线和11号线的特殊情况
                if(node[i].name=="龙柏新村") {
                    node[i].time2prev = node[i].time2first - node[i-5].time2first;
                    node[i].prev = &node[i - 5];
                }
                else if(node[i].name =="上海赛车场") {
                    node[i].time2prev = node[i].time2first - node[i-4].time2first;
                    node[i].prev = &node[i - 4];
                }
                else if(node[i].name == "虹桥路" && node[i].line == 4) { // 4号线是环路
                    node[i].time2prev = node[i].time2first - node[i-1].time2first;
                    node[i].prev = &node[i - 1];
                    node[i-25].time2prev = 2;
                    node[i-25].prev = & node[i];
                    cout<<node[i-25].name<<" - "<<node[i].name<<" 环线"<<endl;
                    list[lineNum-1].emplace_back(node[i]);
                    // 检查站名，记录不重复的
                    int j;
                    for(j = 0; j < station.size(); j++) {
                        if (station[j] == station_name)
                            break;
                    }
                    if(j == station.size()) // 没有重复的
                        station.emplace_back(station_name);
                    list[lineNum-1].emplace_back(node[i-25]);
                    // 检查站名，记录不重复的
                    for(j = 0; j < station.size(); j++) {
                        if (station[j] == node[i-25].name)
                            break;
                    }
                    if(j == station.size()) // 没有重复的
                        station.emplace_back(node[i-25].name);

                }
                else if(station_name == "浦电路") { //浦电路的四号线和6号线是两个站
                    node[i].time2prev = node[i].time2first - node[i-1].time2first;
                    node[i].prev = &node[i - 1];
                    if(node[i].line == 4)
                        station_name = node[i].name = "浦电路4";
                    else if(node[i].line == 6)
                        station_name = node[i].name = "浦电路6";
                    cout<< node[i].name<<endl;
                }
                else {
                    node[i].time2prev = node[i].time2first - node[i-1].time2first;
                    node[i].prev = &node[i - 1];
                }
            }
            if(node[i].name != "宜山路" && node[i].name !="虹桥路"){ // 宜山路要等到这一条线都遍历完再插入
                list[lineNum-1].emplace_back(node[i]); // 将站点加入到list中. lineNum从1开始，但是列表是从0开始的.

                // 检查站名，记录不重复的
                int j;
                for(j = 0; j < station.size(); j++) {
                    if (station[j] == station_name)
                        break;
                }
                if(j == station.size()) // 没有重复的
                    station.emplace_back(station_name);
            }
//            cout<<"line: "<<node[i].line <<", order: "<<node[i].order<<
//                ", "<<node[i].name<<", time to first: "<<node[i].time2first<<endl;



    }



    // 下面的输出都用于debug， 可以去掉
        cout<<"station size: "<<station.size()<<endl;
//        for(auto ll : list) {
//            cout<<"line "<<ll[0].line<<" has "<<ll.size()<<" stations."<<endl;
//            for(auto l : ll) {
//                cout<<"line: "<<l.line<<" , "<<l.name<<" , time to previous: "<<l.time2prev<<endl;
//            }
//        }
//        cout<<"there are "<<list.size()<<" lines."<<endl;

    }

    /**
     * 按照station的大小生成graph
     */
    void generate_graph() {
        // resize初始化
        GRAPH.resize(station.size(), vector<Tuple>(station.size()));

        //按照station的顺序，把graph元素的第一个值赋进去
        for(int i = 0; i < GRAPH.size(); i++) {
            for(int j = 0; j < GRAPH[i].size(); j++) {
                get<0>(GRAPH[i][j]) = station[j];
                get<1>(GRAPH[i][j]) = (i == j) ? 0:UINT_MAX; //全部初始化为无符号的无穷大, 对角线上为0
                get<2>(GRAPH[i][j]) = 0; //初始化不在一条线上，则第三个变量为0
            }
        }

        // 按照list， 把graph的元素的第二个值填进去， graph[i][j]表示序号为i的站到序号为[j]的站所需要花费的时间
        // 如果两个站不相邻，则weight为无穷大
        // 是否相邻按照node的prev来判断, node[j].prev = i, 则相邻
        for(auto & line_i : list) {
            for(int line_node = (int)line_i.size()-1; line_node >= 0; line_node--) { //line_node>0是因为是首发站，没有prev
                // 遍历station找站对应的序号
                int prev_order = 0, order = 0;
                if(line_node ==0 && line_i[0].line != 4)
                    continue;
                for(int station_i = 0; station_i < station.size(); station_i++) {
                    if(station[station_i] == line_i[line_node].name)
                        order = station_i;
                    else if(station[station_i] == line_i[line_node].prev->name)
                        prev_order = station_i;
                }
                get<1>(GRAPH[prev_order][order]) = line_i[line_node].time2prev;
                get<1>(GRAPH[order][prev_order]) = line_i[line_node].time2prev;
                get<2>(GRAPH[order][prev_order]) = line_i[line_node].line;
                get<2>(GRAPH[prev_order][order]) = line_i[line_node].line;
            }
        }

//      输出用于debug， 可以去掉。。。
//        for(const auto& graph_row : GRAPH) {
//            for(const auto& graph_col : graph_row) {
//                cout<<get<2>(graph_col)<<", ";
//            }
//            cout<<endl;
//        }
    }

    /**
     * 查找某一地铁站，并打印其相邻的站点个数个站点名称
     * @param stationName
     * @return 返回地铁站在graph中的序号
     */
    int find_station_order(const string& stationName) {
        int order = 0;
        while(station[order] != stationName)
            order++;
        //输出跟他相连的顶点的个数
        int count = 0;
        vector<string> adjacency;
        for(const auto& x: GRAPH[order]){
            if(get<1>(x) !=UINT_MAX && get<1>(x)!= 0) {
                count++;
                adjacency.emplace_back(get<0>(x));
            }
        }
        cout<<stationName<<" 连接了"<<count<<"个站点"<<endl;
        cout<<"分别是：";
        for(const auto& x: adjacency)
            cout<<x<<", ";
        cout<<endl;
        return order;

    }

    bool isExist(const string& name) {
        for(const auto & x:station)
            if(x == name)
                return true;
        return false;
    }

};



#endif //PJ2_GRAPH_H
