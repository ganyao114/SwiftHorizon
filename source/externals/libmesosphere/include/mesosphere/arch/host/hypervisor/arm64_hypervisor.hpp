//
// Created by swift_gan on 2023/8/16.
//

#pragma once
#include <vapours.hpp>
#include <mesosphere/arch/host/arm64_interface.hpp>
#include <Hypervisor/Hypervisor.h>
#include <queue>

namespace ams::kern::arch::host::cpu::hpv {

    struct TablePointer {
        u64 physAddr;
        u64* table;
        TablePointer* subtables[512];
    };

    class ARM64Hypervisor {
    public:

    private:

    };

}
