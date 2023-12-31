#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib_tar.h"

/**
 * You are free to use this file to write tests for your implementation
 */

void debug_dump(const uint8_t *bytes, size_t len) {
    for (int i = 0; i < len;) {
        printf("%04x:  ", (int) i);

        for (int j = 0; j < 16 && i + j < len; j++) {
            printf("%02x ", bytes[i + j]);
        }
        printf("\t");
        for (int j = 0; j < 16 && i < len; j++, i++) {
            printf("%c ", bytes[i]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s tar_file\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1] , O_RDONLY);
    if (fd == -1) {
        perror("open(tar_file)");
        return -1;
    } 

    int check;
    int ret;

    check = check_archive(fd);/*
    printf("check_archive returned %d\n", check);

    int ex = exists(fd, "hello/aaa.txt");
    printf("exists returned %d\n", ex);

    ret = is_dir(fd, "hello/dir/");
    printf("is_dir right returned %d\n", ret);

    ret = is_dir(fd, "hello/aaa.txt");
    printf("is_dir wrong returned %d\n", ret);

    ret = is_file(fd, "hello/aaa.txt");
    printf("is_file right returned %d\n", ret);

    ret = is_file(fd, "hello/dir/");
    printf("is_file wrong returned %d\n", ret);
    */

    //check = 1;//check-10;
    char* entries[check];
    for (int i = 0; i < check; i++) {
        entries[i] = (char*) malloc(sizeof(char[512]));
    }
    size_t no_entr = check;
    ret = list(fd, "hello/", entries, &no_entr);
    for (int i = 0; i < check; i++) {
        printf("-%s-\n", entries[i]);
        free(entries[i]);
    }
    printf("listed: %ld\t\texpected: 11\n", no_entr);
    printf("return value: %d\t\texpected: nonzero\n", ret);

    /*
    size_t len = 512;
    char reader[len];
    ret = read_file(fd, "hello/lll", 0, (uint8_t*) reader, &len);
    printf("\nFile read:\n");
    printf("+++++\n%s\n+++++\n",reader);
    printf("Bytes written to dest: %ld\t\texpected: 10\n", len);
    printf("Return value: %d\t\texpected: 0\n", ret);
    */

    return 0;
}