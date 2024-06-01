#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

#include <ctype.h>

#include "HTable.h"

#include "firstPassUtils.h"

extern char externList[500];

char *int2binary(int num, int bits)
{
    char *binary;
    int i;

    binary = malloc(bits + 1);
    if (binary == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (i = bits - 1; i >= 0; i--)
    {
        binary[i] = ((num & 1) == 1) ? '1' : '0';
        num >>= 1;
    }

    binary[bits] = '\0';
    return binary;
}

int char2int(char *word)

{

    if (word[0] == '#')
    {
        if (word[1] == '+')
        {
            return atoi(word + 2);
        }
        else
        {
            return atoi(word + 1);
        }
    }
    if (word[0] == '+')
    {
        return atoi(word + 1);
    }
    return atoi(word);
}

void printArray(char **arr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (arr[i] != NULL)
        {
            printf("%s\n", arr[i]);
        }
    }
}

void newMachine(char *line)
{
    int len, i = 0;
    len = strlen(line);

    for (i = 0; i < len; i++)
    {
        if (line[i] == '0')
        {
            line[i] = '.';
        }
        else
        {
            line[i] = '/';
        }
    }
}

void change_file_extension(char *new_filename, char *new_ext)
{
    char *dot = strrchr(new_filename, '.');

    if (dot == NULL)
    {
        strcat(new_filename, ".");
        strcat(new_filename, new_ext);
    }
    else
    {
        strcpy(dot + 1, new_ext);
    }
}

void write_files(HashTable *table, char **Iarr, char **Darr, int IC, int DC, char *file_name)
{
    int i = 0, j = 0, lineNum;
    FILE *fp;

    /* writing OB file */
    change_file_extension(file_name, "ob");

    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Failed to open file for writing.");
        return;
    }
    fprintf(fp, "\t\t%d \t%d\n", IC, DC);
    for (i = 0; i < IC; i++)
    {
        newMachine(Iarr[i]);
        fprintf(fp, "%04d\t\t%s\n", (i + 100), Iarr[i]);
    }
    lineNum = i;
    for (i = 0; i < DC; i++)
    {
        newMachine(Darr[i]);
        fprintf(fp, "%04d\t\t%s\n", ((lineNum + i) + 100), Darr[i]);
    }

    fclose(fp);

    /* ent file   */
    change_file_extension(file_name, "ent");
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Failed to open file for writing.");
        return;
    }

    for (j = 0; j < table->size; j++)
    {

        Entry *entry = table->entries[j];

        while (entry != NULL)
        {
            if (strcmp(get_type(table, entry->key), "entry") == 0)
            {
                fprintf(fp, "%s\t\t%d\n", entry->key, 100 + entry->value);
            }

            entry = entry->next;
        }
    }

    fclose(fp);

    /* ext file   */
    change_file_extension(file_name, "ext");
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Failed to open file for writing.");
        return;
    }

    fprintf(fp, "%s", externList);

    fclose(fp);
}
