#include <iostream>

using namespace std;


struct Ping {
	Ping(){type=0; times=0; cout<<"default construct, type:"<<type<<endl;}
	Ping(int t):type(t){times=0; cout<<"parameter construct, type:"<<type<<endl;}
	~Ping(){cout<<"destroy, type:"<<type<<endl;}
	void print(){cout<<"type:"<<type<<endl;}
	int type;
	int times;
};


