#include "reloc.h"


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
        printf("usage: %s [-V] <str1> <str2> <input_file> <output_file>\n"
               "\n"
               "Options:\n"
               "--version (-V) - Display version and exit\n"
               "\n"
               "Positional arguments:\n"
               "str1           - Search string\n"
               "str2           - Replacement string\n"
               "input_file     - Input file name\n"
               "output_file    - Output file name\n"
               "\n"
               "Example:\n"
               "%s /original/string /new/string input.bin output.bin\n"
               "\n", program, program);
        exit(1);
    }

    char *needle = strdup(argv[1]);
    char *replacement = strdup(argv[2]);
    char *input_file = strdup(argv[3]);
    char *output_file = strdup(argv[4]);
    RelocData *info = reloc_read(input_file);
    if (!info) {
        reloc_perror(input_file);
        exit(reloc_error);
    }
    size_t records = 0;
    size_t replacement_length = strlen(replacement);

    for (size_t i = 0; i < info->size; i++) {
        RelocMatch *match = NULL;
        if (!(match = reloc_match(&info->data[i], needle))) {
            if (reloc_error) {
                reloc_perror("reloc_match: ");
                exit(reloc_error);
            }
            // No match found
            continue;
        }

        if (replacement_length > match->length) {
            fprintf(stderr, "Replacement string is too long ("SIZE_T_FMT " > " SIZE_T_FMT ")\n", replacement_length, match->length);
            free(match);
            continue;
        }
        // Replace string
        reloc_replace(match, replacement);
        free(match);
        records++;
    }

    // Write output file to disk
    reloc_write(info, output_file);
    if (reloc_error) {
        perror(output_file);
        exit(reloc_error);
    }

    // Report number of strings processed
    printf(SIZE_T_FMT"\n", records);

    free(needle);
    free(replacement);
    free(input_file);
    free(output_file);
    reloc_deinit_data(info);
    return 0;
}
