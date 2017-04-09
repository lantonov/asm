
Os_MutexCreate:
Os_MutexDestroy:
        str  xzr, [x1]
        ret


Os_MutexLock:
        stp  x29, x30, [sp, -16]!
        mov  x29, x1
        mov  x1, 100
Os_MutexLock.L1:
        mov  w2, 0x01
     ldaxrb  w3, [x29]
     stlxrb  w4, w2, [x29]
       cbnz  w4, Os_MutexLock.L1
        cbz  w3, Os_MutexLock.L4
        sub  x1, x1, 1
       cbnz  x1, Os_MutexLock.L1
Os_MutexLock.L3:
        mov  w0, 0x0101
Os_MutexLock.L3_xchg:
      ldaxr  w3, [x29]
      stlxr  w4, w0, [x29]
       cbnz  w4, Os_MutexLock.L3_xchg        
       tbnz  w3, 0, Os_MutexLock.L2
Os_MutexLock.L4:
        ldp  x29, x30, [sp], 16
        ret
Os_MutexLock.L2:
        mov  x0, x29
        mov  x1, FUTEX_WAIT_PRIVATE
        mov  x2, 0x0101
        mov  x3, 0
        mov  x4, 0
        mov  x5, 0
        mov  x8, sys_futex
        svc  0
          b  Os_MutexLock.L3


Os_MutexUnlock:
        stp  x29, x30, [sp, -16]!
        mov  x29, x1
        ldr  w0, [x29]
        cmp  w0, 1      // expected = 1
        bne  Os_MutexUnlock.L1
        mov  w1, 0      // desired = 0
Os_MutexUnlock.L0:
      ldaxr  w2, [x29]  // w2 = obj
        cmp  w2, w0     // cmp obj, expected
        bne  Os_MutexUnlock.L1
       stxr  w4, w1, [x29]      // obj = desired
       cbnz  w4, Os_MutexUnlock.L0
          b  Os_MutexUnlock.L3
Os_MutexUnlock.L1:
       strb  wzr, [x29]
        mov  x1, 200
Os_MutexUnlock.L2:
       ldrb  w0, [x29]
       tbnz  w0, 0, Os_MutexUnlock.L3
        sub  x1, x1, 1
       cbnz  x1, Os_MutexUnlock.L2
       strb  wzr, [x29, 1]
        mov  x0, x29
        mov  x1, FUTEX_WAKE_PRIVATE
        mov  x2, 0x01
        mov  x3, 0
        mov  x4, 0
        mov  x5, 0
        mov  x8, sys_futex
        svc  0
        tst  w0, w0
        bmi  Failed_sys_futex_MutexUnlock
Os_MutexUnlock.L3:
        ldp  x29, x30, [sp], 16
        ret


Os_EventCreate:
Os_EventDestroy:
        stp  xzr, xzr, [x1]
        ret


Os_EventSignal:
        stp  x29, x30, [sp, -16]!
        mov  x29, x1
Os_EventSignal.incr:
      ldaxr  w0, [x29]
        add  w0, w0, 1
      stlxr  w4, w0, [x29]
       cbnz  w4, Os_EventSignal.incr
        mov  x0, x29
        mov  x1, FUTEX_WAKE_PRIVATE
        mov  x2, 0x01
        mov  x3, 0
        mov  x4, 0
        mov  x5, 0
        mov  x8, sys_futex
        svc  0
        tst  w0, w0
        bmi  Failed_sys_futex_EventSignal
        ldp  x29, x30, [sp], 16
        ret


Os_EventWait:
        stp  x24, x30, [sp, -16]!
        stp  x28, x29, [sp, -16]!
        mov  x28, x1    // x28 = cv
        mov  x29, x2    // x29 = mutex
        add  x5, x28, 8
        ldr  w24, [x28]
        ldr  x4, [x5]
        cmp  x29, x4
        bne  Os_EventWait.L4
