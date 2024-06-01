#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "Preprocessor.h"

#include "HTable.h"

#define TABLE_SIZE 256

void actionTable(HashTable *Atable)

{

    set(Atable, "mov", 0, "instruction");

    set(Atable, "cmp", 1, "instruction");

    set(Atable, "add", 2, "instruction");

    set(Atable, "sub", 3, "instruction");

    set(Atable, "not", 4, "instruction");

    set(Atable, "clr", 5, "instruction");

    set(Atable, "lea", 6, "instruction");

    set(Atable, "inc", 7, "instruction");

    set(Atable, "dec", 8, "instruction");

    set(Atable, "jmp", 9, "instruction");

    set(Atable, "bne", 10, "instruction");

    set(Atable, "red", 11, "instruction");

    set(Atable, "prn", 12, "instruction");

    set(Atable, "jsr", 13, "instruction");

    set(Atable, "rts", 14, "instruction");

    set(Atable, "stop", 15, "instruction");
}

unsigned int hash(const char *key, int size)

{

    unsigned int hashval = 0;

    int i = 0;

    for (i = 0; i < strlen(key); i++)
    {

        hashval += key[i] * (i + 1);
    }

    return hashval % size;
}

HashTable *create_table(int size)
{

    int i = 0;
    HashTable *table = malloc(sizeof(HashTable));
    table->entries = calloc(size, sizeof(Entry *));
    table->size = size;

    for (i = 0; i < size; i++)
    {
        table->entries[i] = NULL;
    }

    /* Initialize the "type" field of each entry to NULL */
    for (i = 0; i < size; i++)
    {
        Entry *entry = table->entries[i];
        while (entry != NULL)
        {
            entry->type = NULL;
            entry = entry->next;
        }
    }

    return table;
}

void set(HashTable *table, const char *key, int value, const char *type)
{
    unsigned int index = 0;
    Entry *entry;

    index = hash(key, table->size);
    entry = table->entries[index];

    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            entry->value = value;
            entry->type = my_strdup(type);
            return;
        }
        entry = entry->next;
    }

    entry = malloc(sizeof(Entry));
    if (!entry)
    {
        return; /* Out of memory error */
    }

    entry->key = my_strdup(key);
    entry->value = value;
    entry->type = my_strdup(type);
    entry->next = table->entries[index];
    table->entries[index] = entry;

    /* Free the newly allocated entry in case there is an error condition */
    if (!table->entries[index])
    {
        free(entry->key);
        free(entry->type);
        free(entry);
    }
}

int get(HashTable *table, const char *key)
{
    unsigned int index = hash(key, table->size);
    Entry *entry = table->entries[index];

    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
        entry = entry->next;
    }

    /* If key is not found, return a special value indicating failure */
    return -1;
}

const char *get_type(HashTable *table, const char *key)
{
    unsigned int index = hash(key, table->size);
    Entry *entry = table->entries[index];

    while (entry != NULL)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->type;
        }
        entry = entry->next;
    }

    /* If key is not found, return NULL or a special value indicating failure */
    return NULL;
}

void increment_values(HashTable *table, int increment)
{
    int i = 0;

    for (i = 0; i < table->size; i++)
    {
        Entry *entry = table->entries[i];

        while (entry != NULL)
        {
            if (strcmp(entry->type, "data") == 0)
            {                              /* check if type is "data"*/
                entry->value += increment; /* increment value */
            }

            entry = entry->next;
        }
    }
}

void print_table(HashTable *table)
{

    int i = 0;

    for (i = 0; i < table->size; i++)
    {

        Entry *entry = table->entries[i];

        while (entry != NULL)
        {

            printf("%s: %d\n", entry->key, entry->value);

            entry = entry->next;
        }
    }
}