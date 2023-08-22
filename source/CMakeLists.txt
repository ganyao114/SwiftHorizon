enable_language(ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

# Add the module directory to the list of paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Arch detection
include(DetectArchitecture)
if (NOT DEFINED ARCHITECTURE)
    message(FATAL_ERROR "Unsupported architecture encountered. Ending CMake generation.")
endif()
message(STATUS "Target architecture: ${ARCHITECTURE}")

# Setup platform
add_definitions(-DATMOSPHERE_IS_MESOSPHERE=1)
add_definitions(-DATMOSPHERE_GIT_REVISION="1.0")
add_definitions(-DMESOSPHERE_ENABLE_THIS_ASSERT=1)
add_definitions(-DMESOSPHERE_BUILD_FOR_AUDITING=1)
if (LINUX OR ANDROID)
    add_definitions(-DATMOSPHERE_OS_LINUX=1)
    add_definitions(-DATMOSPHERE_BOARD_HOST_OS=1)
elseif (APPLE)
    add_definitions(-DATMOSPHERE_OS_MACOS=1)
    add_definitions(-DATMOSPHERE_BOARD_HOST_OS=1)
elseif (WIN32)
    add_definitions(-DATMOSPHERE_OS_WINDOWS=1)
    add_definitions(-DATMOSPHERE_BOARD_HOST_OS=1)
elseif (QEMU)
    add_definitions(-DATMOSPHERE_OS_HORIZON=1)
    add_definitions(-DATMOSPHERE_BOARD_QEMU_VIRT=1)
else ()
    add_definitions(-DATMOSPHERE_OS_HORIZON=1)
    add_definitions(-DATMOSPHERE_BOARD_NINTENDO_NX=1)
endif ()

# Setup cpu arch
if ("arm" IN_LIST ARCHITECTURE)
    add_definitions(-DATMOSPHERE_ARCH_ARM=1)
elseif ("arm64" IN_LIST ARCHITECTURE)
    add_definitions(-DATMOSPHERE_ARCH_ARM64=1)
    add_definitions(-DATMOSPHERE_ARCH_ARM_V8A=1)
elseif ("x86_64" IN_LIST ARCHITECTURE)
    add_definitions(-DATMOSPHERE_ARCH_X64=1)
elseif ("x86" IN_LIST ARCHITECTURE)
    add_definitions(-DATMOSPHERE_ARCH_X86=1)
else ()

endif()


add_subdirectory(externals)
add_subdirectory(base)
add_subdirectory(cpu)
add_subdirectory(kernel)