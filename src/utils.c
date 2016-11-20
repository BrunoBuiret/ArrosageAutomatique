#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "utils.h"
#include "config.h"
#include "src/xmalloc.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

bool is_file(const char *path)
{
#ifdef HAVE_UNISTD_H
    return access(path, F_OK) != -1;
#else
    FILE *file;
    
    if(file = fopen(fname, "r"))
    {
        fclose(file);
        
        return true;
    }
    
    return false;
#endif
}

char* actions_path(const char *type, unsigned int i, const char *action)
{
    char *number = uint_to_string(i);
    char *path = (char*) xmalloc(
        strlen("/var/www/html/data/") +
        strlen(type) +
        strlen(number) +
        (action != NULL ? sizeof(char) + strlen(action) : 0) +
        strlen(".txt")
    );
    
    strcpy(path, "/var/www/html/data/");
    strcat(path, type);
    strcat(path, number);
    
    if(action != NULL)
    {
        strcat(path, "_");
        strcat(path, action);
    }
    
    strcat(path, ".txt");
    
    free(number);
    
    return path;
}

char* uint_to_string(unsigned int input)
{
    char *output = (char*) xmalloc(
        input > 0
        ? (int) ((ceil(log10(input)) + 1) * sizeof(char))
        : sizeof(char)
    );
    
    sprintf(output, "%u", input);
    
    return output;
}
