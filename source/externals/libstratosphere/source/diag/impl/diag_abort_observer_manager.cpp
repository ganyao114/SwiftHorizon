/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "diag_abort_observer_manager.hpp"

namespace ams::diag::impl {

    AbortObserverManager *GetAbortObserverManager() {
        AMS_FUNCTION_LOCAL_STATIC(AbortObserverManager, s_manager);

        return std::addressof(s_manager);
    }

    SdkAbortObserverManager *GetSdkAbortObserverManager() {
        AMS_FUNCTION_LOCAL_STATIC(SdkAbortObserverManager, s_manager);

        return std::addressof(s_manager);
    }

}
