#include "engine.h"
#include "server.h"

int main(int argc, char *argv[]) {
    engine_t eng;
    eng.start((const char*)"127.0.0.1", 8001, new rpc_server_t());
    eng.run();
    return 0;
}
