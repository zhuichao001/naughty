#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <libaio.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define FILEPATH "./test_aio.txt"

int main() {
    io_context_t context;
    struct iocb io[1], *p[1] = {&io[0]};
    struct io_event e[1];
    unsigned nr_events = 10;
    struct timespec timeout;
    char *wbuf;
    int wbuflen = 1024;
    int ret, num = 0, i;

    posix_memalign((void **)&wbuf, 512, wbuflen);

    memset(wbuf, '@', wbuflen);
    memset(&context, 0, sizeof(io_context_t));

    timeout.tv_sec = 0;
    timeout.tv_nsec = 10000000;

    int fd = open(FILEPATH, O_CREAT|O_RDWR|O_DIRECT, 0644); // 1. 打开要进行异步IO的文件
    if (fd < 0) {
        printf("open error: %d\n", errno);
        return 0;
    }

    if (0 != io_setup(nr_events, &context)) {               // 2. 创建一个异步IO上下文
        printf("io_setup error: %d\n", errno);
        return 0;
    }

    io_prep_pwrite(&io[0], fd, wbuf, wbuflen, 0);           // 3. 创建一个异步IO任务

    if ((ret = io_submit(context, 1, p)) != 1) {            // 4. 提交异步IO任务
        printf("io_submit error: %d\n", ret);
        io_destroy(context);
        return -1;
    }

    while (1) {
        ret = io_getevents(context, 1, 1, e, &timeout);     // 5. 获取异步IO的结果
        if (ret < 0) {
            printf("io_getevents error: %d\n", ret);
            break;
        }

        if (ret > 0) {
            printf("result, res2: %d, res: %d\n", e[0].res2, e[0].res);
            break;
        }
    }

    return 0;
}
