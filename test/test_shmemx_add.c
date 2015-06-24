#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_SHMEMX_ADD(TYPE)                                           \
  do {                                                                  \
    static TYPE remote = (TYPE)0;                                       \
    for (int i = 0; i < npes; i++)                                      \
      shmemx_add(&remote, (TYPE)(mype + 1), i);                         \
    shmem_barrier_all();                                                \
    if (remote != (TYPE)(npes * (npes + 1) / 2)) {                      \
      fprintf(stderr,                                                   \
              "PE %i observed error with shmemx_add(%s, ...)\n",        \
              mype, #TYPE);                                             \
      rc = EXIT_FAILURE;                                                \
    }                                                                   \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  int rc = EXIT_SUCCESS;
  TEST_SHMEMX_ADD(int);
  TEST_SHMEMX_ADD(long);
  TEST_SHMEMX_ADD(long long);

  shmem_barrier_all();
  shmem_finalize();
  return rc;
}
