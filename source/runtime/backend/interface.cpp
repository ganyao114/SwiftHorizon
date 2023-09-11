//
// Created by 甘尧 on 2023/9/7.
//
#pragma once

#include "runtime/common/types.h"
#include "runtime/include/sruntime.h"

namespace swift::runtime {

struct Interface::Impl final {
    Impl(Interface* interface, Config conf) : interface(interface), conf(std::move(conf)) {}

    Interface* interface;
    Config conf;
};

Interface::Interface(Config config) : impl(std::make_unique<Impl>(this, std::move(config))) {}

}  // namespace swift::runtime