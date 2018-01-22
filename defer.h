#ifndef defer_h
#define defer_h

#ifdef __cplusplus
#ifdef __clang__
#include <setjmp.h>

struct defer_t {
    jmp_buf env1;
    jmp_buf env2;
    long rsp;

    ~defer_t() {
        int n = setjmp(env2);
        __asm__ __volatile__("movq %%rsp, %0\n" :"=m"(rsp) ::);

        if (n == 0) {
            longjmp(env1, 1);
        }
    }
};

#define defer2(l, ...) \
({ \
    defer_t* np = &defer_node_##l; \
    if (1 == setjmp(np->env1)) { \
        __asm__ __volatile__("movq %0, %%rsp\n" : :"m"(np->rsp) :"rsp"); \
        __VA_ARGS__; \
        longjmp(np->env2, 1); \
    } \
    np; \
})
#define defer1(l, ...) defer_t defer_node_##l, *dn_ptr_##l = defer2(l, ##__VA_ARGS__)
#define defer(...) defer1(__line__, ##__VA_ARGS__)

#endif
#endif
#endif
