#include <iostream>
#include <string>
#include "bellman_ford.cpp"
#include "Dijkstra.cpp"



int main() {

    Graph graph;
    graph.initGraph();
    graph.generate_graph();
    string  station1="������", station2="����㳡";
    int cd_int;


        // ����ֱ���������վ��Ϣ����ÿ������վ���ڽӾ���
        //    cout<<graph.find_station_order("���ʹ��")<<endl;


    //string�� ����debug�� mainҪ�ĳ�utf8(ͬʱ�ļ�ҲΪutf8)��������GBK,
    while(1){
        cout<<"��ѡ�� 0���˳���  1�����������յ�"<<endl;
        cin>>cd_int;
        if(cd_int == 0){
            break;
        }
        else if(cd_int == 1){
            cout<<"���������յ� �м��ÿո�"<<endl;
            cin>>station1;
            cin>>station2;
            //վ���Ƿ����
            if(!graph.isExist(station1) || !graph.isExist(station2)){
                cout<<"not a subway station name"<<endl;
                exit(0);
            }
            cout<<"ѡ���㷨�� 1��Dijkstra��   2��Bellman_ford"<<endl;
            cin>>cd_int;
            if(cd_int==1)
                graph_dijkstra(graph,station1,station2);
            else if(cd_int==2)
                graph_bellman(graph,station1, station2);
        }
        else {
            cout<<"����!"<<endl;
        }


    }
    return 0;

}
