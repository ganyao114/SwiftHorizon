#include <mesosphere.hpp>

namespace ams::kern::arch::host {

    bool UserspaceAccess::ClearMemory(void *dst, size_t size) {
        return false;
    }

    bool UserspaceAccess::ClearMemoryAligned32Bit(void *dst, size_t size) {
        return false;
    }

    bool UserspaceAccess::ClearMemoryAligned64Bit(void *dst, size_t size) {
        return false;
    }

    bool UserspaceAccess::CopyMemoryFromUser(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::CopyMemoryFromUserAligned32Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::CopyMemoryToUserAligned64Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::CopyMemoryToUser(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::CopyMemoryToUserAligned32Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::FlushDataCache(uintptr_t start, uintptr_t end) {
        return false;
    }

    bool UserspaceAccess::StoreDataCache(uintptr_t start, uintptr_t end) {
        return false;
    }

    bool UserspaceAccess::ReadIoMemory8Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::ReadIoMemory16Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::ReadIoMemory32Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::WriteIoMemory8Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::WriteIoMemory16Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::WriteIoMemory32Bit(void *dst, const void *src, size_t size) {
        return false;
    }

    bool UserspaceAccess::InvalidateDataCache(uintptr_t start, uintptr_t end) {
        return false;
    }

}