//
// Created by 甘尧 on 2023/9/8.
//

#include <utility>
#include "hir_builder.h"

namespace swift::runtime::ir {

Edge::Edge(HIRBlock* src, HIRBlock* dest) : src_block(src), dest_block(dest) {}

HIRBlock::HIRBlock(Block* block) : block(block) {}

HIRValueList& HIRBlock::GetHIRValues() { return values; }

u16 HIRBlock::GetOrderId() { return order_id; }

void HIRBlock::AddIncomingEdge(Edge* edge) { incoming_edges.push_back(*edge); }

void HIRBlock::AddOutgoingEdge(Edge* edge) { outgoing_edges.push_back(*edge); }

bool HIRBlock::HasIncomingEdges() { return !incoming_edges.empty(); }

bool HIRBlock::HasOutgoingEdges() { return !outgoing_edges.empty(); }

u16 HIRBlock::MaxValueCount() { return 0; }
u16 HIRBlock::MaxBlockCount() { return 0; }

HIRValue::HIRValue(u16 id, const Value& value, HIRBlock* block) : value(value), block(block) {
    if (auto def = value.Def(); def) {
        def->SetId(id);
    }
}

HIRFunction::HIRFunction(Function* function,
                         const Location& begin,
                         const Location& end,
                         HIRPools& pools)
        : function(function), begin(begin), end(end), pools(pools) {}

void HIRFunction::AppendBlock(Location start, Location end_) {
    auto hir_block = pools.blocks.Create(new Block(start));
    hir_block->block->SetEndLocation(end_);
    hir_block->order_id = block_order_id++;
    blocks.push_back(*hir_block);
    current_block = hir_block;
}

void HIRFunction::AppendInst(Inst* inst) {
    current_block->block->AppendInst(inst);
    if (inst->HasValue()) {
        auto hir_value = pools.values.Create(value_order_id++, Value{inst}, current_block);
        current_block->values.push_back(*hir_value);
    }
}

HIRBlockList& HIRFunction::GetHIRBlocks() { return blocks; }

void HIRFunction::AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional) {
    assert(src && dest);
    bool dest_was_dominated = dest->HasIncomingEdges();
    auto edge = pools.edges.Create(src, dest);
    if (conditional) {
        edge->flags |= Edge::CONDITIONAL;
    }
    src->AddOutgoingEdge(edge);
    dest->AddIncomingEdge(edge);
    if (dest_was_dominated) {
        for (auto& incoming : dest->GetIncomingEdges()) {
            incoming.flags &= ~Edge::DOMINATES;
        }
    }
}

void HIRFunction::RemoveEdge(Edge* edge) {}

void HIRFunction::MergeAdjacentBlocks(HIRBlock* left, HIRBlock* right) {}

bool HIRFunction::SplitBlock(HIRBlock* new_block, HIRBlock* old_block) { return false; }

void HIRFunction::EndBlock(Terminal terminal) {
    current_block->block->SetTerminal(std::move(terminal));
    current_block = {};
}
u16 HIRFunction::MaxBlockCount() { return block_order_id + 1; }
u16 HIRFunction::MaxValueCount() { return value_order_id + 1; }

HIRPools::HIRPools(u32 func_cap)
        : functions(func_cap)
        , blocks(func_cap * 8)
        , edges(func_cap * 16)
        , values(func_cap * 256)
        , mem_arena(func_cap * 512) {}

HIRBuilder::HIRBuilder(u32 func_cap) : pools(func_cap) {}

void HIRBuilder::AppendFunction(Location start, Location end) {
    current_function = pools.functions.Create(new Function(start), start, end, pools);
    hir_functions.push_back(*current_function);
}

HIRFunctionList& HIRBuilder::GetHIRFunctions() { return hir_functions; }

void HIRBuilder::SetLocation(Location location) { current_location = location; }

void HIRBuilder::If(const terminal::If& if_) {
    ASSERT_MSG(!current_function, "current function is null!");
    current_function->EndBlock(if_);
}

void HIRBuilder::Switch(const terminal::Switch& switch_) {
    ASSERT_MSG(!current_function, "current function is null!");
    current_function->EndBlock(switch_);
}

void HIRBuilder::LinkBlock(const terminal::LinkBlock& link) {
    ASSERT_MSG(!current_function, "current function is null!");
    current_function->EndBlock(link);
}

}  // namespace swift::runtime::ir
