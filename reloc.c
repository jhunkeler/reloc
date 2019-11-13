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


void show_version() {
    printf("%s\n", VERSION);
}


int main(int argc, char *argv[]) {
    char *program = argv[0];
    char *program_relative = strrchr(program, DIRSEP);
    if (program_relative) {
        program = program_relative + 1;
    }

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-V")) {
            show_version();
            exit(0);
        }
    }

    if (argc < 5) {
        printf("%s [-V] <str1> <str2> <input_file> <output_file>\n"
               "\n"
               "Arguments:\n"
               "--version (-V) - Display version and exit\n"
               "str1           - Pattern to search for\n"
               "str2           - Replace str1 with contents of str2\n"
               "input_file     - Path to input file\n"
               "output_file    - Path to output file\n"
               "\n"
               "Example:\n"
               "%s /original/path /new/path input.bin output.bin\n"
               "\n", program, program);
        exit(1);
    }

    char *needle = strdup(argv[1]);
    char *replacement = strdup(argv[2]);
    char *input_file = strdup(argv[3]);
    char *output_file = strdup(argv[4]);
    RelocData *info = reloc_read(input_file);
    size_t records = 0;

    for (size_t i = 0; i < info->size; i++) {
        RelocMatch *match = NULL;
        if (!(match = reloc_match(&info->data[i], needle))) {
            // No match found
            continue;
        }
        // Replace string
        reloc_replace(match, replacement);
        free(match);
        records++;
    }

    // Write output file to disk
    reloc_write(info, output_file);

    // Report number of strings processed
    printf(SIZE_T_FMT"\n", records);

    free(needle);
    free(replacement);
    free(input_file);
    free(output_file);
    reloc_deinit_data(info);
    return 0;
}
