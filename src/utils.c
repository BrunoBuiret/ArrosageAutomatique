/* 
 * @author Thomas Arnaud <thomas.arnaud@etu.univ-lyon1.fr>
 * @author Bruno Buiret <bruno.buiret@etu.univ-lyon1.fr>
 * @author Alexis Rabilloud <alexis.rabilloud@etu.univ-lyon1.fr>
 * @version 1.0
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include "utils.h"
#include "config.h"
#include "src/xmalloc.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#include <time.h>
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

char* actions_path(const char *type, unsigned int i, const char *option)
{
    char *number = uint_to_string(i);
    char *path = (char*) xmalloc(
        strlen("/var/www/html/data/") +
        strlen(type) +
        strlen(number) +
        (option != NULL ? sizeof(char) + strlen(option) : 0) +
        strlen(".txt")
    );
    
    strcpy(path, "/var/www/html/data/");
    strcat(path, type);
    strcat(path, number);
    
    if(option != NULL)
    {
        strcat(path, "_");
        strcat(path, option);
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

void print_log(const char *format, ...)
{
    if(want_verbose)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        va_list ap;

        va_start(ap, format);
        printf("[%2d-%2d-%4d %2d:%2d:%2d] ",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year,
            tm.tm_hour, tm.tm_min, tm.tm_sec
        );
        vprintf(format, ap);
        printf("\n");
        va_end(ap);
    }
}