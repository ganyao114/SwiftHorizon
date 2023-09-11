//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/common/object_pool.h"
#include "runtime/ir/function.h"
#include "runtime/ir/module.h"

namespace swift::runtime::ir {

class HIRBlock;
class HIRFunction;

class Edge {
public:
    enum Flags : u8 {
        CONDITIONAL = 1 << 0,
        DOMINATES = 1 << 1,
    };

    explicit Edge(HIRBlock* src, HIRBlock* dest);

    IntrusiveListNode outgoing_edges;
    IntrusiveListNode incoming_edges;

    HIRBlock* src_block;
    HIRBlock* dest_block;

    u8 flags{};
};

class HIRBlock {
    friend class HIRFunction;

public:
    explicit HIRBlock(Block* block);

    void AddOutgoingEdge(Edge* edge);
    void AddIncomingEdge(Edge* edge);
    bool HasIncomingEdges();
    bool HasOutgoingEdges();

    auto& GetIncomingEdges() { return incoming_edges; }

    auto& GetOutgoingEdges() { return outgoing_edges; }

    IntrusiveListNode list_node;

private:
    Block* block;
    IntrusiveList<&Edge::outgoing_edges> outgoing_edges;
    IntrusiveList<&Edge::incoming_edges> incoming_edges;
};

using HIRBlockList = IntrusiveList<&HIRBlock::list_node>;

class HIRFunction {
public:
    explicit HIRFunction(const Location& begin, const Location& end, ObjectPool<HIRBlock>& block_pool, ObjectPool<Edge>& edge_pool);

    void AddBlock(Block *block);
    void AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional = false);
    void RemoveEdge(Edge *edge);
    void MergeAdjacentBlocks(HIRBlock* left, HIRBlock* right);

private:
    Location begin;
    Location end;
    ObjectPool<Edge>& hir_edge_pool;
    ObjectPool<HIRBlock> &hir_block_pool;
    HIRBlockList blocks{};
    HIRBlock* current_block{};
};

class HIRBuilder {
public:
private:
    ObjectPool<Edge> hir_edge_pool;
    ObjectPool<HIRBlock> hir_block_pool;
};

}  // namespace swift::runtime::ir