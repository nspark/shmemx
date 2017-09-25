#ifndef __SHMEM_DEBUG_H__
#define __SHMEM_DEBUG_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <shmem.h>

#define RESET    "\x1b[0m"
#define BOLD     "\x1b[1m"

#define RED      "\x1b[31m"
#define YELLOW   "\x1b[33m"

#define ERROR    BOLD RED "error (PE %d): " RESET

#define ERRTAG   "\x1b[31;1m[ERROR]\x1b[0m "
#define WARNTAG  "\x1b[33m[WARNING]\x1b[0m "

#define shmem_global_exit(STATUS)                               \
  /* fprintf(stderr, "shmem_global_exit(%d)\n", (STATUS)) */

#define SHMEM_FATAL_ERROR(NAME, ERRMSG)         \
  do {                                          \
    fprintf(stderr, BOLD "%s:%d: " RESET ERROR  \
            "%s() called with %s\n",            \
            __FILE__, __LINE__, shmem_my_pe(),  \
            (NAME), (ERRMSG));                  \
    shmem_global_exit(EXIT_FAILURE);            \
  } while (false)

#define SHMEM_FATAL_ERROR_FMT(NAME, ERRFMT, ...)        \
  do {                                                  \
    fprintf(stderr, BOLD "%s:%d: " RESET ERROR          \
            "%s() called with " ERRFMT "\n",            \
            __FILE__, __LINE__, shmem_my_pe(),          \
            (NAME), __VA_ARGS__);                       \
    shmem_global_exit(EXIT_FAILURE);                    \
  } while (false)

