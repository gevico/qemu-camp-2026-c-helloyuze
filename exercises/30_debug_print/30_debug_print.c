#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#if DEBUG_LEVEL >= 2
#define DEBUG_PRINT(fmt, ...)                                                    \
    do {                                                                         \
        printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__,         \
               ##__VA_ARGS__);                                                   \
    } while (0)
#elif DEBUG_LEVEL == 1
#define DEBUG_PRINT(fmt, ...)                                                    \
    do {                                                                         \
        printf("DEBUG: " fmt "\n", ##__VA_ARGS__);                              \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) do { } while (0)
#endif



























//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

#line 45 "30_debug_print.c"
// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}
