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
    explicit HIRFunction(const Location& begin,
                         const Location& end,
                         ObjectPool<HIRBlock>& block_pool,
                         ObjectPool<Edge>& edge_pool);

    template <typename... Args> Inst* AppendInst(OpCode op, const Args&... args) {
        assert(current_block);
        auto inst = new Inst(op);
        inst->SetArgs(args...);
        current_block->block->AppendInst(inst);
        return inst;
    }

    void AddBlock(Block* block);
    void AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional = false);
    void RemoveEdge(Edge* edge);
    void MergeAdjacentBlocks(HIRBlock* left, HIRBlock* right);

private:
    Location begin;
    Location end;
    ObjectPool<Edge>& hir_edge_pool;
    ObjectPool<HIRBlock>& hir_block_pool;
    HIRBlockList blocks{};
    HIRBlock* current_block{};
};

class HIRBuilder {
public:
#define INST0(name, ret, ...)                                                                      \
    ret name() { return ret{current_function->AppendInst(OpCode::name)}; }
#define INST1(name, ret, ...)                                                                      \
    template <typename... Args> ret name(const Args&... args) {                                    \
        return ret{current_function->AppendInst(OpCode::name, args...)};                           \
    }
#define INST2(name, ret, ...)                                                                      \
    template <typename... Args> ret name(const Args&... args) {                                    \
        return ret{current_function->AppendInst(OpCode::name, args...)};                           \
    }
#define INST3(name, ret, ...)                                                                      \
    template <typename... Args> ret name(const Args&... args) {                                    \
        return ret{current_function->AppendInst(OpCode::name, args...)};                           \
    }
#define INST4(name, ret, ...)                                                                      \
    template <typename... Args> ret name(const Args&... args) {                                    \
        return ret{current_function->AppendInst(OpCode::name, args...)};                           \
    }
#include "ir.inc"
#undef INST0
#undef INST1
#undef INST2
#undef INST3
#undef INST4

    void If(const terminal::If& if_);

    void Switch(const terminal::Switch& switch_);

    void LinkBlock(const terminal::LinkBlock& switch_);

private:
    ObjectPool<Edge> hir_edge_pool;
    ObjectPool<HIRBlock> hir_block_pool;
    HIRFunction* current_function{};
};

}  // namespace swift::runtime::ir