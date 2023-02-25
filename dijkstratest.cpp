#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#define INF 1e6
using namespace std;

string mapname = "makemap/map005.txt";
string inputfile = "test/seed005/seed005w1k10.txt";



struct vec2 
{
    int y, x;
    bool operator<(const vec2& v2) const{
        return x == v2.x ? y<v2.y : x<v2.x;
    }
};

struct Dijkstra
{
    int n;
    vector<vector<int>> Map;
    vector<vec2> WaterPos, HousePos, dxdy;
    vector<vector<bool>> isWaterPos;
    Dijkstra(int N, vector<vec2>& waterpos, const vector<vec2>& housepos)
    : n(N), WaterPos(waterpos), HousePos(housepos), Map(n, vector<int>(n, 0)), dxdy({{0,1},{0,-1},{1,0},{-1,0}}), isWaterPos(N, vector<bool>(N,false)) { }

    //家を始点、水源までの最短距離を求める。キューから取り出した点が水源であれば探索を終了。パスを求め、親と破壊コストの配列を返して終了。
    vector<pair<vec2, int>> searchmin(vec2 house){
        for(auto water:WaterPos) isWaterPos[water.y][water.x] = true; // 水源を確認

        vector<pair<vec2, int>> path; //親の座標、破壊に必要なパワー。マップ値をパワーに設定する。
        vector<vector<bool>> isminimum(n, vector<bool>(n, false)); //最小値が決定しているか
        vector<vector<int>> cost(n, vector<int>(n, INF)); //各点のコスト
        vector<vector<vec2>> parent(n, vector<vec2>(n, {-1,-1}));//各点の親

        priority_queue<pair<int, vec2>, vector<pair<int, vec2>>, greater<pair<int, vec2>>> Pque;
        cost[house.y][house.x] = 0;
        parent[house.y][house.x] = {house.y, house.x};//houseの親はhouse
        Pque.push({0, house});

        while(true){
            pair<int, vec2> pv = Pque.top();
            Pque.pop();

            cout << pv.second.y << " " << pv.second.x << " " <<  pv.first << endl;
            if(isWaterPos[pv.second.y][pv.second.x]){
                path.push_back({{pv.second.y, pv.second.x}, Map[pv.second.y][pv.second.x]});
                break;
            }
            
            for(auto nv:dxdy){
                int nvy = pv.second.y + nv.y;
                int nvx = pv.second.x + nv.x;
                if(nvy<0 || nvy>=n || nvx<0 || nvx>=n || isminimum[nvy][nvx]) continue;

                //コストの比較
                int newcost = min(cost[nvy][nvx], cost[pv.second.y][pv.second.x] + Map[nvy][nvx]);
                if(cost[nvy][nvx] > newcost){
                    cost[nvy][nvx] = newcost;
                    parent[nvy][nvx] = {pv.second.y, pv.second.x};
                }
                Pque.push({cost[nvy][nvx], {nvy, nvx}});
            }

            isminimum[pv.second.y][pv.second.x] = true; //最短距離確定
        }
        vec2 pathv = path[0].first;
        cout << pathv.y << " " << pathv.x << " " << cost[pathv.y][pathv.x] << endl;
        // while(pathv.y!=parent[pathv.y][pathv.x].y || pathv.x!=parent[pathv.y][pathv.x].x){//houseの親はhouse
        //     int pvy = parent[pathv.y][pathv.x].y;
        //     int pvx = parent[pathv.y][pathv.x].x;
        //     path.push_back({{pvy, pvx}, Map[pvy][pvx]});
        // }
        return path;
    }

    void readmap(){
        ifstream mapfile(mapname);
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                mapfile >> Map[i][j];
            }
        }
    }
};

int main(){
    ifstream InputFile(inputfile);
    int n, w, k, c;
    InputFile >> n >> w >> k >> c;

    cout << n << " " << w << " " << k << " " << c << endl;
    vector<vector<int>> DestLevel(n, vector<int>(n));
    vector<vec2> WaterPos(w), HousePos(k); 

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            InputFile >> DestLevel[i][j];
        }
    }
    for(int i=0; i<w; i++) InputFile >> WaterPos[i].y >> WaterPos[i].x;
    for(int i=0; i<k; i++) InputFile >> HousePos[i].y >> HousePos[i].x;

    cout << "OK" << endl;
    return 0;
}