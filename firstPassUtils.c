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

#define MAX_LINE_LEN 81
#define MAX_TOKENS 50

extern char *Darr, *Iarr;
extern HashTable Atable;
extern bool errorFound;

void tokenizeLine(char *line, char **tokens, int *num_tokens)
{

    char *delim = " \t\n,()";

    char *token;

    *num_tokens = 0;

    token = strtok(line, delim); /* split line into tokens */

    while (token != NULL && *num_tokens < MAX_TOKENS)
    {

        *(tokens + *num_tokens) = token; /* assign token to array using pointer arithmetic */

        (*num_tokens)++;

        token = strtok(NULL, delim);
    }
}

void tag2Table(char **Iarr, char **Darr, char *Ltype, char *line, int *word_num, HashTable *Atable, HashTable *Symbol_Table, int *DC, int *IC, char *token, char **tokens, int *num_tokens)
{
    int j = 0, len = 0;
    char tag[80];

    Ltype = strcpy(Ltype, lineType(tokens[1], Atable));

    if (strcmp(Ltype, "data") == 0 || strcmp(Ltype, "string") == 0)
    {
        if (get(Symbol_Table, tokens[0]) != -1)
        {
            printf("\n ERROR: There is a tag with the same name in the Table already! \n");
        }

        *word_num = 0;

        /* remove ':' from end of tag */
        len = strlen(tokens[0]);
        strncpy(tag, tokens[0], len - 1);
        tag[len - 1] = '\0';

        set(Symbol_Table, tag, *DC, "data"); /* Adding Data tag to Data Tag hashtable*/

        Asm2Machine(Darr, &tokens[1], *num_tokens, DC, Atable, Symbol_Table, Ltype);

        *word_num = num_of_words(tokens[0], Atable, line, Ltype, DC);

        /**DC += 1;*/
    }

    else if (strcmp(Ltype, "extern") == 0 || strcmp(Ltype, "entry") == 0)

    {

        if (strcmp(Ltype, "extern") == 0)

        {

            token = strtok(line, " \t\n");

            token = strtok(NULL, " \t\n");

            j = 0;

            while (token != NULL)

            {

                if (strcmp(tokens[j], ".extern") == 0)

                {

                    continue;
                }

                /* remove ':' from end of tag */
                len = strlen(tokens[j]);
                strncpy(tag, tokens[j], len - 1);
                tag[len - 1] = '\0';

                set(Symbol_Table, tag, 0, "external");

                /* *DC = *DC + 1; */

                j++;

                token = strtok(NULL, " \t\n");
            }
        }
    }

    else if (strcmp(Ltype, "action") == 0)

    {

        /* remove ':' from end of tag */
        len = strlen(tokens[0]);
        strncpy(tag, tokens[0], len - 1);
        tag[len - 1] = '\0';

        set(Symbol_Table, tag, *IC, "code"); /* putting the Tag in the instructions Label Table*/

        /* *IC = *IC + 1; */

        Asm2Machine(Iarr, tokens, *num_tokens, IC, Atable, Symbol_Table, Ltype);

        /* *IC = *IC + L; */
    }
}

int operand_num(char **tokens, int *num_tokens, HashTable *Atable)

{

    int i = 0;

    while (get(Atable, tokens[i]) == -1) /* finding the index of the action word in the given line */

    {

        i++;
    }

    return (*num_tokens - i); /* returns the number of tokens after the action, this is all the operands */
}

bool isCommentOrSpace(char *str) /* check if comment line */

{

    while (*str != '\0' && isspace((unsigned char)*str))
    {

        str++;
    }

    return (*str == '\0' || *str == ';');
}

bool isTag(char *str) /* check if first word is a tag*/

{

    if (str[strlen(str) - 1] == ':') /* checking if the last char of the first word is ':' */

    {

        return true;
    }

    return false;
}

char *lineType(char *word, HashTable *Atable)
{
    if (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0 || strcmp(word, ".extern") == 0 || strcmp(word, ".entry") == 0)
    {
        return &word[1];
    }

    else if (get(Atable, word) != -1)
    {
        return "action";
    }
    else
    {
        printf("\n ERROR: Undefined line type \n");
        errorFound = true;
        return word;
    }
}

