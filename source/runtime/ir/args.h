//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpragma-pack"
#endif

#include "runtime/common/types.h"

namespace swift::runtime::ir {

class Inst;
class Arg;

enum class ArgType : u8 { Void = 0, Value, Imm, Uniform, Cond, Flags, Operand, Lambda };
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

enum class Cond : u8 {
    EQ = 0,
    NE,
    CS,
    CC,
    MI,
    PL,
    VS,
    VC,
    HI,
    LS,
    GE,
    LT,
    GT,
    LE,
    AL,
    NV,
    HS = CS,
    LO = CC,
};

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

// 4G uniform buffer
#pragma pack(1)
class Uniform {
public:
    explicit Uniform(u32 offset, ValueType type);

    u32 GetOffset();
    ValueType GetType();

private:
    u32 offset{};
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

struct Flags {
    enum Value : u32 {
        Carry = 1 << 0,
        Overflow = 1 << 1,
        Signed = 1 << 2,
        Zero = 1 << 3,
        Negate = 1 << 4,
        All = Carry | Overflow | Signed | Zero | Negate
    };

    constexpr Flags() : value() {}
    constexpr Flags(Value flag) : value(flag) {}
    Value value;
};

enum class OperandOp : u8 { Plus = 0, Minus, Multi, LSL, LSR };

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
        Flags flags;
    } inner{};

    explicit ArgClass() : type(ArgType::Void) {}

    explicit ArgClass(const Value& v) {
        inner.value = v;
        type = ArgType::Value;
    }

    explicit ArgClass(const Imm& v) {
        inner.imm = v;
        type = ArgType::Imm;
    }

    explicit ArgClass(const OperandOp& v) {
        inner.operand = v;
        type = ArgType::Operand;
    }

    explicit ArgClass(const Uniform& v) {
        inner.uniform = v;
        type = ArgType::Uniform;
    }

    explicit ArgClass(const Lambda& v) {
        inner.lambda = v;
        type = ArgType::Lambda;
    }

    explicit ArgClass(const Cond& v) {
        inner.cond = v;
        type = ArgType::Cond;
    }

    explicit ArgClass(const Flags& v) {
        inner.flags = v;
        type = ArgType::Flags;
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

    explicit Operand(const Value& left, const Imm& right, Op op = OperandOp::Plus);

    explicit Operand(const Value& left, const Value& right, Op op = OperandOp::Plus);

    [[nodiscard]] Op GetOp() const;

private:
    Op op{OperandOp::Minus};
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
    constexpr Arg(const Flags& v) : value(v) {}
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
        } else if constexpr (std::is_same<T, Flags>::value) {
            assert(value.type == ArgType::Flags);
            return value.inner.flags;
        } else {
            assert(0);
        }
    }

private:
    ArgClass value;
};

}  // namespace swift::runtime::ir
