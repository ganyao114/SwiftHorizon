add_library(runtime STATIC
        include/address_space.h
        include/config.h
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
        ir/terminal.cpp
        ir/terminal.h
        ir/module.cpp
        ir/module.h
        ir/function.cpp
        ir/function.h
        ir/hir_builder.cpp
        ir/hir_builder.h
        backend/translate_table.h
        backend/interpreter.cpp
        backend/interpreter.h
        backend/context.h
        ir/ir_meta.cpp)

find_package(Boost 1.57 REQUIRED)
target_link_libraries(runtime PUBLIC base vapours Boost::boost)