//
// Created by 甘尧 on 2023/9/7.
//
#pragma once

#include "runtime/common/types.h"

namespace swift::runtime {

struct InterpCache {
    std::vector<u8> frame_buffer;
};

struct InterpFrame {
    u32 common_arg;
    u32 handle_offset;
};

class Interpreter {
public:

private:
    u32 buffer_cursor = 0;
    std::vector<u8> frame_buffer{};
    u32 current_instr{};
    uintptr_t interp_handle_base;
};

}

