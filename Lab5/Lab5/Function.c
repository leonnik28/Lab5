#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Function.h"

#define CACHE_SIZE 3
#define MAX_KEY 256
#define MAX_IP 16
#define HASH_CONST 66

unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = hash * HASH_CONST + *str++;
    }
    return hash % CACHE_SIZE;
}

void add_to_cache(Cache* cache, const char* key, const char* value) {
    struct Cache_entry* entry = (struct Cache_entry*)malloc(sizeof(struct Cache_entry));
    entry->key = (char*)malloc(strlen(key));
    strcpy_s(entry->key, strlen(entry->key), key);
    entry->value = (char*)malloc(strlen(value));
    strcpy_s(entry->value, strlen(entry->value), value);
    entry->next = NULL;
    entry->prev = NULL;
    if (cache->size < CACHE_SIZE) {
        if (cache->head == NULL) {
            cache->tail = entry;
            cache->head = cache->tail;
            cache->size = 1;
        }
        else {
            entry->prev = cache->tail;
            cache->tail = entry;
            if (cache->tail->prev != NULL) {
                cache->tail->prev->next = cache->tail;
            }
            cache->size++;
        }
    }
    else {
        search_same(entry, cache);
    }
    int index = hash(key);
    cache->hash_table[index] = entry;
}


void free_cache(Cache* cache) {
    struct Cache_entry* tmp = cache->tail;
    while (tmp != NULL) {
        if (tmp->prev == NULL) {
            free(tmp);
            break;
        }
        tmp = tmp->prev;
        free(tmp->next);
    }
}

void search_same(struct Cache_entry* entry, Cache* cache) {
    struct Cache_entry* tmp = cache->tail;
    struct Cache_entry* tmp2 = cache->tail->prev;
    struct Cache_entry* tmp_3 = entry->prev;
    cache->tail = entry;
    struct Cache_entry* tmp_new = cache->tail;
    while (tmp != cache->head) {
        if (tmp == entry) {
            do {
                tmp_new->prev = tmp_3;
                tmp_new->prev->next = tmp_new;
                tmp_3 = tmp_3->prev;
                tmp_new = tmp_new->prev;
            } while (tmp_3 != cache->head && tmp_3 != NULL);
            break;
        }
        tmp->prev = tmp;
        tmp_new->prev = tmp;
        if (tmp->next == NULL) cache->tail->prev->next = cache->tail;
        tmp2->next = tmp;
        tmp = tmp2;
        tmp_new = tmp_new->prev;
        tmp2 = tmp2->prev;
    }
    cache->tail->next = NULL;
    tmp_new->prev = NULL;
    cache->head = tmp_new;
}

void cache_bypass(Cache* cache, const char* key) {
    struct Cache_entry* entry = cache->tail;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            search_same(entry, cache);
            return;
        }
        entry = entry->prev;
    }
}

struct Cache_entry* find_in_table(Cache* cache, const char* key, int index) {
    if (cache->hash_table[index] != NULL && strcmp(cache->hash_table[index]->key, key) == 0) {
        return cache->hash_table[index];
    }
    return NULL;
}

char* find_in_file_two_type(Cache* cache, const char* key, FILE* file) {
    char buffer[MAX_KEY];
    while (fgets(buffer, sizeof(buffer), file)) {
        char record_key[MAX_KEY];
        char record_ip[MAX_IP];
        if (sscanf_s(buffer, "%s %s", record_key, _countof(record_key), record_ip, _countof(record_ip)) < 2) {
            continue;
        }
        if (strcmp(record_key, key) == 0) {
            cache->tail->value = strdup(record_ip);
            return strdup(record_ip);
        }
    }
    return cache->tail->value;
}

void first_add(Cache* cache) {
    cache->hash_table = (struct Cache_entry**)malloc(sizeof(struct Cache_entry*));
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->hash_table[i] = NULL;
    }
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;

}

void write_all_cache(Cache* cache) {
    struct Cache_entry* tmp = cache->tail;
    while (tmp != NULL) {
        printf("%s %s\n", tmp->key, tmp->value);
        tmp = tmp->prev;
    }
}


char* get_ip(Cache* cache, const char* key, const char* filename) {
    int index = hash(key);
    struct Cache_entry* entry = find_in_table(cache, key, index);
    cache_bypass(cache, key);
    if (entry != NULL) {
        return entry->value;
    }

    errno_t err;
    FILE* file;
    err = fopen_s(&file, filename, "r");
    file_open(err);

    char buffer[MAX_KEY];
    while (fgets(buffer, sizeof(buffer), file)) {
        char record_key[MAX_KEY];
        char record_ip[MAX_IP];
        if (sscanf_s(buffer, "%s %s", record_key, _countof(record_key), record_ip, _countof(record_ip)) < 2) {
            continue;
        }
        if (strcmp(record_key, key) == 0) {
            fseek(file, 0L, SEEK_SET);
            add_to_cache(cache, key, record_ip);
            char* type_two = find_in_file_two_type(cache, cache->tail->value, file);
            if (type_two != NULL) {
                fclose(file);
                return type_two;
            }
        }
    }
    fclose(file);
    return NULL;
}