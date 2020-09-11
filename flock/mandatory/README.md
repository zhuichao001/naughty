# Acquire a lock in the first terminal:
```sh
$ ./fcntl_lock dir/lockfile  
(wait for a while)  
^C  
```

# Try to read the file in the second terminal:
```sh
$ cat dir/lockfile  
(hangs until ^C is pressed in the first terminal)  
hello  
```
