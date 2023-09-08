#include "kernel/host/kernel_main.h"
#include <catch2/catch_test_macros.hpp>
#include "runtime/ir/block.h"

TEST_CASE("Test kernel-init") {
    swift::horizon::kernel::InitForKernel();
}

TEST_CASE("Test runtime-init") {
    using namespace swift::runtime::ir;
    Inst::InitializeSlabHeap(0x100000);
    Block::InitializeSlabHeap(0x10000);
    const auto &meta = GetIRMetaInfo(OpCode::ZeroExtend);
    auto inst = Inst::Create(OpCode::ZeroExtend, Imm(true), Cond::AL, Operand{Value(nullptr), Imm(true)});
    assert(inst);
    auto arg = inst->GetArg<Operand>(2);
    inst->SetArg(2, arg);
}