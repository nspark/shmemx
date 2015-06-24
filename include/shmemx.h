#ifndef __SHMEMX_H__
#define __SHMEMX_H__

#include <stdint.h>
#include <shmem.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 201112L)

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

#endif

#endif
