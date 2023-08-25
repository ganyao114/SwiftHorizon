add_library(kernel_core STATIC
        kern_k_scheduler.cpp
        arch/arm64/kern_k_thread_context.cpp
        ../ams/kern_kernel_instantiations.cpp
        ../ams/arch/arm64/init/kern_init_core.cpp
        start.cpp
        kern_cpu.cpp
        exception_vectors.cpp
        kern_panic.cpp
        kernel_main.h)
target_link_libraries(kernel_core PRIVATE base vapours mesosphere)