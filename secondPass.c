#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "HTable.h"
#include "assemblyUtils.h"
#include "firstPass.h"
#include "firstPassUtils.h"
#include "secondPass.h"

/* extern HashTable *Atable;
extern HashTable *Symbol_Table; */

#define MAX_LINE_LEN 81
#define MAX_TOKENS 50

extern char **Iarr;

int secondary_pass(HashTable *Atable, HashTable *Symbol_Table, int num_tokens, char *filename)
{

    FILE *file;
    int i = 0, IC = 0;
    char line[256];
    /* char token[256]; */
    char *tokens[MAX_TOKENS];
    char firstWord[80];
    /* int num_lines = 0; */
    /* bool tag = false; */
    /* int word_num = 0; */

    file = fopen(filename, "r");

    while (fgets(line, MAX_LINE_LEN, file) != NULL)
    {

        if (isCommentOrSpace(line))
        { /* don't read comment line */

            continue;
        }

        tokenizeLine(line, tokens, &num_tokens);

        strcpy(firstWord, tokens[0]);
        if (isTag(tokens[0]))
        {
            strcpy(firstWord, tokens[1]);
        }

        if (strcmp(firstWord, ".data") == 0 || strcmp(firstWord, ".string") == 0 || strcmp(firstWord, ".extern") == 0)
        {

            continue;
        }

        else if (strcmp(firstWord, ".entry") == 0)
        {

            int value;
            i = 1;

            while (i < num_tokens)
            {
                value = get(Symbol_Table, tokens[i]); /* adding all entry tags to the tag table with no value */
                set(Symbol_Table, tokens[i], value, "entry");
                i++;
            }
        }
        else
        {

            if (isTag(tokens[0]))
            {
                IC = Asm2Machine2(&tokens[1], IC, num_tokens, Atable, Symbol_Table); /* write function that translates from second word of instruction */
            }
            else
            {
                IC = Asm2Machine2(tokens, IC, (num_tokens + 1), Atable, Symbol_Table); /* write function that translates from second word of instruction */
            }
        }
    }
    return 0;
}
