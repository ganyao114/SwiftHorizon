//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/common/types.h"

namespace swift::runtime::ir {

class Inst;
class Arg;

enum class ArgType : u8 { Void = 0, Value, Imm, Uniform, Cond, Operand, Lambda };
enum class ValueType : u8 {
    VOID = 0,
    BOOL,
    U8,
    U16,
    U32,
    U64,
    U128,
    U256,
    V8,
    V16,
    V32,
    V64,
    V128,
    V256
};

enum class Cond : u8 { AL = 0 };

struct Void {};

#pragma pack(1)
class Imm {
public:
    explicit Imm(bool value);
    explicit Imm(u8 value);
    explicit Imm(u16 value);
    explicit Imm(u32 value);
    explicit Imm(u64 value);

private:
    ValueType type{ValueType::VOID};

    union {
        bool imm_bool;
        u8 imm_u8;
        u16 imm_u16;
        u32 imm_u32;
        u64 imm_u64;
    } inner{};
};

#pragma pack(1)
class Value {
public:
    constexpr Value(Inst* in = {}) : inst(in) {}

private:
    Inst* inst{};
    ValueType type{};
};

// 64K Uniform buffer
#pragma pack(1)
class Uniform {
public:
    explicit Uniform(u16 offset, ValueType type);

    u16 GetOffset();
    ValueType GetType();

private:
    u16 offset{};
    ValueType type{};
};

#pragma pack(1)
struct DataClass {
    ArgType type{ArgType::Void};
    union {
        Value value;
        Imm imm;
        Uniform uniform;
    } inner{};
};

class Lambda {
public:
    using FuncAddr = DataClass;

private:
    FuncAddr address;
};

enum OperandOp : u8 { Plus = 0, Minus };

#pragma pack(1)
struct ArgClass {
    ArgType type;
    union {
        Value value;
        Imm imm;
        Cond cond;
        OperandOp operand;
        Uniform uniform;
        Lambda lambda;
    } inner{};

    constexpr ArgClass() : type(ArgType::Void) {}

    constexpr ArgClass(const Value& v) {
        inner.value = v;
        type = ArgType::Value;
    }

    constexpr ArgClass(const Imm& v) {
        inner.imm = v;
        type = ArgType::Imm;
    }

    constexpr ArgClass(const OperandOp& v) {
        inner.operand = v;
        type = ArgType::Operand;
    }

    constexpr ArgClass(const Uniform& v) {
        inner.uniform = v;
        type = ArgType::Uniform;
    }

    constexpr ArgClass(const Lambda& v) {
        inner.lambda = v;
        type = ArgType::Lambda;
    }

    constexpr ArgClass(const Cond& v) {
        inner.cond = v;
        type = ArgType::Cond;
    }

    constexpr ArgClass& operator=(const Uniform& v) {
        inner.uniform = v;
        type = ArgType::Uniform;
        return *this;
    }

    constexpr ArgClass& operator=(const Lambda& v) {
        inner.lambda = v;
        type = ArgType::Lambda;
        return *this;
    }

    constexpr ArgClass& operator=(const Imm& v) {
        inner.imm = v;
        type = ArgType::Imm;
        return *this;
    }
};

class Operand {
    friend class Inst;

public:
    using Type = ArgClass;
    using Op = OperandOp;

    constexpr Operand() = default;

    explicit Operand(const Value& left, const Imm& right, Op op = Plus);

    explicit Operand(const Value& left, const Value& right, Op op = Plus);

    [[nodiscard]] Op GetOp() const;

private:
    Op op{Minus};
    Type left{};
    Type right{};
};

class Arg {
    friend class Inst;

public:
    constexpr Arg() : value() {}
    constexpr Arg(const Void& v) : value() {}
    constexpr Arg(const ArgClass& v) : value(v) {}
    constexpr Arg(const Value& v) : value(v) {}
    constexpr Arg(const Imm& v) : value(v) {}
    constexpr Arg(const Cond& v) : value(v) {}
    constexpr Arg(const Operand& v) : value(v.GetOp()) {}
    constexpr Arg(const Operand::Op v) : value(v) {}
    constexpr Arg(const Uniform& v) : value(v) {}
    constexpr Arg(const Lambda& v) : value(v) {}

    [[nodiscard]] constexpr bool IsImm() const { return value.type == ArgType::Imm; }

    [[nodiscard]] constexpr bool IsValue() const { return value.type == ArgType::Value; }

    [[nodiscard]] constexpr bool IsOperand() const { return value.type == ArgType::Operand; }

    template <typename T> constexpr T& Get() {
        if constexpr (std::is_same<T, Value>::value) {
            assert(value.type == ArgType::Value);
            return value.inner.value;
        } else if constexpr (std::is_same<T, Imm>::value) {
            assert(value.type == ArgType::Imm);
            return value.inner.imm;
        } else if constexpr (std::is_same<T, Uniform>::value) {
            assert(value.type == ArgType::Uniform);
            return value.inner.uniform;
        } else if constexpr (std::is_same<T, Cond>::value) {
            assert(value.type == ArgType::Cond);
            return value.inner.cond;
        } else if constexpr (std::is_same<T, OperandOp>::value) {
            assert(value.type == ArgType::Operand);
            return value.inner.operand;
        } else if constexpr (std::is_same<T, Lambda>::value) {
            assert(value.type == ArgType::Lambda);
            return value.inner.lambda;
        } else {
            assert(0);
        }
    }

private:
    ArgClass value;
};

}  // namespace swift::runtime::ir
