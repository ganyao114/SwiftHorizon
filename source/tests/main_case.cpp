#include <catch2/catch_test_macros.hpp>
#include "kernel/host/kernel_main.h"
#include "runtime/ir/hir_builder.h"
#include "runtime/ir/opts/cfg_analysis_pass.h"
#include "runtime/ir/opts/local_elimination_pass.h"
#include "runtime/backend/mem_map.h"

TEST_CASE("Test kernel-init") { swift::horizon::kernel::InitForKernel(); }

TEST_CASE("Test runtime-init") {
    using namespace swift::runtime::backend;
    using namespace swift::runtime::ir;
    Inst::InitializeSlabHeap(0x100000);
    Block::InitializeSlabHeap(0x10000);
    Function::InitializeSlabHeap(0x2000);
    HIRBuilder hir_builder{1};
    hir_builder.AppendFunction(Location{0}, Location{0x10});
    Local local_arg1{
            .id = 0,
            .type = ValueType::U32,
    };
    Local local_arg2{
            .id = 1,
            .type = ValueType::U32,
    };
    auto const1 = hir_builder.LoadImm(Imm(UINT32_MAX));
    hir_builder.StoreLocal(local_arg1, const1);
    auto local1 = hir_builder.LoadLocal(local_arg1);
    hir_builder.StoreLocal(local_arg2, local1);
    auto local2 = hir_builder.LoadLocal(local_arg2);
    hir_builder.Return();
    CFGAnalysisPass::Run(&hir_builder);
    LocalEliminationPass::Run(&hir_builder);
    assert(local2.Defined());

    MemMap mem_arena{0x100000, true};

    auto res = mem_arena.Map(0x100000, 0, MemMap::ReadExe, false);
    ASSERT(res);

}