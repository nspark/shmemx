#ifndef __SHMEMX_H__
#define __SHMEMX_H__

#include <stdint.h>
#include <shmem.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 201112L)

/*
  SHMEMX RMA Routines using C11 Generics
 */

#define shmemx_put(dst, src, len, pe)                           \
  _Generic((dst),                                               \
           double*:      shmem_double_put,                      \
           float*:       shmem_float_put,                       \
           int*:         shmem_int_put,                         \
           long*:        shmem_long_put,                        \
           long double*: shmem_longdouble_put,                  \
           long long*:   shmem_longlong_put,                    \
           short*:       shmem_short_put,                       \
           uint32_t*:    shmem_put32,                           \
           uint64_t*:    shmem_put64)(dst, src, len, pe)


#define shmemx_get(dst, src, len, pe)                           \
  _Generic((dst),                                               \
           double*:      shmem_double_get,                      \
           float*:       shmem_float_get,                       \
           int*:         shmem_int_get,                         \
           long*:        shmem_long_get,                        \
           long double*: shmem_longdouble_get,                  \
           long long*:   shmem_longlong_get,                    \
           short*:       shmem_short_get,                       \
           uint32_t*:    shmem_get32,                           \
           uint64_t*:    shmem_get64)(dst, src, len, pe)

// N.B., shmem_char_p is missing from shmem.h in OSHMEM 1.8.6
#define shmemx_p(dst, val, pe)                                  \
  _Generic((dst),                                               \
           short*:       shmem_short_p,                         \
           int*:         shmem_int_p,                           \
           long*:        shmem_long_p,                          \
           long long*:   shmem_longlong_p,                      \
           float*:       shmem_float_p,                         \
           double*:      shmem_double_p,                        \
           long double*: shmem_longdouble_p)(dst, val, pe)

#define shmemx_g(dst, pe)                                       \
  _Generic((dst),                                               \
           char*:        shmem_char_g,                          \
           short*:       shmem_short_g,                         \
           int*:         shmem_int_g,                           \
           long*:        shmem_long_g,                          \
           long long*:   shmem_longlong_g,                      \
           float*:       shmem_float_g,                         \
           double*:      shmem_double_g,                        \
           long double*: shmem_longdouble_g)(dst, pe)

#endif

#endif
