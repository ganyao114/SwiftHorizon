file(GLOB_RECURSE MESOSPHERE_KERN_SRC source/*.cpp)
file(GLOB_RECURSE MESOSPHERE_INIT_SRC source/init/*.cpp)
file(GLOB_RECURSE MESOSPHERE_LIBC_SRC source/libc/*.cpp)
file(GLOB_RECURSE MESOSPHERE_SVC_SRC source/svc/*.cpp)

add_library(mesosphere STATIC ${MESOSPHERE_KERN_SRC}
        ${MESOSPHERE_INIT_SRC}
        ${MESOSPHERE_LIBC_SRC}
        ${MESOSPHERE_SVC_SRC})

target_include_directories(mesosphere PUBLIC include)
target_link_libraries(mesosphere PRIVATE vapours)