int count_lines(FILE *file)
{

    int num_lines = 0;

    char ch = ' ';

    while ((ch = fgetc(file)) != EOF)
    {

        if (ch == '\n')
        {

            num_lines++;
        }
    }

    /* If the last line doesn't end with a newline, count it too */

    if (num_lines > 0 && ch != '\n')
    {

        num_lines++;
    }

    fseek(file, 0, SEEK_SET);

    return num_lines;
}

bool isRegister(char *str)
{

    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

    int num_registers = 8, i = 0;

    for (i = 0; i < num_registers; i++)
    {

        if (strcmp(str, registers[i]) == 0)
        {

            return true;
        }
    }

    return false;
}

int num_of_words(char *token, HashTable *Atable, char *str, char *Ltype, int *DC)

{

    int num = 0, i = 0;

    int len = strlen(str);

    if (strcmp(Ltype, "data") == 0 || strcmp(Ltype, "string") == 0)

    {

        token = strtok(str, " \t\n");

        token = strtok(NULL, " \t\n"); /* skip the first word, i.e. ".data" or ".string" */

        if (strcmp(Ltype, "data") == 0)

        {

            while (token != NULL)

            {

                num++;

                token = strtok(NULL, " \t\n");
            }
        }

        else if (strcmp(Ltype, "string") == 0)

        {

            num = strlen(token) - 1; /* count the number of chars in string without " ", we count the first char instead of '/0' */
        }

        else

        {

            while (token != NULL)

            {

                num++;

                token = strtok(NULL, " \t\n");
            }
        }

        return num;
    }

    else

    {

        for (i = 0; i < len; i++)

        {

            if (get(Atable, &str[i]) != -1)

            {

                continue;
            }

            num++; /* counting the tags, numbers and registers */

            if (strcmp(Ltype, "entry") == 0 || strcmp(Ltype, "extern") == 0)

            {

                num--; /* skip the .entry/.extern word */
            }

            if (isRegister(&str[i]) && isRegister(&str[i - 1]))

            {

                num--;
            }
        }

        return num;
    }
}

int type2num(char *Ltype)

/* this function converts line type to a number following this rule: "data"-0, "string"-1, "entry"-2, "extern"-3, "action"-4*/

{

    if (strcmp(Ltype, "data") == 0)
    {

        return 0;
    }
    else if (strcmp(Ltype, "string") == 0)
    {

        return 1;
    }
    else if (strcmp(Ltype, "action") == 0)
    {

        return 2;
    }
    else if (strcmp(Ltype, "entry") == 0)

    {

        return 3;
    }

    else if (strcmp(Ltype, "extern") == 0)
    {

        return 4;
    }

    return -1;
}

char *opMethod(char *token)

{

    if (token[0] == '#')
    {

        return "00";
    }

    else if (isRegister(token))
    {

        return "11";
    }

    else
    {

        return "01";
    }
}

bool isBranch(char *opcode)

{

    return (strcmp(opcode, "1001") == 0 || strcmp(opcode, "1010") == 0 || strcmp(opcode, "1101") == 0);
}

void Asm2Machine(char **arr, char **tokens, int num_tokens, int *Counter, HashTable *Atable, HashTable *Symbol_Table, char *Ltype)

