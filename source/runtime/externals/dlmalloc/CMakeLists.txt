add_definitions(-DONLY_MSPACES=1)
add_library(dlmalloc STATIC
    malloc.h
    malloc.c)