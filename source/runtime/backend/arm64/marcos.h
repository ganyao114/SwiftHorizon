//
// Created by 甘尧 on 2023/3/12.
//

#pragma once

#if defined(__APPLE__)
#define cdecl(s) _##s
#else
#define cdecl(s) s
#endif

#define ifin(thing, ...) _ifin(thing, __COUNTER__, __VA_ARGS__)
#define _ifin(thing, line, ...) __ifin(thing, line, __VA_ARGS__)
#define __ifin(thing, line, ...) irp da_op##line, __VA_ARGS__ N .ifc thing,\da_op##line
#define endifin endif N .endr

.macro .def name
    .global cdecl(\name)
    .align 4
    cdecl(\name):
.endm

#define L1_CODE_CACHE_BITS 18
#define L1_CODE_CACHE_HASH 0x3FFFF
#define L2_CODE_CACHE_BITS 23
#define L2_CODE_CACHE_HASH 0x7FFFFF

#define PAGE_BITS               12
#define PAGE_MASK               4095
#define VALID_PAGE_INDEX_BITS   20

// ctx offsets
#define OFF_PC              272
#define OFF_NZCV            280
#define OFF_VECTOR          288
#define OFF_FPCR            836
#define OFF_FPSR            832
#define OFF_EXCLUSIVE_STATE 800
#define OFF_EXCLUSIVE_ADDR  808
#define OFF_EXCLUSIVE_VALUE 816

#define OFF_L1_CACHE        0
#define OFF_L2_CACHE        8
#define OFF_PT              16
#define OFF_INTERFACE       24
#define OFF_EXCEPTION       32
#define OFF_RSB_PTR         72

// regs
#define ctx         x19
#define vregs       x23
#define args        x20
#define base        x21
#define cache       x22
#define rsb_ptr     x25
#define pt          x15
#define xn          x4
#define xd          x5
#define xt          x6
#define xt2         x7
#define xm          x8
#define xs          x9
#define xa          x10
#define wn          w4
#define wd          w5
#define wt          w6
#define wt2         w7
#define wm          w8
#define ws          w9
#define wa          w10
#define arg         w26
#define arg_x       x26
#define handle      w27
#define handle_x    x27
#define state       x28
#define value       x24
#define value_w     w24
#define ip          x11
#define pc          ip
#define ip0         x16
#define ip1         x17
#define ip2         x12
#define ip3         x13
#define ip4         x14
#define ipw         w11
#define ipw0        w16
#define ipw1        w17
#define ipw2        w12
#define ipw3        w13
#define ipw4        w14
#define paddr       x3
#define tmp0        w3
#define tmp1        w2
#define tmp2        w1
#define tmp3        w0
#define tmpx0       x3
#define tmpx1       x2
#define tmpx2       x1
#define tmpx3       x0
#define ips0        s11
#define ips1        s12
#define ips2        s13
#define ips3        s14
#define ipd0        d11
#define ipd1        d12
#define ipd2        d13
#define ipd3        d14
#define ipv0        v11
#define ipv1        v12
#define ipv2        v13
#define ipv3        v14
#define ipb0        b11
#define iph0        h11
#define iph1        h12
#define iph2        h13
#define iph3        h14
#define ipq0        q11
#define ipq1        q12

#define bt  b10
#define bd  b10

#define sm  s8
#define sn  s9
#define sd  s10
#define st  s10

#define hm  h8
#define hn  h9
#define hd  h10
#define ht  h10

#define dm  d8
#define dn  d9
#define dd  d10
#define dt  d10

#define qm  q8
#define qn  q9
#define qd  q10
#define qt  q10

// common operations
#define load_next       ldp arg, handle, [args], #8

.macro goto_next
    add handle_x, handle_x, base
    br handle_x
.endm

.macro go_link
    add handle_x, handle_x, base
    blr handle_x
.endm

.macro next_instr
    load_next
    goto_next
.endm

.macro save_caller_regs
    stp pt, x30, [sp, #-16]!
    mrs ip, nzcv
    str ipw, [ctx, #OFF_NZCV]
    mrs ip, fpsr
    str ipw, [ctx, #OFF_FPSR]
    mrs ip, fpcr
    str ipw, [ctx, #OFF_FPCR]
.endm

.macro restore_caller_regs
	ldp pt, x30, [sp], #16
    ldr ipw, [ctx, #OFF_NZCV]
    msr nzcv, ip
    ldr ipw, [ctx, #OFF_FPSR]
    msr fpsr, ip
    ldr ipw, [ctx, #OFF_FPCR]
    msr fpcr, ip
.endm

#define common_rn(x)  ubfx x, arg, 6, 6
#define common_rd(x)  ubfx x, arg, 0, 6
#define common_rt(x)  ubfx x, arg, 0, 6
#define common_vt(x)  ubfx x, arg, 0, 6
#define common_rt2(x)  ubfx x, arg, 12, 6
#define common_vt2(x)  ubfx x, arg, 12, 6
#define common_rs(x)  ubfx x, arg, 18, 6
#define common_imm20(x)  ubfx x, arg, 12, 20
#define common_imm16(x)  ubfx x, arg, 16, 16
#define common_imm14(x)  ubfx x, arg, 18, 14
#define common_imm10(x)  ubfx x, arg, 22, 10
#define scale_rm(x)  ubfx x, arg, 10, 5
#define scale_rn(x)  ubfx x, arg, 5, 5
#define scale_rd(x)  ubfx x, arg, 0, 5
#define scale_vm(x)  ubfx x, arg, 10, 5
#define scale_vn(x)  ubfx x, arg, 5, 5
#define scale_vd(x)  ubfx x, arg, 0, 5
#define scale_imm4(x)  ubfx x, arg, 16, 4
#define extract(x, p, s)  ubfx x, arg, p, s
#define extract64(x, p, s)  ubfx x, arg_x, p, s
#define set_w(v, x)   str v, [ctx, x, lsl #2]
#define get_w(v, x)   ldr v, [ctx, x, lsl #2]
#define set_x(v, x)   str v, [ctx, x, lsl #3]
#define get_x(v, x)   ldr v, [ctx, x, lsl #3]
#define set_d(v, x)   str v, [vregs, x, lsl #3]
#define get_d(v, x)   ldr v, [vregs, x, lsl #3]
#define set_q(v, x)   str v, [vregs, x, lsl #4]
#define get_q(v, x)   ldr v, [vregs, x, lsl #4]

#define load_b_arg(x)   ldrb x, [args], #1
#define load_h_arg(x)   ldrh x, [args], #2
#define load_w_arg(x)   ldr x, [args], #4
#define load_x_arg(x)   ldr x, [args], #8
#define push_x_arg(x)   str x, [args, #-8]!

#define load_guest_sp(x) ldr x, [ctx, #256]

#define COND_LIST eq, ne, cs, cc, mi, pl, vs, vc, hi, ls, ge, lt, gt, le, nv, al
#define MEM_OPS b, h, w, x, sb, sh, sw
#define LDR_OPS b, h, w, x, sb, sh, sw
#define STR_OPS b, h, w, x
#define VMEM_OPS b, h, s, d, q
#define ADDR_MODES no_offset, offset, post_index, pre_index
#define BOOL_LIST true, false