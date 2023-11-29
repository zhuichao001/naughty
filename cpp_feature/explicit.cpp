
class A {
public :
    A(int n):num(n){}
private:
    int num;
};


class B {
public :
    explicit B(int n, char t='D'):num(n),type(t){}
private:
    int num;
    char type;
};


int main() {
    A t1 = 12;

    B t2(13);
    // B t3 = 14;  //error!
    double d=3.14159;
    B t4(d);
    return 0;
}
