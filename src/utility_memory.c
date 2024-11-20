#include "utility_memory.h"


SECTION_CODE void *pic_memcpy(void* dest, const void* src, size_t n) 
{
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    
    // Copy byte by byte
    while (n--) {
        *d++ = *s++;
    }
    
    return dest;
}

SECTION_CODE INT MemCompare( PVOID s1, PVOID s2, INT len)
{
    PUCHAR p = s1;
    PUCHAR q = s2;
    INT charCompareStatus = 0;

    if ( s1 == s2 ) {
        return charCompareStatus;
    }

    while (len > 0)
    {
        if (*p != *q)
        {
            charCompareStatus = (*p >*q)?1:-1;
            break;
        }
        len--;
        p++;
        q++;
    }
    return charCompareStatus;
}

