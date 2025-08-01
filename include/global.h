#ifndef _global_h_
#define _global_h_

#include "macros.h"

#define ARRAY_SIZE(o) (sizeof(o) / sizeof(o[0]))

// Align X to the previous N bytes (N must be power of two)
#define ALIGN_PREV(X, N) ((X) & ~((N)-1))
// Align X to the next N bytes (N must be power of two)
#define ALIGN_NEXT(X, N) ALIGN_PREV(((X) + (N)-1), N)
#define IS_ALIGNED(X, N) (((X) & ((N)-1)) == 0)
#define IS_NOT_ALIGNED(X, N) (((X) & ((N)-1)) != 0)

#define JUT_EXPECT(...)
#define LOGF(FMT, ...)

#define FLOAT_LABEL(x) (*(f32*)&x)
#define DOUBLE_LABEL(x) (*(f64*)&x)

#define _SDA_BASE_(dummy) 0
#define _SDA2_BASE_(dummy) 0

#ifndef offsetof
#define offsetof(type, member) ((size_t) & (((type*)0)->member))
#endif

#ifdef __MWERKS__
#define GLUE(a, b) a##b
#define GLUE2(a, b) GLUE(a, b)
#define STATIC_ASSERT(cond) typedef char GLUE2(static_assertion_failed__line_, __LINE__)[(cond) ? 1 : -1]
#define ALIGN_DECL(ALIGNMENT) __attribute__((aligned(ALIGNMENT)))
#define SECTION_DATA __declspec(section ".data")
#define SECTION_INIT __declspec(section ".init")
#else
#define STATIC_ASSERT(...)
#define ALIGN_DECL(...)
#define SECTION_DATA
#define SECTION_INIT
#endif

#endif