Os_EventWait.L1:
        mov  x1, x29
         bl  Os_MutexUnlock
        mov  x0, x28
        mov  x1, FUTEX_WAIT_PRIVATE
        mov  x2, x24
        mov  x3, 0
        mov  x4, 0
        mov  x5, 0
        mov  x8, sys_futex
        svc  0
Os_EventWait.L3:
        mov  w0, 0x0101
      ldaxr  w3, [x29]
      stlxr  w4, w0, [x29]
       cbnz  w4, Os_EventWait.L3
       tbnz  w3, 0, Os_EventWait.L2
        ldp  x28, x29, [sp], 16
        ldp  x24, x30, [sp], 16
        ret
Os_EventWait.L2:
        mov  x0, x29
        mov  x1, FUTEX_WAIT_PRIVATE
        mov  x2, 0x0101
        mov  x3, 0
        mov  x4, 0
        mov  x5, 0
        mov  x8, sys_futex
        svc  0
          b  Os_EventWait.L3
Os_EventWait.L4:
      ldaxr  x2, [x5]
       cbnz  x2, Os_EventWait.L4_nexp
       stxr  w4, x29, [x5]
       cbnz  w4, Os_EventWait.L4
Os_EventWait.L4_nexp:
        ldr  x2, [x5]
        cmp  x2, x29    // we should have mutex x29 saved at x5
        beq  Os_EventWait.L1
          b  Failed_EventWait


Os_ThreadCreate:
        stp  x21, x30, [sp, -16]!
        stp  x28, x29, [sp, -16]!
        stp  x22, x23, [sp, -16]!
        stp  x24, x25, [sp, -16]!
        stp  x26, x27, [sp, -16]!
        mov  x22, x3    // address of NumaNode struct
        mov  x23, x4    // address of ThreadHandle struct
        mov  x24, x1    // start address
        mov  x25, x2    // parameter to pass
        mov  x1, THREAD_STACK_SIZE
        ldr  w2, [x22, NumaNode.nodeNumber]
         bl  Os_VirtualAllocNuma_GrowsDown
        str  x0, [x23, ThreadHandle.stackAddress]
        mov  x28, x0
        mov  x0, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
        orr  x0, x0, CLONE_THREAD
        add  x1, x28, THREAD_STACK_SIZE
        mov  x2, 0
        mov  x3, 0
        mov  x4, 0
        mov  x8, sys_clone
        svc  0
        tst  w0, w0
        bmi  Failed_sys_clone
        beq  Os_ThreadCreate.WeAreChild
        ldp  x26, x27, [sp], 16
        ldp  x24, x25, [sp], 16
        ldp  x22, x23, [sp], 16
        ldp  x28, x29, [sp], 16
        ldp  x21, x30, [sp], 16
        ret
Os_ThreadCreate.WeAreChild:
        mov  x0, 0
        mov  x1, MAX_LINUXCPUS/8
        add  x2, x22, NumaNode.cpuMask
        mov  x3, 0
        ldr  x4, [x2, 8*0]
        orr  x3, x3, x4
        ldr  x4, [x2, 8*1]
        orr  x3, x3, x4
        cbz  x3, Os_ThreadCreate.DontSetAffinity
        mov  x8, sys_sched_setaffinity
        svc  0
       cbnz  w0, Failed_sys_sched_setaffinity
Os_ThreadCreate.DontSetAffinity:
        mov  x1, x25
        blr  x24
        add  x0, x23, ThreadHandle.mutex
        mov  x1, FUTEX_WAKE_PRIVATE
        mov  x2, 1
        str  w2, [x0]
        mov  x8, sys_futex
        svc  0
        mov  x0, 0
        mov  x8, sys_exit
        svc  0

        
