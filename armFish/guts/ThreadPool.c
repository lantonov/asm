
ThreadPool_Create:
/*
	; in: rcx address of node affinity string
	       push   rbx
	       call   _SetThreadPoolInfo
		xor   ecx, ecx
	       call   Thread_Create
		mov   dword[threadPool.threadCnt], 1
		pop   rbx
		ret
*/
        stp  x21, x30, [sp, -16]!
         bl  Os_SetThreadPoolInfo
        mov  x1, 0
         bl  Thread_Create
        mov  x0, 1
        lea  x16, threadPool+ThreadPool.threadCnt
        str  w0, [x16]
        ldp  x21, x30, [sp], 16
        ret

ThreadPool_Destroy:
/*
	       push   rsi rdi rbx
		mov   edi, dword[threadPool.threadCnt]
*/
        stp  x21, x30, [sp, -16]!
        stp  x14, x15, [sp, -16]!
        lea  x16, [threadPool+ThreadPool.threadCnt]
        ldr  w15, [x16]

ThreadPool_Destroy.NextThread:
/*
		lea   ecx, [rdi-1]
	       call   Thread_Delete
		sub   edi, 1
		jnz   .NextThread
		mov   dword[threadPool.threadCnt], edi

		lea   rdi, [threadPool.nodeTable]
	       imul   ebx, dword[threadPool.nodeCnt], sizeof.NumaNode
		add   rbx, rdi
*/
        sub  x1, x15, 1
         bl  Thread_Delete
        sub  x15, x15, 1
       cbnz  x15, ThreadPool_Destroy.NextThread
        lea  x16, threadPool+ThreadPool.threadCnt
        str  w15, [x16]
        lea  x15, threadPool+ThreadPool.nodeTable
        lea  x16, threadPool+ThreadPool.nodeCnt
        ldr  w4, [x16]
        mov  w5, sizeof.NumaNode
       madd  x21, x4, x5, x15
ThreadPool_Destroy.NextNumaNode:
/*
		mov   rcx, qword[rdi+NumaNode.cmhTable]
		mov   edx, sizeof.CounterMoveHistoryStats
	       call   _VirtualFree
		xor   eax, eax
		mov   qword[rdi+NumaNode.cmhTable], rax
		add   rdi, sizeof.NumaNode
		cmp   rdi, rbx
		 jb   .NextNumaNode
		pop   rbx rdi rsi
		ret
*/
        ldr  x1, [x15, NumaNode.cmhTable]
        mov  x2, sizeof.CounterMoveHistoryStats
         bl  Os_VirtualFree
        str  xzr, [x15, NumaNode.cmhTable]
        add  x15, x15, sizeof.NumaNode
        cmp  x15, x21
        blo  ThreadPool_Destroy.NextNumaNode
        ldp  x14, x15, [sp], 16
        ldp  x21, x30, [sp], 16
        ret

ThreadPool_ReadOptions:
/*
	       push   rbx rsi rdi
		mov   esi, dword[options.threads]
		mov   edi, dword[threadPool.threadCnt]
		cmp   edi, esi
		 je   .Skip
*/
        stp  x21, x30, [sp, -16]!
        stp  x14, x15, [sp, -16]!
        lea  x16, options+Options.threads
        ldr  w14, [x16]
        lea  x16, threadPool+ThreadPool.threadCnt
        ldr  w15, [x16]
        cmp  w14, w15
        beq  ThreadPool_ReadOptions.Skip

ThreadPool_ReadOptions.CheckCreate:
/*
		cmp   edi, esi
		 jb   .Create
*/
        cmp  x15, x14
        blo  ThreadPool_ReadOptions.Create

ThreadPool_ReadOptions.CheckDelete:
/*
		cmp   edi, esi
		 ja   .Delete
	       call   ThreadPool_DisplayThreadDistribution
*/
        cmp  w15, w14
        bhi  ThreadPool_ReadOptions.Delete
         bl  ThreadPool_DisplayThreadDistribution
