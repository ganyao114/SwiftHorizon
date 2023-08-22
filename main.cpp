#include <iostream>
#include "source/kernel/host/kernel_main.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    swift::horizon::kernel::InitForKernel();
    return 0;
}
