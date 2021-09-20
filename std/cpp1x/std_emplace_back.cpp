#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class Si {
  public:
    Si(int i){
        str = to_string(i);
        cout << "Construct" << endl; 
    }

    Si(const Si& other): str(other.str){
        cout << "Copy-Construct" << endl;
    }

    ~Si(){}

  private:
    string str;
};

int main() {
    const int N=4;
    vector<Si> vec;
    vec.reserve(N*2);

    for(int i=0; i<N; i++){
        vec.push_back(Si(i)); //call N times Construct and Copy-Construct
    }

    for(int i=0; i<N; i++){
        vec.emplace_back(i);  //only call N times Construct
    }
}
