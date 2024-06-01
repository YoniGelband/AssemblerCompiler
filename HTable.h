typedef struct Entry {
    char *key;
    int value;
    char *type; 
    struct Entry *next;
} Entry;




typedef struct {

    Entry **entries;

    int size;

} HashTable;

unsigned int hash(const char *key, int size);

HashTable *create_table(int size);

void set(HashTable *table, const char *key, int value, const char *type);

int get(HashTable *table, const char *key);

const char* get_type(HashTable *table, const char *key);

void increment_values(HashTable *table, int increment);

void print_table(HashTable *table);


