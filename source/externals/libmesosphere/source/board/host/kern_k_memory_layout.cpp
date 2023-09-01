//
// Created by 甘尧 on 2023/8/31.
//

#include <mesosphere/kern_common.hpp>
#include <mesosphere/kern_k_typed_address.hpp>

namespace ams::kern {

    size_t KernelVirtualAddressSpaceWidth;
    size_t KernelPhysicalAddressSpaceWidth;

    size_t KernelVirtualAddressSpaceBase;
    size_t KernelVirtualAddressSpaceEnd;
    size_t KernelVirtualAddressSpaceLast;
    size_t KernelVirtualAddressSpaceSize;

    size_t KernelPhysicalAddressSpaceBase;
    size_t KernelPhysicalAddressSpaceEnd;
    size_t KernelPhysicalAddressSpaceLast;
    size_t KernelPhysicalAddressSpaceSize;

    size_t KernelPageTableHeapSize;
    size_t KernelInitialPageHeapSize;

    size_t KernelSlabHeapDataSize;
    size_t KernelSlabHeapSize;

    size_t KernelResourceSize;

}
