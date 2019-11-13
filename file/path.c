#include "path.h"
#include <string.h>
#include <limits.h>
#include <stdint.h>



char *get_file_path(char *file_name)
{
    static char path[PATH_MAX];
    int index;
    int len;

    len = strlen(file_name);

    file_name = format_path(file_name);

    while(index && file_name[index] != '/') index--;

    if(index)
    {
        strncpy(path, file_name, index);
    }
    else
    {
        strcpy(path, "./");
    }

    return path;
}

char *get_file_from_path(char *path)
{
    static char file_name[PATH_MAX];
    uint32_t path_index = strlen(file_name);
    uint32_t file_name_index = 0;

    path = format_path(path);

    while(path_index && path[path_index] != '/') path_index--;
    strncpy(file_name, path + path_index, PATH_MAX);
    return file_name;
}

char *get_file_name_no_ext(char *file_name)
{
    static char file_name_no_ext[PATH_MAX];
    uint32_t index = strlen(file_name);

    while(index && file_name[index] != '.') index--;

    if(index)
    {
        strncpy(file_name_no_ext, file_name, index);
    }
    else
    {
        strcpy(file_name_no_ext, file_name);
    }

    return file_name_no_ext;
}


char *format_path(char *path)
{
    static char formatted_path[PATH_MAX];
    int i;
    int j;

    memset(formatted_path, 0, PATH_MAX);

    for(i = 0, j = 0; path[i]; i++)
    {
        formatted_path[j] = path[i];

        if(path[i] == '\\')
        {
            formatted_path[j] = '/';

            if(path[i + 1] == '\\')
            {
                i++;
            }
        }

        j++;
    }

    formatted_path[j] = '\0';

    return formatted_path;
}

char *append_path_segment(char *path, char *segment)
{
    static char new_path[PATH_MAX];
    
    int path_len = strlen(path);
    path_len = path_len ? path_len - 1: path_len;
    
    strcpy(new_path, path);

    if(path[path_len] != '/')
    {
        strcat(new_path, "/");
    }
    
    strcat(new_path, format_path(segment));

    return new_path;
}