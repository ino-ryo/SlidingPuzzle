
#include <vector>
#include <queue>
#include <iostream>
#include <random>
#include <algorithm>
#include <time.h>
#include <unordered_map>
#include <iomanip>
using namespace std; 
const int SIZE = 16;
const int  MAX_STATE = 181440;
const vector< vector<int> > adjacent{{1, 4, -1, -1},
                                    {0,  2,   5, -1},
                                    {1,  3,   6, -1},
                                    {2,  7,  -1, -1},
                                    {0,  5,   8, -1},
                                    {1,  4,   6,  9},
                                    {2,  5,   7, 10},
                                    {3,  6,  11, -1},
                                    {4,  9,  12, -1},
                                    {5,  8,  10, 13},
                                    {6,  9,  11, 14},
                                    {7,  10, 15, -1},
                                    {8,  13, -1, -1},
                                    {9,  12, 14, -1},
                                    {10, 13, 15, -1},
                                    {11, 14, -1, -1}};
//前処理データがキャッシュメモリにのってないこともある（演算のほうが早いこともある）
const vector<int> adjacent_num{2, 3, 3, 2, 3, 4, 4, 3, 3, 4, 4, 3, 2, 3, 3, 2};
const vector< vector<int> > manhattans{{0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6},
                                        {1, 0, 1, 2, 2, 1, 2, 3, 3, 2, 3, 4, 4, 3, 4, 5},
                                        {2, 1, 0, 1, 3, 2, 1, 2, 4, 3, 2, 3, 5, 4, 3, 4},
                                        {3, 2, 1, 0, 4, 3, 2, 1, 5, 4, 3, 2, 6, 5, 4, 3},
                                        {1, 2, 3, 4, 0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5},
                                        {2, 1, 2, 3, 1, 0, 1, 2, 2, 1, 2, 3, 3, 2, 3, 4},
                                        {3, 2, 1, 2, 2, 1, 0, 1, 3, 2, 1, 2, 4, 3, 2, 3},
                                        {4, 3, 2, 1, 3, 2, 1, 0, 4, 3, 2, 1, 5, 4, 3, 2},
                                        {2, 3, 4, 5, 1, 2, 3, 4, 0, 1, 2, 3, 1, 2, 3, 4},
                                        {3, 2, 3, 4, 2, 1, 2, 3, 1, 0, 1, 2, 2, 1, 2, 3},
                                        {4, 3, 2, 3, 3, 2, 1, 2, 2, 1, 0, 1, 3, 2, 1, 2},
                                        {5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0, 4, 3, 2, 1},
                                        {3, 4, 5, 6, 2, 3, 4, 5, 1, 2, 3, 4, 0, 1, 2, 3},
                                        {4, 3, 4, 5, 3, 2, 3, 4, 2, 1, 2, 3, 1, 0, 1, 2},
                                        {5, 4, 3, 4, 4, 3, 2, 3, 3, 2, 1, 2, 2, 1, 0, 1},
                                        {6, 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0}};
const vector<int> goal_state{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
/*
const vector<unsigned long int> pow_table{
        1152921504606846976,72057594037927936,4503599627370496,
        281474976710656,17592186044416,1099511627776,68719476736,
        4294967296,268435456,16777216,1048576,65536,4096,256,16,1};
*/
void visualizer(vector<int>);
int get_manhattan(vector<int> *);
int get_manhattan_easy(vector<int> *, int, int, int);
unsigned long int hash_func(vector<int>);
//struct Node;
// 宣言
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
    void updatef(int g0){g = g0, f = h + g0;}
    //~Node(){delete parent;}; //これ忘れるとメモリリークおきるらしい
};

int get_manhattan(vector<int> *state)
{   
    int h = 0;
    for(int i=0;i<SIZE;i++){
        if((*state)[i]==0){continue;}
        h += manhattans[i][(*state)[i]-1]; 
    }
    return h;
}
int get_manhattan_easy(vector<int> *state,int h0, int self_pos, int parent_pos)
{   
    h0 -= manhattans[self_pos][(*state)[parent_pos]-1];
    h0 += manhattans[parent_pos][(*state)[parent_pos]-1];
    return h0;
}

/*
unsigned long int hash_func(vector<int> work){
    
    unsigned long int value = 0;
    for(int i=0;i<SIZE;i++){
        value += work[i]*pow_table[i];
    }
    return value;
}
*/
unsigned long int hash_func(vector<int> work){
    int shift = 4; //16 = 2^4
    unsigned long int value = 0;
    for(int i=0;i<16;i++){
        value += ((unsigned long int)work[i] << (shift*(15-i)));
    }
    return value;
}
vector<int> initializer(int n_slide=100){
    //std::random_device engine;  // 非決定論的な乱数生成器を使用する
    vector<int> work(goal_state);
    //copy(goal_state.begin(), goal_state.end(), work.begin());
    int pos = SIZE - 1; //0の位置
    unsigned int next_pos_index, next_pos;
    for(int i=0;i<n_slide;i++){ 
        //next_pos_index = engine() % adjacent_num[pos];
        next_pos_index = rand() % adjacent_num[pos]; //比較しやすいように決定的な乱数生成器を用いる
        next_pos = adjacent[pos][next_pos_index];
        work[pos] = work[next_pos];
        work[next_pos] = 0;
        pos = next_pos;
    }
    work.push_back(pos);
    return work;
}