Os_ThreadJoin:
        stp  x29, x30, [sp, -16]!
        mov  x29, x1
        add  x0, x29, ThreadHandle.mutex
        mov  x1, FUTEX_WAIT_PRIVATE
        mov  x2, 0
        mov  x3, 0
        mov  x4, 0
        mov  x5, 0
        mov  x8, sys_futex
        svc  0
        ldr  x1, [x29, ThreadHandle.stackAddress]
        mov  x2, THREAD_STACK_SIZE
         bl  Os_VirtualFree
        ldp  x29, x30, [sp], 16
        ret


Os_SetThreadPoolInfo:
        lea  x16, threadPool
        mov  x1, 1
        str  w1, [x16, ThreadPool.nodeCnt]
        str  w1, [x16, ThreadPool.coreCnt]

        mov  w0, -1
        add  x16, x16, ThreadPool.nodeTable
        str  w0, [x16, NumaNode.nodeNumber]
        str  w1, [x16, NumaNode.coreCnt]
        str  xzr, [x16, NumaNode.cmhTable]
        str  x16, [x16, NumaNode.parent]
        str  xzr, [x16, NumaNode.cpuMask+8*0]
        str  xzr, [x16, NumaNode.cpuMask+8*1]
        ret


Os_DisplayThreadPoolInfo:
Os_SetPriority_Realtime:
Os_SetPriority_Normal:
Os_SetPriority_Low:
Os_SetPriority_Idle:
        ret


Os_ExitProcess:
// in: x0 exit code
        mov  x8, sys_exit_group
        svc  0


Os_SetStdHandles:
        ret


Os_InitializeTimer:
        ret


Os_GetTime:
// out: x0, x2 such that x0+x2/2^64 = time in ms
        stp  x29, x30, [sp, -16]!
        sub  sp, sp, 64
        mov  x0, 1
        add  x1, sp, 16
        mov  x8, 113
        svc  0
        ldr  x1, [sp, 16]
        ldr  x3, [sp, 24]
        mov  x4, 46573
       movk  x4, 0xf7a0, lsl 16
       movk  x4, 0x10c6, lsl 32
        mov  x5, 1000
        mul  x2, x3, x4
      umulh  x0, x3, x4
       madd  x0, x1, x5, x0
        add  sp, sp, 64
        ldp  x29, x30, [sp], 16
        ret


Os_Sleep:
// in: x1 ms to sleep
        stp  x29, x30, [sp, -16]!
        sub  sp, sp, 64
        mov  x0, 1000
       udiv  x2, x1, x0
       msub  x3, x2, x0, x1
        mul  x0, x0, x0
        mul  x3, x3, x0
        str  x2, [sp, 16]
        str  x3, [sp, 24]
        add  x0, sp, 16
        mov  x1, 0            
        mov  x8, 101
        svc  0
        add  sp, sp, 64
        ldp  x29, x30, [sp], 16
        ret


Os_VirtualAllocNuma_GrowsDown:
        stp  x29, x30, [sp, -16]!
        mov  x5, 0
        mov  x4, -1
        mov  x3, MAP_PRIVATE | MAP_ANONYMOUS | MAP_GROWSDOWN
        mov  x2, 0x03
        mov  x0, 0
        mov  x8, sys_mmap
        svc  0
        tst  x0, x0
        bmi  Failed_sys_mmap_VirtualAlloc
        ldp  x29, x30, [sp], 16
        ret

Os_VirtualAlloc_LargePages:
// in: x1 size
        mov  x0, 0
        mov  x2, 0
        ret

Os_VirtualAllocNuma:
Os_VirtualAlloc:
// in: x1 size
        stp  x29, x30, [sp, -16]!
        mov  x5, 0
        mov  x4, -1
        mov  x3, MAP_PRIVATE | MAP_ANONYMOUS
        mov  x2, 0x03
        mov  x0, 0
        Display "  mmap(size = %x1)"
        mov  x8, sys_mmap
        svc  0
        Display ":       %x0\n"
        tst  x0, x0
        bmi  Failed_sys_mmap_VirtualAlloc
        ldp  x29, x30, [sp], 16
        ret


