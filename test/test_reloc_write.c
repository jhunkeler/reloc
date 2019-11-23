#include "reloc.h"
#include "myassert.h"
#if !defined(_MSC_VER)
#include <unistd.h>  // unlink()
#endif

const char *input_file = "test_data.bin";
const char *output_file = "test_reloc_write.out";


int test_reloc_write() {
    RelocData *info = reloc_read(input_file);
    myassert("failed to populate RelocData struct", info);
    size_t size = reloc_write(info, output_file);
    myassert("incorrect number of bytes written", size == info->size);
    reloc_deinit_data(info);
    if (unlink(output_file) != 0) {
        perror("Could not delete output file");
        return errno;
    }

    return 0;
}


int main() {
    return test_reloc_write();
}
