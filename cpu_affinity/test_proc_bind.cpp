#include <poll.h>
#include "process_bind_cpu.h"

int main(){
    process_bind_cpu(0);
    while(1){
        poll(NULL, 0, 100);
    }
    return 0;
}
