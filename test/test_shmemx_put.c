#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_SHMEMX_PUT(TYPE)                                   \
  do {                                                          \
    static TYPE remote[10];                                     \
    TYPE local[10];                                             \
    for (int i = 0; i < 10; i++)                                \
      local[i] = (TYPE)mype;                                    \
    shmemx_put(remote, local, 10, (mype + 1) % npes);           \
    shmem_barrier_all();                                        \
    for (int i = 0; i < 10; i++)                                \
      if (remote[i] != (TYPE)((mype + npes - 1) % npes)) {      \
        fprintf(stderr,                                         \
                "PE %i received incorrect value "               \
                "for shmemx_put(%s,...)\n", mype, #TYPE);       \
        rc = EXIT_FAILURE;                                      \
      }                                                         \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  int rc = EXIT_SUCCESS;
  TEST_SHMEMX_PUT(double);
  TEST_SHMEMX_PUT(float);
  TEST_SHMEMX_PUT(int);
  TEST_SHMEMX_PUT(long);
  TEST_SHMEMX_PUT(long double);
  TEST_SHMEMX_PUT(long long);
  TEST_SHMEMX_PUT(short);
  TEST_SHMEMX_PUT(uint32_t);
  TEST_SHMEMX_PUT(uint64_t);

  shmem_barrier_all();
  shmem_finalize();
  return rc;
}
