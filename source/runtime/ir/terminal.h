//
// Created by 甘尧 on 2023/9/6.
//

#pragma once

#include "boost/variant.hpp"
#include "runtime/ir/args.h"
#include "runtime/ir/location.h"

namespace swift::runtime::ir {

namespace terminal {

struct Invalid {};
struct ReturnToDispatch {};

struct LinkBlock {
    explicit LinkBlock(const Location& next_)
            : next(next_) {}
    Location next;
};

struct LinkBlockFast {
    explicit LinkBlockFast(const Location& next_)
            : next(next_) {}
    Location next;
};

struct PopRSBHint {};

struct If;
struct CheckBit;
struct CheckHalt;

using Terminal = boost::variant<
        Invalid,
        ReturnToDispatch,
        LinkBlock,
        LinkBlockFast,
        PopRSBHint,
        boost::recursive_wrapper<If>,
        boost::recursive_wrapper<CheckBit>,
        boost::recursive_wrapper<CheckHalt>>;

struct If {
    If(Cond if_, Terminal then_, Terminal else_)
            : if_(if_), then_(std::move(then_)), else_(std::move(else_)) {}
    Cond if_;
    Terminal then_;
    Terminal else_;
};

struct CheckBit {
    CheckBit(Terminal then_, Terminal else_)
            : then_(std::move(then_)), else_(std::move(else_)) {}
    Terminal then_;
    Terminal else_;
};

struct CheckHalt {
    explicit CheckHalt(Terminal else_)
            : else_(std::move(else_)) {}
    Terminal else_;
};

}

using terminal::Terminal;

}  // namespace swift::runtime::ir