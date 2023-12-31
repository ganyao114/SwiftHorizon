//
// Created by 甘尧 on 2022/1/5.
//

#pragma once

#include "types.h"

namespace swift::runtime {

template <typename T, bool destruct = false>
    requires std::is_destructible_v<T> || (!destruct)
class ObjectPool {
public:
    explicit ObjectPool(size_t chunk_size = 64) : new_chunk_size{chunk_size * sizeof(T)} {
        node = &chunks.emplace_back(new_chunk_size);
    }

    template <typename... Args>
        requires std::is_constructible_v<T, Args...>
    T* Create(Args&&... args) {
        return std::construct_at(Memory(), std::forward<Args>(args)...);
    }

    void ReleaseContents() {
        if (chunks.empty()) {
            return;
        }
        Chunk& root{chunks.front()};
        if (root.used_objects == root.num_objects) {
            // Root chunk has been filled, squash allocations into it
            const size_t total_objects{root.num_objects + new_chunk_size * (chunks.size() - 1)};
            chunks.clear();
            chunks.emplace_back(total_objects);
        } else {
            root.Release();
            chunks.resize(1);
        }
        chunks.shrink_to_fit();
        node = &chunks.front();
    }

private:
    struct NonTrivialDummy {
        NonTrivialDummy() noexcept {}
    };

    union Storage {
        Storage() noexcept {}
        ~Storage() noexcept {}

        NonTrivialDummy dummy{};
        T object;
    };

    struct Chunk {
        explicit Chunk() = default;
        explicit Chunk(size_t size)
                : num_objects{size}, storage{std::make_unique<Storage[]>(size)} {}

        Chunk& operator=(Chunk&& rhs) noexcept {
            Release();
            used_objects = std::exchange(rhs.used_objects, 0);
            num_objects = std::exchange(rhs.num_objects, 0);
            storage = std::move(rhs.storage);
        }

        Chunk(Chunk&& rhs) noexcept
                : used_objects{std::exchange(rhs.used_objects, 0)}
                , num_objects{std::exchange(rhs.num_objects, 0)}
                , storage{std::move(rhs.storage)} {}

        ~Chunk() {
            if constexpr (destruct) {
                Release();
            }
        }

        void Release() {
            std::destroy_n(storage.get(), used_objects);
            used_objects = 0;
        }

        size_t used_objects{};
        size_t num_objects{};
        std::unique_ptr<Storage[]> storage;
    };

    [[nodiscard]] T* Memory() {
        Chunk* const chunk{FreeChunk()};
        return &chunk->storage[chunk->used_objects++].object;
    }

    [[nodiscard]] Chunk* FreeChunk() {
        if (node->used_objects != node->num_objects) {
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
