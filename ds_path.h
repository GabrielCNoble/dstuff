#ifndef PATH_H
#define PATH_H

#include <stdint.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>


char *ds_path_GetFileName(char *path);

char *ds_path_GetPath(char *path);

char *ds_path_GetPathAndFileNameNoExt(char *path);

char *ds_path_GetExt(char *path);

char *ds_path_FormatPath(char *path);

char *ds_path_AppendPath(char *path, char *append);

char *ds_path_AppendExt(char *path, char *ext);





/* removes the file name from the end of a path. Essentially returns
the path to the file */
//char *strip_file_name_from_path(char *path);

/* returns the file name from a path */
//char *strip_path_from_file_name(char *path);

/* returns the file extension from a path, if the path
contains a file name at the end. */
//char *strip_path_from_ext(char *path);

/* returns only the path (with potentially a
file name at the end) without a file extension */
//char *strip_ext_from_path(char *path);

///* deprecated */
//char *strip_decorations_from_path(char *path);

//int32_t get_index_from_path(char *path);

/* convert paths to unix style */
//char *format_path(char *path);

//char *append_ext(char *file_name, char *ext);

//char *append_path(char *path, char *append);

//uint32_t is_dir(char *path);




#ifdef DS_PATH_IMPLEMENTATION



char *ds_path_GetFileName(char *path)
{
    static char file_name[PATH_MAX];
    int32_t path_len = strlen(path);
    int32_t path_index = path_index;

    path = ds_path_FormatPath(path);

    while(path_index >= 0 && path[path_index] != '/') path_index--;
    if(path_index >= 0) path_index++;
    else path_index = 0;

    strncpy(file_name, path + path_index, PATH_MAX);
    file_name[path_len - path_index] = '\0';
    return file_name;
}

char *ds_path_GetPath(char *path)
{
    static char stripped_path[PATH_MAX];
    uint32_t length = strlen(path);
    uint32_t index = length;
    path = ds_path_FormatPath(path);
    while(index && path[index] != '/') index--;

    if(!index || index == length)
    {
        return "";
    }

    strncpy(stripped_path, path, index);
    stripped_path[index] = '\0';

    return stripped_path;
}

char *ds_path_GetPathAndFileNameNoExt(char *path)
{
    static char stripped_path[PATH_MAX];
    uint32_t length = strlen(path);
    uint32_t index = length;
    
    path = ds_path_FormatPath(path);

    while(index && path[index] != '.')index--;

    if(!index)
    {
        return path;
    }
    
    strncpy(stripped_path, path, index);
    stripped_path[index] = '\0';

    return stripped_path;
}

char *ds_path_GetExt(char *path)
{
    static char stripped_path[PATH_MAX];
    uint32_t index = strlen(path);

    while(index && path[index] != '.') index--;
    if(!index)
    {
        return "";
    }

    strcpy(stripped_path, path + index + 1);
    
    return stripped_path;
}

char *ds_path_FormatPath(char *path)
{
    static char formatted_path[PATH_MAX];
    uint32_t formatted_index = 0;
    memset(formatted_path, 0, PATH_MAX);

    for(uint32_t index = 0; path[index]; index++)
    {
        formatted_path[formatted_index] = path[index];

        if(path[index] == '\\')
        {
            formatted_path[formatted_index] = '/';

            if(path[index + 1] == '\\')
            {
                index++;
            }
        }

        formatted_index++;
    }

    formatted_path[formatted_index] = '\0';

    return formatted_path;
}

char *ds_path_AppendPath(char *path, char *append)
{
    static char new_path[PATH_MAX];
    uint32_t path_len = strlen(path);

    strcpy(new_path, ds_path_FormatPath(path));
    append = ds_path_FormatPath(append);
    
    if(path_len)
    {
        path_len--;
    }

    if(path[path_len] != '/' && append[0] != '/')
    {
        strcat(new_path, "/");
    }

    strcat(new_path, append);

    return new_path;
}

char *ds_path_DropPath(char *path)
{
    static char new_path[PATH_MAX];
    uint32_t path_index = strlen(path) - 1;
//    uint32_t path_end = path_length;
    
    strcpy(new_path, ds_path_FormatPath(path));
    if(path_index)
    {
        if(new_path[path_index] == '/')
        {
            path_index--;
        }
        
        while(path_index && new_path[path_index - 1] != '/') path_index--;
    }
    
    new_path[path_index] = '\0';
    return new_path;
}

char *ds_path_AppendExt(char *path, char *ext)
{
    static char file_name_ext[PATH_MAX];
    int32_t index;
    if(strcmp(ds_path_GetExt(path), ext))
    {
        strcpy(file_name_ext, path);
        index = strlen(file_name_ext);

        while(index >= 0 && file_name_ext[index] != '.')
        {
            index--;
        }

        if(index >= 0)
        {
            file_name_ext[index] = '\0';
        }

        if(ext[0] != '.')
        {
            strcat(file_name_ext, ".");
        }

        strcat(file_name_ext, ext);
        return file_name_ext;
    }

    return file_name_ext;
}






//char *strip_file_name_from_path(char *path)
//{
//    static char stripped_path[PATH_MAX];
//    uint32_t index;
//    uint32_t length;
//    length = strlen(path);
//    index = length;
//    path = format_path(path);
//    while(index && path[index] != '/') index--;
//
//    if(!index || index == length)
//    {
//        return path;
//    }
//
//    strncpy(stripped_path, path, index);
//    stripped_path[index] = '\0';
//
//    return stripped_path;
//}

