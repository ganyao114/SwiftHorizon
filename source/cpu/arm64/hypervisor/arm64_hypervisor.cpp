//
// Created by 甘尧 on 2023/8/18.
//

#include "arm64_hypervisor.h"

namespace swift::horizon::cpu::arm64::hpv {

    ARM64Hypervisor::ARM64Hypervisor(s32 core_id) : ARM64Interface(core_id) {}

}