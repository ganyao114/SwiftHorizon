//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "instr.h"

namespace swift::runtime::ir {

class Block : public SlabObject<Block, true> {
public:
    Block() = default;

private:
    IntrusiveListNode list_node;
    IntrusiveList<&Inst::list_node> inst_list;
};

}  // namespace swift::runtime::ir