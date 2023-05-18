#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Function.h"

#define MAX_DOMAIN 256

int main() {
    Cache* new = malloc(sizeof(Cache));
    first_add(new);
    const char* filename = "dns.txt";

    int work = 1;
    while (work) {
        int pick;
        char domain[MAX_DOMAIN];
        printf("\nMenu:\n1 - Find\n2 - Add\n3 - View cache\n4 - Exit\nYour pick: ");
        while (scanf_s("%d", &pick) == 0 || pick < 1 || pick > 4) {
            printf("\nInvalid pick\nYour pick: ");
            rewind(stdin);
        }
        switch (pick) {
        case 1:
            printf("Input domain: ");
            scanf_s("%s", domain, _countof(domain));
            printf("IP %s: %s\n", domain, get_ip(new, domain, filename));
            break;
        case 2:
            write_in_file();
            break;
        case 3:
            write_all_cache(new);
            break;
        default:
            work = 0;
            break;
        }
    }

    free_cache(new);
    free(new);
    return 0;
}