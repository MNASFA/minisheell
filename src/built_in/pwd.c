#include "../execution/execution.h"
#include <limits.h>


void pwd()
{
    char *path;
    path = (char *)malloc(PATH_MAX);
    getcwd(path, PATH_MAX);
    printf("%s\n", path);
    free(path);
}
