
#include <iostream>     
#include <algorithm>    
#include <vector>       

/**
 * upper_bound: find the first item greater than val
 * lower_bound: find the first item less than val
 */

void test0(){
    int arr[] = { 11, 2, 30, 3, 7, 8, 13, 22 };
    const int N = sizeof(arr) / sizeof(arr[0]);

    std::vector<int> vec(arr, arr + N);
    std::sort(vec.begin(), vec.end());  //for binary search

    //std::vector<int>::iterator it = std::upper_bound(vec.begin(), vec.end(), 20);
    std::vector<int>::iterator it = std::upper_bound(vec.begin(), vec.end(), 20, [](int val, int cur){
        return val<cur;
    });  //expect 22
    std::cout << "upper bound is " << *it << std::endl;
}

int main() {
    test0();
    return 0;
}
