//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "base/types.h"
#include "vapours.hpp"
#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/rbtree.hpp>

namespace swift::runtime {

template <auto NodeMember>
using IntrusiveList = typename ams::util::IntrusiveListMemberTraits<NodeMember>::ListType;
using IntrusiveListNode = ams::util::IntrusiveListNode;

using SingleIntrusiveListNode = boost::intrusive::slist_member_hook<>;
template<auto Member, typename Parent = ams::util::impl::GetParentType<Member>>
using SingleIntrusiveList = typename boost::intrusive::slist<Parent, boost::intrusive::member_hook<Parent, SingleIntrusiveListNode, Member>>;

using IntrusiveMapNode = boost::intrusive::set_member_hook<>;
template<auto Member, typename Parent = ams::util::impl::GetParentType<Member>>
using IntrusiveMap = typename boost::intrusive::rbtree<Parent, boost::intrusive::member_hook<Parent, IntrusiveMapNode, Member>, boost::intrusive::compare<std::less<Parent>>>;

}
