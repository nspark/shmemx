#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_SHMEMX_G(TYPE)                                     \
  do {                                                          \
    static TYPE remote;                                         \
    remote = (TYPE)mype;                                        \
    shmem_barrier_all();                                        \
    TYPE val = shmemx_g(&remote, (mype + 1) % npes);            \
    if (val != (TYPE)((mype + 1) % npes)) {                     \
      fprintf(stderr,                                           \
              "PE %i received incorrect value "                 \
              "for shmemx_g(%s, ...)\n", mype, #TYPE);          \
      rc = 1;                                                   \
    }                                                           \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  int rc = 0;
  TEST_SHMEMX_G(char);
  TEST_SHMEMX_G(short);
  TEST_SHMEMX_G(int);
  TEST_SHMEMX_G(long);
  TEST_SHMEMX_G(long long);
  TEST_SHMEMX_G(float);
  TEST_SHMEMX_G(double);
  TEST_SHMEMX_G(long double);

  shmem_barrier_all();
  shmem_finalize();
  return rc;
}
