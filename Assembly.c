#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "HTable.h"
#include "Preprocessor.h"
#include "firstPassUtils.h"
#include "firstPass.h"
#include "assemblyUtils.h"

char **Darr;
char **Iarr;
char externList[500];
bool errorFound;

int main(int argc, char *argv[])
{
    int i;
    char *parsed_file_name;

    if (argc < 2)
    {
        printf("NO ARGUMENTS\n");
        exit(-1);
    }

    for (i = 1; i < argc; i++)
    {
        parsed_file_name = preprocess(argv[i]);
        printf("PROCESSING FILE %s\n", parsed_file_name);
        errorFound = false;
        externList[0] = '\0'; /* initializing external List */
        Initial_Pass(parsed_file_name);
        free(parsed_file_name);
        printf("COMPLETED\n");
    }
    return 0;
}