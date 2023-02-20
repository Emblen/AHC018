#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <set>
#include <queue>
using namespace std;
#define all(x) (x).begin(),(x).end()

string inputfile = "seed005w3k5.txt";
string outputfile = "seed005w3k5out.txt";

struct vec2 {int y, x;};
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
    const string outfilename;
    int n, c;
    long long total_cost;
    int brokehouse;
    vector<vector<int>> is_broken;
    vector<vec2> WaterPos, HousePos;
    vector<vector<int>> DestLevel;
    LocalTester(int N, int C, const vector<vec2>& source_pos, const vector<vec2>& house_pos, vector<vector<int>>& destlevel) 
    : n(N), c(C), WaterPos(source_pos), HousePos(house_pos), DestLevel(destlevel), is_broken(N, vector<int>(N, 0)), total_cost(0), outfilename(outputfile), brokehouse(0) { }

    Response LocalQuery(int y, int x, int power){
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
};

struct Solver
{
    int n, w, c; 
    vector<vec2> WaterPos, HousePos;
    Field field;
    Solver(int N, int W, int C, vector<vec2>& source_pos, const vector<vec2>& house_pos) 
    : n(N), c(C), w(W), WaterPos(source_pos), HousePos(house_pos), field(N, C) { }
//Local    
    // vector<vector<int>> DestLevel;
    // LocalTester localtester;
    // Solver(int N, int W, int C, vector<vec2>& source_pos, const vector<vec2>& house_pos, vector<vector<int>>& destlevel) 
    // : n(N), c(C), w(W), WaterPos(source_pos), HousePos(house_pos), field(N, C), localtester(N, C, source_pos, house_pos, destlevel), DestLevel(destlevel) { }

    void solve(){
        cout << "#solve start" << endl;

        for(auto house:HousePos){
            vec2 source = WaterPos[NearestWater(house)]; //最も近い水源を見つける
            move(house, source);
        }
        return;
    }

    int NearestWater(vec2 house){
        int mindis=1e5, nearest=-1;
        for(int i=0; i<WaterPos.size(); i++){
            int cmpdis = abs(house.y - WaterPos[i].y) + abs(house.x - WaterPos[i].x);
            if(mindis > cmpdis){
                mindis = cmpdis;
                nearest = i;
            }
        }
        return nearest;
    }

    void move(vec2 start, vec2 goal){
        //goalに向かって縦方向、横方向に直線移動する
        // down/up
        if(start.y < goal.y) for(int y = start.y; y <= goal.y; y++) destruct(y, start.x);
        else for(int y = start.y; y >= goal.y; y--) destruct(y, start.x);
        // right/left
        if(start.x < goal.x) for(int x = start.x; x <= goal.x; x++) destruct(goal.y, x);
        else for(int x = start.x; x >= goal.x; x--) destruct(goal.y, x);
    }

    void destruct(int row, int column) {
        const int power = 50;
//Serve
        while (!field.is_broken[row][column]) {
            Response result = field.query(row, column, power);
//Local
        // while (!localtester.is_broken[row][column]) {
            // Response result = localtester.LocalQuery(row, column, power);
            if (result == Response::finish) {
                cerr << "#total_cost=" << field.total_cost << endl;
                exit(0);
            } else if (result == Response::invalid) {
                cerr << "#invalid: y=" << row << " x=" << column << endl;
                exit(1);
            }
        }
        WaterPos.push_back({row, column}); //掘削した岩盤の座標を水源に追加
    }
};

int main(){
//Local
    // ifstream InputFile(inputfile);
    // int n, w, k, c;
    // InputFile >> n >> w >> k >> c;

    // vector<vector<int>> DestLevel(n, vector<int>(n));
    // vector<vec2> WaterPos(w), HousePos(k); 

    // for(int i=0; i<n; i++){
    //     for(int j=0; j<n; j++){
    //         InputFile >> DestLevel[i][j];
    //     }
    // }
    // for(int i=0; i<w; i++) InputFile >> WaterPos[i].y >> WaterPos[i].x;
    // for(int i=0; i<k; i++) InputFile >> HousePos[i].y >> HousePos[i].x;
    // for(auto v:WaterPos) cout << v.y << " " << v.x << endl;
    // for(auto v:HousePos) cout << v.y << " " << v.x << endl;
    // for(auto house:HousePos) cout << "(" << house.y << ", " << house.x << ") = " << DestLevel[house.y][house.x] << endl;
    // Solver solver(n, w, c, WaterPos, HousePos, DestLevel);
    // solver.solve();

//提出
    int n, w, k, c;
    cin >> n >> w >> k >> c;
    vector<vec2> WaterPos(w), HousePos(k); 
    for(int i=0; i<w; i++) cin >> WaterPos[i].y >> WaterPos[i].x;
    for(int i=0; i<k; i++) cin >> HousePos[i].y >> HousePos[i].x;
    Solver solver(n, w, c, WaterPos, HousePos);
    solver.solve();

    cout << "#finished" << endl;
    return 0;
}
