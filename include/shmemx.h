#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)

#ifndef __SHMEMX_H__
#define __SHMEMX_H__

#include <stdint.h>
#include <shmem.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 201112L)

/*
  SHMEMX Value-Based Collectives
 */

#define SHMEMX_DECL_BROADCAST(TYPE)                                     \
  TYPE shmemx_ ## TYPE ## _broadcast(                                   \
    TYPE value, int root, int start, int log_stride, int size, long *pSync);

SHMEMX_DECL_BROADCAST(int)
SHMEMX_DECL_BROADCAST(long)
SHMEMX_DECL_BROADCAST(float)
SHMEMX_DECL_BROADCAST(double)

#undef SHMEMX_DECL_BROADCAST

/*
  N.B., cannot force array-to-pointer decay because (ARG) may not be a pointer!
 */
#define shmemx_broadcast(ARG, ...)                                      \
  _Generic((ARG),                                                       \
           int*   : shmem_broadcast32,                                  \
           long*  : shmem_broadcast64,                                  \
           float* : shmem_broadcast32,                                  \
           double*: shmem_broadcast64,                                  \
           int    : shmemx_int_broadcast,                               \
           long   : shmemx_long_broadcast,                              \
           float  : shmemx_float_broadcast,                             \
           double : shmemx_double_broadcast)((ARG), __VA_ARGS__)

#endif // #if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 201112L)

#endif // #ifndef __SHMEMX_H__

#endif // #if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)
