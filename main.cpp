#include <iostream>
#include <string>
#include "bellman_ford.cpp"
#include "Dijkstra.cpp"



int main() {

    Graph graph;
    graph.initGraph();
    graph.generate_graph();
    string  station1="江湾镇", station2="人民广场";
    int cd_int;


        // 用于直接输出地铁站信息，和每个地铁站的邻接矩阵
        //    cout<<graph.find_station_order("世纪大道")<<endl;


    //string， 对于debug， main要改成utf8(同时文件也为utf8)，否则用GBK,
    while(1){
        cout<<"请选择： 0：退出；  1：输入起点和终点"<<endl;
        cin>>cd_int;
        if(cd_int == 0){
            break;
        }
        else if(cd_int == 1){
            cout<<"输入起点和终点 中间用空格；"<<endl;
            cin>>station1;
            cin>>station2;
            //站点是否存在
            if(!graph.isExist(station1) || !graph.isExist(station2)){
                cout<<"not a subway station name"<<endl;
                exit(0);
            }
            cout<<"选择算法： 1：Dijkstra；   2：Bellman_ford"<<endl;
            cin>>cd_int;
            if(cd_int==1)
                graph_dijkstra(graph,station1,station2);
            else if(cd_int==2)
                graph_bellman(graph,station1, station2);
        }
        else {
            cout<<"重输!"<<endl;
        }


    }
    return 0;

}
