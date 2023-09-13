enable_language(ASM)

if ("arm64" IN_LIST ARCHITECTURE)
    set(RUNTIME_ARCH_SOURCE backend/arm64/context.S)
else ()
    set(RUNTIME_ARCH_SOURCE )
endif ()

add_library(runtime STATIC
        include/sruntime.h
        include/address_space.h
        include/config.h
        common/types.h
        common/slab_alloc.cc
        common/slab_alloc.h
        common/object_pool.h
        common/common_funcs.h
        common/variant_util.h
        common/logging.h
        common/logging.cpp
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
        ir/function.cpp
        ir/function.h
        ir/module.cpp
        ir/module.h
        ir/hir_builder.cpp
        ir/hir_builder.h
        ir/ir_meta.cpp
        backend/translate_table.h
        backend/interpreter.cpp
        backend/interpreter.h
        backend/context.h
        backend/address_space.cpp
        backend/address_space.h
        backend/interface.cpp
        ${RUNTIME_ARCH_SOURCE})



find_package(Boost 1.57 REQUIRED)
target_link_libraries(runtime PUBLIC base vapours Boost::boost)