ThreadPool_ReadOptions.Skip:
/*
		pop   rdi rsi rbx
		ret
*/
        ldp  x14, x15, [sp], 16
        ldp  x21, x30, [sp], 16
        ret
ThreadPool_ReadOptions.Create:
/*
		mov   ecx, edi
	       call   Thread_Create
		add   edi, 1
		mov   dword[threadPool.threadCnt], edi
		jmp   .CheckCreate
*/
        mov  w1, w15
         bl  Thread_Create
        add  w15, w15, 1
        lea  x16, threadPool+ThreadPool.threadCnt
        str  w15, [x16]
          b  ThreadPool_ReadOptions.CheckCreate
ThreadPool_ReadOptions.Delete:
/*
		sub   edi, 1
		mov   ecx, edi
	       call   Thread_Delete
		mov   dword[threadPool.threadCnt], edi
		jmp   .CheckDelete
*/
        sub  w15, w15, 1
        mov  w1, w15
         bl  Thread_Delete
        lea  x16, threadPool+ThreadPool.threadCnt
        str  w15, [x16]
          b  ThreadPool_ReadOptions.CheckDelete

ThreadPool_NodesSearched_TbHits:
/*
		xor   ecx, ecx
		xor   eax, eax
		xor   edx, edx
	.next_thread:
		mov   r8, qword[threadPool.threadTable+8*rcx]
		add   rax, qword[r8+Thread.nodes]
		add   rdx, qword[r8+Thread.tbHits]
		add   ecx, 1
		cmp   ecx, dword[threadPool.threadCnt]
		 jb   .next_thread
		ret
*/
        lea  x4, threadPool+ThreadPool.threadCnt
        lea  x5, threadPool+ThreadPool.threadTable
        ldr  w4, [x4]
        mov  w1, 0
        mov  x1, 0
        mov  x2, 0
ThreadPool_NodesSearched_TbHits.next_thread:
        ldr  x8, [x5, x0, lsl 3]
        ldr  x6, [x8, Thread.nodes]
        ldr  x7, [x8, Thread.tbHits]
        add  x0, x0, x6
        add  x2, x2, x7
        add  w1, w1, 1
        cmp  w1, w4
        blo  ThreadPool_NodesSearched_TbHits.next_thread
        ret

ThreadPool_DisplayThreadDistribution:
/*
	       push   rbx rsi rdi r14 r15
		lea   rdi, [Output]
		lea   rsi, [threadPool.nodeTable]
	       imul   r15d, dword[threadPool.nodeCnt], sizeof.NumaNode
		add   r15, rsi
*/
        stp  x21, x30, [sp, -16]!
        stp  x14, x15, [sp, -16]!
        stp  x24, x25, [sp, -16]!
        lea  x15, Output
        lea  x14, threadPool+ThreadPool.nodeTable
        lea  x16, threadPool+ThreadPool.nodeCnt
        ldr  w4, [x16]
        mov  x5, sizeof.NumaNode
       madd  x25, x4, x5, x14

ThreadPool_DisplayThreadDistribution.NextNode:
/*
		mov   rax, 'info str'
	      stosq
		mov   rax, 'ing node'
	      stosq
		mov   al, ' '
	      stosb
	     movsxd   rax, dword[rsi+NumaNode.nodeNumber]
	       call   PrintSignedInteger
		mov   rax, ' has thr'
	      stosq
		mov   rax, 'eads'
	      stosd
		 or   ebx, -1
*/
        lea  x1, sz_thread_format
      ldrsw  x0, [x14, NumaNode.nodeNumber]
        sub  sp, sp, 16
        str  x0, [sp]
        add  x2, sp, 0
         bl  PrintFancy
        add  sp, sp, 16

        mov  w21, -1
