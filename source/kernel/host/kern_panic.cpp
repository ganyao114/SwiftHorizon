//
// Created by 甘尧 on 2023/8/24.
//

#include "base/types.h"
#include "mesosphere/kern_k_scheduler.hpp"

namespace ams::kern {

    NORETURN NOINLINE void Panic(const char *file, int line, const char *format, ...) __attribute__((format(printf, 3, 4))) {
        abort();
    }

    NORETURN NOINLINE void Panic() {
        abort();
    }

}