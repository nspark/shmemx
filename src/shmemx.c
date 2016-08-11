#include <stdbool.h>
#include <shmem.h>
#include "shmemx.h"

#define SHMEMX_DEFN_BROADCAST(TYPE)                                     \
  TYPE shmemx_ ## TYPE ## _broadcast(                                   \
    TYPE value, int root, int start, int log_stride, int size, long *pSync) \
  {                                                                     \
    static TYPE itemp;                                                  \
    static TYPE otemp;                                                  \
    itemp = value;                                                      \
    shmemx_broadcast(&otemp, &itemp, 1,                                 \
                     root, start, log_stride, size, pSync);             \
    return (shmem_my_pe() == root ? itemp : otemp);                     \
  }

SHMEMX_DEFN_BROADCAST(int)
SHMEMX_DEFN_BROADCAST(long)
SHMEMX_DEFN_BROADCAST(float)
SHMEMX_DEFN_BROADCAST(double)

#undef SHMEMX_DEFN_BROADCAST
