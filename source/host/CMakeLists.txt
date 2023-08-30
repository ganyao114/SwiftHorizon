add_library(host STATIC
        virtual_vector.cpp
        virtual_vector.h
        host_memory.cpp
        host_memory.h
)

target_link_libraries(host PUBLIC base)