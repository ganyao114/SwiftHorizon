//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "base/types.h"
#include "vapours.hpp"
#include <boost/intrusive/slist.hpp>

namespace swift::runtime {

template <auto NodeMember>
using IntrusiveList = typename ams::util::IntrusiveListMemberTraits<NodeMember>::ListType;
using IntrusiveListNode = ams::util::IntrusiveListNode;

using SingleIntrusiveListNode = boost::intrusive::slist_member_hook<>;
template<class Parent, SingleIntrusiveListNode Parent::*Member>
using SingleIntrusiveList = typename boost::intrusive::slist<Parent, boost::intrusive::member_hook<Parent, SingleIntrusiveListNode, Member>>;

}
