#include<map>
#include<vector>
#include<map>
#include<algorithm>
#include<iostream>

using namespace std;

//for min-heap
bool cmp_pair(const pair<int,int> &a, const pair<int,int> &b){
    return a.second > b.second;
}

void print_heap(vector<pair<int,int>> &v){
    cout << "start ----" << endl;
    for (auto e : v) {
        cout << e.first << ':' << e.second << endl;
    }
    cout << "end ----" << endl;
}

int main(){
    vector<pair<int,int>> v;
    v.push_back(make_pair(2,1));
    v.push_back(make_pair(3,8));
    v.push_back(make_pair(6,2));
    v.push_back(make_pair(12,10));
    make_heap(v.begin(), v.end(), cmp_pair);
    //print_heap(v); //TODO
    while(!v.empty()){
        pop_heap(v.begin(), v.end(), cmp_pair);
        pair<int, int> cur = v.back();
        v.pop_back();
        int point = cur.first;
        int cost = cur.second;
        cout<< "pop point:" << point << ",cost:" << cost << endl;
        //print_heap(v); //TODO
    }

    return 0;
}
