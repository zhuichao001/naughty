set -x

g++ -o test_mqcreate mqcreate.c -lpthread -lrt
g++ -o test_mqnotifyrecv mqnotifyrecv.c -lpthread -lrt
g++ -o test_mqsend mqsend.c -lpthread -lrt
g++ -o test_mqreceive mqreceive.c -lpthread -lrt

./test_mqcreate /test1

#open another shell to exec
./test_mqsend /test1 50 16

./test_mqnotifyrecv /test1
#./test_mqreceive /test1

