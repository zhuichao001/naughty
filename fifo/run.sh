mkfifo fifo.key
gcc -g fifo_reader.c -o reader 
gcc -g fifo_writer.c -o writer 

./writer data.txt fifo.key >a &
./reader fifo.key 
