//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "args.h"
#include "runtime/common/slab_alloc.h"

namespace swift::runtime::ir {

class Block;
class Inst;

template <typename T>
concept InstAllocator = requires(T allocator, Inst* inst) {
    { allocator.New() } -> std::convertible_to<Inst*>;
    { allocator.Delete(inst) };
};

class Inst : public SlabObject<Inst, true> {
    friend class Block;
public:
    static constexpr auto max_args = 6;

    template<typename... Args>
    void SetArgs(const Args&... args) {
        constexpr auto arg_count = sizeof...(args);
        static_assert(arg_count <= max_args);
        int index{};
        auto arg_index = [&] (const Arg& arg) -> int {
            auto res = index;
            if (arg.IsOperand()) {
                index += 3;
            } else {
                index++;
            }
            return res;
        };
        (SetArg(arg_index(args), args), ...);
    }

    template<InstAllocator Allocator, typename... Args>
    static Inst *Create(Allocator &allocator, const Args&... args) {
        auto inst = allocator.New();
        inst->SetArgs(args...);
        return inst;
    }

    template<typename... Args>
    static Inst *Create(const Args&... args) {
        auto inst = new Inst();
        inst->SetArgs(args...);
        return inst;
    }

    explicit Inst() = default;

    Arg& ArgAt(int index);
    void SetArg(int index, const Arg &arg);
    void SetArg(int index, const Void &arg);
    void SetArg(int index, const Value &arg);
    void SetArg(int index, const Imm &arg);
    void SetArg(int index, const Cond &arg);
    void SetArg(int index, const Operand::Op &arg);
    void SetArg(int index, const Uniform &arg);
    void SetArg(int index, const Lambda &arg);
    void SetArg(int index, const Operand &arg);

    template <typename T> T GetArg(int index) {
        if constexpr (std::is_same<T, Operand>::value) {
            assert(arguments[index].IsOperand());
            Operand operand{};
            operand.op = arguments[index++].Get<Operand::Op>();
            std::visit<void>([&](auto left) { operand.left = left; }, arguments[index++].value);
            std::visit<void>([&](auto right) { operand.right = right; }, arguments[index++].value);
            return operand;
        } else {
            return arguments[index].Get<T>();
        }
    }

private:
    IntrusiveListNode list_node;
    std::array<Arg, max_args> arguments{};
    u16 num_use{};
};

}