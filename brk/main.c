#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char *Malloc(int n) {
	char *mem = (char *)malloc(n);
	if (mem == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return mem;
}

int main(void) {
	void *hstart = sbrk(0);
	if (hstart != (void *)-1) {
		printf ("heap start: %p\n", hstart);
	}

	char *pmem = Malloc(64);
	hstart = sbrk(0);
	if (hstart != (void *)-1) {
		printf ("pmem:%p, heap end:%d, heap size on each load: %p\n", pmem, hstart, (char *)hstart-pmem);
	}
	free(pmem);

	char *pmem2 = Malloc(128);
	hstart = sbrk(0);
	if (hstart != (void *)-1) {
		printf ("pmem2:%p, heap end:%d, heap size on each load: %p\n", pmem2, hstart, (char *)hstart-pmem2);
	}
	free(pmem2);

	char *pmem3 = Malloc(132*1024); //mmap asign space
	hstart = sbrk(0);
	if (hstart != (void *)-1) {
		printf ("pmem3:%p, heap end:%d, heap size on each load: %p\n", pmem3, hstart, (char *)hstart-pmem3);
	}
	free(pmem3);

	return 0;
}
