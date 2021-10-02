#Multiple processes are linked to the same SO dynamic library, code segments are shared, data segments are not shared
make
./run &
sleep 5
./run
