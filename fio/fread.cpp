#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

const char *kFileName = "./test.txt"; 

int test(bool isflush, bool issync){
    fprintf(stderr, "[======FLAG isflush:%d issync:%d======]\n", isflush, issync);

    char c[] = "This is text data.\n";
    const int N = 256;
    char buff[N];

    FILE *fp = fopen(kFileName, "a+");
    fwrite(c, sizeof(char), strlen(c), fp);

    if(isflush){
        fflush(fp); // from userspance to kernal
    }

    if(issync){
        fsync(fileno(fp));  //from kernal to disk
    }

    {
        FILE *fp = fopen(kFileName, "r");
        fseek(fp, 0, SEEK_SET);
        size_t count = fread(buff, sizeof(char), N, fp);
        fprintf(stderr, "Read %d bytes:%.*s\n errmsg:%s\n", count, count, buff, strerror(errno));
        fclose(fp);
    }

    {
        FILE *fp = fopen(kFileName, "r");
        fseek(fp, 0, SEEK_SET);
        size_t count = fread(buff, sizeof(char), N, fp);
        fprintf(stderr, "Read from head %d bytes:%.*s\n errmsg:%s\n", count, count, buff, strerror(errno));
        fclose(fp);
    }

    fclose(fp);
    return 0;
}

int main () {
    test(false, false); //will not read latest append data
    test(true, false);  //will read out
    test(false, true);  //will not
    test(true, true);   //will read out
    return 0;
}

