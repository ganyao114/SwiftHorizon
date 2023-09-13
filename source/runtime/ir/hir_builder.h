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

struct HostGPR {
    static constexpr auto INVALID = u16(-1);
    u16 id{INVALID};
};

struct HostFPR {
    static constexpr auto INVALID = u16(-1);
    u16 id{INVALID};
};

struct SpillSlot {
    static constexpr auto INVALID = u16(-1);
    u16 offset{INVALID};
};

class DataContext {
public:
    virtual u16 MaxValueCount() = 0;
};

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

#pragma pack(push, 1)
struct ValueAllocated {
    enum Type : u8 {
        NONE,
        GPR,
        FPR,
        MEM
    };
    Type type;
    union {
        HostGPR host_gpr;
        HostFPR host_fpr;
        SpillSlot spill_slot;
    };

    [[nodiscard]] bool Allocated() const {
        return type != NONE;
    }

    explicit ValueAllocated() : type(NONE) {}
};
#pragma pack(pop)

#pragma pack(push, 4)
struct HIRValue {
    Value value;
    HIRBlock *block;
    IntrusiveListNode list_node{};
    ValueAllocated allocated{};

    explicit HIRValue(u16 id, const Value& value, HIRBlock* block);
};
#pragma pack(pop, 4)

using HIRValueList = IntrusiveList<&HIRValue::list_node>;

class HIRBlock : public DataContext {
    friend class HIRFunction;

public:
    explicit HIRBlock(Block* block);

    void AddOutgoingEdge(Edge* edge);
    void AddIncomingEdge(Edge* edge);
    bool HasIncomingEdges();
    bool HasOutgoingEdges();

    auto& GetIncomingEdges() { return incoming_edges; }

    auto& GetOutgoingEdges() { return outgoing_edges; }

    u16 MaxValueCount() override;

    IntrusiveListNode list_node;

private:
    u16 order_id{};
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
                         ObjectPool<Edge>& edge_pool,
                         ObjectPool<HIRValue>& value_pool);

    template <typename... Args> Inst* AppendInst(OpCode op, const Args&... args) {
        ASSERT(current_block);
        auto inst = new Inst(op);
        inst->SetArgs(args...);
        current_block->block->AppendInst(inst);
        if (inst->HasValue()) {
            auto hir_value = hir_value_pool.Create(value_order_id++, Value{inst}, current_block);
            values.push_back(*hir_value);
        }
        return inst;
    }

    void AddBlock(Block* block);
    void AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional = false);
    void RemoveEdge(Edge* edge);
    void MergeAdjacentBlocks(HIRBlock* left, HIRBlock* right);
    bool SplitBlock(HIRBlock *new_block, HIRBlock *old_block);

    void EndBlock(Terminal terminal);

private:

    struct {
        u32 current_slot{0};
    } spill_stack{};

    Location begin;
    Location end;
    u16 block_order_id{};
    u16 value_order_id{};
    ObjectPool<Edge>& hir_edge_pool;
    ObjectPool<HIRBlock>& hir_block_pool;
    ObjectPool<HIRValue>& hir_value_pool;
    HIRBlockList blocks{};
    HIRValueList values{};
    HIRBlock* current_block{};
};

class HIRBuilder {
public:

    explicit HIRBuilder(u32 func_cap = 1);

#define INST(name, ret, ...)                                                                      \
    template <typename... Args> ret name(const Args&... args) {                                    \
        return ret{current_function->AppendInst(OpCode::name, args...)};                           \
    }
#include "ir.inc"
#undef INST

    void SetLocation(const Location &location);

    void If(const terminal::If& if_);

    void Switch(const terminal::Switch& switch_);

    void LinkBlock(const terminal::LinkBlock& switch_);

private:
    // temp objects
    ObjectPool<HIRFunction> hir_function_pool;
    ObjectPool<HIRBlock> hir_block_pool;
    ObjectPool<Edge> hir_edge_pool;
    ObjectPool<HIRValue> hir_value_pool;
    HIRFunction* current_function{};
};

}  // namespace swift::runtime::ir