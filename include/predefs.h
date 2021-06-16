#ifndef __PREDEFS_H__
#define __PREDEFS_H__

#define NULL_PTR ((void *)0)

#if defined(__x86_64__) || defined(__aarch64__)
#define TYPE_BASE(address, type, field) \
    ((type *)((char *)(address) - (unsigned long)(&((type *)0)->field)))

#elif defined(__i386__) || defined(__arm__)
#define TYPE_BASE(address, type, field) \
    ((type *)((char *)(address) - (unsigned int)(&((type *)0)->field)))

#else

#endif

/*a should be 2^n, such as 2, 4, 6 ,8 ...*/

#define ALIGN_UP(x, a) (((x) + (a)-1) & ~(a - 1))

#define ALIGN_DOWN(x, a) ((x) & ~(a - 1))

#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define PAGE_START(x) ((x)&PAGE_MASK)
#define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)
#define PAGE_END(x) PAGE_START((x) + (PAGE_SIZE - 1))

#endif /*__PREDEFS_H__*/