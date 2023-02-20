#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

string inputfile = "seed000w3k5.txt";
string outputfile = "seed000w3k5out.txt";
struct vec2 {int y, x;};

struct Solver
{
    int n, c; 
    vector<vec2> WaterPos, HousePos;
    Solver(int N, const vector<vec2>& source_pos, const vector<vec2>& house_pos, int C) : n(N), WaterPos(WaterPos), HousePos(HousePos), c(C) { }

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
