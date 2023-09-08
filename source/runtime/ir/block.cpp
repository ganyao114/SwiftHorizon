//
// Created by 甘尧 on 2023/9/6.
//

#include "block.h"

namespace swift::runtime::ir {
Terminal Block::GetTerminal() const { return block_term; }
void Block::SetTerm(Terminal term) { block_term = term; }
bool Block::HasTerminal() const { return !block_term.empty(); }
}  // namespace swift::runtime::ir