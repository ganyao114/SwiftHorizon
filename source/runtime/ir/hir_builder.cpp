//
// Created by 甘尧 on 2023/9/8.
//

#include "hir_builder.h"

namespace swift::runtime::ir {

Edge::Edge(HIRBlock* src, HIRBlock* dest) : src_block(src), dest_block(dest) {}

HIRBlock::HIRBlock(Block* block) : block(block) {}

void HIRBlock::AddIncomingEdge(Edge* edge) { incoming_edges.push_back(*edge); }

void HIRBlock::AddOutgoingEdge(Edge* edge) { outgoing_edges.push_back(*edge); }

bool HIRBlock::HasIncomingEdges() { return !incoming_edges.empty(); }

bool HIRBlock::HasOutgoingEdges() { return !outgoing_edges.empty(); }

HIRFunction::HIRFunction(const Location& begin, const Location& end, ObjectPool<HIRBlock>& block_pool, ObjectPool<Edge>& edge_pool)
        : begin(begin), end(end), hir_block_pool(block_pool), hir_edge_pool(edge_pool) {}

void HIRFunction::AddBlock(Block* block) {
    auto hir_block = hir_block_pool.Create(block);
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
        for (auto &incoming : dest->GetIncomingEdges()) {
            incoming.flags &= ~Edge::DOMINATES;
        }
    }
}

void HIRFunction::RemoveEdge(Edge* edge) {

}

void HIRFunction::MergeAdjacentBlocks(HIRBlock* left, HIRBlock* right) {

}

}  // namespace swift::runtime::ir
