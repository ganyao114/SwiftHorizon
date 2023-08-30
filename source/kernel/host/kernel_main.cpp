//
// Created by 甘尧 on 2023/8/21.
//

#include <cstdint>
#include "kernel_main.h"
#include "mesosphere.hpp"
#include "host/host_memory.h"

namespace ams::kern::init {
    void InitializeCorePhase1(uintptr_t misc_unk_debug_phys_addr, void **initial_state);
}

namespace swift::horizon::kernel {

    using namespace ams::kern;

    static HostMemory host_memory{HostMemory::CreateForKern(kernel_space_size)};

    void InitForKernel() {
        init::KInitialPageAllocator::State state{
                .start_address = reinterpret_cast<uintptr_t>(host_memory.BackingBasePointer()),
                .end_address = reinterpret_cast<uintptr_t>(host_memory.BackingBasePointer() + host_memory.BackingSize())};
        InitialProcessBinaryLayout layout{};
        std::array<void *, 2> boot_args{&state, &layout};
        init::InitializeCorePhase1(0, boot_args.data());
    }
}
