#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <set>
#include <queue>
#include <tuple>
#include <random>
#include <time.h>
using namespace std;
#define all(x) (x).begin(),(x).end()

string inputfile = "test/seed005w1k10.txt";
string outputfile = "test/seed005w1k10map.txt";
string mapname = "makemap/mapdata005.txt";

struct vec2 
{
    int y, x;
    bool operator<(const vec2& v2) const{
        return x == v2.x ? y<v2.y : x<v2.x;
    }
};
enum class Response {not_broken, broken, finish, invalid};

struct Field
{
    int n;
    int c;
    vector<vector<int>> is_broken;
    long long total_cost;

    Field(int N, int C) 
    : n(N), c(C), is_broken(N, vector<int>(N, 0)), total_cost(0) {}

    Response query(int y, int x, int power) {
        total_cost += power + c;
        cout << y << " " << x << " " << power << endl;
        int r;
        cin >> r;
        switch (r) {
        case 0:
            return Response::not_broken;
        case 1:
            is_broken[y][x] = 1;
            return Response::broken;
        case 2:
            is_broken[y][x] = 1;
            return Response::finish;
        default:
            return Response::invalid;
        }
    }
};

struct LocalTester
{
    const string outfilename, mapfilename;
    int n, c;
    long long total_cost;
    int repdotnum;
    vector<vector<int>> is_broken;
    vector<vec2> WaterPos, HousePos;
    vector<vector<int>> DestLevel, mapdata;
    vector<vector<bool>> mapcheck;
    vector<pair<int, int>> dxdy;
    LocalTester(int N, int C, const vector<vec2>& source_pos, const vector<vec2>& house_pos, vector<vector<int>>& destlevel) 
    : n(N), c(C), WaterPos(source_pos), HousePos(house_pos), DestLevel(destlevel), is_broken(N, vector<int>(N, 0)), total_cost(0), outfilename(outputfile), mapfilename(mapname), repdotnum(20), mapdata(N, vector<int>(N, 0)), mapcheck(N, vector<bool>(N, 0)), dxdy({{0,1},{0,-1}, {1,0}, {-1,0}}) { }

    Response LocalQuery(int y, int x, int power){//掘削の出力、DestLevel、is_brokenの管理
        total_cost += power + c;
        fileout(y, x, power);

        int r = getJudge(y, x, power);
        switch (r) {
        case 0:
            return Response::not_broken;
        case 1:
            is_broken[y][x] = 1;
            return Response::broken;
        case 2:
            is_broken[y][x] = 1;
            return Response::finish;
        default:
            return Response::invalid;
        }
    }

    void fileout(int y, int x, int power){
        ofstream OutputFile;
        OutputFile.open(outfilename, ios::app);
        OutputFile << y << " " << x << " " << power << endl;
        OutputFile.close();
    }

    int getJudge(int y, int x, int power){
        DestLevel[y][x] -= power;
        int residue = DestLevel[y][x];
        
        if(residue <= 0) return 1; 
        else return 0; 
    } 

    void makemap(){//代表点の掘削を行い、マップを作成する
        for(int i=0; i<repdotnum; i++){
            for(int j=0; j<repdotnum; j++){
                for(int k=0; k<3; k++){
                    int y = n/repdotnum*i;
                    int x = n/repdotnum*j;
                    if(is_broken[y][x]) continue;

                    int power = 50;
                    Response result = LocalQuery(y, x, power);
                    if(result == Response::broken) mapdata[y][x] = power*(k+1);

                    if(k==2 && !is_broken[y][x]) mapdata[y][x] = Random(power*(k+1),300);
                    //壊れなかったら適当に大きな数字にする
                }
            }
        }
        verticalthred();
        horizontalthred();
        assignmapvalue();
        outmap();
    }