ThreadPool_DisplayThreadDistribution.ThreadLoop:
/*
		add   ebx, 1
		cmp   ebx, dword[threadPool.threadCnt]
		jae   .ThreadLoopDone
		mov   rax, qword[threadPool.threadTable+8*rbx]
		cmp   rsi, qword[rax+Thread.numaNode]
		jne   .ThreadLoop
		mov   al, ' '
	      stosb
		mov   eax, ebx
	       call   PrintUnsignedInteger
		jmp   .ThreadLoop
*/
        add  w21, w21, 1
        lea  x16, threadPool+ThreadPool.threadCnt
        ldr  w4, [x16]
        cmp  w21, w4
        bhs  ThreadPool_DisplayThreadDistribution.ThreadLoopDone
        mov  w0, ' '
       strb  w0, [x15], 1
        mov  x0, x21
         bl  PrintUInt
          b  ThreadPool_DisplayThreadDistribution.ThreadLoop

ThreadPool_DisplayThreadDistribution.ThreadLoopDone:
/*
       PrintNewLine
		add   rsi, sizeof.NumaNode
		cmp   rsi, r15
		 jb   .NextNode
	       call   _WriteOut_Output
*/
        PrintNewLine
        add  x14, x14, sizeof.NumaNode
        cmp  x14, x15
        blo  ThreadPool_DisplayThreadDistribution.NextNode
         bl  Os_WriteOut_Output

ThreadPool_DisplayThreadDistribution.Return:
/*
		pop   r15 r14 rdi rsi rbx
		ret
*/
        ldp  x24, x25, [sp], 16
        ldp  x14, x15, [sp], 16
        ldp  x21, x30, [sp], 16
        ret


ThreadPool_StartThinking:
/*
	; in: rbp address of position
	;     rcx address of limits struct
	;            this will be copied to the global limits struct
	;            so that search threads can see it
	       push   rbp rbx rsi rdi r13 r14 r15
virtual at rsp
  .moveList rb sizeof.ExtMove*MAX_MOVES
  .lend rb 0
end virtual
.localsize = ((.lend-rsp+15) and (-16))
*/
        stp  x20, x30, [sp, -16]!
        stp  x21, x14, [sp, -16]!
        stp  x15, x23, [sp, -16]!
        stp  x24, x25, [sp, -16]!
ThreadPool_StartThinking.moveList = 0
ThreadPool_StartThinking.localsize = sizeof.ExtMove*MAX_MOVES + ThreadPool_StartThinking.moveList
ThreadPool_StartThinking.moveList = (ThreadPool_StartThinking.moveList+15) & -16
/*
	 _chkstk_ms   rsp, .localsize
		sub   rsp, .localsize
		mov   rsi, rcx
		mov   r15, rbp
		mov   r14, qword[threadPool.threadTable+8*0]
	; rsi = address of limits
	; r15 = gui position
	; r14 = main thread
*/
        sub  sp, sp, ThreadPool_StartThinking.localsize
        mov  x14, x1
        mov  x25, x21
        lea  x16, threadPool+ThreadPool.threadTable+8*0
        ldr  x24, [x16]
/*
		mov   rcx, r14
	       call   Thread_WaitForSearchFinished
		xor   eax, eax
		mov   byte[signals.stop], al
		mov   byte[signals.stopOnPonderhit], al
		lea   rcx, [limits]
		mov   rdx, rsi
	       call   Limits_Copy
*/
        mov  x1, x24
         bl  Thread_WaitForSearchFinished
        lea  x16, signals
       strb  w0, [x16, Signals.stop]
       strb  w0, [x16, Signals.stopOnPonderhit]
        lea  x1, limits
        mov  x2, x14
         bl  Limits_Copy

