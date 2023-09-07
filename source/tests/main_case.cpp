#include "kernel/host/kernel_main.h"
#include <catch2/catch_test_macros.hpp>
#include "runtime/ir/block.h"

TEST_CASE("Test kernel-init") {
    swift::horizon::kernel::InitForKernel();
}

TEST_CASE("Test runtime-init") {
    using namespace swift::runtime::ir;
    Inst::InitializeSlabHeap(0x100);
    auto inst = Inst::Create(Imm(true), Cond::AL, Operand{Value(nullptr), Imm(true)});
    assert(inst);
    auto arg = inst->GetArg<Operand>(2);
    inst->SetArg(2, arg);
}