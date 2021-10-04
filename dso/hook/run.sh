cd dep && make
cd .. && make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./dep
./run_hook
