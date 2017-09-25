#include <stdlib.h>
#include <stdint.h>
#include <shmem_debug.h>

long pSync[SHMEM_REDUCE_SYNC_SIZE];

int main()
{
  for (int i = 0; i < SHMEM_REDUCE_SYNC_SIZE; i++)
    pSync[i] = SHMEM_SYNC_VALUE;
  shmem_init();

  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();

  {
    int *mem1 = shmem_malloc(1024*sizeof(int));
    int *mem2 =       malloc(1024*sizeof(int));

    shmem_put(mem1, mem2, 32, (mype + 1) % npes);
    shmem_put(mem2, mem2, 32, (mype + 1) % npes); // error: mem2 not symmetric
    shmem_put(mem1, mem2, 32, npes);              // error: npes out of range

    shmem_free(mem1);
    shmem_free(mem2); // error: mem2 not symmetric
    shmem_free(NULL);
  }

  {
    uint64_t *mem1 = shmem_malloc(1024*sizeof(uint64_t));
    uint64_t *mem2 = shmem_malloc(1024*sizeof(uint64_t));

    // error: root argument should be equal across all PEs
    shmem_broadcast64(mem2, mem1,       256, mype, 0, 0, npes, pSync);
    // error: nelems argument should be equal across all PEs
    shmem_broadcast64(mem2, mem1, 64 * mype,    0, 0, 0, npes, pSync);
    // error: pSync should be symmetric
    shmem_broadcast64(mem2, mem1,       256,    0, 0, 0, npes, NULL);

    shmem_free(mem1);
    shmem_free(mem2);
  }

  {
    // error: size argument should be equal across all PEs
    int *mem1 = shmem_malloc(1024*sizeof(int) + mype);
    shmem_free(mem1);
  }

  shmem_finalize();

  return 0;
}
