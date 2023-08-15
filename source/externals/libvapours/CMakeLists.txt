file(GLOB_RECURSE VAPOURS_UTIL_SRC source/util/*.cpp)
file(GLOB_RECURSE VAPOURS_UTIL_RESULT source/result/*.cpp)
file(GLOB_RECURSE VAPOURS_UTIL_DD source/dd/*.cpp)

add_library(vapours STATIC ${VAPOURS_UTIL_SRC}
        ${VAPOURS_UTIL_RESULT}
        ${VAPOURS_UTIL_DD})

target_include_directories(vapours PUBLIC include)
target_link_libraries(vapours PRIVATE mesosphere)