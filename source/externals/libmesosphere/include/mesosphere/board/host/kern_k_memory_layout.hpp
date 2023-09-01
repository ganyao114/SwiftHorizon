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
#pragma once
#include <mesosphere/kern_common.hpp>
#include <mesosphere/kern_k_typed_address.hpp>

namespace ams::kern {

    constexpr inline KPhysicalAddress MainMemoryAddress = 0x40000000;

    constexpr size_t MainMemorySize    = 4_GB;
    constexpr size_t MainMemorySizeMax = 8_GB;

    constexpr size_t KernelAslrAlignment = 2_MB;
    constexpr size_t KernelSlabHeapGapsSizeMax        = 2_MB - 64_KB;

    /* NOTE: This is calculated from KThread slab counts, assuming KThread size <= 0x800. */
    constexpr size_t KernelPageBufferHeapSize         = 0x3E0000;
    constexpr size_t KernelSlabHeapAdditionalSize     = 0x148000;
    constexpr size_t KernelPageBufferAdditionalSize   = 0x33C000;

    extern size_t KernelVirtualAddressSpaceWidth;
    extern size_t KernelPhysicalAddressSpaceWidth;

    extern size_t KernelVirtualAddressSpaceBase;
    extern size_t KernelVirtualAddressSpaceEnd;
    extern size_t KernelVirtualAddressSpaceLast;
    extern size_t KernelVirtualAddressSpaceSize;

    extern size_t KernelPhysicalAddressSpaceBase;
    extern size_t KernelPhysicalAddressSpaceEnd;
    extern size_t KernelPhysicalAddressSpaceLast;
    extern size_t KernelPhysicalAddressSpaceSize;

    extern size_t KernelPageTableHeapSize;
    extern size_t KernelInitialPageHeapSize;

    extern size_t KernelSlabHeapDataSize;
    extern size_t KernelSlabHeapSize;

    extern size_t KernelResourceSize;

}
