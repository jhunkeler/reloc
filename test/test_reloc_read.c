#include "reloc.h"
#include "myassert.h"

char test_case[] =
    "\xAB\xCD\xEF the quick brown fox\n\x00"
    "jumps over\n\x00 \xEE\x09"
    "\xAB\xCD\xEF the quick brown fox\n\x00"
    "jumps over\n\x00 \xEE\x09"
    "\xBBthe lazy\n\x00 dog\n"
    "jumps over\n\x00 \xEE\x09"
    "\xAB\xCD\xEF the quick brown fox\n\x00"
    "\xBBthe lazy\n\x00 dog\n";
const char *input_file = "input.bin";


int test_reloc_read() {
    RelocData *info = reloc_read(input_file);
    myassert("failed to populate RelocData struct", info);
    myassert("info->size is incorrect", info->size == sizeof(test_case));
    myassert("info->path is incorrect", !strcmp(info->path, input_file));
    myassert("info->data is uninitialized", info->data);
    free(info);
    return 0;
}

int test_reloc_read_verify() {
    RelocData *info = reloc_read(input_file);
    myassert("failed to populate RelocData struct", info);
    myassert("info->data != input_data contents", !memcmp(test_case, info->data, info->size));
    free(info);
    return 0;
}

int main() {
    int result = 0;
    FILE *fp = fopen(input_file, "w+b");
    myassert("failed to write input file with test_case data", fp);
    fwrite(test_case, sizeof(char), sizeof(test_case), fp);
    fclose(fp);

    result += test_reloc_read();
    result += test_reloc_read_verify();
    return result;
}
