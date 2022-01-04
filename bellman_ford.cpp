#include <vector>
#include <string>
#include <tuple>
#include <list>
#include <iostream>
#include "bellman_ford.h"



int find_order(const Matrix& gh, const string& stationName) {
    int order = 0;
    while (get<0>(gh[0][order]) != stationName)
        order++;
    return order;
}

int print_all(const vector<tuple<string,string,int,int>>& vt) {
    int line_count= 0, line = 0;
    for(int i = vt.size()-1; i>=0; i--){
        cout<<get<1>(vt[i])<<" - line "<<get<3>(vt[i])<<" - ";
        if(line != get<3>(vt[i])){
            line = get<3>(vt[i]);
            line_count++;
        }
    }
    return line_count-1;
}

int print_simple(const vector<tuple<string,string,int,int>>& vt) { // �����ۼƻ��˴���
    int line = 0, line2 , line_count = 0;
    for(int i = vt.size()-1; i>=0; i--){
        line2 = get<3>(vt[i]);
        if(line != line2){
            cout<<get<1>(vt[i])<<" - line "<<get<3>(vt[i])<<" - ";
            line = line2;
            line_count++;
        }
    }
    return line_count-1;
}

void Bellman::insert_edge(const Matrix &G) {
    for (int g_i = 0; g_i < G.size(); g_i++) {
        for (int g_j = g_i + 1; g_j < G[g_i].size(); g_j++) { // ֻ��Ҫȡ�Խ����ϰ벿��
            if (get<1>(G[g_i][g_j]) != UINT_MAX) {
                //����
                // (from, to, weight, line)
                Edge.emplace_back(g_i, g_j, get<1>(G[g_i][g_j]), get<2>(G[g_i][g_j]));
                Edge.emplace_back(g_j, g_i, get<1>(G[g_i][g_j]), get<2>(G[g_i][g_j]));
                // ��Ȩͼ��from��to��Ҫinsertһ��
            }
        }
    }
}

void Bellman::bellman_ford(const Matrix &G, const string &source_name, const string &destination,
                           const vector<string> &station)  {
    vector<int> distance; // (�Լ������֣�����source�ľ��룬 ��һ��������line��)
    // distance���±���graph��station��˳��һ��
    bool updated = false;
    distance.resize(vertex);
    for (int i = 0; i < vertex; i++) {
        distance[i] = INT_MAX;
    }

    int source_order = find_order(G, source_name);
    // ��㵽������Ϊ0, source_name�������source_order
    distance[source_order] = 0;
    list<tuple<string, string, int, int>> node_list[vertex];
    vector<tuple<string, string, int,int>> nn;
    for(int i = 0; i < vertex; i++)
        nn.emplace_back(tuple<string, string, int,int>());
    nn[source_order] = make_tuple(source_name,source_name,0,0);
    for (int i = 0; i < vertex; i++) {
        updated = false;
        for (const auto &e :Edge) {
            int me = get<0>(e), prev = get<1>(e), e_weight = get<2>(e), e_line = get<3>(e);
            if (distance[prev] != INT_MAX && me!= source_order &&
                    (distance[prev] + e_weight < distance[me])) { // e will be counted, and e.to will be updated
                distance[me] = distance[prev] + e_weight;
                updated = true;
                if(me == find_order(G,destination))
                    node_list[me].emplace_back(make_tuple(station[me], station[prev], e_weight, e_line));
                else
                    node_list[me].emplace_back(make_tuple(station[me], station[prev], e_weight, e_line));
                nn[me] = make_tuple(station[me], station[prev], e_weight, e_line);
            }
        }
        if (!updated) {  //Until no more updates, out of the loop
            break;
        }
    }
    if (updated) {
        cout << "There is a negative cycle." << endl;
    } else {
        // �����·��
        int des_order = find_order(G, destination);
        vector<tuple<string,string,int,int>> answer;
        auto pointer = nn[des_order];
        answer.emplace_back(pointer);
        while(get<1>(pointer) != get<0>(nn[source_order])) {
            auto condition = [pointer](tuple<string, string, int, int> x) { return get<0>(x) == get<1>(pointer); };
            pointer = *find_if(nn.begin(), nn.end(), condition);
            answer.emplace_back(pointer);
        }
        while(1) {
            cout << "ѡ�������ʽ�� 1�� ȫ�����룻 2��������м�վ�㣻  0���˳�" << endl;
            int cd;
            cin >> cd;
            if (cd == 1) {
                cout<<source_name<<" -> "<<destination<<" , Ԥ��ͨ��ʱ��: "<<distance[des_order]<<"����"<<endl;
                int line_count = print_all(answer);
                cout << destination << endl;
                cout<<"�ۼƻ���"<<line_count<<"��"<<endl;
            }
            else if (cd == 2) {
                cout<<source_name<<" -> "<<destination<<" , Ԥ��ͨ��ʱ��: "<<distance[des_order]<<"����"<<endl;
                int line_count = print_simple(answer);
                cout << destination << endl;
                cout<<"�ۼƻ���"<<line_count<<"��"<<endl;
            }
            else if(cd == 0)
                break;
            else
                cout << "����"<<endl;
        }

    }

}





