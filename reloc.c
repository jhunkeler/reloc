#include "reloc.h"

RelocMatch *reloc_match(char *haystack, const char *needle) {
    char *data = haystack;
    size_t needle_size = strlen(needle);
    RelocMatch *match = NULL;

    // Search the needle in the data
    if (!(memcmp(data, needle, needle_size))) {
        if (!(match = calloc(1, sizeof(RelocMatch)))) {
            reloc_error = RELOC_ENOMEM;
            return NULL;
        }
        size_t data_end = strlen(data);
        match->begin = data;
        match->end = match->begin + needle_size;
        match->length = match->end - match->begin;
        match->post = match->end;
        match->post_length = strlen(match->post);
        match->total_length = data_end;
    }

    reloc_error = RELOC_ESUCCESS;
    return match;
}


RelocData *reloc_read(const char *filename) {
    char *data = NULL;
    FILE *fp = NULL;
    size_t size = 0;
    RelocData *result;

    // Open file for reading in binary mode
    if (!(fp = fopen(filename, "rb"))) {
        reloc_error = RELOC_EREAD;
        return NULL;
    }

    // Determine file size
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    if (!(data = calloc(sizeof(char), size + 1))) {
        reloc_error = RELOC_ENOMEM;
        return NULL;
    }

    // Read data into array
    fread(data, sizeof(char), size, fp);
    if (!(result = (RelocData *)malloc(sizeof(RelocData)))) {
        reloc_error = RELOC_ENOMEM;
        return NULL;
    }
    fclose(fp);

    result->size = size;
    result->data = data;
    result->path = strdup(filename);
    reloc_error = RELOC_ESUCCESS;
    return result;
}


size_t reloc_write(RelocData *finfo, const char *filename) {
    size_t bytes = 0;
    FILE *fp;

    // Open file for writing in binary mode
    if (!(fp = fopen(filename, "w+b"))) {
        reloc_error = RELOC_EWRITE;
        return 0;
    }

    // Write data
    bytes = fwrite(finfo->data, sizeof(char), finfo->size, fp);
    fclose(fp);

    reloc_error = RELOC_ESUCCESS;
    return bytes;
}


void reloc_deinit_data(RelocData *finfo) {
    if (finfo) {
        if (finfo->path) {
            free(finfo->path);
        }
        free(finfo);
    }
}


void reloc_replace(RelocMatch *match, const char *rstr) {
    size_t rstr_length = strlen(rstr);
    char *data = match->begin;
    size_t i = 0;

    // Overwrite data with replacement string
    while (i < rstr_length) {
        *data = *rstr;
        data++;
        rstr++;
        i++;
    }

    i = 0;
    // Append remaining bytes not part of the original match to the replaced string
    char *post = match->post;
    while (i < match->post_length) {
        *data = *post;
        data++;
        post++;
        i++;
    }

    // Destroy bytes between the end of the original data and the end of the replaced string
    memset(data, NULLBYTE, post - data);
}