void visualizer(vector<int> state){
    for(int i=0;i<4;i++){
        cout << setw(3) << state.at(4*i) << setw(3) << state.at(4*i+1) << setw(3) << state.at(4*i+2) << setw(3) << state.at(4*i+3) << endl;
    }
    cout << endl;
}
//template <class Node, class vector<Node> *, class unordered_map<unsigned long int, Node> *>
void expand(Node *node, vector<Node> *expanded, unordered_map<unsigned long int, Node> *hash_mp){
    int next_pos;
    unsigned long int ID;
    //copy(node.state.begin(), node.state.end(), work.begin());
    //cout <<"start expand" << endl;
    for(int i=0;i<adjacent_num[node->pos];i++){
        next_pos = adjacent[node->pos][i];
        if(next_pos == node->parent_pos){continue;}
        node->state[node->pos] = node->state[next_pos];
        node->state[next_pos] = 0;
        Node child_node; 
        ID = hash_func(node->state);
        if((*hash_mp)[ID].parent_pos){
            if((*hash_mp)[ID].g > node->g+1){
                (*hash_mp)[ID].parentID = node->selfID;
                (*hash_mp)[ID].updatef(node->g+1);
                (*hash_mp)[ID].doOpen();
            }
            (*expanded)[i] = (*hash_mp)[ID];
        }else{
            int h0 = get_manhattan_easy(&node->state, node->h, next_pos, node->pos);
            child_node.initNode(node->state, node->g + 1, h0, next_pos, ID, node->selfID, node->pos);
            (*expanded)[i] = child_node;
            (*hash_mp)[ID] = child_node;
        }
        //cout << child_node.selfID <<endl;
        node->state[next_pos] = node->state[node->pos];
        node->state[node->pos] = 0;
    }
    //cout <<"end expand" << endl;
}

int main(){
    unsigned long int id;
    int pos;
    const int loop = 1;
    const int maxf = 100;
    const int slides = 300;
    int expanded_num = 0;
    vector<Node> expanded(5);
    vector< vector<int> > init_state_vec(loop);
    for(int j=0;j<loop;j++){
        init_state_vec[j] = initializer(slides);
    }
    double time = 0;
    Node final_node;
    //visualizer(init_state);
    clock_t startt = clock();
    for(int i = 0; i < loop; i++){
        cout << i << endl;
        unordered_map<unsigned long int, Node> hash_mp;  
        vector< queue<unsigned long int> > open_vec(maxf); //FIFO queue
        Node init_node, node0;
        Node *node;
        vector<int> init_state = init_state_vec[i];
        visualizer(init_state);
        init_node.done = true;
        init_state.pop_back();
        id = hash_func(init_state);
        int h0 = get_manhattan(&init_state);
        init_node.initNode(init_state,0,h0,init_state_vec[i][SIZE],id,node0.selfID,-1);
        hash_mp[init_node.selfID] = init_node;
        open_vec[init_node.f].push(init_node.selfID);
        int minf = init_node.f;
        cout << "initial f value " <<  init_node.f << endl;
        while(true){
            ////cout <<"start while loop" << endl;
            int j;
            for(j=minf;j<maxf;j++) {
                if(!open_vec[j].empty()){
                    id = open_vec[j].front();
                    open_vec[j].pop();
                    minf = j;
                    break;
                }
            }
            if(j == maxf){cout << "Failed" << endl; break;}
            if(node->state == goal_state) { //ゴールチェック
                final_node = *node;
                break;
            }node = &hash_mp[id];
            if(node->close){
                //cout << "skip" << endl;
                continue;
            }else{node->doClose();}
            expand(node, &expanded, &hash_mp);
            for(int i=0;i<adjacent_num[node->pos];i++){
                int next_pos = adjacent[node->pos][i];
                if(next_pos == node->parent_pos){continue;}
                open_vec[expanded[i].f].push( expanded[i].selfID );
                expanded_num += 1;
            }
            //break;
            ////cout <<"end while loop" << endl;
        }
        //visualizer(final_node.state);
    }
    clock_t endt = clock();
    time += static_cast<double>(endt - startt) / CLOCKS_PER_SEC;
    cout << "time " << time << "[s]" << endl;
    cout << "second per solve " << time/loop << endl;
    cout << "expanded number " << expanded_num << endl;
    cout << "expand per second " << expanded_num/time << endl;
    visualizer(final_node.state);

    //Node fnode = close_table.at(final_node.parent_val);
    //visualizer(fnode.state);
    
    /*
    while(!fnode.done){
        visualizer(fnode.state);
        fnode = close_table.at(fnode.parent_val);
    }
    */
   return 0 ;
}