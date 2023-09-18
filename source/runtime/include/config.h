//
// Created by 甘尧 on 2023/9/7.
//

#pragma once

#include <span>

namespace swift::runtime {

struct UniformDesc {
    uint32_t offset;
    uint32_t size;
};

struct Config {
    bool enable_jit;
    bool enable_asm_interp;
    u32 uniform_buffer_size;
    std::vector<UniformDesc> buffers_static_alloc; // 静态分配建议
};

}