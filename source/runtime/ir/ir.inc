// Read Write
INST1(LoadLocal, Value, Local)
INST2(StoreLocal, Void, Local, Value)
INST1(LoadUniform, Value, Uniform)
INST2(StoreUniform, Void, Uniform, Value)
INST1(LoadMemory, Value, Operand)
INST2(StoreMemory, Void, Operand, Value)
INST1(LoadMemoryTSO, Value, Operand)
INST2(StoreMemoryTSO, Void, Operand, Value)
INST3(CompareAndSwap, Value, Operand, Value, Value)

// Help
INST4(CallLambda, Value, Lambda, Value, Value, Value)

// Imm
INST1(LoadImm, Value, Imm)

// Help
INST1(PushRSB, Void, Value)
INST0(PopRSB, Void)

// Calculator
INST2(AddImm, Value, Value, Imm)
INST2(AddValue, Value, Value, Value)
INST2(SubImm, Value, Value, Imm)
INST2(SubValue, Value, Value, Value)
// With Carry
INST2(AdcImm, Value, Value, Imm)
INST2(AdcValue, Value, Value, Value)
INST2(SbbImm, Value, Value, Imm)
INST2(SbbValue, Value, Value, Value)
// Mul & Div
INST2(MulImm, Value, Value, Imm)
INST2(MulValue, Value, Value, Value)
INST2(DivImm, Value, Value, Imm)
INST2(DivValue, Value, Value, Value)
INST2(ZeroExtend, Value, Value, Imm)
INST2(SignExtend, Value, Value, Imm)
// And & Or
INST2(AndImm, Value, Value, Imm)
INST2(OrImm, Value, Value, Imm)
INST2(AndValue, Value, Value, Value)
INST2(OrValue, Value, Value, Value)
INST2(XorValue, Value, Value, Value)
// Get flag from result
INST1(GetCarry, Value, Value)
INST1(GetOverFlow, Value, Value)
INST1(GetZero, Value, Value)
INST1(GetNegate, Value, Value)
INST1(GetNegZero, Value, Value)
INST1(GetParity, Value, Value)
INST1(GetNZCV, Value, Value)
INST1(GetAllFlags, Value, Value)
// Shift
// LogicalShiftLeft
INST2(Lsl, Value, Value, Imm)
// LogicalShiftRight
INST2(Lsr, Value, Value, Imm)
// ArithmeticShiftRight
INST2(Asr, Value, Value, Imm)
// RotateRight
INST2(Rr, Value, Value, Imm)
// Bit
INST3(BitExtract, Value, Value, Imm, Imm)
INST4(BitInsert, Value, Value, Value, Imm, Imm)
INST3(BitClear, Value, Value, Imm, Imm)
INST2(TestBit, Value, Value, Imm)


