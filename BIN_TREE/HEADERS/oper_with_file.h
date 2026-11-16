#ifndef OPER_WITH_FILE_H
#define OPER_WITH_FILE_H

#include <cstdlib>
#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdint>

#define FREE_MASS(array)                                \
    free(array);                                        \
    array = NULL;                                       

enum Mode
{
    SORT_FOR_BEGIN,
    SORT_FOR_END,
    ORIGINAL
};

enum Erros
{
    UNVALIBLE_FILE_SIZE,
    FILE_NOT_OPEN,
    NOT_ALLOCATE,
    INCORRECT_MODE_FOR_PRINT,
    DATA_NOT_GET
};

struct FileData
{
    size_t size_of_file;
    size_t count_of_strings;
    char* original_text;
};


struct FileData read_file_and_get_data(const char* FILE_NAME);
ssize_t get_file_size(const char* name);

#endif