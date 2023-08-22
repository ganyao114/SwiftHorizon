//
// Created by 甘尧 on 2023/8/21.
//

#include <cstdint>

namespace ams::kern::init {
    void InitializeCorePhase1(uintptr_t misc_unk_debug_phys_addr, void **initial_state);
}

namespace swift::horizon::kernel {
    void InitForKernel() {
        ams::kern::init::InitializeCorePhase1(0, nullptr);
    }
}
