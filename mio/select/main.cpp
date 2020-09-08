#include "server.h"

int main(int argc,char *argv[]) {
    server_engine_t svr("127.0.0.1", 9033);
    svr.start();
    return 0;
}
