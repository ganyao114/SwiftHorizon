//
// Created by 甘尧 on 2023/9/8.
//

#include <utility>
#include "hir_builder.h"

namespace swift::runtime::ir {

Edge::Edge(HIRBlock* src, HIRBlock* dest) : src_block(src), dest_block(dest) {}

HIRBlock::HIRBlock(Block* block) : block(block) {}

void HIRBlock::AddIncomingEdge(Edge* edge) { incoming_edges.push_back(*edge); }

void HIRBlock::AddOutgoingEdge(Edge* edge) { outgoing_edges.push_back(*edge); }

bool HIRBlock::HasIncomingEdges() { return !incoming_edges.empty(); }

bool HIRBlock::HasOutgoingEdges() { return !outgoing_edges.empty(); }

u16 HIRBlock::MaxValueCount() { return 0; }

HIRValue::HIRValue(u16 id, const Value& value, HIRBlock* block) : value(value), block(block) {
    if (auto def = value.Def(); def) {
        def->SetId(id);
    }
}

HIRFunction::HIRFunction(const Location& begin,
                         const Location& end,
                         ObjectPool<HIRBlock>& block_pool,
                         ObjectPool<Edge>& edge_pool,
                         ObjectPool<HIRValue>& value_pool)
        : begin(begin)
        , end(end)
        , hir_block_pool(block_pool)
        , hir_edge_pool(edge_pool)
        , hir_value_pool(value_pool) {
    AddBlock(new Block(begin));
}

void HIRFunction::AddBlock(Block* block) {
    auto hir_block = hir_block_pool.Create(block);
    hir_block->order_id = block_order_id++;
    blocks.push_back(*hir_block);
    current_block = hir_block;
}

void HIRFunction::AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional) {
    assert(src && dest);
    bool dest_was_dominated = dest->HasIncomingEdges();
    auto edge = hir_edge_pool.Create(src, dest);
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

HIRBuilder::HIRBuilder(u32 func_cap)
        : hir_function_pool(func_cap)
        , hir_block_pool(func_cap * 8)
        , hir_edge_pool(func_cap * 16)
        , hir_value_pool(func_cap * 256) {}

void HIRBuilder::SetLocation(const Location& location) {}

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
