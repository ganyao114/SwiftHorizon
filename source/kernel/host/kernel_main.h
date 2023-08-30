//
// Created by 甘尧 on 2023/8/21.
//
#pragma once

#include "base/types.h"

namespace swift::horizon::kernel {
    constexpr auto kernel_space_size = 4_GB;

    void InitForKernel();
}
