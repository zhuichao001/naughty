#include <stdio.h>
#include <time.h>
#include "cal.h"

void calculate(const char *owner, long long num){
	time_t timer = time(NULL);
	printf("%s start time %s\n", owner, ctime(&timer));

	double p=3.141592654;
	for(long long i=0; i<num; ++i){
		p = p*p;
	}

	timer = time(NULL);
	printf("%s end time %s\n", owner, ctime(&timer));
}

