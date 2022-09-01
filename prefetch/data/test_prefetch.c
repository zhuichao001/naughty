#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int binarySearch(int *array, int number_of_elements, int key) {
    int low = 0, high = number_of_elements-1, mid;
    while(low <= high) {
        mid = (low + high)/2;
#ifdef DO_PREFETCH
        // low path
        __builtin_prefetch (&array[(mid + 1 + high)/2], 0, 1);
        // high path
        __builtin_prefetch (&array[(low + mid - 1)/2], 0, 1);
#endif

        if(array[mid] < key)
            low = mid + 1; 
        else if(array[mid] == key)
            return mid;
        else if(array[mid] > key)
            high = mid-1;
    }
    return -1;
}

int main() {
    int SIZE = 1024*1024*128;
    int *array =  malloc(SIZE*sizeof(int));
    for (int i=0; i<SIZE; i++){ //install ordered data
        array[i] = i;
    }

    srand(time(NULL));
    int NUM_LOOKUPS = 1024*1024*8;
    int *lookups = malloc(NUM_LOOKUPS * sizeof(int));
    for (int i=0;i<NUM_LOOKUPS;i++){
        lookups[i] = rand() % SIZE;
    }

    for (int i=0;i<NUM_LOOKUPS;i++){
        int result = binarySearch(array, SIZE, lookups[i]);
    }

    free(array);
    free(lookups);
    return 0;
}
