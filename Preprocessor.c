#include <stdio.h>

#include <stdlib.h>

#include <string.h>



#define MAX_LINE_LENGTH 80

#define MAX_MCR_LENGTH 2048

#define MAX_NUM_MCRS 100



/* Define a struct to hold macro information */  

struct macro {

    char name[MAX_LINE_LENGTH];

    char content[MAX_MCR_LENGTH];

};



/* Keep track of the number of macros and an array of macros */ 

int num_macros = 0;

struct macro macros[MAX_NUM_MCRS];



/* implementation of strdup */

char *my_strdup(const char *src) {

    size_t len = strlen(src) + 1;

    char *dst = malloc(len);

    if (dst == NULL) {

        return NULL;

    }

    memcpy(dst, src, len);

    

    return dst;

}



/* Function to add a new macro to the list */ 

void add_macro(char *name, char *content) 

{

    if (num_macros >= MAX_NUM_MCRS) {

        printf("Error: too many macros.\n");

        exit(1);

    }

    strcpy(macros[num_macros].name, name);

    strcpy(macros[num_macros].content, content);

    num_macros++;

}



/* Function to preprocess the file */

char *preprocess(char *filename) 

{

    FILE *fp, *output_file;

    

    char line[MAX_LINE_LENGTH];

    char mcr_name[MAX_LINE_LENGTH];

    char mcr_content[MAX_MCR_LENGTH];

    char new_filename[256];

    int in_mcr = 0, i = 0;

    

    filename = strcat(filename, ".as");

    fp = fopen(filename, "r");

    if (!fp) 

    {

        printf("Error: could not open file.\n");

        exit(1);

    }

    fseek (fp, 0 , SEEK_END);

    ftell(fp);

    fseek (fp, 0, SEEK_SET);







    /* Replace ".as" with ".am" in the filename */ 

    strncpy(new_filename, filename, strlen(filename) - 3);

    new_filename[strlen(filename) - 3] = '\0';

    strcat(new_filename, ".am");



    /* Create the new file */ 

    output_file = fopen(new_filename, "w");

    if (output_file == NULL) 

    {

        printf("Error creating file\n");

        exit(1);

    }

    

    /* Loop through each line of the file */

    while (fgets(line, MAX_LINE_LENGTH, fp)) 

    {

    /* If we encounter a "mcr" block, start tracking the macro content */

    if (strncmp(line, "mcr", 3) == 0) 

    {

        in_mcr = 1;

        sscanf(line, "mcr %s", mcr_name);

        mcr_content[0] = '\0';

    } 

    /* If we encounter an "endmcr" line, add the macro to the list */

    else if (strncmp(line, "endmcr", 6) == 0) 

    {

        in_mcr = 0;

        add_macro(mcr_name, mcr_content);

    } 

    /* If we're in a macro block, add the line to the macro content */

    else if (in_mcr)

     {

        strcat(mcr_content, line);

     } 

    else 

    {

        /* Loop through each macro and replace its name with its content in the line */

        for (i = 0; i < num_macros; i++) {

            char *name = macros[i].name;

            char *content = macros[i].content;

            char *p = strstr(line, name);

            while (p) 

            {

                /* Replace the macro name with its content */

                char tmp[MAX_LINE_LENGTH];

                int pos = p - line;

                strncpy(tmp, line, pos);

                tmp[pos] = '\0';

                strcat(tmp, content);

                strcat(tmp, p + strlen(name));

                strcpy(line, tmp);

                p = strstr(line + pos + strlen(content), name);

            }

        }

        /* Write the modified line to the output file */

        fprintf(output_file, "%s", line);

    }

}



    fclose(fp);

    fclose(output_file);

    return(my_strdup(new_filename));

    

}