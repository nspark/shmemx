#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_SHMEMX_P(TYPE)                                     \
  do {                                                          \
    static TYPE remote;                                         \
    shmemx_p(&remote, (TYPE)mype, (mype + 1) % npes);           \
    shmem_barrier_all();                                        \
    if (remote != (TYPE)((mype + npes - 1) % npes)) {           \
      fprintf(stderr,                                           \
              "PE %i received incorrect value "                 \
              "for shmemx_p(%s, ...)\n", mype, #TYPE);          \
      rc = 1;                                                   \
    }                                                           \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  int rc = 0;
  TEST_SHMEMX_P(short);
  TEST_SHMEMX_P(int);
  TEST_SHMEMX_P(long);
  TEST_SHMEMX_P(long long);
  TEST_SHMEMX_P(float);
  TEST_SHMEMX_P(double);
  TEST_SHMEMX_P(long double);

  shmem_barrier_all();
  shmem_finalize();
  return rc;
}
