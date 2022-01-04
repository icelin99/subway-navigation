//
// Created by shirley ICE on 1/3/22.
//

#ifndef PJ2_GRAPH_H
#define PJ2_GRAPH_H

// ��debugʱ�����Ҫ���������Ҫ��txt�ļ�����ĳ�utf8�� �������console�������Ҫ��txt����ĳ�gbk
// �˴����ĳ���GBK
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

const int MAX_NUM = 350;

typedef  struct Node{  // station
    string name; // վ������
    int line; // �ڼ�����
    int time2first; // ���׷�վ��ʱ��
    Node *prev; // ǰһ����
    int time2prev; //����ǰһ�����ʱ�䣨weight��
}Node;

typedef pair<string, int> Pair; // <���֣�Ȩ��>, ���ǵ��������ݲ���, ����Ϊtuple
typedef tuple<string, uint, uint> Tuple; // <վ����������վ��Ȩ�أ�������վ��������������>







class Graph{
public:
    Node node[MAX_NUM];
    int lineNum; // ����������������
    vector<string> station; // �ܹ��ĵ���վ����
    vector<vector<Tuple>> GRAPH; // ��ͼ����
    vector<vector<Node>> list;

    /**
     * ��ʼ��ͼ,����graph
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
            ss >> node[i].line; // line������
            ss >> station_name; // ��վ��
            ss >> node[i].time2first; // �����׷�վ��ʱ�������ӣ�
            node[i].name = station_name;

            if(base_line != node[i].line) { // ����һ������
                base_line = node[i].line;
                node[i].time2prev = 0; // ����ʼ����weight=0
                node[i].prev = nullptr;
                //list
                this->lineNum++;
                list.emplace_back(vector<Node>());  // �¼�һ��

            }
            else { //��ͬһ������
                // ����10���ߺ�11���ߵ��������
                if(node[i].name=="�����´�") {
                    node[i].time2prev = node[i].time2first - node[i-5].time2first;
                    node[i].prev = &node[i - 5];
                }
                else if(node[i].name =="�Ϻ�������") {
                    node[i].time2prev = node[i].time2first - node[i-4].time2first;
                    node[i].prev = &node[i - 4];
                }
                else if(node[i].name == "����·" && node[i].line == 4) { // 4�����ǻ�·
                    node[i].time2prev = node[i].time2first - node[i-1].time2first;
                    node[i].prev = &node[i - 1];
                    node[i-25].time2prev = 2;
                    node[i-25].prev = & node[i];
                    cout<<node[i-25].name<<" - "<<node[i].name<<" ����"<<endl;
                    list[lineNum-1].emplace_back(node[i]);
                    // ���վ������¼���ظ���
                    int j;
                    for(j = 0; j < station.size(); j++) {
                        if (station[j] == station_name)
                            break;
                    }
                    if(j == station.size()) // û���ظ���
                        station.emplace_back(station_name);
                    list[lineNum-1].emplace_back(node[i-25]);
                    // ���վ������¼���ظ���
                    for(j = 0; j < station.size(); j++) {
                        if (station[j] == node[i-25].name)
                            break;
                    }
                    if(j == station.size()) // û���ظ���
                        station.emplace_back(node[i-25].name);

                }
                else if(station_name == "�ֵ�·") { //�ֵ�·���ĺ��ߺ�6����������վ
                    node[i].time2prev = node[i].time2first - node[i-1].time2first;
                    node[i].prev = &node[i - 1];
                    if(node[i].line == 4)
                        station_name = node[i].name = "�ֵ�·4";
                    else if(node[i].line == 6)
                        station_name = node[i].name = "�ֵ�·6";
                    cout<< node[i].name<<endl;
                }
                else {
                    node[i].time2prev = node[i].time2first - node[i-1].time2first;
                    node[i].prev = &node[i - 1];
                }
            }
            if(node[i].name != "��ɽ·" && node[i].name !="����·"){ // ��ɽ·Ҫ�ȵ���һ���߶��������ٲ���
                list[lineNum-1].emplace_back(node[i]); // ��վ����뵽list��. lineNum��1��ʼ�������б��Ǵ�0��ʼ��.

                // ���վ������¼���ظ���
                int j;
                for(j = 0; j < station.size(); j++) {
                    if (station[j] == station_name)
                        break;
                }
                if(j == station.size()) // û���ظ���
                    station.emplace_back(station_name);
            }
//            cout<<"line: "<<node[i].line <<", order: "<<node[i].order<<
//                ", "<<node[i].name<<", time to first: "<<node[i].time2first<<endl;



    }



    // ��������������debug�� ����ȥ��
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
     * ����station�Ĵ�С����graph
     */
    void generate_graph() {
        // resize��ʼ��
        GRAPH.resize(station.size(), vector<Tuple>(station.size()));

        //����station��˳�򣬰�graphԪ�صĵ�һ��ֵ����ȥ
        for(int i = 0; i < GRAPH.size(); i++) {
            for(int j = 0; j < GRAPH[i].size(); j++) {
                get<0>(GRAPH[i][j]) = station[j];
                get<1>(GRAPH[i][j]) = (i == j) ? 0:UINT_MAX; //ȫ����ʼ��Ϊ�޷��ŵ������, �Խ�����Ϊ0
                get<2>(GRAPH[i][j]) = 0; //��ʼ������һ�����ϣ������������Ϊ0
            }
        }

        // ����list�� ��graph��Ԫ�صĵڶ���ֵ���ȥ�� graph[i][j]��ʾ���Ϊi��վ�����Ϊ[j]��վ����Ҫ���ѵ�ʱ��
        // �������վ�����ڣ���weightΪ�����
        // �Ƿ����ڰ���node��prev���ж�, node[j].prev = i, ������
        for(auto & line_i : list) {
            for(int line_node = (int)line_i.size()-1; line_node >= 0; line_node--) { //line_node>0����Ϊ���׷�վ��û��prev
                // ����station��վ��Ӧ�����
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

//      �������debug�� ����ȥ��������
//        for(const auto& graph_row : GRAPH) {
//            for(const auto& graph_col : graph_row) {
//                cout<<get<2>(graph_col)<<", ";
//            }
//            cout<<endl;
//        }
    }

    /**
     * ����ĳһ����վ������ӡ�����ڵ�վ�������վ������
     * @param stationName
     * @return ���ص���վ��graph�е����
     */
    int find_station_order(const string& stationName) {
        int order = 0;
        while(station[order] != stationName)
            order++;
        //������������Ķ���ĸ���
        int count = 0;
        vector<string> adjacency;
        for(const auto& x: GRAPH[order]){
            if(get<1>(x) !=UINT_MAX && get<1>(x)!= 0) {
                count++;
                adjacency.emplace_back(get<0>(x));
            }
        }
        cout<<stationName<<" ������"<<count<<"��վ��"<<endl;
        cout<<"�ֱ��ǣ�";
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
