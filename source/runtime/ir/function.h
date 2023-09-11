//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/ir/block.h"

namespace swift::runtime::ir {

class Function : public SlabObject<Function, true> {
public:

private:
    BlockMap blocks;
};

}  // namespace swift::runtime::ir