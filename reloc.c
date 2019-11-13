#include "reloc.h"


RelocMatch *reloc_match(char *haystack, const char *needle) {
    char *data = haystack;
    size_t needle_size = strlen(needle);
    RelocMatch *match = NULL;

    // Search the needle in the data
    if (!(memcmp(data, needle, needle_size))) {
        if (!(match = calloc(1, sizeof(RelocMatch)))) {
            fprintf(stderr,"Failed to allocate RelocMatch structure: %s\n", strerror(errno));
            exit(1);
        }
        size_t data_end = strlen(data);
        match->begin = data;
        match->end = match->begin + needle_size;
        match->length = match->end - match->begin;
        match->post = match->end;
        match->post_length = strlen(match->post);
        match->total_length = data_end;
    }
    return match;
}


RelocData *reloc_read(const char *filename) {
    char *data = NULL;
    FILE *fp = NULL;
    size_t size = 0;
    RelocData *result;

    // Open file for reading in binary mode
    if (!(fp = fopen(filename, "rb"))) {
        fprintf(stderr, "Cannot open %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    // Determine file size
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    if (!(data = calloc(sizeof(char), size + 1))) {
        fprintf(stderr, "Failed to allocate data array: %s\n", strerror(errno));
        exit(1);
    }

    // Read data into array
    fread(data, sizeof(char), size, fp);
    if (!(result = (RelocData *)malloc(sizeof(RelocData)))) {
        fprintf(stderr, "Failed to allocate RelocData structure: %s\n", strerror(errno));
        exit(1);
    }
    result->size = size;
    result->data = data;
    result->path = strdup(filename);
    return result;
}


size_t reloc_write(RelocData *finfo, const char *filename) {
    FILE *fp;

    // Open file for writing in binary mode
    if (!(fp = fopen(filename, "w+b"))) {
        fprintf(stderr,"Cannot open %s for writing: %s\n", filename, strerror(errno));
        exit(1);
    }

    // Write data
    return fwrite(finfo->data, sizeof(char), finfo->size, fp);
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
    if (rstr_length > match->length) {
        fprintf(stderr, "Replacement string is too long ("SIZE_T_FMT " > " SIZE_T_FMT ")\n", rstr_length, match->length);
        return;
    }
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
    memset(data, '\0', post - data);
}
