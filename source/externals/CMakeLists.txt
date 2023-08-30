add_subdirectory(libvapours)
add_subdirectory(libmesosphere)

if (LINUX OR ANDROID OR APPLE OR WIN32)
    add_subdirectory(fmt)
    add_subdirectory(catch)
endif ()