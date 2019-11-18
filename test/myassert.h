#define myassert(message, condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "%s:%d:%s :: %s\n", \
                    __FILE__, \
                    __LINE__, \
                    __FUNCTION__, \
                    message); \
            return 1; \
        } \
    } while (0)
