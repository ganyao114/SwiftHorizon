//
// Created by 甘尧 on 2023/9/7.
//

#pragma once

#include <memory>
#include "config.h"

namespace swift::runtime {

class Instance {};

class Interface {
public:
    explicit Interface(Config config);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}  // namespace swift::runtime