//char *strip_path_from_file_name(char *path)
//{
//    static char file_name[PATH_MAX];
//    int32_t path_index = strlen(path);
//
//    path = format_path(path);
//
//    while(path_index >= 0 && path[path_index] != '/') path_index--;
//    if(path_index >= 0) path_index++;
//    else path_index = 0;
//
//    strncpy(file_name, path + path_index, PATH_MAX);
//    file_name[strlen(path + path_index)] = '\0';
//    return file_name;
//}

//char *strip_ext_from_path(char *path)
//{
//    static char stripped_path[PATH_MAX];
//    uint32_t length = strlen(path);
//    uint32_t index = length;
//
//    while(index && path[index] != '.')
//    {
//        if(path[index] == '/' || path[index]  == '\\')
//        {
//            index = length;
//            break;
//        }
//        index--;
//    }
//
//    if(index)
//    {
//        strncpy(stripped_path, path, index);
//        stripped_path[index] = '\0';
//    }
//    else
//    {
//        strcpy(stripped_path, path);
//    }
//
//    return stripped_path;
//}

//char *strip_path_from_ext(char *path)
//{
//    static char stripped_path[PATH_MAX];
//    uint32_t index = strlen(path);
//
//    while(index && path[index] != '.') index--;
//    if(!index)
//    {
//        return "";
//    }
//
//    strcpy(stripped_path, path + index + 1);
//    stripped_path[0] = '\0';
//
//    return stripped_path;
//}

//char *strip_decorations_from_path(char *path)
//{
//    static char stripped_path[PATH_MAX];
//    char *current_stripped_path;
//    uint32_t length;
//    uint32_t current_length;
//
//    strcpy(stripped_path, path);
//    length = strlen(stripped_path);
//    while(1)
//    {
//        current_stripped_path = strip_ext_from_path(stripped_path);
//        current_length = strlen(current_stripped_path);
//        if(current_length == length)
//        {
//            break;
//        }
//        strcpy(stripped_path, current_stripped_path);
//        length = current_length;
//    }
//
//    return stripped_path;
//}

//int32_t get_index_from_path(char *path)
//{
//    int32_t last_dot;
//    int32_t first_dot;
//    uint32_t index;
//    char value_str[64];
//    uint32_t value_str_index;
//    last_dot = strlen(path);
//
//    while(last_dot >= 0)
//    {
//        while(last_dot >= 0 && path[last_dot] != '.') last_dot--;
//
//        first_dot = last_dot - 1;
//
//        while(first_dot >= 0 && path[first_dot] != '.') first_dot--;
//
//        if(first_dot >= 0)
//        {
//            /* if we got here, we found something of the form  '.'(first_dot)?(may be a number, may be empty)'.'(last_dot).
//            Now we test to see if it's a valid number */
//            for(index = first_dot + 1, value_str_index = 0; index < last_dot; index++)
//            {
//                if(path[index] >= '0' && path[index] <= '9')
//                {
//                    value_str[value_str_index] = path[index];
//                    value_str_index++;
//                }
//                else
//                {
//                    /* not a valid number, so restart the search from the first '.' */
//                    last_dot = first_dot;
//                    break;
//                }
//            }
//
//            value_str[value_str_index] = '\0';
//            if(value_str_index)
//            {
//                /* found a valid number */
//                return atoi(value_str);
//            }
//        }
//        else
//        {
//            break;
//        }
//    }
//
//    return -1;
//}

//char *format_path(char *path)
//{
//    static char formatted_path[PATH_MAX];
//    int i;
//    int j;
//
//    memset(formatted_path, 0, PATH_MAX);
//
//    for(i = 0, j = 0; path[i]; i++)
//    {
//        formatted_path[j] = path[i];
//
//        if(path[i] == '\\')
//        {
//            formatted_path[j] = '/';
//
//            if(path[i + 1] == '\\')
//            {
//                i++;
//            }
//        }
//
//        j++;
//    }
//
//    formatted_path[j] = '\0';
//
//    return formatted_path;
//}

//char *append_ext(char *file_name, char *ext)
//{
//    static char file_name_ext[PATH_MAX];
//    int32_t index;
//    if(strcmp(strip_path_from_ext(file_name), ext))
//    {
//        strcpy(file_name_ext, file_name);
//        index = strlen(file_name_ext);
//
//        while(index >= 0 && file_name_ext[index] != '.')
//        {
//            index--;
//        }
//
//        if(index >= 0)
//        {
//            file_name_ext[index] = '\0';
//        }
//
//        if(ext[0] != '.')
//        {
//            strcat(file_name_ext, ".");
//        }
//
//        strcat(file_name_ext, ext);
//        return file_name_ext;
//    }
//
//    return file_name;
//}

//char *append_path(char *path, char *append)
//{
//    static char new_path[PATH_MAX];
//
//    int path_len = strlen(path);
//    path_len = path_len ? path_len - 1: path_len;
//
//    strcpy(new_path, path);
//
//    if(path[path_len] != '/')
//    {
//        strcat(new_path, "/");
//    }
//
//    strcat(new_path, format_path(append));
//
//    return new_path;
//}

//uint32_t is_dir(char *path)
//{
//    DIR *probe;
//    probe = opendir(path);
//    if(probe)
//    {
//        closedir(probe);
//    }
//    return !!probe;
//}

#endif


#endif // PATH_H
