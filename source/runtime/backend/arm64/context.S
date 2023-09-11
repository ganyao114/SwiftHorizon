#include "marcos.h"

save_host_callee:
    stp x19, x20, [sp, #-16]!
    stp x21, x22, [sp, #-16]!
    stp x23, x24, [sp, #-16]!
    stp x25, x26, [sp, #-16]!
    stp x27, x28, [sp, #-16]!

    stp q8, q9,   [sp, #-32]!
    stp q10, q11, [sp, #-32]!
    stp q12, q13, [sp, #-32]!
    stp q14, q15, [sp, #-32]!
    ret

restore_host_callee:
    ldp q14, q15, [sp], #32
    ldp q12, q13, [sp], #32
    ldp q10, q11, [sp], #32
    ldp q8, q9,   [sp], #32

    ldp x27, x28, [sp], #16
    ldp x25, x26, [sp], #16
    ldp x23, x24, [sp], #16
    ldp x21, x22, [sp], #16
    ldp x19, x20, [sp], #16
    ret

.def a64_interpreter_entry
    stp x29, x30, [sp, #-16]!
    bl save_host_callee

    mov ctx, x0
    mov state, x1
    mov args, x2
    mov base, x3
    add vregs, ctx, #OFF_VECTOR
    // load cpsr
    ldr ipw0, [ctx, #OFF_NZCV]
    msr nzcv, ip0
    ldr ipw0, [ctx, #OFF_FPSR]
    msr fpsr, ip0
    ldr ipw0, [ctx, #OFF_FPCR]
    msr fpcr, ip0
    // load cache_ptr
    ldr cache, [state, #OFF_L2_CACHE]
    // load pt
    ldr pt, [state, #OFF_PT]
    // load rsb
    ldr rsb_ptr, [state, #OFF_RSB_PTR]
    cbnz args, go_guest

.def a64_return_dispatcher
code_dispatcher:

#define pc_index xn
#define l1_cache ip0
#define l1_index ip1
#define l1_start ip2
#define l2_cache cache
#define l2_index xd
#define l2_start xm

    // align pc
    ldr pc, [ctx, #OFF_PC]
    lsr pc_index, pc, 2

    // query l1 cache
    ldr l1_cache, [state, #OFF_L1_CACHE]
    eor l1_index, pc_index, pc_index, lsr #L1_CODE_CACHE_BITS
    and l1_index, l1_index, #L1_CODE_CACHE_HASH
    add l1_start, l1_cache, l1_index, lsl #4

1:  // l1 cache looper
    ldr l1_index, [l1_start], #0x10
    cbz l1_index, 2f
    sub l1_index, l1_index, pc
    cbnz l1_index, 1b
    ldr args, [l1_start, #-0x8]
    load_next
    go_link

2:  // query l2 cache
    eor l2_index, pc_index, pc_index, lsr #L2_CODE_CACHE_BITS
    and l2_index, l2_index, #L2_CODE_CACHE_HASH
    add l2_start, l2_cache, l2_index, lsl #4

3:  // l2 looper
    ldr l2_index, [l2_start], #0x10
    cbz l2_index, return_host
    sub l2_index, l2_index, pc
    cbnz l2_index, 3b
    ldr args, [l2_start, #-0x8]
    cbz args, return_host

    // write to l1 cache
    ldr l2_index, [l1_start, #-0x8]
    add l2_index, l2_index, #1
    cbz l2_index, go_guest // check if l1 cache is full
    stp pc, args, [l1_start, #-0x10]

#undef pc_index
#undef l1_cache
#undef l1_index
#undef l1_start
#undef l2_cache
#undef l2_index
#undef l2_start

go_guest:
    load_next
    go_link

.def a64_return_host
return_host:
    // write back rsb
    str rsb_ptr, [state, #OFF_RSB_PTR]
    // writeback cpsr
    mrs ip0, nzcv
    str ipw0, [ctx, #OFF_NZCV]
    mrs ip0, fpsr
    str ipw0, [ctx, #OFF_FPSR]
    mrs ip0, fpcr
    str ipw0, [ctx, #OFF_FPCR]
    // load excption
    ldr w0, [state, #OFF_EXCEPTION]
    str wzr, [state, #OFF_EXCEPTION]
    bl restore_host_callee
    ldp x29, x30, [sp], #16
    ret

.def a64_check_halt
    ldr w0, [state, #OFF_EXCEPTION]
    cbnz w0, return_host
    next_instr

.def a64_set_x_imm
    load_x_arg(value)
    set_x(value, arg_x)
    next_instr

.def a64_set_w_imm
    load_b_arg(ipw0)
    set_w(arg, ip0)
    next_instr

.def a64_set_d_imm
    load_x_arg(ipd0)
    set_q(ipq0, arg_x)
    next_instr

.def a64_set_pc_reg
    get_x(value, arg_x)
    str value, [ctx, #OFF_PC]
    next_instr

.def a64_set_x_value
    set_x(value, arg_x)
    next_instr

.def a64_is_zero_x
    get_x(value, arg_x)
	cmp value, xzr
	cset value, eq
    next_instr

.def a64_is_zero_w
    get_x(ip0, arg_x)
	cmp ipw0, wzr
	cset value, eq
    next_instr

.def a64_test_bit_x
    extract(wd, 0, 6)
    extract(ipw0, 6, 8)
    get_x(xd, xd)
    mov ip1, #1
    lsl ip1, ip1, ip0
    ands value, xd, ip1
	cset value, ne
    next_instr

.def a64_test_bit_w
    extract(wd, 0, 6)
    extract(ipw0, 6, 8)
    get_x(xd, xd)
    mov ipw1, #1
    lsl ipw1, ipw1, ipw0
    ands ipw2, wd, ipw1
	cset value, ne
    next_instr

.def a64_link_block
    load_x_arg(pc)
    str pc, [ctx, #OFF_PC]
    ldr args, [cache, arg_x, lsl #3]
    cbz args, return_host
    next_instr

.def a64_terminal_pop_rsb
    ldr xt, [ctx, #OFF_PC]
    ldp ipw0, ipw1, [rsb_ptr], #-16
    cbnz ipw0, 1f
    add rsb_ptr, rsb_ptr, #512
    ldp ipw0, ipw1, [rsb_ptr]
1:
    sub ipw0, wt, ipw0
    cbnz ipw0, code_dispatcher
    ldr args, [cache, ip1, lsl #3]
    cbz args, return_host
    next_instr

.def a64_terminal_pop_rsb_return_host
    ldr xt, [ctx, #OFF_PC]
    ldp ipw0, ipw1, [rsb_ptr], #-16
    cbnz ipw0, 1f
    add rsb_ptr, rsb_ptr, #512
    ldp ipw0, ipw1, [rsb_ptr]
1:
    sub ipw0, wt, ipw0
    cbnz ipw0, code_dispatcher
    ldr args, [cache, ip1, lsl #3]
    b return_host

.def a64_set_pc_imm
    load_x_arg(pc)
    str pc, [ctx, #OFF_PC]
    next_instr

.def a64_update_pc_return_dispatcher
    load_x_arg(pc)
    str pc, [ctx, #OFF_PC]
    b code_dispatcher

.def a64_update_pc_return_host
    load_x_arg(pc)
    str pc, [ctx, #OFF_PC]
    b return_host

.def a64_terminal_check_bit
	cbz value, check_bit_false
	next_instr
check_bit_false:
	add args, args, arg_x
	next_instr

.macro terminal_check_cond_def cond
.def a64_terminal_check_cond_\cond\()
	b.\cond check_cond_pass_\cond\()
	next_instr
check_cond_pass_\cond\():
	add args, args, arg_x
	next_instr
.endm

.irp cond, COND_LIST
    terminal_check_cond_def \cond
.endr

.macro def_movz shift
.def a64_movz_lsl_\shift
    extract(ipw0, 0, 16)
    extract(wd, 16, 5)
    orr value, xzr, ip0, lsl \shift
    set_x(value, xd)
	next_instr
.endm

.macro def_movk shift
.def a64_movk_lsl_\shift
    extract(wd, 16, 5)
    get_x(value, xd)
    bfi value, arg_x, \shift, 16
    set_x(value, xd)
	next_instr
.endm

.macro def_movn shift
.def a64_movn_lsl_\shift
    extract(ipw0, 0, 16)
    extract(wd, 16, 5)
    orn value, xzr, ip0, lsl \shift
    set_x(value, xd)
	next_instr
.endm

.irp shift, 0, 16, 32, 48
    def_movz \shift
    def_movk \shift
    def_movn \shift
.endr

.def a64_clrex
    str wzr, [ctx, #OFF_EXCLUSIVE_STATE]
	next_instr

.def a64_dsb
    dsb sy
    next_instr

.def a64_dmb
    dmb sy
    next_instr

.def a64_mrs_w
    extract(wd, 0, 6)
    extract(wn, 6, 16)
    ldr ipw0, [state, xn]
    set_x(ip0, xd)
    next_instr

.def a64_msr_w
    extract(wd, 0, 6)
    extract(wn, 6, 16)
    get_x(ip0, xd)
    str ipw0, [state, xn]
    next_instr

.def a64_mrs_x
    extract(wd, 0, 6)
    extract(wn, 6, 16)
    ldr ip0, [state, xn]
    set_x(ip0, xd)
    next_instr

.def a64_msr_x
    extract(wd, 0, 6)
    extract(wn, 6, 16)
    get_x(ip0, xd)
    str ip0, [state, xn]
    next_instr

.macro def_mrs, op
.def a64_mrs_\op
    mrs xd, \op
    set_x(xd, arg_x)
    next_instr
.endm

.macro def_msr, op
.def a64_msr_\op
    get_x(ip0, arg_x)
    msr \op, xd
    next_instr
.endm

.irp op, nzcv, fpcr, fpsr, cntpct_el0, cntvct_el0
    def_mrs \op
.endr

.irp op, nzcv, fpcr, fpsr
    def_msr \op
.endr

.def a64_call_host_function_0
	save_caller_regs

	load_x_arg(ip0) // func
	ldr x0, [state, #OFF_INTERFACE]

	blr ip0
	mov value, x0

	restore_caller_regs
    next_instr

.def a64_call_host_function_1
	save_caller_regs

	load_x_arg(ip0) // func
	ldr x0, [state, #OFF_INTERFACE]
	load_x_arg(x1) // arg0

	blr ip0
	mov value, x0

	restore_caller_regs
    next_instr

.def a64_call_host_function_2
	save_caller_regs

	load_x_arg(ip0) // func
	ldr x0, [state, #OFF_INTERFACE]
	load_x_arg(x1) // arg0
	load_x_arg(x2) // arg1

	blr ip0
	mov value, x0

	restore_caller_regs
    next_instr

.def a64_call_host_function_3
	save_caller_regs

	load_x_arg(ip0) // func
	ldr x0, [state, #OFF_INTERFACE]
	load_x_arg(x1) // arg0
	load_x_arg(x2) // arg1
	load_x_arg(x3) // arg2

	blr ip0
	mov value, x0

	restore_caller_regs
    next_instr

.def a64_call_host_function_4
	save_caller_regs

	load_x_arg(ip0) // func
	ldr x0, [state, #OFF_INTERFACE]
	load_x_arg(x1) // arg0
	load_x_arg(x2) // arg1
	load_x_arg(x3) // arg2
	load_x_arg(x4) // arg3

	blr ip0
	mov value, x0

	restore_caller_regs
    next_instr

.def a64_set_return
	load_x_arg(xt) // set lr
    str xt, [ctx, #240]
    ldr ip0, [rsb_ptr]
    cbnz ip0, 1f
    sub rsb_ptr, rsb_ptr, #512
1:
    stp wt, arg, [rsb_ptr, #8]!
    next_instr

.def a64_cfinv
    mrs ip0, nzcv
    eor ip0, ip0, 0x20000000
    msr nzcv, ip0
    next_instr


