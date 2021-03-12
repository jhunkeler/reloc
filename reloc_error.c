#include "reloc.h"

int reloc_error = 0;
const char *reloc_errors[] = {
    "Success",
    "Read error",
    "Write error",
    "Verification failure",
    "Length too long",
    "Memory allocation failure",
    NULL,
};


const char *reloc_strerror(int code) {
    return reloc_errors[code];
}

void reloc_perror(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, reloc_strerror(reloc_error));
}
