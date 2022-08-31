#include <assert.h>

template <class Type, int Dim>
class Point {
public:
    Point(Type coords[ Dim ]) {
        for(int index = 0; index <Dim; ++index){
            _coords[index] = coords[index];
        }
    }

    Type& operator[](int index) {
        assert(index<Dim && index>=0);
        return _coords[index];
    }

    Type operator[](int index) const {
        assert(index<Dim && index>=0);
        return _coords[index];
    }

private:
    Type _coords[Dim];
};

int main(){
    int pos[3] = {1,2,3};
    Point<int, 3> p(pos);
    return 0;
}