    void verticalthred(){
        int blocknum = n/repdotnum; //隣の代表点との距離
        for(int i=0; i<repdotnum; i++){
            int repx = blocknum*i;//横方向の代表点
            for(int j=0; j<repdotnum; j++){
                int repy = blocknum*j;
                int diffneighbor;
                if(j==repdotnum-1) diffneighbor = 0;
                else diffneighbor = mapdata[repy+10][repx] - mapdata[repy][repx];

                for(int k=0; k<blocknum; k++){
                    int y = repy + k;
                    mapdata[y][repx] = mapdata[repy][repx] + (diffneighbor/blocknum)*k;
                    mapcheck[y][repx] = true;
                    // cout << y << " " << repx << " " << mapdata[y][repx] << endl;
                }
            }
        }
    }
    void horizontalthred(){
        int blocknum = n/repdotnum; //隣の代表点との距離
        for(int i=0; i<repdotnum; i++){
            int repy = blocknum*i;//横方向の代表点
            for(int j=0; j<repdotnum; j++){
                int repx = blocknum*j;
                int diffneighbor;
                if(j==repdotnum-1) diffneighbor = 0;
                else diffneighbor = mapdata[repy][repx+10] - mapdata[repy][repx];

                for(int k=0; k<blocknum; k++){
                    int x = repx + k;
                    mapdata[repy][x] = mapdata[repy][repx] + (diffneighbor/blocknum)*k;
                    mapcheck[repy][x] = true;
                    // cout << repy << " " << x << " " << mapdata[repy][x] << endl;
                }
            }
        }
    }
    void assignmapvalue(){
        // //縦方向に近傍の平均値をとる
        // for(int x=1; x<n; x++){
        //     for(int y=1; y<n; y++){
        //         if(mapcheck[y][x]) continue;
        //         int ischeckednum = 0; //上下左右の値が決まっているか
        //         int value = 0;
        //         for(auto v:dxdy){
        //             if(y+v.first<0 || y+v.first>=n || x+v.second<0 || x+v.second>=n || !mapcheck[y+v.first][x+v.second]) continue;
        //             ischeckednum++;
        //             value += mapdata[y+v.first][x+v.second];
        //         }
        //         mapdata[y][x] = value/ischeckednum;          
        //         mapcheck[y][x] = true;
        //         // cout << y << " " << x << " " << mapdata[y][x] << endl;      
        //     }
        // }

        int blocknum = n/repdotnum; //隣の代表点との距離
        // version2
        for(int x=1; x<n; x++){
            if(x%blocknum==0) continue;
            for(int j=0; j<repdotnum; j++){
                int repy = blocknum*j;
                int diffneighbor;
                if(j==repdotnum-1) for(int k=1; k<blocknum; k++) mapdata[repy+k][x] = (mapdata[repy][x] + mapdata[repy+k][x-1])/2;
                else{
                    diffneighbor = mapdata[repy+10][x] - mapdata[repy][x];
                    for(int k=1; k<blocknum; k++){
                        mapdata[repy+k][x] = mapdata[repy][x] + (diffneighbor/blocknum)*k;
                        mapcheck[repy+k][x] = true;
                        // cout << repy+k << " " << x << " " << mapdata[repy+k][x] << endl;
                    }
                }
            }
        }
    }

    int Random(int min, int max){
	    return min + (max - min) * rand() / RAND_MAX;
    }

    void outmap(){
        ofstream Map;
        Map.open(mapname);
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                Map << mapdata[i][j] << " ";
            }
            Map << endl;
        }
    }
};

struct Solver
{
    int n, w, k, c; 
    vector<vec2> WaterPos, HousePos;
    Field field;
//Submit
    // Solver(int N, int W, int K, int C, vector<vec2>& source_pos, const vector<vec2>& house_pos) 
    // : n(N), c(C), w(W), k(K), WaterPos(source_pos), HousePos(house_pos), field(N, C) { }
//Local    
    vector<vector<int>> DestLevel;
    LocalTester localtester;
    Solver(int N, int W, int K, int C, vector<vec2>& source_pos, const vector<vec2>& house_pos, vector<vector<int>>& destlevel) 
    : n(N), w(W), k(K), c(C), WaterPos(source_pos), HousePos(house_pos), field(N, C), localtester(N, C, source_pos, house_pos, destlevel), DestLevel(destlevel) { }

