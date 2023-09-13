//
// Created by 甘尧 on 2023/9/7.
//
#pragma once

#include "runtime/common/types.h"
#include "runtime/include/sruntime.h"
#include "runtime/ir/function.h"
#include "runtime/backend/translate_table.h"

namespace swift::runtime {

constexpr static auto l1_cache_bits = 18;

struct Interface::Impl final {
    Impl(Interface* interface, Config conf) : interface(interface), conf(std::move(conf)) {}

    Interface* interface;
    Config conf;
    ir::BlockMap ir_blocks{};
    ir::FunctionMap ir_functions{};
    TranslateTable l1_code_cache{l1_cache_bits};
};

Interface::Interface(Config config) : impl(std::make_unique<Impl>(this, std::move(config))) {}

}  // namespace swift::runtime