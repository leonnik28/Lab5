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
    struct Cache_entry** hash_table;
} Cache;

void first_add(Cache* cache);
unsigned int hash(const char* str);
void write_all_cache(Cache* cache);
void add_to_cache(Cache* cache, const char* key, const char* value);
void cache_bypass(Cache* cache, const char* key);
struct Cache_entry* find_in_table(Cache* cache, const char* key, int index);
char* find_in_file_two_type(Cache* cache, const char* key, FILE* file);
void search_same(struct Cache_entry* entry, Cache* cache);
char* get_ip(Cache* cache, const char* key, const char* filename);
void write_in_file();
void file_open(errno_t err);
void free_cache(Cache* cache);
#endif