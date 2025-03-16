set(DIR_SRCS src/fdb_kvdb.c
             src/fdb_utils.c
             src/fdb.c)
add_library(flashdb STATIC ${DIR_SRCS})