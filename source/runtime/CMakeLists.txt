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
        ir/block.h
        backend/translate_table.h
        backend/interpreter.cpp
        backend/interpreter.h
        backend/context.h
        ir/terminal.cpp
        ir/terminal.h)

find_package(Boost 1.57 REQUIRED)
target_link_libraries(runtime PUBLIC base vapours Boost::boost)