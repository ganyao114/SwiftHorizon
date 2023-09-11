//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/ir/instr.h"
#include "runtime/ir/terminal.h"

namespace swift::runtime::ir {

#pragma pack(4)
class Block : public SlabObject<Block, true> {
public:
    explicit Block() = default;

    explicit Block(Location location) : location(location) {}

    explicit Block(u32 id, Location location) : id(id), location(location) {}

    [[nodiscard]] Terminal GetTerminal() const;
    void SetTerm(Terminal term);
    [[nodiscard]] bool HasTerminal() const;

    template <typename... Args> Inst *AppendInst(OpCode op, const Args&... args) {
        auto inst = new Inst(op);
        inst->SetArgs(args...);
        AppendInst(inst);
        return inst;
    }

    void AppendInst(Inst *inst);

#define INST0(name, ret, ...) ret name(){ return ret{AppendInst(OpCode::name)}; }
#define INST1(name, ret, ...) template <typename... Args> ret name(const Args&... args){ return ret{AppendInst(OpCode::name, args...)}; }
#define INST2(name, ret, ...) template <typename... Args> ret name(const Args&... args){ return ret{AppendInst(OpCode::name, args...)}; }
#define INST3(name, ret, ...) template <typename... Args> ret name(const Args&... args){ return ret{AppendInst(OpCode::name, args...)}; }
#define INST4(name, ret, ...) template <typename... Args> ret name(const Args&... args){ return ret{AppendInst(OpCode::name, args...)}; }
#include "ir.inc"
#undef INST0
#undef INST1
#undef INST2
#undef INST3
#undef INST4

    bool operator<(const Block &rhs) const {
        return location < rhs.location;
    }

    bool operator>(const Block &rhs) const {
        return location > rhs.location;
    }

    bool operator==(const Block &rhs) const {
        return location == rhs.location;
    }

    union {
        IntrusiveMapNode map_node{};
        IntrusiveListNode list_node;
    };

private:
    u32 id{};
    Location location{0};
    InstList inst_list{};
    Terminal block_term{};
};

using BlockList = IntrusiveList<&Block::list_node>;
using BlockMap = IntrusiveMap<&Block::map_node>;

}  // namespace swift::runtime::ir