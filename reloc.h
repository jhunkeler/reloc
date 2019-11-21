#ifndef RELOC_RELOC_H
#define RELOC_RELOC_H

#define _GNU_SOURCE 1

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"

#if defined(__MINGW32__) || defined(__MINGW64__)
#define DIRSEP '\\'
#define SIZE_T_FMT "%I64u"
#elif defined(_MSC_VER)
#define DIRSEP '\\'
#define SIZE_T_FMT "%lu"
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
#define DIRSEP '/'
#define SIZE_T_FMT "%lu"
#endif

#define NULLBYTE '\0'

extern int reloc_error;

enum {
    RELOC_ESUCCESS=0,
    RELOC_EREAD,
    RELOC_EWRITE,
    RELOC_EVERIFY,
    RELOC_ELENGTH,
    RELOC_ENOMEM,
};

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

const char *reloc_strerror(int code);
void reloc_perror(const char *msg);

RelocMatch *reloc_match(char *haystack, const char *needle);
RelocData *reloc_read(const char *filename);
size_t reloc_write(RelocData *finfo, const char *filename);
void reloc_deinit_data(RelocData *finfo);
void reloc_replace(RelocMatch *match, const char *rstr);


#endif //RELOC_RELOC_H
