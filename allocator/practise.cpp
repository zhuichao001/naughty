
#include "raw_alloc.h"
#include <vector>
#include <iostream>

using namespace std;


int main() {
    int arr[] = {1,2,4,8,16};

    vector<int, naughty::allocator<int> > vi(arr, arr+5);
    for(int i=0; i<sizeof(arr)/sizeof(arr[0]); ++i){
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
