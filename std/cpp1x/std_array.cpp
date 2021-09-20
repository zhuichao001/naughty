
#include <array>
#include <iostream>
#include <algorithm>

void test1(){
    constexpr int len = 4;
    std::array<int, len> arr = {1, 2, 3, 4};
    
    if(!arr.empty()){
        std::cout << "size:" << arr.size() << std::endl;
    }
    
    for (auto &i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    /*
    std::sort(arr.begin(), arr.end(), [](int a, int b) {
        return b < a;
    });
    */

    std::sort(arr.begin(), arr.end());
    int * data = arr.data();
    const int size = arr.size();
    for(int i=0; i<size; ++i){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int main(){
    test1();
}
