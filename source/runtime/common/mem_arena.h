//
// Created by 甘尧 on 2022/1/5.
//

#pragma once

#include "types.h"

namespace swift::runtime {

class MemArena {
public:
    explicit MemArena(size_t chunk_size = 4096) : new_chunk_size{chunk_size} {
        node = &chunks.emplace_back(new_chunk_size);
    }

    template <typename T, typename... Args>
        requires std::is_constructible_v<T, Args...> && (!std::is_destructible_v<T>)
    T* Create(Args&&... args) {
        return std::construct_at(Memory<T>(), std::forward<Args>(args)...);
    }

private:
    struct NonTrivialDummy {
        NonTrivialDummy() noexcept {}
    };

    struct Chunk {
        explicit Chunk() = default;
        explicit Chunk(size_t size) : num_size{size}, storage(size) {}

        Chunk& operator=(Chunk&& rhs) noexcept {
            used_size = std::exchange(rhs.used_size, 0);
            num_size = std::exchange(rhs.num_size, 0);
            storage = std::move(rhs.storage);
            return *this;
        }

        Chunk(Chunk&& rhs) noexcept
                : used_size{std::exchange(rhs.used_size, 0)}
                , num_size{std::exchange(rhs.num_size, 0)}
                , storage{std::move(rhs.storage)} {}

        size_t used_size{};
        size_t num_size{};
        Vector<u8> storage;
    };

    template <typename T> [[nodiscard]] T* Memory() {
        Chunk* const chunk{FreeChunk(sizeof(T))};
        auto result = &chunk->storage[chunk->used_size];
        chunk->used_size += sizeof(T);
        return reinterpret_cast<T*>(result);
    }

    [[nodiscard]] Chunk* FreeChunk(size_t size) {
        if (node->used_size + size <= node->num_size) {
            return node;
        }
        node = &chunks.emplace_back(new_chunk_size);
        return node;
    }

    Chunk* node{};
    Vector<Chunk> chunks;
    size_t new_chunk_size{};
};

}  // namespace swift::runtime
