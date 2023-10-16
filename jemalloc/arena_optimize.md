
## 设置arena的数量
```
export MALLOC_CONF=narenas:xxx  # xxx最大为1024
```

## 设置当前线程独享arena
```
unsigned thread_set_je_exclusive_arena() {
    unsigned arena_old, arena_new;
    size_t sz = sizeof(unsigned);

    /* Bind to a manual arena. */
    if (mallctl("arenas.create", &arena_new, &sz, NULL, 0)) {
        std::cout << "Jemalloc arena create error\n";
        return 0;
    }
    if (mallctl("thread.arena", &arena_old, &sz, &arena_new, sizeof(arena_new))) {
        std::cout << "Thread bind to jemalloc arena error\n";
        return 0;
    }
    return arena_new;
}
```