Os_VirtualFree:
// in: x1 address
//     x2 size
        stp  x29, x30, [sp, -16]!
        cbz  x1, Os_VirtualFree.Null
        mov  x0, x1
        mov  x1, x2
        Display "munmap(size = %x1, addr = %x0)"
        mov  x8, sys_munmap
        svc  0
        Display ": %x0\n"
        cmp  w0, 0
        bne  Failed_sys_unmap_VirtualFree
Os_VirtualFree.Null:
        ldp  x29, x30, [sp], 16
        ret


Os_WriteOut_Output:
       adrp  x1, Output
        add  x1, x1, :lo12:Output
Os_WriteOut:
// in: x1 address of string start
// in: x27 address of string end
        stp  x29, x30, [sp, -16]!
        sub  x2, x27, x1
        mov  x0, 1
        mov  x8, 64
        svc  0
        ldp  x29, x30, [sp], 16
        ret


Os_ReadStdIn:
// in: x1 address of buffer
//     x2 max bytes to read
        stp  x29, x30, [sp, -16]!
        mov  x0, 0
        mov  x8, 63
        svc  0
        ldp  x29, x30, [sp], 16
        ret


Os_ParseCommandLine:
// initializes ioBuffer struct  
        stp  x29, x30, [sp, -64]!
        stp  x22, x23, [sp, 16]
        stp  x24, x25, [sp, 32]
        stp  x27, x28, [sp, 48]
       adrp  x29, ioBuffer
        add  x29, x29, :lo12:ioBuffer
        mov  x1, 4096
        str  x1, [x29,IOBuffer.inputBufferSizeB]
         bl  Os_VirtualAlloc
        str  x0, [x29,IOBuffer.inputBuffer]
        ldp  x22, x23, [sp, 16]
        ldp  x24, x25, [sp, 32]
        ldp  x27, x28, [sp, 48]
        ldp  x28, x30, [sp], 64
        ret


Failed_sys_futex_EventSignal:
	lea  x27, sz_error_sys_futex_EventSignal
          b  Failed
Failed_EventWait:
        lea  x27, sz_error_EventWait
          b  Failed
Failed_sys_futex_MutexUnlock:
        lea  x27, sz_error_sys_futex_MutexUnlock
          b  Failed
Failed_sys_clone:
        lea  x27, sz_error_sys_clone
          b  Failed
Failed_sys_sched_setaffinity:
	lea  x27, sz_error_sys_sched_setaffinity
          b  Failed
Failed_sys_mmap_VirtualAlloc:
        lea  x27, sz_error_sys_mmap_VirtualAlloc
          b  Failed
Failed_sys_unmap_VirtualFree:
        lea  x27, sz_error_sys_unmap_VirtualFree
          b  Failed
Failed:
// x27 address of null terminated string
        mov  x29, x0
        mov  x1, x27
        lea  x27, Output
         bl  PrintString

        lea  x1, sz_failed_x0
         bl  PrintString
        mov  x0, x29
         bl  PrintHex
        mov  w0, 10
       strb  w0, [x27], 1
        lea  x27, Output
         bl  Os_ErrorBox
        mov  x0, 1
         bl  Os_ExitProcess


Os_ErrorBox:
// x27 address of null terminated string
        stp  x29, x30, [sp, -16]!
        mov  x1, x27
         bl  StringLength
        mov  x2, x0
        mov  x1, x27
        mov  x0, 1
        mov  x8, 64
        svc  0
        ldp  x29, x30, [sp], 16
        ret     


Os_CheckCPU:
        stp  x27, x30, [sp, -16]!
        mov  x0, 0
// make sure that rook attacks are page aligned
        lea  x27, sz_error_rook_page
        add  x0, x0, :lo12:RookAttacksSTUFF
       cbnz  x0, Failed
// make sure that bishop attacks are page aligned        
        lea  x27, sz_error_bishop_page
        add  x0, x0, :lo12:BishopAttacksSTUFF
       cbnz  x0, Failed
        ldp  x27, x30, [sp], 16
        ret



