#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>
#include "shmemx.h"

#define TEST_PUT(TYPE)                                  \
  do {                                                  \
    static TYPE remote;                                 \
    TYPE local = mype;                                  \
    shmemx_put(&remote, &local, 1, (mype + 1) % npes);  \
    shmem_barrier_all();                                \
    if (remote != (TYPE)((mype + npes - 1) % npes)) {   \
      fprintf(stderr,                                   \
              "PE %i received incorrect value "         \
              "for shmemx_put(%s,...)\n", mype, #TYPE); \
    }                                                   \
  } while (false)

int main(/*int argc, char* argv[]*/) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();
  shmem_barrier_all();

  TEST_PUT(double);
  TEST_PUT(float);
  TEST_PUT(int);
  TEST_PUT(long);
  TEST_PUT(long double);
  TEST_PUT(long long);
  TEST_PUT(short);
  TEST_PUT(uint32_t);
  TEST_PUT(uint64_t);

  shmem_barrier_all();
  shmem_finalize();
  return 0;
}
