#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

string inputfile = "seed000w3k5.txt";
string outputfile = "seed000w3k5out.txt";

struct vec2 {int y, x;};
enum class Response {not_broken, broken, finish, invalid};

struct Field
{
    int n;
    int c;
    vector<vector<int>> is_broken;
    int total_cost;

    Field(int N, int C) : n(N), c(C), is_broken(N, vector<int>(N, 0)), total_cost(0) {}

    Response query(int y, int x, int power) {
        total_cost += power + c;
        cout << y << " " << x << " " << power << endl; // endl does flush
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
    int n, c;
    vector<vec2> WaterPos, HousePos;
    vector<vector<int>> DestLevel;
    LocalTester(int N, const vector<vec2>& source_pos, const vector<vec2>& house_pos, int C, vector<vector<int>>& DestLevel) : n(N), WaterPos(WaterPos), HousePos(HousePos), c(C), DestLevel(DestLevel) { }

    
};

struct Solver
{
    int n, w, c; 
    vector<vec2> WaterPos, HousePos;
    Field field;
    Solver(int N, int W, int C, const vector<vec2>& source_pos, const vector<vec2>& house_pos) : n(N), c(C), w(W),  WaterPos(WaterPos), HousePos(HousePos), field(N, C) { }

    void solve(){
        for(auto house:HousePos){
            vec2 source = WaterPos[NearestWater(house)]; //最も近い水源を見つける
            move(house, source);
        }
    }

    int NearestWater(vec2 house){
        int mindis=1e4, nearest=-1;
        for(int i=0; i<w; i++){
            int cmpdis = abs(house.y - WaterPos[i].y) + abs(house.x - WaterPos[i].x);
            if(mindis > cmpdis){
                mindis = cmpdis;
                nearest = i;
            }
        }
    }

    void move(vec2 start, vec2 goal){
        // down/up
        if(start.y < goal.y) for(int y = start.y; y < goal.y; y++) destruct(y, start.x);
        else for(int y = start.y; y > goal.y; y--) destruct(y, start.x);
        // right/left
        if(start.x < goal.x) for(int x = start.x; x < goal.x; x++) destruct(goal.y, x);
        else for(int x = start.x; x > goal.x; x--) destruct(goal.y, x);
    }

    void destruct(int row, int column) {
        const int power = 50;
        while (!field.is_broken[row][column]) {
            Response result = field.query(row, column, power);
            if (result == Response::finish) {
                cerr << "total_cost=" << field.total_cost << endl;
                exit(0);
            } else if (result == Response::invalid) {
                cerr << "invalid: y=" << row << " x=" << column << endl;
                exit(1);
            }
        }
    }
};

int main(){
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
    for(auto v:WaterPos) cout << v.y << " " << v.x << endl;
    for(auto v:HousePos) cout << v.y << " " << v.x << endl;

    // cin >> n >> w >> k >> c;
    // vector<vec2> WaterPos(w), HousePos(k); 
    // for(int i=0; i<w; i++) cin >> WaterPos[i].y >> WaterPos[i].x;
    // for(int i=0; i<k; i++) cin >> HousePos[i].y >> HousePos[i].x;

    return 0;
}
