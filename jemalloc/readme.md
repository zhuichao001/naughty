## 主动打印Stats信息
```
// reference: https://jemalloc.net/jemalloc.3.html
void malloc_stats_print(void (*write_cb)(void *, const char *), // 回调函数，可以写入文件
        void *cbopaque, // 回调函数参数
        const char *opts); // stats的一些选项，如"J"是导出json格式
```

## 程序结束后自动打印Stats
```
export MALLOC_CONF=stats_print:true
```

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

## 调整空闲内存回收时间：
```
export MALLOC_CONF=dirty_decay_ms:xxx  # -1为不释放dirty extents，易发生OOM
export MALLOC_CONF=muzzy_decay_ms:xxx  # -1为不释放muzzy extents，易发生OOM
```

## 调整ncached_max默认参数:
```
export MALLOC_CONF=tcache_nslots_small_min:xxx,tcache_nslots_small_max:xxx,lg_tcache_nslots_mul:xxx
```

## Slab size设置方法：
```
export MALLOC_CONF="slab_sizes:1-4096:17|100-200:1|128-128:2" # -左右表示size范围，:后设置page数，|分割各个不同的size范围
```
