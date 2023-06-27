#include <map>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

//for min-heap
bool comparer(const std::pair<int,int> &a, const std::pair<int,int> &b){
    return a.second > b.second;
}

void print_heap(std::vector<std::pair<int,int>> &v){
    std::cout << "start print----" << std::endl;
    for (auto e : v) {
        std::cout << e.first << ':' << e.second << std::endl;
    }
    std::cout << "end print----" << std::endl;
}

int main(){
    std::vector<std::pair<int,int>> v;
    v.push_back(std::make_pair(2,1));
    v.push_back(std::make_pair(3,8));
    v.push_back(std::make_pair(6,2));
    v.push_back(std::make_pair(12,10));
    std::make_heap(v.begin(), v.end(), comparer);

    v.push_back(std::make_pair(7,3));
    std::push_heap(v.begin(), v.end(), comparer);

    while(!v.empty()){
        std::pop_heap(v.begin(), v.end(), comparer);
        std::pair<int, int> cur = v.back();
        v.pop_back();
        int point = cur.first;
        int cost = cur.second;
        std::cout<< "pop point:" << point << ",cost:" << cost << std::endl;
    }

    return 0;
}
