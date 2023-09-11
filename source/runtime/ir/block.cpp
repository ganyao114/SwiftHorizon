//
// Created by 甘尧 on 2023/9/6.
//

#include "block.h"

namespace swift::runtime::ir {
Terminal Block::GetTerminal() const { return block_term; }
void Block::SetTerm(Terminal term) { block_term = std::move(term); }
bool Block::HasTerminal() const { return !block_term.empty(); }

void Block::AppendInst(Inst* inst) {
    Inst::Validate(inst);
    inst_list.push_back(*inst);
}

}  // namespace swift::runtime::ir