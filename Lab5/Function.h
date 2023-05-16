#ifndef LAB5_HEADER
#define LAB5_HEADER

struct Cache_entry {
    char* key;
    char* value;
    struct Cache_entry* next;
    struct Cache_entry* prev;
};

typedef struct {
    int size;
    struct Cache_entry* head;
    struct Cache_entry* tail;
} Cache;

void add_to_cache(Cache* cache, const char* key, const char* value);
char* find_in_cache_two_type(Cache* cache, const char* value);
struct Cache_entry* find_in_cache(Cache* cache, const char* key);
void search_same(struct Cache_entry* entry, Cache* cache);
char* get_ip(Cache* cache, const char* key, const char* filename);
void write_cache();
void file_open(errno_t err);
void free_cache(Cache* cache);
#endif
