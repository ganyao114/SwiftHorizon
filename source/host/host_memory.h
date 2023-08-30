#pragma once

#include <memory>
#include "base/types.h"
#include "host/virtual_vector.h"

namespace swift {

class HostMemory {
public:
    static HostMemory CreateForUser(int handle, size_t backing_size_, size_t virtual_size_);

    static HostMemory CreateForKern(size_t backing_size_);

    ~HostMemory();

    HostMemory(const HostMemory& other) = delete;
    HostMemory& operator=(const HostMemory& other) = delete;

    HostMemory(HostMemory&& other) noexcept;
    HostMemory& operator=(HostMemory&& other) noexcept;

    void Map(size_t virtual_offset, size_t host_offset, size_t length);

    void Unmap(size_t virtual_offset, size_t length);

    void Free(size_t host_offset, size_t length);

    void Protect(size_t virtual_offset, size_t length, bool read, bool write, bool exec);

    [[nodiscard]] u8* BackingBasePointer() noexcept { return backing_base; }
    [[nodiscard]] const u8* BackingBasePointer() const noexcept { return backing_base; }

    [[nodiscard]] u8* VirtualBasePointer() noexcept { return virtual_base; }
    [[nodiscard]] const u8* VirtualBasePointer() const noexcept { return virtual_base; }

    [[nodiscard]] size_t BackingSize() noexcept { return backing_size; }
    [[nodiscard]] const size_t BackingSize() const noexcept { return backing_size; }

private:
    explicit HostMemory(size_t backing_size_);
    explicit HostMemory(int handle, size_t backing_size_, size_t virtual_size_);

    size_t backing_size{};
    size_t virtual_size{};

    class Impl;
    std::unique_ptr<Impl> impl;
    u8* backing_base{};
    u8* virtual_base{};
    size_t virtual_base_offset{};

    std::unique_ptr<swift::VirtualVector<u8>> fallback_buffer;
};

}
