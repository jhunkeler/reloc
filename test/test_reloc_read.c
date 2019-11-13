#include "reloc.h"
#include "myassert.h"

const char *input_file = "input.bin";
const char *output_file = "output.bin";
char test_case[] =
    "\xAB\xCD\xEF the quick brown fox\n\x00"
    "jumps over\n\x00 \xEE\x09"
    "\xAB\xCD\xEF the quick brown fox\n\x00"
    "jumps over\n\x00 \xEE\x09"
    "\xBBthe lazy\n\x00 dog\n"
    "\xAB\xCD\xEF the quick brown fox\n\x00"
    "\xBBthe lazy\n\x00 dog\n";


int test_reloc_read() {
    RelocData *info = NULL;
    if (!(info = reloc_read(input_file))) {
        fprintf(stderr, "Failed to open '%s' for writing\n", input_file);
        return 1;
    }
    myassert("info->size is incorrect", info->size == sizeof(test_case));
    myassert("info->path is incorrect", !strcmp(info->path, input_file));
    myassert("info->data is uninitialized", info->data);
    return 0;
}

int main() {
    FILE *fp;
    if (!(fp = fopen(input_file, "w+b"))) {
        fprintf(stderr, "Failed to open '%s' for writing\n", input_file);
        return 1;
    }
    fwrite(test_case, sizeof(char), sizeof(test_case), fp);
    fclose(fp);

    return test_reloc_read();
}
