#include "kernel/host/kernel_main.h"
#include <catch2/catch_test_macros.hpp>
#include <thread>


TEST_CASE("Test kernel-init") {
    swift::horizon::kernel::InitForKernel();
}