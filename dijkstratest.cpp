#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <tuple>
#define INF 1e6
using namespace std;

string mapname = "makemap/mapdata022.txt";
string inputfile = "test/seed022/seed022w1k10.txt";

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
    vector<pair<vec2, int>> searchmin(vec2 house, vec2 water){
        //探索範囲の削減。最も近い水源と家の座標を用いる。いらない。
        int miny = 0, minx = 0, maxy = n, maxx = n; 
        // int diffy = water.y - house.y;
        // int diffx = water.x - house.x;
        // if(abs(diffy) > abs(diffx)){
        //     if(diffy > 0) miny = house.y;
        //     else maxy = house.y;
        // }
        // else{
        //     if(diffx > 0) minx = house.x;
        //     else maxx = house.x;
        // }
        
        for(auto water:WaterPos) isWaterPos[water.y][water.x] = true; // 水源を確認

        vector<pair<vec2, int>> path; //親の座標、破壊に必要なパワー。マップ値をパワーに設定する。
        vector<vector<bool>> isminimum(n, vector<bool>(n, false)); //最小値が決定しているか
        vector<vector<int>> cost(n, vector<int>(n, INF)); //各点のコスト
        vector<vector<vec2>> parent(n, vector<vec2>(n, {-1,-1}));//各点の親

        priority_queue<pair<int, vec2>, vector<pair<int, vec2>>, greater<pair<int, vec2>>> Pque;
        cost[house.y][house.x] = Map[house.y][house.x];
        parent[house.y][house.x] = {house.y, house.x};//houseの親はhouse
        Pque.push({0, house});

        while(true){
            pair<int, vec2> pv = Pque.top();
            Pque.pop();
            int pvy = pv.second.y;
            int pvx = pv.second.x;
            // cout << pvy << " " << pvx << " " <<  pv.first << "parent: " << parent[pvy][pvx].y << ", " <<  parent[pvy][pvx].x << endl;
            //水源に到達したら探索を終了
            if(isWaterPos[pvy][pvx]){
                path.push_back({{pvy, pvx}, Map[pvy][pvx]});
                break;
            }
            if(isminimum[pvy][pvx]) continue;
            
            for(auto nv:dxdy){
                int nvy = pvy + nv.y;
                int nvx = pvx + nv.x;
                if(nvy<miny || nvy>=maxy || nvx<minx || nvx>=maxx || isminimum[nvy][nvx]) continue;

                //コストの比較
                int newcost = min(cost[nvy][nvx], cost[pvy][pvx] + Map[nvy][nvx]);
                if(cost[nvy][nvx] > newcost){
                    cost[nvy][nvx] = newcost;
                    parent[nvy][nvx] = {pvy, pvx};
                }
                Pque.push({cost[nvy][nvx], {nvy, nvx}});
            }

            isminimum[pvy][pvx] = true; //最短距離確定
        }
        vec2 pathv = path[0].first;
        while(pathv.y!=parent[pathv.y][pathv.x].y || pathv.x!=parent[pathv.y][pathv.x].x){//houseの親はhouse
            int pvy = parent[pathv.y][pathv.x].y;
            int pvx = parent[pathv.y][pathv.x].x;
            path.push_back({{pvy, pvx}, Map[pvy][pvx]});
            WaterPos.push_back({pvy, pvx});
            pathv.y = pvy; 
            pathv.x = pvx;
        }
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

struct Solver
{
    int n, w, k, c; 
    vector<vec2> WaterPos, HousePos;
    vector<vector<int>> Map;
    Dijkstra dijkstra;
    Solver(int N, int W, int K, int C, vector<vec2>& source_pos, const vector<vec2>& house_pos) 
    : n(N), w(W), k(K), c(C), WaterPos(source_pos), HousePos(house_pos), Map(N, vector<int>(N, 0)), dijkstra(N, source_pos, house_pos) { }

    pair<int, int> NearestWater(vec2 house){
        int mindis=1e5, nearest=-1;
        for(int i=0; i<WaterPos.size(); i++){
            int cmpdis = abs(house.y - WaterPos[i].y) + abs(house.x - WaterPos[i].x);
            if(mindis > cmpdis){
                mindis = cmpdis;
                nearest = i;
            }
        }
        return {nearest, mindis};
    }
};

int main(){
    ifstream InputFile(inputfile);
    int n, w, k, c;
    InputFile >> n >> w >> k >> c;

    vector<vector<int>> DestLevel(n, vector<int>(n));
    vector<vec2> WaterPos(w), HousePos(k); 
    vector<vector<bool>> isbroken(n, vector<bool>(n, false));

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            InputFile >> DestLevel[i][j];
        }
    }
    for(int i=0; i<w; i++) InputFile >> WaterPos[i].y >> WaterPos[i].x;
    for(int i=0; i<k; i++) InputFile >> HousePos[i].y >> HousePos[i].x;

    Solver solver(n, w, k, c, WaterPos, HousePos);
    solver.dijkstra.readmap();

    ofstream output("testdijkstra.txt");
    for(int i=0; i<w; i++) output << WaterPos[i].y << " " << WaterPos[i].x << " " << 50 << endl;

    priority_queue<tuple<int, int, vec2>, vector<tuple<int, int, vec2>>, greater<tuple<int, int, vec2>>> Pque;
    for(int i=0; i<k; i++){
        pair<int, int> nearest = solver.NearestWater(HousePos[i]);
        Pque.push({nearest.second, i, WaterPos[nearest.first]});
    }
    while(!Pque.empty()){
        tuple<int, int, vec2> tmptuple = Pque.top();
        Pque.pop();
        vec2 house = HousePos[get<1>(tmptuple)];
        vec2 source = WaterPos[solver.NearestWater(house).first]; //最も近い水源を見つける(更新されている可能性があるので再度探索)
        vector<pair<vec2, int>> path;
        path = solver.dijkstra.searchmin(house, WaterPos[0]);

        for(int i=1; i<path.size(); i++){
            int y = path[i].first.y;
            int x = path[i].first.x;
            if(isbroken[y][x]) continue;

            output << y << " " << x << " " << path[i].second << endl;
            isbroken[y][x] = true;
        }
    }
    
    cout << "finished" << endl;
    return 0;
}