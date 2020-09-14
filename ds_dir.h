#ifndef DS_DIR_H
#define DS_DIR_H


#include <dirent.h>
#include <limits.h>
#include <stdint.h>

#include "ds_path.h"

enum DS_DIR_ENTRY_TYPE
{
    DS_DIR_ENTRY_TYPE_DIR = 0,
    DS_DIR_ENTRY_TYPE_FILE,
    DS_DIR_ENTRY_TYPE_SELF,
    DS_DIR_ENTRY_TYPE_PARENT
};

struct ds_dir_entry_t
{
    uint32_t type;
    char name[PATH_MAX];
};

struct ds_dir_list_t
{
    DIR *dir;
    char path[PATH_MAX];
    uint32_t max_entry_count;
    uint32_t entry_count;
    struct ds_dir_entry_t *entries;
};

void ds_dir_OpenDir(struct ds_dir_list_t *dir, char *dir_name);

void ds_dir_CloseDir(struct ds_dir_list_t *dir);

void ds_dir_GoUp(struct ds_dir_list_t *dir);

void ds_dir_GoDown(struct ds_dir_list_t *dir, char *name);

void ds_dir_EnumerateEntries(struct ds_dir_list_t *dir);

uint32_t ds_dir_IsDir(char *path);



#ifdef DS_DIR_IMPLEMENTATION


void ds_dir_OpenDir(struct ds_dir_list_t *dir, char *dir_name)
{
    if(dir->dir)
    {
        closedir(dir->dir);
    }
    
    dir->dir = opendir(dir_name);
    strcpy(dir->path, ds_path_FormatPath(dir_name));
    ds_dir_EnumerateEntries(dir);
}

void ds_dir_CloseDir(struct ds_dir_list_t *dir)
{
    if(dir)
    {
        closedir(dir->dir);
        if(dir->entries)
        {
            mem_Free(dir->entries);
        }
    }
}

void ds_dir_GoUp(struct ds_dir_list_t *dir)
{
    ds_dir_OpenDir(dir, ds_path_DropPath(dir->path));
}

void ds_dir_GoDown(struct ds_dir_list_t *dir, char *name)
{
    ds_dir_OpenDir(dir, ds_path_AppendPath(dir->path, name));
}

void ds_dir_EnumerateEntries(struct ds_dir_list_t *dir)
{
    struct dirent *dir_entry;
    uint32_t dir_offset = 0;
    static char path[PATH_MAX];
    dir->entry_count = 0;
    rewinddir(dir->dir);
    
    while(1)
    {
        while((dir_entry = readdir(dir->dir)) && dir->entry_count < dir->max_entry_count)
        {
            struct ds_dir_entry_t *entry = dir->entries + dir->entry_count;
            strcpy(entry->name, dir_entry->d_name);
            strcpy(path, ds_path_AppendPath(dir->path, dir_entry->d_name));
            DIR *probe = opendir(ds_path_FormatPath(path));
            if(probe)
            {
                closedir(probe);
                if(!strcmp(dir_entry->d_name, "."))
                {
                    entry->type = DS_DIR_ENTRY_TYPE_SELF;
                }
                else if(!strcmp(dir_entry->d_name, ".."))
                {
                    entry->type = DS_DIR_ENTRY_TYPE_PARENT;
                }
                else
                {
                    entry->type = DS_DIR_ENTRY_TYPE_DIR;
                }
            }
            else
            {
                entry->type = DS_DIR_ENTRY_TYPE_FILE;
            }
            
            dir->entry_count++;
        }
        
        if(!dir_entry)
        {
            break;
        }
        
        dir_offset = telldir(dir->dir);
        
        while((dir_entry = readdir(dir->dir)))
        {
            dir->max_entry_count++;
        }
                
        dir->entries = mem_Realloc(dir->entries, sizeof(struct ds_dir_entry_t ) * dir->max_entry_count);
        seekdir(dir->dir, dir_offset);
    }
}

uint32_t ds_dir_IsDir(char *path)
{
    DIR *probe = opendir(path);
    closedir(probe);
    return probe != NULL;
}


#endif // DS_DIR_IMPLEMENTATION

#endif // DS_DIR_H
