#include <stdio.h>
#include <vector>

void erase(std::vector<int> &vec, int a) {
    for (auto iter = vec.begin(); iter != vec.end();) { // 这个正确
        if (*iter == a) {
            iter = vec.erase(iter);
        } else {
            ++iter;
        }
    }
}

int main(){
    int a[]={3,1,5,30,19,7,6,2};
    std::vector<int> vec(a, a+sizeof(a)/sizeof(a[0]));
    erase(vec, 7);
    return 0;
}
