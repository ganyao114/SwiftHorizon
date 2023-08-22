file(GLOB MESOSPHERE_KERN_SRC source/*.cpp)
file(GLOB MESOSPHERE_INIT_SRC source/init/*.cpp)
file(GLOB MESOSPHERE_LIBC_SRC source/libc/*.cpp)
file(GLOB MESOSPHERE_SVC_SRC source/svc/*.cpp)

if (LINUX OR ANDROID OR APPLE OR WIN32)
    file(GLOB MESOSPHERE_PLATFORM_SRC source/arch/host/*.cpp)
else ()
    file(GLOB MESOSPHERE_PLATFORM_SRC source/arch/arm64/*)
endif()

if ("arm64" IN_LIST ARCHITECTURE)
    set(MESOSPHERE_PLATFORM_SRC ${MESOSPHERE_PLATFORM_SRC}
            source/arch/arm64/kern_k_debug.cpp
            source/arch/arm64/kern_k_thread_context.cpp)
endif ()

add_library(mesosphere STATIC ${MESOSPHERE_KERN_SRC}
        ${MESOSPHERE_INIT_SRC}
        ${MESOSPHERE_LIBC_SRC}
        ${MESOSPHERE_SVC_SRC}
        ${MESOSPHERE_PLATFORM_SRC})

target_include_directories(mesosphere PUBLIC include)
target_link_libraries(mesosphere PRIVATE vapours)