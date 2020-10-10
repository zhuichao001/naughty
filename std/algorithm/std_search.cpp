#include<iostream>
#include<algorithm>
#include<vector>


void case_search() {
    int arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
    int len = sizeof(arr)/sizeof(arr[0]);
    std::vector<int> vec(arr, arr+len);

    std::sort(vec.begin(), vec.end());
    if (std::binary_search(vec.begin(), vec.end(), 3)) std::cout << "found!\n";

    std::sort(vec.begin(), vec.end(), [](int a, int b)->bool{return a<b;});
    if (std::binary_search(vec.begin(), vec.end(), 6, [](int a,int b)->bool{return a==b;})) std::cout << "found!\n";

    int needle[] = {4, 5};
    std::vector<int>::iterator it;
    it = std::search(vec.begin(), vec.end(), needle, needle+2);
    if (it != vec.end()) std::cout << "needle found at:" << (it-vec.begin()) << '\n';

    it = std::search(vec.begin(), vec.end(), needle, needle+2, [](int a,int b)->bool{return a==b;});
    if (it != vec.end()) std::cout << "needle found at:" << (it-vec.begin()) << '\n';

    it = std::search_n(vec.begin(), vec.end(), 2, 30);
    if (it != vec.end()) std::cout << "two 30s found at:" << (it-vec.begin()) << '\n';
}

int main(){
    case_search();
    return 0;
}
