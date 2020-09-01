#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char *fmalloc(int n) {
	char *mem = (char *)malloc(n);
	if (mem == NULL) {
		perror("malloc");
		exit (EXIT_FAILURE);
	}
	return mem;
}

int main(void)
{
	void *tret;
	char *pmem, *pmem2, *pmem3;

	tret = sbrk(0);
	if (tret != (void *)-1) {
		printf ("heap start: %p\n", tret);
	}

	pmem = fmalloc(64); //get 0x20ff8 space
	tret = sbrk(0);
	if (tret != (void *)-1) {
		printf ("pmem:%p, heap end:%d, heap size on each load: %p\n", pmem, tret, (char *)tret-pmem);
	}

	pmem2 = fmalloc(64); //reuse 0x20ff8 space
	tret = sbrk(0);
	if (tret != (void *)-1) {
		printf ("pmem2:%p, heap end:%d, heap size on each load: %p\n", pmem2, tret, (char *)tret-pmem2);
	}


	pmem3 = fmalloc(131*1024); //mmap asign space
	tret = sbrk(0);
	if (tret != (void *)-1) {
		printf ("pmem3:%p, heap end:%d, heap size on each load: %p\n", pmem3, tret, (char *)tret-pmem3);
	}

	free(pmem);
	free(pmem2);
	free(pmem3);
	return 0;
}
