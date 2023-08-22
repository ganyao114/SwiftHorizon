//
// Created by swift_gan on 2023/8/16.
//

#include <mesosphere.hpp>
#include "kern_debug_log_impl.hpp"

namespace ams::kern {

#if defined(MESOSPHERE_DEBUG_LOG_USE_STDOUT)

    bool KDebugLogImpl::Initialize() {
        return true;
    }

    void KDebugLogImpl::PutChar(char c) {
        /* TODO */
        AMS_UNUSED(c);
    }

    void KDebugLogImpl::Flush() {
        /* ... */
    }

    void KDebugLogImpl::Save() {
        /* ... */
    }

    void KDebugLogImpl::Restore() {
        /* ... */
    }

#endif

}
