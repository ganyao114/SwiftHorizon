//
// Created by 甘尧 on 2023/9/8.
//

#include <utility>
#include "hir_builder.h"

namespace swift::runtime::ir {

Edge::Edge(HIRBlock* src, HIRBlock* dest) : src_block(src), dest_block(dest) {}

HIRBlock::HIRBlock(Block* block, HIRValueMap& values, HIRPools& pools)
        : block(block), values(values), pools(pools) {}

void HIRBlock::AppendInst(Inst* inst) { block->AppendInst(inst); }

HIRValueMap& HIRBlock::GetHIRValues() { return values; }

u16 HIRBlock::GetOrderId() const { return order_id; }

void HIRBlock::AddIncomingEdge(Edge* edge) { incoming_edges.push_back(*edge); }

void HIRBlock::AddOutgoingEdge(Edge* edge) { outgoing_edges.push_back(*edge); }

bool HIRBlock::HasIncomingEdges() { return !incoming_edges.empty(); }

bool HIRBlock::HasOutgoingEdges() { return !outgoing_edges.empty(); }

Block* HIRBlock::GetBlock() { return block; }

InstList& HIRBlock::GetInstList() { return block->GetInstList(); }

u16 HIRBlock::MaxValueCount() { return 0; }
u16 HIRBlock::MaxBlockCount() { return 0; }
u16 HIRBlock::MaxLocalId() { return 0; }

HIRValue::HIRValue(u16 id, const Value& value, HIRBlock* block) : value(value), block(block) {
    if (auto def = value.Def(); def) {
        def->SetId(id);
    } else {
        ASSERT_MSG(false, "Invalid value!");
    }
}

void HIRValue::Use(Inst* inst, u8 idx) {
    auto use = block->pools.uses.Create(inst, idx);
    uses.push_back(*use);
}

void HIRValue::UnUse(Inst* inst, u8 idx) {
    auto itr = std::find_if(uses.begin(), uses.end(), [inst, idx](auto& itr) -> auto {
        return itr.inst == inst && itr.arg_idx == idx;
    });
    if (itr != uses.end()) {
        uses.erase(itr);
    }
}

HIRUse::HIRUse(Inst* inst, u8 arg_idx) : inst(inst), arg_idx(arg_idx) {}

HIRFunction::HIRFunction(Function* function,
                         const Location& begin,
                         const Location& end,
                         HIRPools& pools)
        : function(function), begin(begin), end(end), pools(pools) {
    AppendBlock(begin);
}

void HIRFunction::AppendBlock(Location start, Location end_) {
    auto hir_block = pools.blocks.Create(new Block(start), values, pools);
    hir_block->block->SetEndLocation(end_);
    hir_block->order_id = block_order_id++;
    blocks.push_back(*hir_block);
    current_block = hir_block;
}

void HIRFunction::AppendInst(Inst* inst) {
    current_block->AppendInst(inst);
    if (inst->HasValue()) {
        values.insert(*pools.values.Create(value_order_id++, Value{inst}, current_block));
    }
    for (int i = 0; i < Inst::max_args; ++i) {
        auto& arg = inst->ArgAt(i);
        if (arg.IsValue()) {
            auto value = inst->GetArg<Value>(i);
            if (auto hir_value = GetHIRValue(value); hir_value) {
                hir_value->Use(inst, i);
            }
        } else if (arg.IsLambda() && arg.Get<Lambda>().IsValue()) {
            auto value = inst->GetArg<Lambda>(i).GetValue();
            if (auto hir_value = GetHIRValue(value); hir_value) {
                hir_value->Use(inst, i);
            }
        }
    }
    switch (inst->GetOp()) {
        case OpCode::LoadLocal:
        case OpCode::StoreLocal: {
            auto local = inst->GetArg<Local>(0);
            max_local_id = std::max(local.id, max_local_id);
            break;
        }
        default:
            break;
    }
}

void HIRFunction::DestroyHIRValue(HIRValue* value) {
    values.erase(*value);
    value->block->block->DestroyInst(value->value.Def());
}

HIRBlockList& HIRFunction::GetHIRBlocks() { return blocks; }

HIRValueMap& HIRFunction::GetHIRValues() { return values; }

HIRValue* HIRFunction::GetHIRValue(const Value& value) {
    if (auto itr = values.find(value); itr != values.end()) {
        return itr.operator->();
    } else {
        return {};
    }
}

void HIRFunction::AddEdge(HIRBlock* src, HIRBlock* dest, bool conditional) {
    ASSERT(src && dest);
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
u16 HIRFunction::MaxLocalId() { return max_local_id; }

HIRPools::HIRPools(u32 func_cap)
        : functions(func_cap)
        , blocks(func_cap * 8)
        , edges(func_cap * 16)
        , values(func_cap * 256)
        , uses(func_cap * 256)
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
