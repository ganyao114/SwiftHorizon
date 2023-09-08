//
// Created by 甘尧 on 2023/9/8.
//


#include "instr.h"

namespace swift::runtime::ir {

constexpr ArgType Void = ArgType::Void;
constexpr ArgType Value = ArgType::Value;
constexpr ArgType Imm = ArgType::Imm;
constexpr ArgType Uniform = ArgType::Uniform;
constexpr ArgType Local = ArgType::Local;
constexpr ArgType Cond = ArgType::Cond;
constexpr ArgType Flags = ArgType::Flags;
constexpr ArgType Operand = ArgType::Operand;
constexpr ArgType Lambda = ArgType::Lambda;

static const std::array ir_meta_infos {
#define INST0(name, ret, ...) IRMeta{OpCode::name, #name, ret, {__VA_ARGS__}},
#define INST1(name, ret, ...) IRMeta{OpCode::name, #name, ret, {__VA_ARGS__}},
#define INST2(name, ret, ...) IRMeta{OpCode::name, #name, ret, {__VA_ARGS__}},
#define INST3(name, ret, ...) IRMeta{OpCode::name, #name, ret, {__VA_ARGS__}},
#define INST4(name, ret, ...) IRMeta{OpCode::name, #name, ret, {__VA_ARGS__}},
#include "ir.inc"
#undef INST0
#undef INST1
#undef INST2
#undef INST3
#undef INST4
};

const IRMeta &GetIRMetaInfo(OpCode op_code) {
    assert(op_code != OpCode::Void);
    return ir_meta_infos[static_cast<u8>(op_code) - 1];
}

}
