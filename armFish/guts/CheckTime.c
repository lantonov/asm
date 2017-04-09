
CheckTime:
/*
        ; we must:
        ;   set signals stop if the search is to be aborted (soon)
        ;   and determine a good resetCnt to send to all threads
        ;     lower values of resetCnt lead to to better resolution but increased polling
        ;     conversely for higher values of resetCnt

ProfileInc CheckTime

               push   rbx rsi rdi
 AssertStackAligned   'CheckTime'

	; if MAX_RESETCNT is exactly calls to search per second
	; then this value of resetCnt should put us back here in 1 second
	; this is obviously too much when using time mgmt
	; so .Reset4Time reduces this

                mov   esi, MAX_RESETCNT		; fall through count

        ; Of course, setting MAX_RESETCNT = number of calls to search per second
        ; is impossible to measure and also dangerous. So really we have
        ;   MAX_RESETCNT ~= (number of calls to search per second) * X
        ; where X is a number between 0 and 1.
        ; We then expect to be back here in X seconds.

                xor   eax, eax
                cmp   al, byte[limits.ponder]
                jne   .Return

                cmp   al, byte[limits.useTimeMgmt]
                 je   .DontUseTimeMgmt
                mov   rdi, qword[time.maximumTime]
*/
        stp  x21, x30, [sp, -16]!
        stp  x14, x15, [sp, -16]!
        mov  x14, (MAX_RESETCNT>>0 ) & 0x0ff
       movk  x14, (MAX_RESETCNT>>16) & 0x0ff, lsl 16

CheckTime.Reset4Time:
/*
        ; rdi is target time
               call   _GetTime
                sub   rax, qword[time.startTime]
                add   rax, 1
        ; rax is elapsed time
                sub   rdi, rax
                 jb   .Stop

        ; If rdi ms are remaining, attemp to put us back here in X*rdi/2 ms.
        ; The values of rdi at this point are in geometric progression.
        ; On tested machine, this ends a 'go movetime 10000' in 10000 ms
        ; with approx 130 calls to CheckTime.
GD String, 'ms remaining: '
GD Int64, rdi
                mov   eax, MAX_RESETCNT/2000
                mul   rdi
                add   rax, MIN_RESETCNT ; resetCnt should be at least 50
                adc   rdx, 0            ; if mul overflows, there is lots of
                jnz   .Return           ; time and use fall through count
                cmp   rsi, rax
              cmova   esi, eax
*/
         bl  Os_GetTime
        lea  x16, time
        ldr  x4, [x16, Time.startTime]
        add  x0, x0,1
        sub  x0, x0, x4
       subs  x15, x15, x0
        blo  CheckTime.Stop
        mov  x0, MAX_RESETCNT/2000
      umulh  x2, x0, x15
        mul  x0, x0, x15
       adds  x0, x0, MIN_RESETCNT
        adc  x2, x2, xzr
       cbnz  x2, CheckTime.Return
        cmp  x14, x0
       csel  w14, w0, w14, hi

        
CheckTime.Return:
/*

GD String, ' resetCnt: '
GD Int64, rsi
GD NewLine

	; set resetCnt for all threads to esi
                mov   ecx, dword[threadPool.threadCnt]
*/
        lea  x16, threadPool
        ldr  w1, [x16, ThreadPool.threadCnt]
        add  x16, x16, ThreadPool.threadTable
CheckTime.ResetNextThread:
/*
                sub   ecx, 1
                mov   rax, qword[threadPool.threadTable+8*rcx]
                mov   dword[rax+Thread.resetCnt], esi
                jnz   .ResetNextThread

                pop   rdi rsi rbx
                ret
*/
        sub  w1, w1, 1
        ldr  x0, [x16, x1, lsl 3]
        str  w14, [x10, Thread.resetCnt]
       cbnz  w1, CheckTime.ResetNextThread
        ldp  x14, x15, [sp], 16
        ldp  x21, x30, [sp], 16
        ret

CheckTime.Stop:
/*
                mov   byte[signals.stop], -1
                pop   rdi rsi rbx
                ret
*/
        mov  w0, -1
        lea  x16, signals
       strb  w0, [x16, Signals.stop]
        ldp  x14, x15, [sp], 16
        ldp  x21, x30, [sp], 16
        ret
CheckTime.DontUseTimeMgmt:
/*
                mov   edi, dword[limits.movetime]
               test   edi, edi
                jnz   .Reset4Time
                mov   rdi, qword[limits.nodes]
               test   rdi, rdi
                 jz   .Return           ; use fall through count

               call   ThreadPool_NodesSearched_TbHits
                add   rax, 1
*/
        lea  x16, limits
        ldr  w15, [x16, Limits.movetime]
       cbnz  w15, CheckTime.Reset4Time
        ldr  x15, [x16, Limits.nodes]
        cbz  x15, CheckTime.Return
         bl  ThreadPool_NodesSearched_TbHits
        add  x0, x0, 1

CheckTime.Reset4Nodes:
/*
        ; rdi is target nodes
        ; rax is elapsed nodes
                sub   rdi, rax
                 jb   .Stop

        ; if rdi nodes are remaining, attemp to put us back here rdi/3 nodes later
        ; the division is by 6 because half of the nodes are from qsearch
        ; the values of rdi at this point are in geometric progression
        ; this ends 'go nodes 1000000' with 1000053 nodes
        ; with 22 calls to CheckTime
GD String, 'nodes remaining: '
GD Int64, rdi
                mov   rax, (1 shl 63)/3
                mul   rdi               ; rdx = rdi/6
                add   rdx, MIN_RESETCNT
                cmp   rsi, rdx
              cmova   esi, edx
                jmp   .Return
*/
       subs  x15, x15, x0
        blo  CheckTime.Stop
        mov  x0, 0xaaaaaaaaaaaaaaaa
       movk  x0, 0x2aaa, lsl 48
      umulh  x2, x0, x15
        add  x2, x2, MIN_RESETCNT
        cmp  x14, x2
       csel  w14, w2, w14, hi
          b  CheckTime.Return

        

