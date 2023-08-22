//
// Created by 甘尧 on 2023/8/18.
//

#pragma once

#include "cpu/arm64/arm64_interface.h"
#include <Hypervisor/Hypervisor.h>
#include <queue>

namespace swift::horizon::cpu::arm64::hpv {

    struct TablePointer {
        u64 phys_addr;
        u64 *table;
        TablePointer *sub_tables[512];
    };

    class ARM64Hypervisor : public ARM64Interface {
    public:
        explicit ARM64Hypervisor(s32 coreId);

    private:
        hv_vcpu_t v_cpu;
        hv_vcpu_exit_t *exit_info;
    };

}// namespace swift::horizon::cpu::arm64::hpv