/*
	lea   rcx, [rbx+Thread.rootPos]
	       call   Position_CopyToSearch
		xor   eax, eax
		mov   dword[rbx+Thread.rootDepth], eax
		mov   qword[rbx+Thread.nodes], rax
		mov   qword[rbx+Thread.tbHits], rax
		mov   byte[rbx+Thread.maxPly], al
                mov   dword[r14+Thread.resetCnt], eax
                mov   dword[r14+Thread.callsCnt], MAX_RESETCNT  ; main thread already has min
*/
         bl  Position_SetPieceLists
        add  x1, x24, Thread.rootPos
         bl  Position_CopyToSearch
        mov  w0, MIN_RESETCNT
        str  wzr, [x24, Thread.rootDepth]
        str  xzr, [x24, Thread.nodes]
        str  xzr, [x24, Thread.tbHits]
       strb  wzr, [x24, Thread.maxPly]
        str  wzr, [x24, Thread.resetCnt]
        str  w0, [x24, Thread.callsCnt]
/*
		lea   rsi, [limits.moveVec]
		lea   rdi, [.moveList]
		mov   ecx, dword[limits.moveVecSize]
	       test   ecx, ecx
		jnz   .use_searchmoves
		mov   rbx, qword[rbp+Pos.state]
	       call   Gen_Legal
*/
        lea  x14, limits+Limits.moveVec
        add  x15, sp, ThreadPool_StartThinking.moveList
        lea  x16, limits+Limits.moveVecSize
        ldr  w1, [x16]
       cbnz  w1, ThreadPool_StartThinking.use_searchmoves
        ldr  x21, [x20, Pos.state]
         bl  Gen_Legal
ThreadPool_StartThinking.have_moves:
/*
		lea   rsi, [.moveList]
		lea   rcx, [r14+Thread.rootPos+Pos.rootMovesVec]
	       call   RootMovesVec_Clear
*/
        add  x14, sp, ThreadPool_StartThinking.moveList
        add  x1, x24, Thread.rootPos+Pos.rootMovesVec
         bl  RootMovesVec_Clear
ThreadPool_StartThinking.push_moves:
/*
		cmp   rsi, rdi
		jae   .push_moves_done
		lea   rcx, [r14+Thread.rootPos+Pos.rootMovesVec]
		mov   edx, dword[rsi+ExtMove.move]
		add   rsi, sizeof.ExtMove
	       call   RootMovesVec_PushBackMove
		jmp   .push_moves
*/
        cmp  x14, x15
        bhs  ThreadPool_StartThinking.push_moves_done
        add  x1, x24, Thread.rootPos+Pos.rootMovesVec
        ldr  w2, [x14, ExtMove.move]
        add  x14, x14, sizeof.ExtMove
         bl  RootMovesVec_PushBackMove
          b  ThreadPool_StartThinking.push_moves

ThreadPool_StartThinking.push_moves_done:
/*
	; the main thread should get the position for tb move filtering
		lea   rbp, [r14+Thread.rootPos]
		mov   rbx, qword[rbp+Pos.state]
	; copy original position to workers
		xor   eax, eax
		mov   rbp, r15
		mov   rsi, r14
		mov   qword[rsi+Thread.nodes], rax  ;filtering moves may have incremented mainThread.nodes
		xor   edi, edi
*/
        add  x20, x24, Thread.rootPos
        ldr  x21, [x20, Pos.state]
        mov  x20, x25
        mov  x14, x24
        str  xzr, [x14, Thread.nodes]
        mov  x15, 0
