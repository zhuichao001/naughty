#include <stdio.h>
#include <unistd.h>
#include "cal.h"

int main() {
	if ( fork() == 0 ) {
		calculate("master");
		//execl("./worker", NULL);
	}else{

		calculate("master");
		printf("DONE\n");
	}
	return 0;
}