    void solve(){
        cout << "#solve start" << endl;
        //代表点の掘削
        localtester.makemap();
        cout << localtester.total_cost << endl;
    



//         priority_queue<tuple<int, int, vec2>, vector<tuple<int, int, vec2>>, greater<tuple<int, int, vec2>>> Pque;
//         for(int i=0; i<k; i++){
//             pair<int, int> nearest = NearestWater(HousePos[i]);
//             Pque.push({nearest.second, i, WaterPos[nearest.first]});
//         }
        
//         while(!Pque.empty()){
//             tuple<int, int, vec2> tmptuple = Pque.top();
//             Pque.pop();
//             vec2 house = HousePos[get<1>(tmptuple)];
//             vec2 source = WaterPos[NearestWater(house).first]; //最も近い水源を見つける(更新されている可能性があるので再度探索)
//             move(house, source);
//         }
//         return;
//     }

//     pair<int, int> NearestWater(vec2 house){
//         int mindis=1e5, nearest=-1;
//         for(int i=0; i<WaterPos.size(); i++){
//             int cmpdis = abs(house.y - WaterPos[i].y) + abs(house.x - WaterPos[i].x);
//             if(mindis > cmpdis){
//                 mindis = cmpdis;
//                 nearest = i;
//             }
//         }
//         return {nearest, mindis};
//     }

//     void move(vec2 start, vec2 goal){
//         //goalに向かって縦方向、横方向に直線移動する
//         // down/up
//         if(start.y < goal.y) for(int y = start.y; y <= goal.y; y++) destruct(y, start.x);
//         else for(int y = start.y; y >= goal.y; y--) destruct(y, start.x);
//         // right/left
//         if(start.x < goal.x) for(int x = start.x; x <= goal.x; x++) destruct(goal.y, x);
//         else for(int x = start.x; x >= goal.x; x--) destruct(goal.y, x);
//     }

//     void destruct(int row, int column) {
//         const int power = 80;
// //Submit
//         // if(field.is_broken[row][column]) return;
//         // while (!field.is_broken[row][column]) {
//         //     Response result = field.query(row, column, power);
// //Local
//         if(localtester.is_broken[row][column]) return;
//         while (!localtester.is_broken[row][column]) {
//             Response result = localtester.LocalQuery(row, column, power);
//             if (result == Response::finish) {
//                 cerr << "#total_cost=" << field.total_cost << endl;
//                 exit(0);
//             } else if (result == Response::invalid) {
//                 cerr << "#invalid: y=" << row << " x=" << column << endl;
//                 exit(1);
//             }
//         }
//         WaterPos.push_back({row, column}); //掘削した岩盤の座標を水源に追加
    }
};

int main(){
//Local
    srand((unsigned int)time(NULL));
    ifstream InputFile(inputfile);
    int n, w, k, c;
    InputFile >> n >> w >> k >> c;

    vector<vector<int>> DestLevel(n, vector<int>(n));
    vector<vec2> WaterPos(w), HousePos(k); 

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            InputFile >> DestLevel[i][j];
        }
    }
    for(int i=0; i<w; i++) InputFile >> WaterPos[i].y >> WaterPos[i].x;
    for(int i=0; i<k; i++) InputFile >> HousePos[i].y >> HousePos[i].x;
    // for(auto house:HousePos) cout << "(" << house.y << ", " << house.x << ") = " << DestLevel[house.y][house.x] << endl;
    Solver solver(n, w, k, c, WaterPos, HousePos, DestLevel);
    solver.solve();

//Submit
    // int n, w, k, c;
    // cin >> n >> w >> k >> c;
    // vector<vec2> WaterPos(w), HousePos(k); 
    // for(int i=0; i<w; i++) cin >> WaterPos[i].y >> WaterPos[i].x;
    // for(int i=0; i<k; i++) cin >> HousePos[i].y >> HousePos[i].x;
    // Solver solver(n, w, k, c, WaterPos, HousePos);
    // solver.solve();

    cout << "#finished" << endl;
    return 0;
}