{

    int i = 1, z = 0, len = 0, j = 0, k = 0, n = 0, charVal = 0, offset = 1;

    int typeNum = -1;

    char ic[15] = "\0", opcode[5] = {0}, tmp1[10] = {0}, tmp[15] = {0}, letter[15] = {0};

    typeNum = type2num(Ltype);

    switch (typeNum)
    {

    case 0:

        /* check if token at index 0 is a tag, and decrement i if it is*/

        if (isTag(tokens[0]))
        {
            offset--;
            i--;
        }

        /* loop through remaining tokens, converting each number to binary and storing in arr */

        while (i < (num_tokens - offset))
        {
            if (tokens[i][z] == '+')
            {
                strcpy(tmp, (int2binary((char2int(tokens[i++])), 14)));
                strcpy(arr[*Counter], tmp);
                *Counter = *Counter + 1;
                z = 0;
            }
            else if (tokens[i][z] == '-')
            {
                strcpy(tmp, int2binary(char2int(tokens[i++]), 14));
                strcpy(arr[*Counter], tmp);
                *Counter = *Counter + 1;
                z = 0;
            }
            else
            {
                strcpy(tmp, int2binary(char2int(tokens[i++]), 14));
                strcpy(arr[*Counter], tmp);
                *Counter = *Counter + 1;
                z = 0;
            }
        }

        break;

    case 1:

        /* j = 1;  skip first char */

        if (!isTag(tokens[0]))

        {

            i--;
        }

        i++;

        len = strlen(tokens[i]); /* no perantices or '\0' since we will add it later */

        while (j < len)

        {

            charVal = (int)tokens[i][j++];

            if (charVal == (int)'"')
            {
                continue;
            }

            if (charVal == '\0')
            {
                continue;
            }

            strcpy(letter, int2binary(charVal, 14));

            strcpy(arr[*Counter], letter);

            *Counter = *Counter + 1;
        }

        strcpy(arr[*Counter], "00000000000000"); /* assinging the NULL termonator '\0' */

        *Counter = *Counter + 1;
        break;

    case 2:

        if (!isTag(tokens[0]))
        {
            i--;
        }

        strcpy(opcode, int2binary(get(Atable, tokens[i]), 4));

        i++; /* moving to first Operand */

        if (isBranch(opcode))
        {
            if (!(isRegister(tokens[i]) && isalpha(tokens[i][0])))
            {
                if ((num_tokens - (i + 1) == 2))
                {
                    strcat(ic, opMethod(tokens[i + 1]));
                    strcat(ic, opMethod(tokens[i + 2]));
                    strcat(ic, opcode);
                    strcat(ic, "001000");
                    strcpy(arr[*Counter], ic);
                    *Counter = *Counter + 1;
                }
                else
                {
                    strcpy(ic, "0000"); /* since we only have methods 0,1,3 we don't need parametrs method*/
                    strcat(ic, opcode);
                    strcat(ic, "00");
                    strcat(ic, opMethod(tokens[i]));
                    strcat(ic, "00");
                    strcpy(arr[*Counter], ic);
                    *Counter = *Counter + 1;
                }
            }
        }
        else
        {
            strcpy(ic, "0000"); /* since we only have methods 0,1,3 we don't need parametrs method*/
            strcat(ic, opcode);
            n = i;

            for (k = i; k < num_tokens; k++)
            {
                if ((num_tokens - n) == 1 && (num_tokens - k) == 1)
                {
                    strcat(ic, "00");
                }
                strcat(ic, opMethod(tokens[k]));
            }

            strcpy(tmp1, ic);

            if (strlen(tmp1) == 12)
            {
                strcat(tmp1, "00");
            }
            else
            {
                while (strlen(tmp1) != 14)
                {
                    strcat(tmp1, "00");
                }
            }
            strcpy(arr[*Counter], tmp1);
            *Counter = *Counter + 1;
        }

        for (; i < num_tokens; i++) /* dealing with tokens, translating word to machine*/
        {
            if (tokens[i][0] == '#')
            {
                strcpy(tmp1, int2binary(char2int(tokens[i]), 12));
                strcat(tmp1, "00");
                strcpy(arr[*Counter], tmp1);
                (*Counter)++;
            }

            else if (isRegister(tokens[i]))
            {
                if (((i + 1) < num_tokens) && isRegister(tokens[i + 1])) /* special case, 2 registers*/
                {

                    strcpy(tmp1, int2binary((tokens[i++][1] - '0'), 6));
                    strcat(tmp1, int2binary((tokens[i][1] - '0'), 6));
                    strcat(tmp1, "00");
                    strcpy(arr[*Counter], tmp1);
                    (*Counter)++;
                }
                else
                {
                    if (i + 1 == num_tokens)
                    {
                        strcpy(tmp1, int2binary((tokens[i][1] - '0'), 12));
                        strcat(tmp1, "00");
                        strcpy(arr[*Counter], tmp1);
                        (*Counter)++;
                    }
                    else
                    {
                        strcpy(tmp1, int2binary((tokens[i][1] - '0'), 6));
                        strcat(tmp1, "00000000");
                        strcpy(arr[*Counter], tmp1);
                        (*Counter)++;
                    }
                }
            }
            else if (isalpha(tokens[i][0]))
            {

                (*Counter)++;
            }
            else
            {
                printf("\n ERROR: Undefiend token!");
                errorFound = true;
            }
        }

        break;

    default:
        printf("\nERROR: This line doesn't need to be coded in machine language!\n");
    }
}
