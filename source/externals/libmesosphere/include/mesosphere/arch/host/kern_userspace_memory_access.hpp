#pragma once
#include <mesosphere/kern_common.hpp>

namespace ams::kern::arch::host {

    void UserspaceAccessFunctionAreaBegin();

    class UserspaceAccess {
        public:
            static bool CopyMemoryFromUser(void *dst, const void *src, size_t size);
            static bool CopyMemoryFromUserAligned32Bit(void *dst, const void *src, size_t size);
            static bool CopyMemoryFromUserAligned64Bit(void *dst, const void *src, size_t size);
            static bool CopyMemoryFromUserSize32Bit(void *dst, const void *src);
            static s32  CopyStringFromUser(void *dst, const void *src, size_t size);

            static bool CopyMemoryToUser(void *dst, const void *src, size_t size);
            static bool CopyMemoryToUserAligned32Bit(void *dst, const void *src, size_t size);
            static bool CopyMemoryToUserAligned64Bit(void *dst, const void *src, size_t size);
            static bool CopyMemoryToUserSize32Bit(void *dst, const void *src);
            static s32  CopyStringToUser(void *dst, const void *src, size_t size);

            static bool ClearMemory(void *dst, size_t size);
            static bool ClearMemoryAligned32Bit(void *dst, size_t size);
            static bool ClearMemoryAligned64Bit(void *dst, size_t size);
            static bool ClearMemorySize32Bit(void *dst);

            static bool UpdateLockAtomic(u32 *out, u32 *address, u32 if_zero, u32 new_orr_mask);
            static bool UpdateIfEqualAtomic(s32 *out, s32 *address, s32 compare_value, s32 new_value);
            static bool DecrementIfLessThanAtomic(s32 *out, s32 *address, s32 compare);

            static bool StoreDataCache(uintptr_t start, uintptr_t end);
            static bool FlushDataCache(uintptr_t start, uintptr_t end);
            static bool InvalidateDataCache(uintptr_t start, uintptr_t end);

            static bool ReadIoMemory32Bit(void *dst, const void *src, size_t size);
            static bool ReadIoMemory16Bit(void *dst, const void *src, size_t size);
            static bool ReadIoMemory8Bit(void *dst, const void *src, size_t size);
            static bool WriteIoMemory32Bit(void *dst, const void *src, size_t size);
            static bool WriteIoMemory16Bit(void *dst, const void *src, size_t size);
            static bool WriteIoMemory8Bit(void *dst, const void *src, size_t size);
    };


    void UserspaceAccessFunctionAreaEnd();

}