//
// Created by 甘尧 on 2023/8/18.
//

#include "base/types.h"
#include "mesosphere/kern_k_scheduler.hpp"

namespace ams::kern {

    void KScheduler::ScheduleImpl() {
        this->m_state.needs_scheduling.Store<std::memory_order_release>(false);
        auto interrupt_task_runnable = this->m_state.interrupt_task_runnable;
        if (interrupt_task_runnable) {

        }
    }

}