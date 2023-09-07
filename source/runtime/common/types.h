//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "base/types.h"
#include "vapours.hpp"

namespace swift::runtime {

template <auto NodeMember>
using IntrusiveList = typename ams::util::IntrusiveListMemberTraits<NodeMember>::ListType;
using IntrusiveListNode = ams::util::IntrusiveListNode;

}
