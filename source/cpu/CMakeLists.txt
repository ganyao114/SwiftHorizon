add_library(cpu_core STATIC
        arm64/arm64_interface.h
        arm64/hypervisor/arm64_hypervisor.cpp
        arm64/hypervisor/arm64_hypervisor.h)

target_link_libraries(cpu_core PUBLIC base)

if (APPLE AND ("arm64" IN_LIST ARCHITECTURE))
    find_library(HYPERVISOR Hypervisor)
    target_link_libraries(cpu_core PRIVATE ${HYPERVISOR})
endif ()