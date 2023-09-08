//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/ir/instr.h"
#include "runtime/ir/terminal.h"

namespace swift::runtime::ir {

class Block : public SlabObject<Block, true> {
public:
    Block() = default;

    Terminal GetTerminal() const;
    void SetTerm(Terminal term);
    bool HasTerminal() const;

private:
    IntrusiveListNode list_node;
    IntrusiveList<&Inst::list_node> inst_list;
    Terminal block_term;
};

}  // namespace swift::runtime::ir