
#include "defer.h"
#include <stdio.h>

static int tests()
{
    struct a
    {
        ~a() {
            printf("hello world\n");
        }
    };

    int var = 9;
    if (1) {
        puts("normal 1");
        defer(delete (new a));
    }

    defer(printf("deffered %d\n", var));
    puts("normal 2");
    return 0;
}

int main()
{
    tests();
    puts("sss\n");
    return 0;
}
