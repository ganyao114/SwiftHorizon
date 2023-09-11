//
// Created by 甘尧 on 2023/9/6.
//

#include "instr.h"

namespace swift::runtime::ir {

Inst::Inst(OpCode code) : op_code(code) {}

void Inst::SetArg(int index, const Arg& arg) { arguments[index] = arg; }

Arg& Inst::ArgAt(int index) { return arguments[index]; }

void Inst::SetArg(int index, const Void& arg) { arguments[index] = arg; }

void Inst::SetArg(int index, const Value& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Imm& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Cond& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Flags& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Local& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Uniform& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Lambda& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Operand::Op& arg) { arguments[index] = arg; }

void Inst::SetArg(int index, const Operand& arg) {
    arguments[index++] = arg;
    arguments[index++] = arg.left;
    arguments[index++] = arg.right;
}

void Inst::Use(const Value& value) {
    value.Def()->num_use++;

    if (IsPseudoOperation()) {
        auto insert_point = value.Def();
        while (insert_point->next_pseudo_inst) {
            insert_point = insert_point->next_pseudo_inst;
            assert(insert_point->GetArg<Value>(0).Def() == value.Def());
        }
        insert_point->next_pseudo_inst = this;
    }
}

void Inst::UnUse(const Value& value) {
    value.Def()->num_use--;

    if (IsPseudoOperation()) {
        auto insert_point = value.Def();
        while (insert_point->next_pseudo_inst != this) {
            insert_point = insert_point->next_pseudo_inst;
            assert(insert_point->GetArg<Value>(0).Def() == value.Def());
        }
        insert_point->next_pseudo_inst = next_pseudo_inst;
        next_pseudo_inst = {};
    }
}

bool Inst::IsPseudoOperation() {
    switch (op_code) {
        case OpCode::GetCarry:
        case OpCode::GetOverFlow:
        case OpCode::GetSigned:
        case OpCode::GetNegate:
        case OpCode::GetZero:
        case OpCode::GetNZCV:
        case OpCode::GetNegZero:
            return true;
        default:
            return false;
    }
}

Inst* Inst::GetPseudoOperation(OpCode code) {
    auto pseudo_inst = next_pseudo_inst;
    while (pseudo_inst) {
        if (pseudo_inst->op_code == code) {
            assert(pseudo_inst->GetArg<Value>(0).Def() == this);
            return pseudo_inst;
        }
        pseudo_inst = pseudo_inst->next_pseudo_inst;
    }
    return {};
}

void Inst::Validate(Inst* inst) {
    assert(inst);
    assert(inst->op_code >= OpCode::Void && inst->op_code < OpCode::COUNT);
    if (inst->op_code > OpCode::Void && inst->op_code < OpCode::BASE_COUNT) {
        auto &ir_info = GetIRMetaInfo(inst->op_code);
        int inner_arg_index{};
        int arg_index{};
        while (inner_arg_index < Inst::max_args) {
            auto &inst_arg = inst->ArgAt(inner_arg_index);
            auto arg_type = ir_info.arg_types[arg_index];
            assert(inst_arg.GetType() == arg_type);
            arg_index++;
            if (arg_type == ArgType::Operand) {
                inner_arg_index += 3;
            } else {
                inner_arg_index++;
            }
        }
    } else {
        switch (inst->op_code) {
            case OpCode::SetLocation: {
                assert(inst->ArgAt(0).GetType() == ArgType::Imm);
                break;
            }
            default:
                abort();
                break;
        }
    }
}

}  // namespace swift::runtime::ir