
#include <vector>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
using namespace std; 
const vector<unsigned long int> pow_table{
        1152921504606846976,72057594037927936,4503599627370496,
        281474976710656,17592186044416,1099511627776,68719476736,
        4294967296,268435456,16777216,1048576,65536,4096,256,16,1};
static vector<long int> fact_table{20922789888000,1307674368000,6227020800,479001600,39916800,3628800,362880, 40320,5040, 720, 120, 24, 6, 2, 1, 0,};
const vector<int> goal_state{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
const int SIZE = 16;
long int new_hash_func(vector<int> work){
    //vector<int> work(state);
    //copy(state.begin(), state.end(), work.begin());
    int i,j;
    long int value = 0;
    for(i=0;i<SIZE;i++){
        value += fact_table[i]*work[i];
        for(j=i+1;j<SIZE;j++){
            if(work[i]<work[j]) work[j] -= 1;
        }
    }
    return value;
}
unsigned long int hash_func(vector<int> work){
    
    unsigned long int value = 0;
    for(int i=0;i<16;i++){
        value += work[i]*pow_table[i];
        //cout << value << endl;
    }
    return value;
}

struct Node{
    vector<int> state = vector<int> (SIZE);
    unsigned long int selfID, parentID;
    int parent_pos;
    int h=0,g = 1024,f=0;
    int pos;
    bool open=true,close=false;
    bool done = false;
    void doOpen(){open = true, close=false;}
    void doClose(){close = true, open=false;}
    void initNode(vector<int> state0, int g0, int h0, int pos0, unsigned long int selfID0, unsigned long int parentID0, int parent_pos0){
        //copy(state0.begin(), state0.end(), work.begin());
        state = state0;
        pos = pos0; //0のインデックス
        selfID = selfID0; //自分のハッシュ値
        parentID = parentID0; //親のハッシュ値
        parent_pos = parent_pos0; //親のインデックス
        h = h0;
        g = g0;
        f = h + g;
    }
    //~Node(){delete parent;}; //これ忘れるとメモリリークおきるらしい
};

int main(){
    unordered_map<unsigned long int, Node> hash_mp;  
    unordered_map<long int, Node> new_hash_mp; 
    cout << hash_func(goal_state) << endl;
    cout << new_hash_func(goal_state) << endl; 
    double time = 0;
    int loop = 1000000;
    clock_t startt = clock();
    for(int i = 0; i < loop; i++){
       new_hash_func(goal_state);
    }
    clock_t endt = clock();
    time = static_cast<double>(endt - startt) / CLOCKS_PER_SEC;
    cout << "new time " << time << "[s]" << endl;
    time = 0;
    startt = clock();
    for(int i = 0; i < loop; i++){
       hash_func(goal_state);
    }
    endt = clock();
    time = static_cast<double>(endt - startt) / CLOCKS_PER_SEC;
    cout << "old time " << time << "[s]" << endl;

    long int testv = new_hash_func(goal_state);
    time = 0;
    startt = clock();
    Node node;
    for(int i = 0; i < loop; i++){
        Node node;
        new_hash_mp[testv] = node;
        Node node2 = new_hash_mp[testv];
    }
    endt = clock();
    time = static_cast<double>(endt - startt) / CLOCKS_PER_SEC;
    cout << "new time " << time << "[s]" << endl;

    unsigned long int testv2 = hash_func(goal_state);
    time = 0;
    startt = clock();
    for(int i = 0; i < loop; i++){
        Node node;
        hash_mp[testv2] = node;
        Node node2 = hash_mp[testv2];
    }
    endt = clock();
    time = static_cast<double>(endt - startt) / CLOCKS_PER_SEC;
    cout << "old time " << time << "[s]" << endl;
    return 0;
}