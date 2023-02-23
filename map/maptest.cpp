#include <iostream>
#include <fstream>
using namespace std;

int main(){
    int n=200;
    ofstream out("mapout.txt");
    for(int i=0; i<n; i++){
        for(int i=0; i<n; i++){
            out << i%3 << " ";
        }
        out << endl;
    }
    return 0;
}