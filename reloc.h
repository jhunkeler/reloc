#ifndef RELOC_RELOC_H
#define RELOC_RELOC_H

#define _GNU_SOURCE 1

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"

#if defined(_WIN32) || defined(_WIN64)
#define DIRSEP '\\'
#define SIZE_T_FMT "%I64u"
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
#define DIRSEP '/'
#define SIZE_T_FMT "%lu"
#endif


typedef struct {
    size_t size;
    char *path;
    char *data;
} RelocData;


typedef struct {
    char *begin;
    char *end;
    char *post;
    size_t length;
    size_t post_length;
    size_t total_length;
} RelocMatch;


RelocMatch *reloc_match(char *haystack, const char *needle);
RelocData *reloc_read(const char *filename);
size_t reloc_write(RelocData *finfo, const char *filename);
void reloc_deinit_data(RelocData *finfo);
void reloc_replace(RelocMatch *match, const char *rstr);


#endif //RELOC_RELOC_H
