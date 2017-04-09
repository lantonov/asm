// macro names seem to be case insensitive
.macro ClampUnsigned Reg, RegMin, RegMax
        cmp  \Reg, \RegMin
       csel  \Reg, \Reg, \RegMin, hs
        cmp  \Reg, \RegMax
       csel  \Reg, \Reg, \RegMax, ls
.endm

.macro ClampSigned Reg, RegMin, RegMax
        cmp  \Reg, \RegMin
       csel  \Reg, \Reg, \RegMin, ge
        cmp  \Reg, \RegMax
       csel  \Reg, \Reg, \RegMax, le
.endm

.macro Popcnt Reg0, Reg1, Temp2 // its ok if Reg0 and Reg1 are the same
        lsr  \Temp2, \Reg1, 1
        and  \Temp2, \Temp2, 6148914691236517205
        sub  \Temp2, \Reg1, \Temp2
        and  \Reg0, \Temp2, 3689348814741910323
        lsr  \Temp2, \Temp2, 2
        and  \Temp2, \Temp2, 3689348814741910323
        add  \Temp2, \Reg0, \Temp2
        mov  \Reg0, 72340172838076673
        add  \Temp2, \Temp2, \Temp2, lsr 4
        and  \Temp2, \Temp2, 1085102592571150095
        mul  \Reg0, \Temp2, \Reg0
        lsr  \Reg0, \Reg0, 56
.endm

.macro Popcnt16 Reg0, Reg1, Temp2 // its ok if Reg0 and Reg1 are the same
        lsr  \Temp2, \Reg1, 1
        and  \Temp2, \Temp2, 6148914691236517205
        sub  \Temp2, \Reg1, \Temp2
        lsr  \Reg0, \Temp2, 2
        and  \Temp2, \Temp2, 3689348814741910323
        and  \Reg0, \Reg0, 3689348814741910323
        add  \Temp2, \Reg0, \Temp2
        mov  \Reg0, 72340172838076673
        mul  \Reg0, \Temp2, \Reg0
        lsr  \Reg0, \Reg0, 56
.endm

.macro lea Reg, Addr
       adrp  \Reg, \Addr
        add  \Reg, \Reg, :lo12:\Addr
.endm

.macro AddSub T, A, B, C
 .if \T == White
        sub  \A, \B, \C
 .else
        add  \A, \B, \C
 .endif    
.endm

.macro PrintNewLine
        mov  w0, 10
       strb  w0, [x27], 1
.endm

.macro PushAll
        sub  sp, sp, 16*4
        st1  {v28.16b, v29.16b, v30.16b, v31.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v24.16b, v25.16b, v26.16b, v27.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v20.16b, v21.16b, v22.16b, v23.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v16.16b, v17.16b, v18.16b, v19.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v12.16b, v13.16b, v14.16b, v15.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v8.16b, v9.16b, v10.16b, v11.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v4.16b, v5.16b, v6.16b, v7.16b}, [sp]
        sub  sp, sp, 16*4
        st1  {v0.16b, v1.16b, v2.16b, v3.16b}, [sp]
        stp  x30, xzr, [sp, -16]!
        stp  x28, x29, [sp, -16]!
        stp  x26, x27, [sp, -16]!
        stp  x24, x25, [sp, -16]!
        stp  x22, x23, [sp, -16]!
        stp  x20, x21, [sp, -16]!
        stp  x18, x19, [sp, -16]!
        stp  x16, x17, [sp, -16]!
        stp  x14, x15, [sp, -16]!
        stp  x12, x13, [sp, -16]!
        stp  x10, x11, [sp, -16]!
        stp  x8, x9, [sp, -16]!
        stp  x6, x7, [sp, -16]!
        stp  x4, x5, [sp, -16]!
        stp  x2, x3, [sp, -16]!
        stp  x0, x1, [sp, -16]!
        add  x0, sp, 8*32
        str  x0, [sp, 8*31]
        ldr  x0, [sp] 
.endm

.macro PopAll
        ldp  x0, x1, [sp], 16
        ldp  x2, x3, [sp], 16
        ldp  x4, x5, [sp], 16
        ldp  x6, x7, [sp], 16
        ldp  x8, x9, [sp], 16
        ldp  x10, x11, [sp], 16
        ldp  x12, x13, [sp], 16
        ldp  x14, x15, [sp], 16
        ldp  x16, x17, [sp], 16
        ldp  x18, x19, [sp], 16
        ldp  x20, x21, [sp], 16
        ldp  x22, x23, [sp], 16
        ldp  x24, x25, [sp], 16
        ldp  x26, x27, [sp], 16
        ldp  x28, x29, [sp], 16
        ldr  x30, [sp], 16
        ld1  {v0.16b, v1.16b, v2.16b, v3.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v4.16b, v5.16b, v6.16b, v7.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v8.16b, v9.16b, v10.16b, v11.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v12.16b, v13.16b, v14.16b, v15.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v16.16b, v17.16b, v18.16b, v19.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v20.16b, v21.16b, v22.16b, v23.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v24.16b, v25.16b, v26.16b, v27.16b}, [sp]
        add  sp, sp, 16*4
        ld1  {v28.16b, v29.16b, v30.16b, v31.16b}, [sp]
        add  sp, sp, 16*4

.endm

// Display a formated message. Use %[x,i,u]n for displaying
// register xn in hex, signed or unsigned.
// ex: Display "sq: %i14  sq: %i15  line: %x0  bet: %x1\n"
.macro Display Mes
        PushAll
        lea  x1, DisplayLock
         bl  Os_MutexLock
        adr  x1, anom\@
          b  anol\@
anom\@:
        .ascii "\Mes"
        .byte 0
        .balign 4
anol\@:
        lea  x27, DisplayOutput
        mov  x2, sp
        add  x3, sp, 32*8
         bl  PrintFancy
        lea  x1, DisplayOutput
         bl  Os_WriteOut
        lea  x1, DisplayLock
         bl  Os_MutexUnlock
        PopAll
.endm

.macro DisplayPause
        PushAll
        mov  x1, 100
         bl  Os_Sleep
        PopAll     
.endm

.macro ToLower Reg
        sub  \Reg, \Reg, 'A'
        cmp  \Reg, 'Z'-'A'+1
        bhs  loc_Lower\@
        add  \Reg, \Reg, 'a'-'A'
loc_Lower\@:
        add  \Reg, \Reg, 'A'
.endm

