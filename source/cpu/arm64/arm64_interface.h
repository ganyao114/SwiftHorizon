//
// Created by 甘尧 on 2023/8/18.
//

#pragma once

#include "base/types.h"

namespace swift::horizon::cpu::arm64 {

    class ARM64Interface {
    public:

        explicit ARM64Interface(s32 core_id) : core_id(core_id) {}

        virtual void SetPC(u64 addr) = 0;

        virtual u64 GetPC() const = 0;

        virtual u64 GetReg(int index) const = 0;

        virtual void SetReg(int index, u64 value) = 0;

        virtual void Run() = 0;

        virtual u128 GetVectorReg(int index) const = 0;

        virtual void SetVectorReg(int index, u128 value) = 0;

    private:
        s32 core_id;
    };

}
