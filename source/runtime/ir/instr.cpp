//
// Created by 甘尧 on 2023/9/6.
//

#include "instr.h"

namespace swift::runtime::ir {

void Inst::SetArg(int index, const Arg& arg) { arguments[index] = arg; }

Arg& Inst::ArgAt(int index) { return arguments[index]; }

void Inst::SetArg(int index, const Void& arg) { arguments[index] = arg; }

void Inst::SetArg(int index, const Value& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Imm& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Cond& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Flags& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Uniform& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Lambda& arg) { arguments[index] = arg; }
void Inst::SetArg(int index, const Operand::Op &arg) { arguments[index] = arg; }

void Inst::SetArg(int index, const Operand& arg) {
    arguments[index++] = arg;
    arguments[index++] = arg.left;
    arguments[index++] = arg.right;
}

}  // namespace swift::runtime::ir