#include<iostream>
#include<vector>
#include<queue>
#include<iterator>
#include<functional>
#include<algorithm>

// std::priority_queue<int> big_heap;   
// std::priority_queue<int, std::vector<int>, std::less<int> > big_heap2;   
// std::priority_queue<int, std::vector<int>, std::greater<int> > small_heap;


int main(){
    using Heap = std::priority_queue<int>;
    Heap big_heap;   
    big_heap.push(5);
    big_heap.push(9);
    big_heap.push(1);
    big_heap.push(3);
    big_heap.push(6);

   while(!big_heap.empty()) {
       std::cout << big_heap.top() << std::endl;
       big_heap.pop();
   }

   return 0;
}
