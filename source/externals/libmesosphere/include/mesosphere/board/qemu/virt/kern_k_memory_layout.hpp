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

    constexpr inline size_t MainMemorySize    = 4_GB;
    constexpr inline size_t MainMemorySizeMax = 8_GB;

    constexpr size_t KernelAslrAlignment = 2_MB;
    constexpr size_t KernelVirtualAddressSpaceWidth  = size_t(1ul) << 39ul;
    constexpr size_t KernelPhysicalAddressSpaceWidth = size_t(1ul) << 48ul;

    constexpr size_t KernelVirtualAddressSpaceBase  = 0ul - KernelVirtualAddressSpaceWidth;
    constexpr size_t KernelVirtualAddressSpaceEnd   = KernelVirtualAddressSpaceBase + (KernelVirtualAddressSpaceWidth - KernelAslrAlignment);
    constexpr size_t KernelVirtualAddressSpaceLast  = KernelVirtualAddressSpaceEnd - 1ul;
    constexpr size_t KernelVirtualAddressSpaceSize  = KernelVirtualAddressSpaceEnd - KernelVirtualAddressSpaceBase;

    constexpr size_t KernelPhysicalAddressSpaceBase = 0ul;
    constexpr size_t KernelPhysicalAddressSpaceEnd  = KernelPhysicalAddressSpaceBase + KernelPhysicalAddressSpaceWidth;
    constexpr size_t KernelPhysicalAddressSpaceLast = KernelPhysicalAddressSpaceEnd - 1ul;
    constexpr size_t KernelPhysicalAddressSpaceSize = KernelPhysicalAddressSpaceEnd - KernelPhysicalAddressSpaceBase;

    constexpr size_t KernelPageTableHeapSize    = init::KInitialPageTable::GetMaximumOverheadSize(kern::MainMemorySizeMax);
    constexpr size_t KernelInitialPageHeapSize  = 128_KB;

    constexpr size_t KernelSlabHeapDataSize           = 5_MB;
    constexpr size_t KernelSlabHeapGapsSizeMax        = 2_MB - 64_KB;
    constexpr size_t KernelSlabHeapSize               = KernelSlabHeapDataSize + KernelSlabHeapGapsSizeMax;

    /* NOTE: This is calculated from KThread slab counts, assuming KThread size <= 0x800. */
    constexpr size_t KernelPageBufferHeapSize         = 0x3E0000;
    constexpr size_t KernelSlabHeapAdditionalSize     = 0x148000;
    constexpr size_t KernelPageBufferAdditionalSize   = 0x33C000;

    constexpr size_t KernelResourceSize               = KernelPageTableHeapSize + KernelInitialPageHeapSize + KernelSlabHeapSize + KernelPageBufferHeapSize;

}
