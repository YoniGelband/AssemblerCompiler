#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "HTable.h"
#include "firstPassUtils.h"
#include "assemblyUtils.h"
#include "firstPass.h"
#include "secondPass.h"
#include "secondPassUtils.h"
#define MAX_LINE_LEN 81
#define MAX_TOKENS 50

extern char **Iarr;
extern char externList[500];

int Asm2Machine2(char **tokens, int IC, int num_tokens, HashTable *Atable, HashTable *Symbol_Table)
{

    int i;
    char tmp[15];
    int binary_operand;
    /*char A_R_E_bits[2];*/

    for (i = 0; i < num_tokens - 1; i++)
    {
        if (get(Atable, tokens[i]) != -1)
        {
            (IC)++;
            continue;
        }
        if (tokens[i][0] == '#')
        { /* if operand is a number */
            (IC)++;
            continue;
        }
        else if (tokens[i][0] == 'r')
        {
            if ((i + 1) < (num_tokens - 1) && tokens[i + 1][0] == 'r')
            {
                i++;
            }
            /* if operand is a register */
            IC++;
            continue;
        }
        else
        { /* operand is a tag */

            if (strcmp(get_type(Symbol_Table, tokens[i]), "external") == 0)
            {
                strcpy(Iarr[IC], "00000000000001");
                sprintf(tmp, "%s\t\t%d\n", tokens[i], IC + 100);
                strcat(externList, tmp);
                IC++;
            }
            else
            {
                binary_operand = get(Symbol_Table, tokens[i]);
                binary_operand += 100;
                strcpy(tmp, int2binary(binary_operand, 12));
                strcat(tmp, A_R_Etype(tokens[i], Symbol_Table));

                strcpy(Iarr[IC], tmp);
                IC++;
            }
        }
    }

    return IC;
}

char *A_R_Etype(char *token, HashTable *Symbol_Table)
{
    char type[10];

    strcpy(type, get_type(Symbol_Table, token));

    if (strcmp(type, "") == 0)
    {
        printf("ERROR: Invalid tag type!");
        exit(1);
    }
    else if (strcmp(type, "external") == 0)
    {
        return "01";
    }
    else if ((strcmp(type, "code") == 0) || (strcmp(type, "entry") == 0) || (strcmp(type, "data") == 0) || (strcmp(type, "string") == 0))
    {
        return "10";
    }
    else
    {
        printf("ERROR: Invalid tag type!");
        exit(1);
    }
}