#define VERIFY_NONNULL(PTR, NAME)                               \
  do {                                                          \
    if ((PTR) == NULL)                                          \
      SHMEM_FATAL_ERROR_FMT((NAME),                             \
                            "argument %s must be non-null",     \
                            #PTR);                              \
  } while (false)

#define VERIFY_SYMMETRIC(ADDR, NAME)                                    \
  do {                                                                  \
    if (!shmem_addr_accessible((ADDR), shmem_my_pe()))                  \
      SHMEM_FATAL_ERROR_FMT((NAME),                                     \
                            "non-symmetric argument %s", #ADDR);        \
  } while (false)

#define VERIFY_SYMMETRIC_OR_NULL(ADDR, NAME)                            \
  do {                                                                  \
    if (((ADDR) != NULL) &&                                             \
        (!shmem_addr_accessible((ADDR), shmem_my_pe())))                \
      SHMEM_FATAL_ERROR_FMT((NAME),                                     \
                            "non-symmetric argument %s", #ADDR);        \
  } while (false)

#define VERIFY_ACTIVESET(START, LOGSTRIDE, SIZE, NAME)          \
  do {                                                          \
    if ((START) + (1 << (LOGSTRIDE)) * (SIZE) > shmem_n_pes())  \
      SHMEM_FATAL_ERROR((NAME), "invalid active set");          \
  } while (false)

#define VERIFY_PE(PE, NAME)                     \
  do {                                          \
    if ((PE) < 0 || (PE) >= shmem_n_pes())      \
      SHMEM_FATAL_ERROR((NAME), "invalid PE");  \
  } while (false)

static long debug_coll_eq_src = 0;
static long debug_coll_eq_dst = 0;
static long debug_coll_eq_pWork[SHMEM_REDUCE_MIN_WRKDATA_SIZE];
static long debug_coll_eq_pSync[SHMEM_REDUCE_SYNC_SIZE];

#define VERIFY_COLL_EQUALITY(VALUE, START, LOGSTRIDE, SIZE, NAME)       \
  do {                                                                  \
    debug_coll_eq_src = (long)(VALUE);                                  \
    debug_coll_eq_dst = 0;                                              \
    shmem_long_and_to_all(&debug_coll_eq_dst,                           \
                          &debug_coll_eq_src,                           \
                          1, (START), (LOGSTRIDE), (SIZE),              \
                          debug_coll_eq_pWork,                          \
                          debug_coll_eq_pSync);                         \
    long and_all = debug_coll_eq_dst;                                   \
    shmem_long_or_to_all(&debug_coll_eq_dst,                            \
                         &debug_coll_eq_src,                            \
                         1, (START), (LOGSTRIDE), (SIZE),               \
                         debug_coll_eq_pWork,                           \
                         debug_coll_eq_pSync);                          \
    long or_all = debug_coll_eq_dst;                                    \
    if (and_all != or_all)                                              \
      SHMEM_FATAL_ERROR_FMT((NAME),                                     \
                            "argument %s must be equal across all PEs", \
                            #VALUE);                                    \
  } while (false)

#define shmem_init()                                    \
  do {                                                  \
    for (int i = 0; i < SHMEM_REDUCE_SYNC_SIZE; i++)    \
      debug_coll_eq_pSync[i] = SHMEM_SYNC_VALUE;        \
    shmem_init();                                       \
  } while (false)                                       \

#define shmem_malloc(SIZE)                      \
  shmem_malloc((SIZE));                         \
  do {                                          \
    VERIFY_COLL_EQUALITY((SIZE),                \
                         0, 0, shmem_n_pes(),   \
                         "shmem_malloc");       \
  } while (false)

#define shmem_free(PTR)                                 \
  do {                                                  \
    VERIFY_SYMMETRIC_OR_NULL((PTR), "shmem_free");      \
    /* shmem_free((PTR)); */                            \
  } while (false)

#define shmem_int_put(DST, SRC, NELEMS, PE)             \
  do {                                                  \
    VERIFY_SYMMETRIC((DST), "shmem_int_put");           \
    VERIFY_PE((PE), "shmem_int_put");                   \
    /* shmem_int_put((DST), (SRC), (NELEMS), (PE)); */  \
  } while (false)

#define shmem_broadcast64(DST, SRC, NELEMS, ROOT, START,        \
                          LOGSTRIDE, SIZE, PSYNC)               \
  do {                                                          \
    VERIFY_SYMMETRIC((DST), "shmem_broadcast64");               \
    VERIFY_SYMMETRIC((SRC), "shmem_broadcast64");               \
    VERIFY_COLL_EQUALITY((NELEMS),                              \
                         (START), (LOGSTRIDE), (SIZE),          \
                         "shmem_broadcast64");                  \
    VERIFY_COLL_EQUALITY((ROOT),                                \
                         (START), (LOGSTRIDE), (SIZE),          \
                         "shmem_broadcast64");                  \
    VERIFY_ACTIVESET((START), (LOGSTRIDE), (SIZE),              \
                     "shmem_broadcast64");                      \
    VERIFY_SYMMETRIC((PSYNC), "shmem_broadcast64");             \
    /* shmem_broadcast64((DST), (SRC), (NELEMS),       */       \
    /* (ROOT), (START), (LOGSTRIDE), (SIZE), (PSYNC)); */       \
  } while (false)

#if /* OpenSHMEM >= 1.3 */                                      \
  defined(SHMEM_MAJOR_VERSION) && SHMEM_MAJOR_VERSION == 1 &&   \
  defined(SHMEM_MINOR_VERSION) && SHMEM_MINOR_VERSION >= 3

#if defined(shmem_put)
#undef shmem_put
#endif

#define shmem_put(DST, SRC, LEN, PE)                    \
  do {                                                  \
    VERIFY_SYMMETRIC((DST), "shmem_put");               \
    VERIFY_PE((PE), "shmem_put");                       \
    /* _Generic(&*(DST),                             */ \
    /*         char*:        shmem_char_put,         */ \
    /*         short*:       shmem_short_put,        */ \
    /*         int*:         shmem_int_put,          */ \
    /*         long*:        shmem_long_put,         */ \
    /*         long long*:   shmem_longlong_put,     */ \
    /*         float*:       shmem_float_put,        */ \
    /*         double*:      shmem_double_put,       */ \
    /*         long double*: shmem_longdouble_put)   */ \
    /*         (DST, SRC, LEN, PE);                  */ \
  } while (false)

#endif /* OpenSHMEM >= 1.3 */

#define shmem_barrier_all()                     \
  do {                                          \
                                                \
    shmem_barrier_all();                        \
  } while (false)

#endif
