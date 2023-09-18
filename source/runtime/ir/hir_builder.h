//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/common/mem_arena.h"
#include "runtime/common/object_pool.h"
#include "runtime/ir/function.h"
#include "runtime/ir/module.h"

namespace swift::runtime::ir {

class HIRBlock;
class HIRFunction;
class HIRBuilder;
struct HIRPools;
struct HIRValue;

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
    virtual u16 MaxBlockCount() = 0;
    virtual u16 MaxValueCount() = 0;
    virtual u16 MaxLocalId() = 0;
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
    enum Type : u8 { NONE, GPR, FPR, MEM };
    Type type;
    union {
        HostGPR host_gpr;
        HostFPR host_fpr;
        SpillSlot spill_slot;
    };

    [[nodiscard]] bool Allocated() const { return type != NONE; }

    explicit ValueAllocated() : type(NONE) {}
};
#pragma pack(pop)

#pragma pack(push, 1)
struct HIRUse {
    Inst* inst;
    u8 arg_idx;
    IntrusiveListNode list_node{};

    explicit HIRUse(Inst* inst, u8 arg_idx);
};
#pragma pack(pop)

using HIRUseList = IntrusiveList<&HIRUse::list_node>;

#pragma pack(push, 4)
struct HIRValue {
    Value value;
    HIRBlock* block;
    ValueAllocated allocated{};
    HIRUseList uses{};

    IntrusiveMapNode map_node{};

    constexpr HIRValue() : value(), block(nullptr) {};
    constexpr HIRValue(const Value& value) : value(value), block(nullptr){};
    explicit HIRValue(u16 id, const Value& value, HIRBlock* block);

    void Use(Inst* inst, u8 idx);
    void UnUse(Inst* inst, u8 idx);

    // for rbtree compare
    static NOINLINE int Compare(const HIRValue& lhs, const HIRValue& rhs) {
        if (rhs.value.Def()->Id() > lhs.value.Def()->Id()) {
            return 1;
        } else if (rhs.value.Def()->Id() < lhs.value.Def()->Id()) {
            return -1;
        } else {
            return 0;
        }
    }
};
#pragma pack(pop, 4)

struct HIRLocal {
    Local local;
    HIRValue* current_value{};
};

using HIRValueMap = IntrusiveMap<&HIRValue::map_node>;

class HIRBlock : public DataContext {
    friend class HIRFunction;
    friend class HIRValue;

public:
    explicit HIRBlock(Block* block, HIRValueMap& values, HIRPools& pools);

    void AppendInst(Inst* inst);
    HIRValueMap& GetHIRValues();
    [[nodiscard]] u16 GetOrderId() const;

    void AddOutgoingEdge(Edge* edge);
    void AddIncomingEdge(Edge* edge);
    bool HasIncomingEdges();
    bool HasOutgoingEdges();

    auto& GetIncomingEdges() { return incoming_edges; }

    auto& GetOutgoingEdges() { return outgoing_edges; }

    Block* GetBlock();
    InstList& GetInstList();

    u16 MaxBlockCount() override;
    u16 MaxValueCount() override;
    u16 MaxLocalId() override;

    IntrusiveListNode list_node;

private:
    u16 order_id{};
    Block* block;
    HIRPools& pools;
    HIRValueMap& values;
    IntrusiveList<&Edge::outgoing_edges> outgoing_edges;
    IntrusiveList<&Edge::incoming_edges> incoming_edges;
};

using HIRBlockList = IntrusiveList<&HIRBlock::list_node>;

class HIRFunction : public DataContext {
    friend class HIRBuilder;

public:
    explicit HIRFunction(Function* function,
                         const Location& begin,
                         const Location& end,
                         HIRPools& pools);

    template <typename... Args> Inst* AppendInst(OpCode op, const Args&... args) {
        ASSERT(current_block);
        auto inst = new Inst(op);
        inst->SetArgs(args...);
        AppendInst(inst);
        return inst;
    }

    void AppendBlock(Location start, Location end = {});
    void AppendInst(Inst* inst);
    void DestroyHIRValue(HIRValue* value);
    HIRBlockList& GetHIRBlocks();
    HIRValueMap& GetHIRValues();
    HIRValue* GetHIRValue(const Value& value);
    void AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional = false);
    void RemoveEdge(Edge* edge);
    void MergeAdjacentBlocks(HIRBlock* left, HIRBlock* right);
    bool SplitBlock(HIRBlock* new_block, HIRBlock* old_block);

    void EndBlock(Terminal terminal);

    u16 MaxBlockCount() override;
    u16 MaxValueCount() override;
    u16 MaxLocalId() override;

    IntrusiveListNode list_node;

private:
    struct {
        u32 current_slot{0};
    } spill_stack{};

    u16 order_id;
    u16 max_local_id{};
    Function* function;
    Location begin;
    Location end;
    u16 block_order_id{};
    u16 value_order_id{};
    HIRPools& pools;

    HIRBlockList blocks{};
    HIRValueMap values{};
    HIRBlock* current_block{};
};

using HIRFunctionList = IntrusiveList<&HIRFunction::list_node>;

struct HIRPools {
    void ReleaseContents() {
        functions.ReleaseContents();
        blocks.ReleaseContents();
        values.ReleaseContents();
        edges.ReleaseContents();
        uses.ReleaseContents();
    }

    explicit HIRPools(u32 func_cap = 1);

    MemArena mem_arena;
    ObjectPool<HIRFunction, true> functions;
    ObjectPool<HIRBlock, true> blocks;
    ObjectPool<HIRValue> values;
    ObjectPool<Edge> edges;
    ObjectPool<HIRUse> uses;
};

class HIRBuilder {
public:
    explicit HIRBuilder(u32 func_cap = 1);

    void AppendFunction(Location start, Location end = {});

    HIRFunctionList& GetHIRFunctions();

#define INST(name, ret, ...)                                                                       \
    template <typename... Args> ret name(const Args&... args) {                                    \
        return ret{current_function->AppendInst(OpCode::name, args...)};                           \
    }
#include "ir.inc"
#undef INST

    void SetLocation(Location location);

    void If(const terminal::If& if_);

    void Switch(const terminal::Switch& switch_);

    void LinkBlock(const terminal::LinkBlock& switch_);

private:
    HIRPools pools;
    HIRFunctionList hir_functions{};
    Location current_location;
    HIRFunction* current_function{};
};

}  // namespace swift::runtime::ir