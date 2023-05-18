#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Function.h"

#define MAX_DOMAIN 256

void file_open(errno_t err) {

    if (err != 0) {
        printf("ERROR input");
        exit(1);
    }
}

void write_in_file() {
    errno_t err;
    FILE* file;
    err = fopen_s(&file, "dns.txt", "r+");
    file_open(err);
    fseek(file, 0L, SEEK_END);
    char domain[MAX_DOMAIN];
    char ip[MAX_DOMAIN];
    printf("Input DOMAIN: ");
    scanf_s("%s", domain, _countof(domain));
    printf("Input IP: ");
    scanf_s("%s", ip, _countof(ip));

    fprintf(file, "\n%s %s", domain, ip);
    fclose(file);
}