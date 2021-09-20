
static pfd[2];

void open(void){
    if(pipe(pfd) < 0){
        fprintf(stderr, "open pipe error\n");
    }
}

void tell(pid_t pid){
    if(write(pfd[1], "*", 1) != 1){
        fprintf(stderr, "write pipe error\n");
    }
}

void wait(){
    char _;
    if(read(pfd[0], &_, 1) != 1){
        fprintf(stderr, "read pipe error\n");
    }
    if(_ != '*'){
        fprintf(stderr, "read incorrect data\n");

    }
}
