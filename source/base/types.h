#pragma once

#include <cstdint>
#include <string>

namespace swift {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

#ifdef __APPLE__
typedef size_t u64;     ///<  64-bit unsigned integer.
#else
typedef uint64_t u64;     ///<  64-bit unsigned integer.
#endif

using u128 = struct {
    u64 hi;
    u64 lo;
};

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

}  // namespace tapsdk