#include <stdbool.h>
#include "config.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
#include <stdio.h>
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