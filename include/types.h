#ifndef __TYPES_H__
#define __TYPES_H__

typedef char Char;
typedef short Short;
typedef int Int;
typedef long Long;
typedef long long LongLong;

typedef unsigned char Uchar;
typedef unsigned short Ushort;
typedef unsigned int Uint;
typedef unsigned long Ulong;


#if defined(__x86_64__) || defined(__aarch64__)
typedef unsigned long Size_t;
#elif defined(__i386__) || defined(__arm__)
typedef unsigned int Size_t;
#else


#endif


typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int Dword;
typedef unsigned long long Qword;

typedef void Void;

typedef void* Handle;

typedef enum Bool_e
{
    True=1,
    False=0,
}Bool;


#endif /*__TYPES_H__*/