/*
* MIT License
*
* Copyright (c) 2023 Ransel117
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef RANSTRING_H
#include <stdint.h>
#include <stddef.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef size_t   usize;

/*
temporary, only used as a checklist
+ means a checkmark

memcpy +
memmove
strcpy +
strncpy +

strcat +
strncat +

memcmp
strcmp
strcoll
strncmp
strxfrm

memchr +
strchr
strcspn
strpbrk
strrchr
strspn
strstr
strtok

memset +
strerror
strlen +
strnlen +
*/

#if defined(__GNUC__) || defined(__clang__)
#define RSTR_INLINE static inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define RSTR_INLINE static inline __forceinline
#else
#define RSTR_INLINE static inline
#endif

RSTR_INLINE usize ran_strlen(char*);
RSTR_INLINE usize ran_strnlen(const char*, usize);
RSTR_INLINE void *ran_memcpy(void*, const void*, usize);
RSTR_INLINE void *ran_memchr(const void*, i32, usize);
RSTR_INLINE void *ran_memset(char*, i32, usize);
RSTR_INLINE char *ran_strcpy(char*, const char*);
RSTR_INLINE char *ran_strncpy(char*, const char*, usize);
RSTR_INLINE char *ran_strcat(char*, const char*);
RSTR_INLINE char *ran_strncat(char*, const char*, usize);

#endif /* RSTRING_H*/

#ifdef RANSTRING_IMPLEMENTATION
RSTR_INLINE usize ran_strlen(char *str) {
    char *tmp;

    for (tmp = str; *str; str++);

    return str - tmp;
}
RSTR_INLINE usize ran_strnlen(const char *str, usize num) {
    char *p;

    p = memchr(str, 0, num);

    return p ? p - str : num;
}

RSTR_INLINE void *ran_memcpy(void *dest, const void *src, usize num) {
    unsigned char *d, *s;
    d = (unsigned char*)dest;
    s = (unsigned char*)src;

    for (; num; num--) *d++ = *s++;

    return dest;
}

RSTR_INLINE void *ran_memchr(const void *mem, i32 c, usize num) {
    unsigned char *s;

    c = (unsigned char)c;

    for (s = (unsigned char*)mem; num && *s != c; s++, num--);

    return num ? (void *)s : 0;
}

RSTR_INLINE void *ran_memset(char *dest, i32 c, usize num) {
    unsigned char *tmp;
    usize k;

    /*
    * Fill head and tail with minimal branching. Each
    * conditional ensures that all the subsequently used
    * offsets are well-defined and in the dest region.
    */

    tmp = (unsigned char*)dest;

    printf("tmp = dest\n");

    if (!num) return dest;
    printf("!num\n");
    tmp[0] = c;
    tmp[num - 1] = c;

    if (num <= 2) return dest;
    printf("num <= 2\n");
    tmp[1] = c;
    tmp[2] = c;
    tmp[num - 2] = c;
    tmp[num - 3] = c;

    if (num <= 6) return dest;
    printf("num <= 6\n");
    tmp[3] = c;
    tmp[num - 4] = c;

    if (num <= 8) return dest;
    printf("num <= 8\n");

    /*
    * Advance pointer to align it at a 4-byte boundary,
    * and truncate n to a multiple of 4. The previous code
    * already took care of any head/tail that get cut off
    * by the alignment.
    */

    k = -(uintptr_t)tmp & 3;
    tmp += k;
    num -= k;
    num &= -4;

    for (; num; --num, --tmp) *tmp = c;

    return dest;
}

RSTR_INLINE char *ran_strcpy(char *dest, const char *src) {
    for (*dest = *src; (*dest = *src); src++, dest++);

    return dest;
}

RSTR_INLINE char *ran_strncpy(char *dest, const char *src, usize num) {
    usize size;

    size = ran_strnlen(src, num);
    ran_memcpy(dest, src, size);

    if (size == num) return dest;

    return ran_memset(dest, '\0', num - size);
}

RSTR_INLINE char *ran_strcat(char *dest, const char *src) {
    ran_strcpy(dest + ran_strlen(dest), src);
    return dest;
}

RSTR_INLINE char *ran_strncat(char *dest, const char *src, usize num) {
    char *tmp;

    tmp = dest;
    dest += ran_strlen(dest);

    while (num && *src) num--, *dest++ = *src++;

    *dest++ = 0;

    return tmp;
}

#endif /* RANSTRING_IMPLEMENTATION */
