//
// Created by 甘尧 on 2023/9/6.
//

#include "args.h"

namespace swift::runtime::ir {
Imm::Imm(bool value) : type{BOOL} { inner.imm_bool = value; }

Imm::Imm(u8 value) : type{U8} { inner.imm_u8 = value; }

Imm::Imm(u16 value) : type{U16} { inner.imm_u16 = value; }

Imm::Imm(u32 value) : type{U32} { inner.imm_u32 = value; }

Imm::Imm(u64 value) : type{U64} { inner.imm_u64 = value; }

Value::Value(Inst* in) : inst{in} {}

Operand::Operand(const Value& left, const Imm& right, Op op) : left(left), right(right), op(op) {}

Operand::Operand(const Value& left, const Value& right, Op op) : left(left), right(right), op(op) {}

Operand::Op Operand::GetOp() const { return op; }

Uniform::Uniform(u16 offset, Type type) : type(type), offset(offset) {}

u16 Uniform::GetOffset() { return offset; }

Type Uniform::GetType() { return type; }

}  // namespace swift::runtime::ir