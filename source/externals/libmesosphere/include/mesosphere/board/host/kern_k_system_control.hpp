#pragma once
#include <mesosphere/kern_common.hpp>
#include <mesosphere/kern_k_system_control_base.hpp>

namespace ams::kern::board::host {

    class KSystemControl : public KSystemControlBase {
        public:
            /* User access. */
            static void CallSecureMonitorFromUser(ams::svc::lp64::SecureMonitorArguments *args);
    };

}