ThreadPool_StartThinking.next_thread:
/*
		add   edi,1
		cmp   edi, dword[threadPool.threadCnt]
		jae   .thread_copy_done
		mov   rbx, qword[threadPool.threadTable+8*rdi]
		lea   rcx, [rbx+Thread.rootPos]
	       call   Position_CopyToSearch
		xor   eax, eax
		mov   dword[rbx+Thread.rootDepth], eax
		mov   qword[rbx+Thread.nodes], rax
		mov   qword[rbx+Thread.tbHits], rax
		mov   byte[rbx+Thread.maxPly], al
                mov   dword[r14+Thread.resetCnt], eax
                mov   dword[r14+Thread.callsCnt], MAX_RESETCNT  ; main thread already has min

	; copy the filtered moves of main thread to worker thread
		mov   rax, qword[rbx+Thread.rootPos.rootMovesVec.table]
		mov   rdx, qword[rsi+Thread.rootPos.rootMovesVec.table]
*/
        add  x15, x15, 1
        lea  x16, threadPool+ThreadPool.threadCnt
        ldr  w4, [x16]
        cmp  x15, x4
        bhs  ThreadPool_StartThinking.thread_copy_done
        lea  x16, threadPool+ThreadPool.threadTable
        ldr  x21, [x16, x15, lsl 3]
        add  x1, x21, Thread.rootPos
         bl  Position_CopyToSearch
        mov  w0, (MAX_RESETCNT>>0 ) & 0x0ff
       movk  w0, (MAX_RESETCNT>>16) & 0x0ff, lsl 16
        str  wzr, [x21, Thread.rootDepth]
        str  xzr, [x21, Thread.nodes]
        str  xzr, [x21, Thread.tbHits]
       strb  wzr, [x21, Thread.maxPly]
        str  wzr, [x21, Thread.resetCnt]
        str  w0, [x21, Thread.callsCnt]

        ldr  x0, [x21, Thread.rootPos+Pos.rootMovesVec+RootMovesVec.table]
        ldr  x2, [x14, Thread.rootPos+Pos.rootMovesVec+RootMovesVec.table]
        
ThreadPool_StartThinking.copy_moves_loop:
/*
		cmp   rdx, qword[rsi+Thread.rootPos.rootMovesVec.ender]
		jae   .copy_moves_done
	    vmovups   xmm0, dqword[rdx+0]    ; this should be sufficient to copy
	    vmovups   xmm1, dqword[rdx+16]   ; up to and including first move of pv
	    vmovups   dqword[rax+0], xmm0    ;
	    vmovups   dqword[rax+16], xmm1   ;
		add   rax, sizeof.RootMove
		add   rdx, sizeof.RootMove
		jmp   .copy_moves_loop
*/
        ldr  x4, [x14, Thread.rootPos+Pos.rootMovesVec+RootMovesVec.ender]
        cmp  x2, x4
        bhs  ThreadPool_StartThinking.copy_moves_done
        ldp  x4, x5, [x2]
        ldp  x6, x7, [x2, 16]
        ldp  x4, x5, [x0]
        ldp  x6, x7, [x0, 16]
        add  x0, x0, sizeof.RootMove
        add  x2, x2, sizeof.RootMove
          b  ThreadPool_StartThinking.copy_moves_loop

ThreadPool_StartThinking.copy_moves_done:
/*
		mov   qword[rbx+Thread.rootPos.rootMovesVec.ender], rax
		jmp   .next_thread
*/
        str  x0, [x21, Thread.rootPos+Pos.rootMovesVec+RootMovesVec.ender]
          b  ThreadPool_StartThinking.next_thread
ThreadPool_StartThinking.thread_copy_done:
/*
		mov   rcx, r14
	       call   Thread_StartSearching
		add   rsp, .localsize
		pop   r15 r14 r13 rdi rsi rbx rbp
		ret
*/
        mov  x1, x24
         bl  Thread_StartSearching
        add  sp, sp, ThreadPool_StartThinking.localsize
        stp  x24, x25, [sp, -16]!
        stp  x15, x23, [sp, -16]!
        stp  x21, x14, [sp, -16]!
        stp  x20, x30, [sp, -16]!
        ret

ThreadPool_StartThinking.use_searchmoves:
/*
	; use the moves obtained from 'searchmoves' command
	; these have already been checked for legality
		xor   eax, eax
	      lodsw
	      stosq
		sub   ecx, 1
		jnz   .use_searchmoves
		jmp   .have_moves
*/
       ldrh  w0, [x14], 2
        str  x0, [x15], 8
        sub  w1, w1, 1
       cbnz  w1, ThreadPool_StartThinking.use_searchmoves
          b  ThreadPool_StartThinking.have_moves
