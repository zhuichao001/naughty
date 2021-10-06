set -x

make

time ./test_mutex_v0
time ./test_spinlock_v0

time ./test_mutex_v1
time ./test_spinlock_v1

make clean
