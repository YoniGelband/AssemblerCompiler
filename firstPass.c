#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "HTable.h"
#include "firstPassUtils.h"
#include "assemblyUtils.h"
#include "firstPass.h"

#define MAX_LINE_LEN 81

#define MAX_TOKENS 50

#define COL_SIZE 1

#define ROWS 256

extern char **Darr;
extern char **Iarr;
extern bool errorFound;

#include "secondPass.h"

int Initial_Pass(char *file_name)
{

    int IC = 0, DC = 0, i = 0;

    char line[256];

    char token[256];

    char *tokens[MAX_TOKENS];

    char Ltype[20], newLine[81];

    int num_tokens = 0, num_lines = 0;

    bool tag = false;

    int word_num = 0;

    HashTable *Symbol_Table, *Atable;

    FILE *file;

    Darr = calloc(ROWS, sizeof(char **));
    for (i = 0; i < ROWS; i++)
    {
        Darr[i] = calloc(COL_SIZE, sizeof(char *));
        Darr[i][0] = 0;
    }

    Iarr = calloc(ROWS, sizeof(char **));
    for (i = 0; i < ROWS; i++)
    {
        Iarr[i] = calloc(COL_SIZE, sizeof(char *));
        Iarr[i][0] = 0;
    }

    Atable = create_table(16);

    actionTable(Atable); /* creates action table which contains all the legal assembler actions*/

    file = fopen(file_name, "r");

    if (file == NULL)
    {

        printf("\n Error opening the file.\n");
        errorFound = true;

        return -1;
    }

    num_lines = count_lines(file); /* get number of lines in file */

    /*rewind(file);*/

    Symbol_Table = create_table(num_lines);

    if (file == NULL)
    {

        printf("Error opening file: %s\n", file_name);
        errorFound = true;

        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAX_LINE_LEN, file) != NULL)
    {
        /*printf("%s ", line);*/

        if (isCommentOrSpace(line))
        { /* don't read comment line */

            continue;
        }

        strcpy(newLine, line);

        tokenizeLine(line, tokens, &num_tokens); /* splitting line into words without any unnecessary symbols */
        tag = isTag(tokens[0]);

        if (tag) /* deciding if the word is a tag */
        {

            tag2Table(Iarr, Darr, Ltype, newLine, &word_num, Atable, Symbol_Table, &DC, &IC, token, tokens, &num_tokens); /* deciding the table in which to place a tag */
        }

        else if (strcmp(tokens[0], ".data") == 0 || strcmp(tokens[0], ".string") == 0)
        {

            strcpy(Ltype, lineType(tokens[0], Atable));

            Asm2Machine(Darr, tokens, num_tokens, &DC, Atable, Symbol_Table, Ltype); /* translating line to machine language and placing in array */
        }

        else if (strcmp(tokens[0], ".entry") == 0 || strcmp(tokens[0], ".extern") == 0)
        {

            if (strcmp(tokens[0], ".extern") == 0)
            {

                int i = 1;

                while (i < num_tokens)
                {

                    set(Symbol_Table, tokens[i], 0, "external"); /* adding all extern tags to the tag table with no value */

                    i++;
                }
            }
        }

        else if (get(Atable, tokens[0]) != -1)
        {

            strcpy(Ltype, "action");

            Asm2Machine(Iarr, tokens, num_tokens, &IC, Atable, Symbol_Table, Ltype);
        }

        else
        {
            printf("\n ERROR: Line syntax not supported in this assembler! \n");
            errorFound = true;
        }
    }

    increment_values(Symbol_Table, IC);

    /*
    print_table(Symbol_Table);

    printf("\n ===== First Pass ==== \n\n");
    printf("\n Iarr printed below:\n");
    printArray(Iarr, IC);
    printf("\n Darr printed below:\n");
    printArray(Darr, DC);
    */

    if (errorFound == true)
    {
        printf("ERRORS FOUND - STOPPING OPERATIONS\n");
        exit(1);
    }

    secondary_pass(Atable, Symbol_Table, num_tokens, file_name);
    /*
    printf("\n ===== Second Pass ==== \n\n");
    printf("\n Iarr printed below:\n");
    printArray(Iarr, IC);
    */

    write_files(Symbol_Table, Iarr, Darr, IC, DC, file_name);

    fclose(file);

    return 0;
}
