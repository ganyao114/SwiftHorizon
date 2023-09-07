add_library(runtime STATIC
        common/types.h
        common/slab_alloc.cc
        common/slab_alloc.h
        common/object_pool.h
        ir/args.cpp
        ir/args.h
        ir/location.cpp
        ir/location.h
        ir/instr.cpp
        ir/instr.h
        ir/block.cpp
        ir/block.h)

target_link_libraries(runtime PUBLIC base vapours)