#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_SHMEMX_GET(TYPE)                                   \
  do {                                                          \
    static TYPE remote[10];                                     \
    TYPE local[10];                                             \
    for (int i = 0; i < 10; i++)                                \
      remote[i] = (TYPE)mype;                                   \
    shmem_barrier_all();                                        \
    shmemx_get(local, remote, 10, (mype + 1) % npes);           \
    for (int i = 0; i < 10; i++)                                \
      if (local[i] != (TYPE)((mype + 1) % npes)) {              \
        fprintf(stderr,                                         \
                "PE %i received incorrect value "               \
                "for shmemx_get(%s,...)\n", mype, #TYPE);       \
        rc = EXIT_FAILURE;                                      \
      }                                                         \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  int rc = EXIT_SUCCESS;
  TEST_SHMEMX_GET(double);
  TEST_SHMEMX_GET(float);
  TEST_SHMEMX_GET(int);
  TEST_SHMEMX_GET(long);
  TEST_SHMEMX_GET(long double);
  TEST_SHMEMX_GET(long long);
  TEST_SHMEMX_GET(short);
  TEST_SHMEMX_GET(uint32_t);
  TEST_SHMEMX_GET(uint64_t);

  shmem_barrier_all();
  shmem_finalize();
  return rc;
}
