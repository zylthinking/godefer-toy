
#include <setjmp.h>
#include <stdio.h>

struct defer_t {
    jmp_buf env1;
    jmp_buf env2;
    long rsp;

    ~defer_t() {
        int n = setjmp(env2);
        asm("mov %%rsp, %0\n" :"=m"(rsp) ::);
        //printf("save rsp %p %p %p\n", (void *) rsp, this, __builtin_return_address(0));

        if (n == 0) {
            longjmp(env1, 1);
        }
    }
};

#define defer2(l, ...) \
({ \
    defer_t* np = &defer_node_##l; \
    if (1 == setjmp(np->env1)) { \
        __asm__ __volatile ("mov %0, %%rsp\n" : :"m"(np->rsp) :"rsp"); \
        __VA_ARGS__; \
        longjmp(np->env2, 1); \
    } \
    np; \
})
#define defer1(l, ...) defer_t defer_node_##l, *dn_ptr_##l = defer2(l, ##__VA_ARGS__)
#define defer(...) defer1(__line__, ##__VA_ARGS__)

int tests()
{
    struct a{
        ~a() {
            printf("hello world\n");
        }
    };

    int var = 9;
    if (var == 9) {
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
