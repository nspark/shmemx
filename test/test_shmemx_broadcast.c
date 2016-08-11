#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_SHMEMX_BCAST(TYPE)                                 \
  do {                                                          \
    TYPE value = (TYPE)0;                                       \
    if (mype == 0)                                              \
      value = (TYPE)42;                                         \
    shmem_barrier_all();                                        \
    value = shmemx_broadcast(value, 0, 0, 0, npes, pSync);      \
    shmem_barrier_all();                                        \
    if (value != (TYPE)42)                                      \
    {                                                           \
      fprintf(stderr,                                           \
              "PE %i received incorrect value "                 \
              "for shmemx_val_bcast(%s,...)\n", mype, #TYPE);   \
    rc = EXIT_FAILURE;                                          \
    }                                                           \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  static long pSync[SHMEM_BCAST_SYNC_SIZE];
  for (int i = 0; i < SHMEM_BCAST_SYNC_SIZE; i++)
    pSync[i] = SHMEM_SYNC_VALUE;
  shmem_barrier_all();

  int rc = EXIT_SUCCESS;
  TEST_SHMEMX_BCAST(double);
  TEST_SHMEMX_BCAST(float);
  TEST_SHMEMX_BCAST(int);
  TEST_SHMEMX_BCAST(long);

  shmem_barrier_all();
  shmem_finalize();
  return rc;
}
