//
// Created by swift_gan on 2023/8/16.
//

#pragma once
#include <vapours.hpp>
#include <mesosphere/arch/host/kern_cpu_system_registers.hpp>
#include <mesosphere/arch/host/arm64_interface.hpp>
#include <mesosphere/arch/host/hypervisor/arm64_hypervisor.hpp>
#include <mesosphere/kern_select_userspace_memory_access.hpp>
#include <mesosphere/kern_k_typed_address.hpp>

namespace ams::kern::arch::host::cpu {

    constexpr inline size_t InstructionCacheLineSize = 0x40;
    constexpr inline size_t DataCacheLineSize        = 0x40;
    constexpr inline size_t NumPerformanceCounters   = 6;

    constexpr inline size_t NumCores = 4;

    constexpr inline u32 El0Aarch64PsrMask = 0xF0000000;
    constexpr inline u32 El0Aarch32PsrMask = 0xFE0FFE20;

    /* Initialization. */
    NOINLINE void InitializeInterruptThreads(s32 core_id);

    /* Helpers for managing memory state. */
    ALWAYS_INLINE void DataSynchronizationBarrier() {
    }

    ALWAYS_INLINE void DataSynchronizationBarrierInnerShareable() {
    }

    ALWAYS_INLINE void DataSynchronizationBarrierInnerShareableStore() {
    }

    ALWAYS_INLINE void DataMemoryBarrier() {
    }

    ALWAYS_INLINE void DataMemoryBarrierInnerShareable() {
    }

    ALWAYS_INLINE void DataMemoryBarrierInnerShareableStore() {
    }

    ALWAYS_INLINE void InstructionMemoryBarrier() {
    }

    ALWAYS_INLINE void EnsureInstructionConsistency() {
    }

    ALWAYS_INLINE void EnsureInstructionConsistencyFullSystem() {
    }

    ALWAYS_INLINE void Yield() {
    }

    ALWAYS_INLINE void SwitchProcess(u64 ttbr, u32 proc_id) {

    }

    /* Performance counter helpers. */
    ALWAYS_INLINE u64 GetCycleCounter() {
        return 0;
    }

    ALWAYS_INLINE u32 GetPerformanceCounter(s32 n) {
            return 0;
    }

    /* Helper for address access. */
    ALWAYS_INLINE bool GetPhysicalAddressWritable(KPhysicalAddress *out, KVirtualAddress addr, bool privileged = false) {
        return true;
    }

    ALWAYS_INLINE bool GetPhysicalAddressReadable(KPhysicalAddress *out, KVirtualAddress addr, bool privileged = false) {
        return false;
    }

    ALWAYS_INLINE bool CanAccessAtomic(KProcessAddress addr, bool privileged = false) {
        return false;
    }

    ALWAYS_INLINE void StoreDataCacheForInitArguments(const void *addr, size_t size) {
    }

    /* Synchronization helpers. */
    NOINLINE void SynchronizeAllCores();
    void SynchronizeCores(u64 core_mask);

    /* Cache management helpers. */
    void StoreCacheForInit(void *addr, size_t size);

    void FlushEntireDataCache();

    Result InvalidateDataCache(void *addr, size_t size);
    Result StoreDataCache(const void *addr, size_t size);
    Result FlushDataCache(const void *addr, size_t size);

    void InvalidateEntireInstructionCache();

    void ClearPageToZeroImpl(void *);

    ALWAYS_INLINE void ClearPageToZero(void * const page) {
    }

    ALWAYS_INLINE void InvalidateTlbByAsid(u32 asid) {
    }

    ALWAYS_INLINE void InvalidateTlbByAsidAndVa(u32 asid, KProcessAddress virt_addr) {
    }

    ALWAYS_INLINE void InvalidateEntireTlb() {
    }

    ALWAYS_INLINE void InvalidateEntireTlbDataOnly() {
    }

    ALWAYS_INLINE void InvalidateTlbByVaDataOnly(KProcessAddress virt_addr) {
    }

    ALWAYS_INLINE uintptr_t GetCurrentThreadPointerValue() {
    }

    ALWAYS_INLINE void SetCurrentThreadPointerValue(uintptr_t value) {
    }

    ALWAYS_INLINE void SetExceptionThreadStackTop(uintptr_t top) {
    }

    ALWAYS_INLINE void SwitchThreadLocalRegion(uintptr_t tlr) {
    }

}
