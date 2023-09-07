//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/common/types.h"

namespace swift::runtime::ir {

class Inst;
class Arg;

enum Type : u8 { VOID = 0, BOOL, U8, U16, U32, U64, U128, U256, V8, V16, V32, V64, V128, V256 };

enum class Cond : u8 { AL = 0 };

struct Void {};

class Imm {
public:
    explicit Imm(bool value);
    explicit Imm(u8 value);
    explicit Imm(u16 value);
    explicit Imm(u32 value);
    explicit Imm(u64 value);

private:
    Type type;

    union {
        bool imm_bool;
        u8 imm_u8;
        u16 imm_u16;
        u32 imm_u32;
        u64 imm_u64;
    } inner{};
};

class Value {
public:
    explicit Value(Inst* in);

private:
    Type type;
    Inst* inst;
};

// 64K Uniform buffer
class Uniform {
public:
    explicit Uniform(u16 offset, Type type);

    u16 GetOffset();
    Type GetType();

private:
    Type type;
    u16 offset;
};

class Lambda {
public:
    using FuncAddr = std::variant<Value, Imm, Uniform>;

private:
    FuncAddr address;
};

enum OperandOp : u8 { Plus = 0, Minus };

using ArgType = std::variant<Void, Value, Imm, Cond, OperandOp, Uniform, Lambda>;

class Operand {
    friend class Inst;

public:
    using Type = ArgType;
    using Op = OperandOp;

    constexpr Operand() = default;

    explicit Operand(const Value& left, const Imm& right, Op op = Plus);

    explicit Operand(const Value& left, const Value& right, Op op = Plus);

    [[nodiscard]] Op GetOp() const;

private:
    Op op{Minus};
    Type left{Void{}};
    Type right{Void{}};
};

class Arg {
    friend class Inst;
public:
    constexpr Arg() : value(Void{}) {}
    constexpr Arg(const Void& v) : value(v) {}
    constexpr Arg(const Value& v) : value(v) {}
    constexpr Arg(const Imm& v) : value(v) {}
    constexpr Arg(const Cond& v) : value(v) {}
    constexpr Arg(const Operand& v) : value(v.GetOp()) {}
    constexpr Arg(const Operand::Op v) : value(v) {}
    constexpr Arg(const Uniform& v) : value(v) {}
    constexpr Arg(const Lambda& v) : value(v) {}

    [[nodiscard]] constexpr bool IsImm() const { return std::holds_alternative<Imm>(value); }

    [[nodiscard]] constexpr bool IsValue() const { return std::holds_alternative<Value>(value); }

    [[nodiscard]] constexpr bool IsOperand() const {
        return std::holds_alternative<Operand::Op>(value);
    }

    template <typename T>
    constexpr T &Get() {
        return std::get<T>(value);
    }

private:
    ArgType value;
};

}  // namespace swift::runtime::ir
