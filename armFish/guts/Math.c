
Math_Rand_i:
// in: x1 address of Prng
// out: x0 integer in [0,2^64)
        ldr  x0, [x1]
        mov  x2, 0xdd1d
        eor  x0, x0, x0, lsr 12
       movk  x2, 0x4f6c, lsl 16
        eor  x0, x0, x0, lsl 25
       movk  x2, 0xf491, lsl 32
        eor  x0, x0, x0, lsr 27
       movk  x2, 0x2545, lsl 48
        str  x0, [x1]
        mul  x0, x0, x2
        ret



Math_fabs_d_d:
       fabs  d0, d0
        ret


Math_sqrt_d_d:
      fsqrt  d0, d0
        ret

// exp, log, pow brought to us by (along with gcc):

/* @(#)fdlibm.h 1.5 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

Math_scalbn_d_di:
//scalbn(double, int):
        stp     x29, x30, [sp, -96]!
        //adrp    x3, __stack_chk_guard
        //add     x1, x3, :lo12:__stack_chk_guard
        add     x29, sp, 0
        //ldr     x2, [x1]
        //str     x2, [x29, 88]
        mov     x2, 0
        str     d0, [x29, 24]
        ldr     w2, [x29, 28]
        ubfx    x1, x2, 20, 11
        cbnz    w1, Math_scalbn_d_di.L2
        ldr     w1, [x29, 24]
        and     w2, w2, 2147483647
        orr     w2, w2, w1
        cbz     w2, Math_scalbn_d_di.L4
        ldr     d1, Math_scalbn_d_di.LC0
        mov     w1, -50000
        cmp     w0, w1
        fmul    d0, d0, d1
        ldr     d1, Math_scalbn_d_di.LC1
        str     d0, [x29, 24]
        blt     Math_scalbn_d_di.L13
        ldr     w2, [x29, 28]
        ubfx    x1, x2, 20, 11
        sub     w1, w1, #54
        b       Math_scalbn_d_di.L6
Math_scalbn_d_di.L2:
        cmp     w1, 2047
        bne     Math_scalbn_d_di.L6
        fadd    d0, d0, d0
        b       Math_scalbn_d_di.L4
Math_scalbn_d_di.L6:
        add     w1, w0, w1
        cmp     w1, 2046
        ble     Math_scalbn_d_di.L7
        ldr     x0, [x29, 24]
        ldr     d1, Math_scalbn_d_di.LC2
        str     x0, [x29, 48]
        ldr     w0, [x29, 52]
        str     d1, [x29, 40]
        and     w1, w0, -2147483648
        mov     w0, 58428
        movk    w0, 0x7e37, lsl 16
        orr     w0, w1, w0
        str     w0, [x29, 44]
        ldr     d0, [x29, 40]
        b       Math_scalbn_d_di.L13
Math_scalbn_d_di.L7:
        cmp     w1, 0
        ble     Math_scalbn_d_di.L8
        and     w2, w2, -2146435073
        orr     w1, w2, w1, lsl 20
        str     w1, [x29, 28]
        ldr     d0, [x29, 24]
        b       Math_scalbn_d_di.L4
Math_scalbn_d_di.L8:
        cmn     w1, #53
        bge     Math_scalbn_d_di.L9
        mov     w1, 50000
        cmp     w0, w1
        ldr     x0, [x29, 24]
        ble     Math_scalbn_d_di.L10
        str     x0, [x29, 64]
        ldr     d1, Math_scalbn_d_di.LC2
        ldr     w0, [x29, 68]
        and     w1, w0, -2147483648
        mov     w0, 58428
        movk    w0, 0x7e37, lsl 16
        str     d1, [x29, 56]
        orr     w0, w1, w0
        str     w0, [x29, 60]
        ldr     d0, [x29, 56]
        b       Math_scalbn_d_di.L13
Math_scalbn_d_di.L10:
        str     x0, [x29, 80]
        ldr     d1, Math_scalbn_d_di.LC1
        ldr     w0, [x29, 84]
        and     w1, w0, -2147483648
        mov     w0, 28191
        movk    w0, 0x1a5, lsl 16
        str     d1, [x29, 72]
        orr     w0, w1, w0
        str     w0, [x29, 76]
        ldr     d0, [x29, 72]
        b       Math_scalbn_d_di.L13
Math_scalbn_d_di.L9:
        add     w1, w1, 54
        and     w2, w2, -2146435073
        ldr     d1, Math_scalbn_d_di.LC3
        orr     w1, w2, w1, lsl 20
        str     w1, [x29, 28]
        ldr     d0, [x29, 24]
Math_scalbn_d_di.L13:
        fmul    d0, d0, d1
Math_scalbn_d_di.L4:
//        add     x3, x3, :lo12:__stack_chk_guard
//        ldr     x1, [x29, 88]
//        ldr     x0, [x3]
//        eor     x0, x1, x0
//        cbz     x0, Math_scalbn_d_di.L11
//        bl      __stack_chk_fail
//Math_scalbn_d_di.L11:
        ldp     x29, x30, [sp], 96
        ret
Math_scalbn_d_di.LC0:
        .word   0
        .word   1129316352
Math_scalbn_d_di.LC1:
        .word   3271095129
        .word   27618847
Math_scalbn_d_di.LC2:
        .word   2281731484
        .word   2117592124
Math_scalbn_d_di.LC3:
        .word   0
        .word   1016070144



Math_exp_d_d:
//__ieee754_exp(double):
        stp     x29, x30, [sp, -48]!
        // edit: don't check stack stuff
//        adrp    x1, __stack_chk_guard
//        add     x2, x1, :lo12:__stack_chk_guard
        add     x29, sp, 0
//        ldr     x3, [x2]
//        str     x3, [x29, 40]
        mov     x3, 0
        mov     w2, 11841
        movk    w2, 0x4086, lsl 16
        str     d0, [x29, 24]
        ldr     w4, [x29, 28]
        and     w3, w4, 2147483647
        cmp     w3, w2
        mov     x2, x1
        lsr     w5, w4, 31
        bls     Math_exp_d_d.L2
        mov     w0, 2146435071
        cmp     w3, w0
        bls     Math_exp_d_d.L3
        ldr     w0, [x29, 24]
        and     w4, w4, 1048575
        orr     w4, w4, w0
        cbz     w4, Math_exp_d_d.L4
        fadd    d0, d0, d0
        b       Math_exp_d_d.L5
Math_exp_d_d.L4:
        fmov    d1, xzr
        cmp     w5, 0
        fcsel   d0, d0, d1, eq
        b       Math_exp_d_d.L5
Math_exp_d_d.L3:
        fmov    d1, d0
        ldr     d0, Math_exp_d_d.LC0
        fcmpe   d1, d0
        ble     Math_exp_d_d.L22
        ldr     d0, Math_exp_d_d.LC1
        fmul    d0, d0, d0
        b       Math_exp_d_d.L5
Math_exp_d_d.L22:
        ldr     d2, Math_exp_d_d.LC2
        fmov    d0, xzr
        fcmpe   d1, d2
        bpl     Math_exp_d_d.L8
        b       Math_exp_d_d.L5
Math_exp_d_d.L2:
        mov     w1, 11842
        movk    w1, 0x3fd6, lsl 16
        cmp     w3, w1
        bls     Math_exp_d_d.L9
Math_exp_d_d.L8:
        mov     w0, 41649
        // edit Math_exp_d_d.LANCHOR0 is right here
        adr     x1, Math_exp_d_d.LANCHOR0
//        adrp    x1, Math_exp_d_d.LANCHOR0
        movk    w0, 0x3ff0, lsl 16
//        add     x1, x1, :lo12:Math_exp_d_d.LANCHOR0
        cmp     w3, w0
        ldr     d4, [x29, 24]
        sxtw    x0, w5
        bhi     Math_exp_d_d.L10
        ldr     d2, [x1, x0, lsl 3]
        add     x0, x1, x0, lsl 3
        fsub    d4, d4, d2
        ldr     d2, [x0, 16]
        mov     w0, 1
        sub     w0, w0, w5, lsl 1
        b       Math_exp_d_d.L11
Math_exp_d_d.L10:
        add     x0, x1, x0, lsl 3
        ldr     d1, Math_exp_d_d.LC3
        ldr     d0, [x0, 32]
        fmadd   d1, d4, d1, d0
        ldr     d0, Math_exp_d_d.LC4
        fcvtzs  w0, d1
        scvtf   d2, w0
        fmsub   d4, d2, d0, d4
        ldr     d0, Math_exp_d_d.LC5
        fmul    d2, d2, d0
Math_exp_d_d.L11:
        fsub    d0, d4, d2
        str     d0, [x29, 24]
        b       Math_exp_d_d.L12
Math_exp_d_d.L9:
        mov     w1, 1043333119
        cmp     w3, w1
        bhi     Math_exp_d_d.L19
        ldr     d1, Math_exp_d_d.LC1
        fmov    d3, 1.0e+0
        fadd    d1, d0, d1
        fcmpe   d1, d3
        ble     Math_exp_d_d.L12
        fadd    d0, d0, d3
        b       Math_exp_d_d.L5
Math_exp_d_d.L19:
        mov     w0, 0
Math_exp_d_d.L12:
        ldr     d5, [x29, 24]
        fmov    d6, 2.0e+0
        ldr     d3, Math_exp_d_d.LC6
        ldr     d0, Math_exp_d_d.LC7
        fmul    d1, d5, d5
        fmadd   d3, d1, d3, d0
        ldr     d0, Math_exp_d_d.LC8
        fmadd   d3, d1, d3, d0
        ldr     d0, Math_exp_d_d.LC9
        fmadd   d3, d1, d3, d0
        ldr     d0, Math_exp_d_d.LC10
        fmadd   d3, d1, d3, d0
        fmov    d0, 1.0e+0
        fmsub   d1, d1, d3, d5
        fmul    d3, d5, d1
        cbnz    w0, Math_exp_d_d.L14
        fsub    d1, d1, d6
        fdiv    d1, d3, d1
        fsub    d1, d1, d5
        fsub    d0, d0, d1
        b       Math_exp_d_d.L5
Math_exp_d_d.L14:
        fsub    d1, d6, d1
        cmn     w0, #1021
        fdiv    d1, d3, d1
        fsub    d2, d2, d1
        fsub    d2, d2, d4
        fsub    d0, d0, d2
        str     d0, [x29, 32]
        ldr     w1, [x29, 36]
        blt     Math_exp_d_d.L15
        add     w0, w1, w0, lsl 20
        str     w0, [x29, 36]
        ldr     d0, [x29, 32]
        b       Math_exp_d_d.L5
Math_exp_d_d.L15:
        add     w0, w0, 1000
        ldr     d1, Math_exp_d_d.LC11
        add     w0, w1, w0, lsl 20
        str     w0, [x29, 36]
        ldr     d0, [x29, 32]
        fmul    d0, d0, d1
Math_exp_d_d.L5:
        // edit don't do stack check
//        add     x1, x2, :lo12:__stack_chk_guard
//        ldr     x2, [x29, 40]
//        ldr     x0, [x1]
//        eor     x0, x2, x0
//        cbz     x0, Math_exp_d_d.L16
//        bl      __stack_chk_fail
//Math_exp_d_d.L16:
        ldp     x29, x30, [sp], 48
        ret
Math_exp_d_d.LC0:
        .word   4277811695
        .word   1082535490
Math_exp_d_d.LC1:
        .word   2281731484
        .word   2117592124
Math_exp_d_d.LC2:
        .word   3576508497
        .word   -1064875760
Math_exp_d_d.LC3:
        .word   1697350398
        .word   1073157447
Math_exp_d_d.LC4:
        .word   4276092928
        .word   1072049730
Math_exp_d_d.LC5:
        .word   897137782
        .word   1038760431
Math_exp_d_d.LC6:
        .word   1925096656
        .word   1046886249
Math_exp_d_d.LC7:
        .word   3318901745
        .word   -1094992575
Math_exp_d_d.LC8:
        .word   2938494508
        .word   1058100842
Math_exp_d_d.LC9:
        .word   381599123
        .word   -1083784852
Math_exp_d_d.LC10:
        .word   1431655742
        .word   1069897045
Math_exp_d_d.LC11:
        .word   0
        .word   24117248
Math_exp_d_d.LANCHOR0:
ln2HI:
        .word   4276092928
        .word   1072049730
        .word   4276092928
        .word   -1075433918
ln2LO:
        .word   897137782
        .word   1038760431
        .word   897137782
        .word   -1108723217
halF:
        .word   0
        .word   1071644672
        .word   0
        .word   -1075838976


Math_log_d_d:
//__ieee754_log(double):
        sub     sp, sp, #16
        mov     w0, 1048575
        str     d0, [sp, 8]
        ldr     w1, [sp, 12]
        cmp     w1, w0
        bgt     Math_log_d_d.L13
        ldr     w2, [sp, 8]
        and     w0, w1, 2147483647
        orr     w0, w0, w2
        cbnz    w0, Math_log_d_d.L3
        fmov    d1, xzr
        ldr     d0, Math_log_d_d.LC0
        b       Math_log_d_d.L17
Math_log_d_d.L3:
        tbz     w1, #31, Math_log_d_d.L5
        fmov    d1, xzr
        fsub    d0, d0, d0
Math_log_d_d.L17:
        fdiv    d0, d0, d1
        b       Math_log_d_d.L4
Math_log_d_d.L5:
        ldr     d1, Math_log_d_d.LC1
        mov     w0, -54
        fmul    d0, d0, d1
        str     d0, [sp, 8]
        ldr     w1, [sp, 12]
        b       Math_log_d_d.L2
Math_log_d_d.L13:
        mov     w0, 0
Math_log_d_d.L2:
        mov     w2, 2146435071
        cmp     w1, w2
        ble     Math_log_d_d.L6
        ldr     d0, [sp, 8]
        fadd    d0, d0, d0
        b       Math_log_d_d.L4
Math_log_d_d.L6:
        asr     w2, w1, 20
        and     w1, w1, 1048575
        sub     w2, w2, #1023
        fmov    d0, 1.0e+0
        add     w2, w2, w0
        add     w0, w1, 610304
        add     w0, w0, 3940
        and     w0, w0, 1048576
        eor     w3, w0, 1072693248
        orr     w3, w3, w1
        add     w0, w2, w0, lsr 20
        str     w3, [sp, 12]
        add     w2, w1, 2
        and     w2, w2, 1048575
        ldr     d1, [sp, 8]
        cmp     w2, 2
        fsub    d0, d1, d0
        bgt     Math_log_d_d.L7
        fcmp    d0, #0.0
        bne     Math_log_d_d.L8
        fmov    d0, xzr
        cbz     w0, Math_log_d_d.L4
        scvtf   d1, w0
        ldr     d0, Math_log_d_d.LC2
        fmul    d2, d1, d0
        ldr     d0, Math_log_d_d.LC3
        fmadd   d0, d1, d0, d2
        b       Math_log_d_d.L4
Math_log_d_d.L8:
        ldr     d2, Math_log_d_d.LC4
        fmov    d1, 5.0e-1
        fmsub   d2, d0, d2, d1
        fmul    d1, d0, d0
        fmul    d2, d2, d1
        cbnz    w0, Math_log_d_d.L9
        fsub    d0, d0, d2
        b       Math_log_d_d.L4
Math_log_d_d.L9:
        scvtf   d1, w0
        ldr     d3, Math_log_d_d.LC2
        fmsub   d2, d1, d3, d2
        fsub    d2, d2, d0
        ldr     d0, Math_log_d_d.LC3
        fnmsub  d0, d1, d0, d2
        b       Math_log_d_d.L4
Math_log_d_d.L7:
        fmov    d1, 2.0e+0
        ldr     d3, Math_log_d_d.LC5
        ldr     d6, Math_log_d_d.LC6
        mov     w3, 47185
        sub     w2, w1, #397312
        movk    w3, 0x6, lsl 16
        fadd    d1, d0, d1
        sub     w2, w2, #1146
        sub     w1, w3, w1
        scvtf   d5, w0
        orr     w1, w1, w2
        cmp     w1, 0
        fdiv    d1, d0, d1
        fmul    d4, d1, d1
        fmul    d2, d4, d4
        fmadd   d3, d2, d3, d6
        ldr     d6, Math_log_d_d.LC7
        fmadd   d3, d2, d3, d6
        ldr     d6, Math_log_d_d.LC8
        fmadd   d3, d2, d3, d6
        ldr     d6, Math_log_d_d.LC10
        fmul    d3, d3, d4
        ldr     d4, Math_log_d_d.LC9
        fmadd   d4, d2, d4, d6
        ldr     d6, Math_log_d_d.LC11
        fmadd   d4, d2, d4, d6
        fmadd   d2, d2, d4, d3
        ble     Math_log_d_d.L10
        fmov    d3, 5.0e-1
        fmul    d3, d0, d3
        fmul    d3, d3, d0
        fadd    d2, d2, d3
        cbnz    w0, Math_log_d_d.L11
        fmsub   d1, d1, d2, d3
        fsub    d0, d0, d1
        b       Math_log_d_d.L4
Math_log_d_d.L11:
        ldr     d4, Math_log_d_d.LC2
        fmul    d4, d5, d4
        fmadd   d1, d1, d2, d4
        fsub    d1, d3, d1
        b       Math_log_d_d.L18
Math_log_d_d.L10:
        fsub    d2, d0, d2
        cbnz    w0, Math_log_d_d.L12
        fmsub   d0, d1, d2, d0
        b       Math_log_d_d.L4
Math_log_d_d.L12:
        ldr     d3, Math_log_d_d.LC2
        fmul    d3, d5, d3
        fnmsub  d1, d1, d2, d3
Math_log_d_d.L18:
        fsub    d1, d1, d0
        ldr     d0, Math_log_d_d.LC3
        fnmsub  d0, d5, d0, d1
Math_log_d_d.L4:
        add     sp, sp, 16
        ret
Math_log_d_d.LC0:
        .word   0
        .word   -1018167296
Math_log_d_d.LC1:
        .word   0
        .word   1129316352
Math_log_d_d.LC2:
        .word   897137782
        .word   1038760431
Math_log_d_d.LC3:
        .word   4276092928
        .word   1072049730
Math_log_d_d.LC4:
        .word   1431655765
        .word   1070945621
Math_log_d_d.LC5:
        .word   3745403460
        .word   1069740306
Math_log_d_d.LC6:
        .word   2529887198
        .word   1070024292
Math_log_d_d.LC7:
        .word   2485293913
        .word   1070745892
Math_log_d_d.LC8:
        .word   1431655827
        .word   1071994197
Math_log_d_d.LC9:
        .word   3497576095
        .word   1069783561
Math_log_d_d.LC10:
        .word   495876271
        .word   1070363077
Math_log_d_d.LC11:
        .word   2576873988
        .word   1071225241
        ret


Math_pow_d_dd:
//__ieee754_pow(double, double):
        stp     x29, x30, [sp, -160]!
        add     x29, sp, 0
        stp     x21, x22, [sp, 32]
        // edit: don't do this stack check
//        adrp    x21, __stack_chk_guard
//        add     x0, x21, :lo12:__stack_chk_guard
        stp     x23, x24, [sp, 48]
//        ldr     x1, [x0]
//        str     x1, [x29, 152]
        mov     x1,0
        stp     d1, d0, [x29, 80]
        fmov    d0, 1.0e+0
        ldp     w0, w24, [x29, 80]
        stp     x19, x20, [sp, 16]
        str     x25, [sp, 64]
        str     d8, [sp, 72]
        and     w23, w24, 2147483647
        orr     w1, w23, w0
        cbz     w1, Math_pow_d_dd.L2
        ldr     w20, [x29, 92]
        mov     w1, 2146435072
        and     w19, w20, 2147483647
        cmp     w19, w1
        bgt     Math_pow_d_dd.L3
        ldr     w25, [x29, 88]
        ccmp    w25, 0, 4, eq
        ccmp    w23, w1, 0, eq
        bgt     Math_pow_d_dd.L3
        cmp     w23, w1
        ccmp    w0, 0, 4, eq
        beq     Math_pow_d_dd.L4
Math_pow_d_dd.L3:
        ldp     d1, d0, [x29, 80]
        fadd    d0, d0, d1
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L4:
        mov     w22, 0
        tbz     w20, #31, Math_pow_d_dd.L5
        mov     w1, 1128267775
        cmp     w23, w1
        bgt     Math_pow_d_dd.L49
        mov     w1, 1072693247
        cmp     w23, w1
        ble     Math_pow_d_dd.L5
        asr     w1, w23, 20
        sub     w2, w1, #1023
        cmp     w2, 20
        ble     Math_pow_d_dd.L6
        mov     w1, 52
        sub     w1, w1, w2
        lsr     w2, w0, w1
        lsl     w1, w2, w1
        cmp     w0, w1
        bne     Math_pow_d_dd.L5
        and     w2, w2, 1
        mov     w22, 2
        sub     w22, w22, w2
        b       Math_pow_d_dd.L5
Math_pow_d_dd.L6:
        cbnz    w0, Math_pow_d_dd.L7
        mov     w0, 20
        sub     w0, w0, w2
        asr     w1, w23, w0
        lsl     w0, w1, w0
        cmp     w23, w0
        bne     Math_pow_d_dd.L45
        and     w1, w1, 1
        mov     w22, 2
        sub     w22, w22, w1
        b       Math_pow_d_dd.L45
Math_pow_d_dd.L49:
        mov     w22, 2
Math_pow_d_dd.L5:
        cbnz    w0, Math_pow_d_dd.L7
Math_pow_d_dd.L45:
        mov     w0, 2146435072
        cmp     w23, w0
        bne     Math_pow_d_dd.L9
        mov     w0, -1072693248
        add     w0, w19, w0
        orr     w0, w0, w25
        cbnz    w0, Math_pow_d_dd.L10
        ldr     d0, [x29, 80]
        fsub    d0, d0, d0
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L10:
        mov     w0, 1072693247
        cmp     w19, w0
        ble     Math_pow_d_dd.L11
        ldr     d0, [x29, 80]
        tbz     w24, #31, Math_pow_d_dd.L2
Math_pow_d_dd.L13:
        fmov    d0, xzr
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L11:
        tbz     w24, #31, Math_pow_d_dd.L13
        ldr     d0, [x29, 80]
        fneg    d0, d0
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L9:
        mov     w0, 1072693248
        cmp     w23, w0
        bne     Math_pow_d_dd.L14
        ldr     d1, [x29, 88]
        fmov    d0, d1
        tbz     w24, #31, Math_pow_d_dd.L2
        fmov    d0, 1.0e+0
        fdiv    d0, d0, d1
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L14:
        mov     w0, 1073741824
        ldr     d0, [x29, 88]
        cmp     w24, w0
        beq     Math_pow_d_dd.L71
        mov     w0, 1071644672
        cmp     w24, w0
        ccmp    w20, 0, 1, eq
        blt     Math_pow_d_dd.L7
        ldr     d0, [x29, 88]
        bl      Math_sqrt_d_d
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L7:
        ldr     d8, [x29, 88]
        fmov    d0, d8
        bl      Math_fabs_d_d
        str     d0, [x29, 104]
        cbnz    w25, Math_pow_d_dd.L17
        and     w1, w20, 1073741823
        mov     w0, 1072693248
        cmp     w1, w0
        ccmp    w19, wzr, 4, ne
        bne     Math_pow_d_dd.L17
        tbz     w24, #31, Math_pow_d_dd.L69
        fmov    d1, 1.0e+0
        fdiv    d0, d1, d0
Math_pow_d_dd.L69:
        str     d0, [x29, 136]
        tbz     w20, #31, Math_pow_d_dd.L20
        mov     w0, -1072693248
        ldr     d0, [x29, 136]
        add     w19, w19, w0
        orr     w19, w19, w22
        cbnz    w19, Math_pow_d_dd.L21
        fsub    d0, d0, d0
        fdiv    d0, d0, d0
        b       Math_pow_d_dd.L70
Math_pow_d_dd.L21:
        cmp     w22, 1
        bne     Math_pow_d_dd.L20
        fneg    d0, d0
Math_pow_d_dd.L70:
        str     d0, [x29, 136]
Math_pow_d_dd.L20:
        ldr     d0, [x29, 136]
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L17:
        asr     w20, w20, 31
        add     w20, w20, 1
        orr     w0, w20, w22
        cbnz    w0, Math_pow_d_dd.L22
        fsub    d0, d8, d8
        fdiv    d0, d0, d0
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L22:
        sub     w22, w22, #1
        fmov    d1, 1.0e+0
        orr     w20, w22, w20
        fmov    d8, -1.0e+0
        cmp     w20, 0
        mov     w0, 1105199104
        fcsel   d8, d1, d8, ne
        cmp     w23, w0
        ble     Math_pow_d_dd.L24
        mov     w0, 1139802112
        cmp     w23, w0
        ble     Math_pow_d_dd.L25
        mov     w0, 1072693247
        cmp     w19, w0
        bgt     Math_pow_d_dd.L26
        tbz     w24, #31, Math_pow_d_dd.L13
Math_pow_d_dd.L27:
        ldr     d0, Math_pow_d_dd.LC0
Math_pow_d_dd.L71:
        fmul    d0, d0, d0
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L26:
        cmp     w24, 0
        bgt     Math_pow_d_dd.L27
        b       Math_pow_d_dd.L13
Math_pow_d_dd.L25:
        mov     w0, 65534
        movk    w0, 0x3fef, lsl 16
        cmp     w19, w0
        bgt     Math_pow_d_dd.L28
        tbz     w24, #31, Math_pow_d_dd.L73
        b       Math_pow_d_dd.L75
Math_pow_d_dd.L28:
        mov     w0, 1072693248
        cmp     w19, w0
        ble     Math_pow_d_dd.L30
        cmp     w24, 0
        bgt     Math_pow_d_dd.L75
        b       Math_pow_d_dd.L73
Math_pow_d_dd.L30:
        fsub    d0, d0, d1
        fmov    d1, 2.5e-1
        ldr     d2, Math_pow_d_dd.LC2
        fmsub   d1, d0, d1, d2
        fmov    d2, 5.0e-1
        str     d0, [x29, 128]
        fmsub   d1, d0, d1, d2
        fmul    d2, d0, d0
        fmul    d1, d1, d2
        ldr     d2, Math_pow_d_dd.LC3
        fmul    d2, d1, d2
        ldr     d1, Math_pow_d_dd.LC4
        fnmsub  d1, d0, d1, d2
        ldr     d2, Math_pow_d_dd.LC5
        fmadd   d3, d0, d2, d1
        str     d3, [x29, 120]
        str     wzr, [x29, 120]
        ldr     d3, [x29, 120]
        fmsub   d0, d0, d2, d3
        fsub    d0, d1, d0
        b       Math_pow_d_dd.L32
Math_pow_d_dd.L24:
        mov     w1, 1048575
        mov     w0, 0
        cmp     w19, w1
        bgt     Math_pow_d_dd.L33
        ldr     d1, Math_pow_d_dd.LC6
        mov     w0, -53
        fmul    d0, d0, d1
        str     d0, [x29, 104]
        ldr     w19, [x29, 108]
Math_pow_d_dd.L33:
        mov     w3, 39054
        asr     w1, w19, 20
        movk    w3, 0x3, lsl 16
        and     w19, w19, 1048575
        cmp     w19, w3
        sub     w1, w1, #1023
        add     w0, w1, w0
        mov     w2, 0
        orr     w1, w19, 1072693248
        ble     Math_pow_d_dd.L34
        mov     w3, 46713
        mov     w2, 1
        movk    w3, 0xb, lsl 16
        cmp     w19, w3
        ble     Math_pow_d_dd.L34
        sub     w1, w1, #1048576
        add     w0, w0, w2
        mov     w2, 0
Math_pow_d_dd.L34:
        // edit: we have Math_pow_d_dd.LANCHOR0 right here 
        adr     x4, Math.LANCHOR0
//        adrp    x4, Math_pow_d_dd.LANCHOR0
        sxtw    x3, w2
//        add     x4, x4, :lo12:Math.LANCHOR0
        str     w1, [x29, 108]
        fmov    d1, 1.0e+0
        asr     w1, w1, 1
        ldr     d0, [x29, 104]
        orr     w1, w1, 536870912
        ldr     d3, [x4, x3, lsl 3]
        add     w1, w1, 524288
        str     xzr, [x29, 144]
        add     w1, w1, w2, lsl 18
        ldr     d7, Math_pow_d_dd.LC8
        str     w1, [x29, 148]
        add     x1, x4, x3, lsl 3
        fadd    d4, d0, d3
        fsub    d6, d0, d3
        fdiv    d4, d1, d4
        fmul    d5, d6, d4
        str     d5, [x29, 136]
        str     wzr, [x29, 136]
        ldp     d2, d1, [x29, 136]
        fsub    d3, d1, d3
        fmsub   d1, d1, d2, d6
        fadd    d6, d5, d2
        fsub    d0, d0, d3
        ldr     d3, Math_pow_d_dd.LC7
        fmsub   d1, d0, d2, d1
        fmul    d0, d5, d5
        fmadd   d3, d0, d3, d7
        ldr     d7, Math_pow_d_dd.LC9
        fmul    d1, d1, d4
        fmul    d4, d0, d0
        fmadd   d3, d0, d3, d7
        ldr     d7, Math_pow_d_dd.LC10
        fmul    d6, d6, d1
        fmadd   d3, d0, d3, d7
        ldr     d7, Math_pow_d_dd.LC11
        fmadd   d3, d0, d3, d7
        ldr     d7, Math_pow_d_dd.LC12
        fmadd   d0, d0, d3, d7
        fmov    d3, 3.0e+0
        fmadd   d0, d4, d0, d6
        fmadd   d4, d2, d2, d3
        fadd    d4, d4, d0
        str     d4, [x29, 144]
        str     wzr, [x29, 144]
        ldr     d4, [x29, 144]
        fsub    d3, d4, d3
        fmsub   d3, d2, d2, d3
        fsub    d0, d0, d3
        fmul    d0, d0, d5
        ldr     d5, [x1, 32]
        fmadd   d1, d1, d4, d0
        fmadd   d0, d2, d4, d1
        str     d0, [x29, 112]
        str     wzr, [x29, 112]
        ldr     d0, Math_pow_d_dd.LC13
        ldr     d3, [x29, 112]
        fmsub   d2, d2, d4, d3
        ldr     d4, Math_pow_d_dd.LC15
        fsub    d1, d1, d2
        scvtf   d2, w0
        fmul    d1, d1, d0
        ldr     d0, Math_pow_d_dd.LC14
        str     d2, [x29, 128]
        fmadd   d0, d3, d0, d1
        ldr     d1, [x1, 16]
        fadd    d0, d0, d1
        fmadd   d1, d3, d4, d0
        fadd    d1, d1, d5
        fadd    d1, d1, d2
        str     d1, [x29, 120]
        str     wzr, [x29, 120]
        ldr     d1, [x29, 120]
        fsub    d2, d1, d2
        fsub    d2, d2, d5
        fmsub   d3, d3, d4, d2
        fsub    d0, d0, d3
Math_pow_d_dd.L32:
        ldr     d3, [x29, 80]
        mov     w1, 1083179007
        ldr     d4, [x29, 120]
        str     d3, [x29, 144]
        fmul    d0, d0, d3
        str     wzr, [x29, 144]
        ldr     d2, [x29, 144]
        fsub    d1, d3, d2
        fmadd   d0, d1, d4, d0
        fmul    d1, d2, d4
        str     d1, [x29, 112]
        fadd    d2, d0, d1
        str     d2, [x29, 136]
        ldp     w0, w2, [x29, 136]
        cmp     w2, w1
        ble     Math_pow_d_dd.L35
        mov     w1, -1083179008
        add     w1, w2, w1
        orr     w0, w1, w0
        cbnz    w0, Math_pow_d_dd.L75
        ldr     d3, Math_pow_d_dd.LC16
        fsub    d2, d2, d1
        fadd    d3, d0, d3
        fcmpe   d3, d2
        ble     Math_pow_d_dd.L37
Math_pow_d_dd.L75:
        ldr     d1, Math_pow_d_dd.LC0
        b       Math_pow_d_dd.L72
Math_pow_d_dd.L35:
        mov     w1, 52223
        and     w3, w2, 2147483647
        movk    w1, 0x4090, lsl 16
        cmp     w3, w1
        ble     Math_pow_d_dd.L37
        mov     w1, 13312
        movk    w1, 0x3f6f, lsl 16
        add     w1, w2, w1
        orr     w0, w1, w0
        cbnz    w0, Math_pow_d_dd.L73
        fsub    d2, d2, d1
        fcmpe   d0, d2
        bhi     Math_pow_d_dd.L37
Math_pow_d_dd.L73:
        ldr     d1, Math_pow_d_dd.LC1
Math_pow_d_dd.L72:
        fmul    d0, d8, d1
        fmul    d0, d0, d1
        b       Math_pow_d_dd.L2
Math_pow_d_dd.L37:
        and     w1, w2, 2147483647
        mov     w3, 1071644672
        cmp     w1, w3
        mov     w0, 0
        ble     Math_pow_d_dd.L41
        asr     w1, w1, 20
        mov     w3, 1048575
        sub     w0, w1, #1022
        mov     w1, 1048576
        str     xzr, [x29, 128]
        cmp     w2, 0
        asr     w1, w1, w0
        add     w1, w1, w2
        ubfx    x4, x1, 20, 11
        and     w0, w1, 1048575
        sub     w4, w4, #1023
        orr     w0, w0, 1048576
        asr     w3, w3, w4
        bic     w3, w1, w3
        mov     w1, 20
        str     w3, [x29, 132]
        sub     w4, w1, w4
        ldr     d2, [x29, 128]
        asr     w0, w0, w4
        csneg   w0, w0, w0, ge
        fsub    d1, d1, d2
        str     d1, [x29, 112]
Math_pow_d_dd.L41:
        ldr     d2, [x29, 112]
        ldr     d3, Math_pow_d_dd.LC18
        ldr     d4, Math_pow_d_dd.LC21
        fadd    d1, d0, d2
        str     d1, [x29, 128]
        str     wzr, [x29, 128]
        ldr     d1, [x29, 128]
        fsub    d2, d1, d2
        fsub    d0, d0, d2
        ldr     d2, Math_pow_d_dd.LC17
        fmul    d2, d1, d2
        fmadd   d0, d0, d3, d2
        ldr     d2, Math_pow_d_dd.LC19
        fmadd   d3, d1, d2, d0
        fmsub   d1, d1, d2, d3
        ldr     d2, Math_pow_d_dd.LC20
        fsub    d0, d0, d1
        fmul    d1, d3, d3
        fmadd   d0, d3, d0, d0
        fmadd   d2, d1, d2, d4
        ldr     d4, Math_pow_d_dd.LC22
        str     d1, [x29, 128]
        fmadd   d2, d1, d2, d4
        ldr     d4, Math_pow_d_dd.LC23
        fmadd   d2, d1, d2, d4
        ldr     d4, Math_pow_d_dd.LC24
        fmadd   d2, d1, d2, d4
        fmov    d4, 2.0e+0
        fmsub   d1, d1, d2, d3
        fmul    d2, d3, d1
        str     d1, [x29, 120]
        fsub    d1, d1, d4
        fdiv    d1, d2, d1
        fsub    d0, d1, d0
        fmov    d1, 1.0e+0
        fsub    d0, d0, d3
        fsub    d0, d1, d0
        str     d0, [x29, 136]
        ldr     w1, [x29, 140]
        add     w1, w1, w0, lsl 20
        asr     w2, w1, 20
        cmp     w2, 0
        bgt     Math_pow_d_dd.L43
        bl      Math_scalbn_d_di
        str     d0, [x29, 136]
        b       Math_pow_d_dd.L44
Math_pow_d_dd.L43:
        str     w1, [x29, 140]
Math_pow_d_dd.L44:
        ldr     d0, [x29, 136]
        fmul    d0, d8, d0
Math_pow_d_dd.L2:
        // edit: don't do this stack check
//        add     x21, x21, :lo12:__stack_chk_guard
//        ldr     x1, [x29, 152]
//        ldr     x0, [x21]
//        eor     x0, x1, x0
//        cbz     x0, Math_pow_d_dd.L46
//        bl      __stack_chk_fail
//Math_pow_d_dd.L46:
        ldp     x19, x20, [sp, 16]
        ldp     x21, x22, [sp, 32]
        ldp     x23, x24, [sp, 48]
        ldr     x25, [sp, 64]
        ldr     d8, [sp, 72]
        ldp     x29, x30, [sp], 160
        ret

Math_pow_d_dd.LC0:
        .word   2281731484
        .word   2117592124
Math_pow_d_dd.LC1:
        .word   3271095129
        .word   27618847
Math_pow_d_dd.LC2:
        .word   1431655765
        .word   1070945621
Math_pow_d_dd.LC3:
        .word   1697350398
        .word   1073157447
Math_pow_d_dd.LC4:
        .word   4166901572
        .word   1045736971
Math_pow_d_dd.LC5:
        .word   1610612736
        .word   1073157447
Math_pow_d_dd.LC6:
        .word   0
        .word   1128267776
Math_pow_d_dd.LC7:
        .word   1246056175
        .word   1070235176
Math_pow_d_dd.LC8:
        .word   2479479653
        .word   1070433866
Math_pow_d_dd.LC9:
        .word   2837266689
        .word   1070691424
Math_pow_d_dd.LC10:
        .word   1368335949
        .word   1070945621
Math_pow_d_dd.LC11:
        .word   3681528831
        .word   1071345078
Math_pow_d_dd.LC12:
        .word   858993411
        .word   1071854387
Math_pow_d_dd.LC13:
        .word   3694789629
        .word   1072613129
Math_pow_d_dd.LC14:
        .word   341508597
        .word   -1103220768
Math_pow_d_dd.LC15:
        .word   3758096384
        .word   1072613129
Math_pow_d_dd.LC16:
        .word   1697350398
        .word   1016534343
Math_pow_d_dd.LC17:
        .word   212364345
        .word   -1105175455
Math_pow_d_dd.LC18:
        .word   4277811695
        .word   1072049730
Math_pow_d_dd.LC19:
        .word   0
        .word   1072049731
Math_pow_d_dd.LC20:
        .word   1925096656
        .word   1046886249
Math_pow_d_dd.LC21:
        .word   3318901745
        .word   -1094992575
Math_pow_d_dd.LC22:
        .word   2938494508
        .word   1058100842
Math_pow_d_dd.LC23:
        .word   381599123
        .word   -1083784852
Math_pow_d_dd.LC24:
        .word   1431655742
        .word   1069897045


Math.LANCHOR0:
bp:
        .word   0
        .word   1072693248
        .word   0
        .word   1073217536
dp_l:
        .word   0
        .word   0
        .word   1137692678
        .word   1045233131
dp_h:
        .word   0
        .word   0
        .word   1073741824
        .word   1071822851

