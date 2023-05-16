#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Function.h"

#define CACHE_SIZE 3
#define MAX_KEY 256
#define MAX_IP 16

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
        if(tmp->next == NULL) cache->tail->prev->next = cache->tail;
        tmp2->next = tmp;
        tmp = tmp2;
        tmp_new = tmp_new->prev;
        tmp2 = tmp2->prev;
    }
    cache->tail->next = NULL;
    tmp_new->prev = NULL;
    cache->head = tmp_new;
}

struct Cache_entry* find_in_cache(Cache* cache, const char* key) {
    struct Cache_entry* entry = cache->tail;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            search_same(entry, cache);
            return cache->tail;
        }
        entry = entry->prev;
    }
    return NULL;
}

char* find_in_cache_two_type(Cache* cache, const char* value) {
    struct Cache_entry* entry = cache->tail->prev;
    while (entry != NULL) {
        if (strcmp(entry->value, value) == 0) {
            return entry->key;
        }
        entry = entry->prev;
    }
    return NULL;
}

char* get_ip(Cache* cache, const char* key, const char* filename) {
    struct Cache_entry* entry = find_in_cache(cache, key);
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
            fclose(file);
            add_to_cache(cache, key, record_ip);
            char* type_two = find_in_cache_two_type(cache, cache->tail->value);
            if (type_two != NULL) return type_two;
            return cache->tail->value;
        }
    }

    fclose(file);
    return NULL;
}