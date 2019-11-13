#ifndef PATH_H
#define PATH_H


char *get_file_path(char *file_name);

char *get_file_from_path(char *file_name);

char *get_file_name_no_ext(char *file_name);

char *format_path(char *path);

char *append_path_segment(char *path, char *segment);


#endif // PATH_H