set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fdiagnostics-show-option -Wextra -Wredundant-decls -pedantic -Wwrite-strings -Wunused")
include_directories(.)
add_definitions(-DVIXL_CODE_BUFFER_MALLOC)

set(VIXL_SOURCES
        code-buffer-vixl.cc
        compiler-intrinsics-vixl.cc
        cpu-features.cc
        utils-vixl.cc)

set(VIXL_AARCH32
        aarch32/assembler-aarch32.cc
        aarch32/constants-aarch32.cc
        aarch32/instructions-aarch32.cc
        aarch32/location-aarch32.cc
        aarch32/macro-assembler-aarch32.cc
        aarch32/operands-aarch32.cc
        aarch32/disasm-aarch32.cc
        )

set(VIXL_AARCH64
        aarch64/assembler-aarch64.cc
        aarch64/cpu-aarch64.cc
        aarch64/cpu-features-auditor-aarch64.cc
        aarch64/decoder-aarch64.cc
        aarch64/instructions-aarch64.cc
        aarch64/instrument-aarch64.cc
        aarch64/logic-aarch64.cc
        aarch64/macro-assembler-aarch64.cc
        aarch64/operands-aarch64.cc
        aarch64/pointer-auth-aarch64.cc
        aarch64/simulator-aarch64.cc
        aarch64/disasm-aarch64.cc)

#if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
#    add_definitions("-DVIXL_INCLUDE_TARGET_A32")
#    add_definitions("-DVIXL_INCLUDE_TARGET_T32")
#    set(VIXL_SOURCES ${VIXL_SOURCES} ${VIXL_AARCH32})
#elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
#    add_definitions("-DVIXL_INCLUDE_TARGET_AARCH64")
#    set(VIXL_SOURCES ${VIXL_SOURCES} ${VIXL_AARCH64} ${VIXL_AARCH32})
#endif ()

add_library(vixl STATIC
        ${VIXL_SOURCES}
        ${VIXL_AARCH32}
        ${VIXL_AARCH64})

target_include_directories(vixl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})