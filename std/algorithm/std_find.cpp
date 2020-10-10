#include<iostream>
#include<algorithm>
#include<vector>


void case_find() {
    int arr[] = {1, 3, 5, 5, 7, 8, 3, 5, 5};	
    int len = sizeof(arr)/sizeof(arr[0]);

    //find array
    int *p = std::find(arr, arr+len, 7);
    if(p!=arr+len){
	std::cout<<"element found in arr:"<<*p<<std::endl;
    }else{
	std::cout<<"element not found in arr"<<std::endl;
    }

    //find vector
    std::vector<int> vec(arr, arr+len);
    std::vector<int>::iterator it = std::find(vec.begin(), vec.end(), 6);
    if(it==vec.end()){
	std::cout<<"element not found in vec"<<std::endl;
    }else{
	std::cout<<"element found in vec:"<<*it<<std::endl;
    }

    //find adjacent
    it = std::adjacent_find(vec.begin(), vec.end());
    if(it==vec.end()){
	std::cout<<"adjacent element not found in vec"<<std::endl;
    }else{
	std::cout<<"adjacent element found in vec:"<<*it<<std::endl;
    }

    int needle[] = {3, 5, 5};
    //find first of
    it = std::find_first_of(vec.begin(), vec.end(), needle, needle + sizeof(needle)/sizeof(needle[0]));
    if (it!=vec.end()) {
        std::cout << "find_first_of at position " << (it - vec.begin()) << std::endl;
    } else {
        std::cout << "find_first_of not found " << std::endl;
    }
    
    //find end
    it = std::find_end(vec.begin(), vec.end(), needle, needle + sizeof(needle)/sizeof(needle[0]));
    if (it!=vec.end()) {
        std::cout << "find_end at position " << (it - vec.begin()) << std::endl;
    } else {
        std::cout << "find_end not found " << std::endl;
    }
    
    //find if satisfy
    it = std::find_if(vec.begin(), vec.end(), [](int i){return i%2==0;});
    if (it!=vec.end()) {
        std::cout << "find_if at position " << (it - vec.begin()) << std::endl;
    } else {
        std::cout << "find_if not found " << std::endl;
    }
}

int main(){
    case_find();
    return 0;
}
