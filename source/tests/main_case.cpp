#include "kernel/host/kernel_main.h"
#include <catch2/catch_test_macros.hpp>
#include "runtime/ir/function.h"

TEST_CASE("Test kernel-init") {
    swift::horizon::kernel::InitForKernel();
}

TEST_CASE("Test runtime-init") {
    using namespace swift::runtime::ir;
    Inst::InitializeSlabHeap(0x100000);
    Block::InitializeSlabHeap(0x10000);
    Function::InitializeSlabHeap(0x2000);
    const auto &meta = GetIRMetaInfo(OpCode::ZeroExtend);
    auto block = new Block(Location{8});
    auto value1 = block->LoadImm(Imm{(u32) 0x100});
    auto zero = block->GetZero(value1);
    auto value = block->CallLambda(Lambda{zero});
    assert(value.Def());
}