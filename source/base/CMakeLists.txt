add_library(base STATIC
        common_funcs.h
        types.h
        alignment.h
        logging.h
        logging.cpp
        thread.cpp
        thread.h
)

target_link_libraries(base PUBLIC fmt)

if (ANDROID)
    target_link_libraries(base PUBLIC log)
endif ()