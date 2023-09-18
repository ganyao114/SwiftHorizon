//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "runtime/ir/block.h"

namespace swift::runtime::ir {

class Function : public SlabObject<Function, true> {
public:

    explicit Function() = default;

    explicit Function(const Location &location) : location(location) {}

    union {
        NonTriviallyDummy dummy{};
        IntrusiveMapNode map_node;
        IntrusiveListNode list_node;
    };
private:
    Location location;
    BlockMap blocks{};
};

using FunctionList = IntrusiveList<&Function::list_node>;
using FunctionMap = IntrusiveMap<&Function::map_node>;

}  // namespace swift::runtime::ir