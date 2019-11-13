#include "reloc.h"
#include "myassert.h"

char *test_cases[] = {
    "I have a yellow pencil\x00",
    "/one/two/three/four/five\x00\xAB\xCD\x00",
    "\x09\x09\x09 1234567890\x00",
    "123456789/0\x00",
    NULL,
};

const char *test_patterns[] = {
    "yellow",
    "three",
    "1234567890",
    "/",
    NULL,
};

const char *test_solution_begin[] = {
    "yellow pencil",
    "three/four/five",
    "1234567890",
    "/0",
    NULL,
};

const char *test_solution_post[] = {
    " pencil",
    "/four/five",
    "",
    "0",
    NULL,
};

const size_t test_solution_length[] = {
    6,
    5,
    10,
    1,
};

const size_t test_solution_post_length[] = {
    7,
    10,
    0,
    1,
};

const size_t test_solution_total_length[] = {
    13,
    15,
    10,
    2,
};

int test_reloc_match() {
    for (int i = 0; test_cases[i] != NULL; i++) {
        RelocMatch *match = NULL;
        char *ptr = test_cases[i];

        printf("case %d:\n", i);
        while (*ptr != '\0') {
            if (match) break;
            printf("\tpattern=\"%s\", data=\"%s\"\n", test_patterns[i], ptr);
            match = reloc_match(ptr, test_patterns[i]);
            ptr++;
        }
        myassert("Failed to find pattern", match != NULL);
        myassert("Invalid match->begin", !strcmp(match->begin, test_solution_begin[i]));
        printf("\tend=\"%s\", data=\"%s\"\n", match->end, match->begin);
        myassert("Invalid match->end", match->end != NULL);
        printf("\tpost=\"%s\", data=\"%s\"\n", match->post, match->begin);
        myassert("Invalid match->post", !strcmp(match->post, test_solution_post[i]));
        printf("\tlength=%lu\n", match->length);
        myassert("Invalid match->length", match->length == test_solution_length[i]);
        printf("\tpost_length=%lu\n", match->post_length);
        myassert("Invalid match->post_length", match->post_length == test_solution_post_length[i]);
        printf("\ttotal_length=%lu\n", match->total_length);
        myassert("Invalid match->total_length", match->total_length == test_solution_total_length[i]);
        if (match) {
            free(match);
        }
    }
    return 0;
}

int main() {
    return test_reloc_match();
}
