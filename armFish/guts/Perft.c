
Perft_Root:
/*
	       push   rbx rsi rdi r14 r15
virtual at rsp
 .time	   dq ?
 .movelist rb sizeof.ExtMove*MAX_MOVES
 .lend	   rb 0
end virtual
.localsize = ((.lend-rsp+15) and (-16))

	 _chkstk_ms   rsp, .localsize
		sub   rsp, .localsize
*/
Perft_Root.time = 0
Perft_Root.movelist = 8 + Perft_Root.time
Perft_Root.localsize = sizeof.ExtMove*MAX_MOVES + Perft_Root.movelist
Perft_Root.localsize = (Perft_Root.localsize + 15) & -16

        stp  x21, x30, [sp, -16]!
        stp  x28, x29, [sp, -16]!
        stp  x24, x25, [sp, -16]!
        stp  x26, x27, [sp, -16]!
        sub  sp, sp, Perft_Root.localsize
/*
		mov   rbx, qword[rbp+Pos.state]
		mov   r15d, ecx
		xor   r14, r14

	       call   _GetTime
		mov   qword[.time], rax

	       call   SetCheckInfo

		lea   rdi, [.movelist]
		mov   rsi, rdi
	       call   Gen_Legal
		xor   eax, eax
		mov   dword[rdi], eax
*/
        ldr  x21, [x20, Pos.state]
        mov  x25, x1
        mov  x24, 0
         bl  Os_GetTime
        str  x0, [sp, Perft_Root.time]
         bl  SetCheckInfo
        add  x27, sp, Perft_Root.movelist
        mov  x26, x27
         bl  Gen_Legal
        str  wzr, [x27]

Perft_Root.MoveLoop:
/*
		mov   ecx, dword[rsi]
	       test   ecx, ecx
		 jz   .MoveLoopDone
		mov   ecx, dword[rsi]
	       call   Move_GivesCheck
		mov   edx, eax
		mov   ecx, dword[rsi]
	       call   Move_Do__PerftGen_Root
		mov   eax, 1
		lea   ecx, [r15-1]
		cmp   r15d, 1
		jbe   @f
	       call   Perft_Branch
	@@:	add   r14, rax
	       push   rax
		mov   ecx, dword[rsi]
	       call   Move_Undo
*/
        ldr  w1, [x26]
        cbz  w1, Perft_Root.MoveLoopDone
        ldr  w1, [x26]
         bl  Move_GivesCheck
        ldr  w1, [x26]        
       strb  w0, [x21, State.givesCheck]
         bl  Move_Do__PerftGen_Root
        mov  x0, 1
       subs  x1, x25, 1
        bls  1f
         bl  Perft_Branch
1:      add  x24, x24, x0
        mov  x29, x0
        ldr  w1, [x26]        
         bl  Move_Undo
         
/*
		lea   rdi, [Output]
		mov   ecx, dword[rsi]
		mov   edx, dword[rbp+Pos.chess960]
	       call   PrintUciMove
		mov   eax, ' :  '
	      stosd
		pop   rax
	       call   PrintUnsignedInteger
       PrintNewLine
	       call   _WriteOut_Output

		add   rsi, sizeof.ExtMove
		jmp   .MoveLoop
*/
        lea  x27, Output
        ldr  w1, [x26]
        ldr  w2, [x20, Pos.chess960]
         bl  PrintUciMove
        mov  w0, ' '
       strb  w0, [x27], 1
        mov  w0, ':'
       strb  w0, [x27], 1
        mov  w0, ' '
       strb  w0, [x27], 1
        mov  w0, ' '
       strb  w0, [x27], 1
        mov  x0, x29
         bl  PrintUInt
        PrintNewLine
         bl  Os_WriteOut_Output
        add  x26, x26, sizeof.ExtMove
          b  Perft_Root.MoveLoop

Perft_Root.MoveLoopDone:
/*
	       call   _GetTime
		sub   rax, qword[.time]
		mov   qword[.time], rax

		lea   rdi, [Output]
		mov   al, '='
		mov   ecx, 27
	  rep stosb
       PrintNewLine

		mov   rax, 'Total ti'
	      stosq
		mov   rax, 'me (ms) '
	      stosq
		mov   ax, ': '
	      stosw
		mov   rax, qword[.time]
	       call   PrintUnsignedInteger
       PrintNewLine

		mov   rax, 'Nodes se'
	      stosq
		mov   rax, 'arched  '
	      stosq
		mov   ax, ': '
	      stosw
		mov   rax, r14
	       call   PrintUnsignedInteger
       PrintNewLine

		mov   rax, 'Nodes/se'
	      stosq
		mov   rax, 'cond    '
	      stosq
		mov   ax, ': '
	      stosw

		mov   rax, r14
		mov   ecx, 1000
		mul   rcx
		mov   rcx, qword[.time]
		cmp   rcx, 1
		adc   rcx, 0
		div   rcx
	       call   PrintUnsignedInteger
       PrintNewLine

	       call   _WriteOut_Output
*/
         bl  Os_GetTime
        lea  x27, Output
        ldr  x4, [sp, Perft_Root.time]
        sub  x1, x0, x4
        str  x1, [sp, Perft_Root.time]
        mov  x2, 1000
        cmp  x1, 0
       cinc  x1, x1, eq
      scvtf  d0, x24
      scvtf  d1, x1
      scvtf  d2, x2
       fmul  d0, d0, d2
       fdiv  d0, d0, d1
     fcvtzs  x2, d0
        stp  x1, x24, [sp, -32]!
        str  x2, [sp, 16]
        lea  x1, sz_bench_format3
        add  x2, sp, 0
        add  x3, sp, 0
         bl  PrintFancy
        add  sp, sp, 32
         bl  Os_WriteOut_Output

Perft_Root.Done:
/*
		add   rsp, .localsize
		pop   r15 r14 rdi rsi rbx
		ret
*/
        add  sp, sp, Perft_Root.localsize
        ldp  x26, x27, [sp], 16
        ldp  x24, x25, [sp], 16
        ldp  x28, x29, [sp], 16
        ldp  x21, x30, [sp], 16
        ret


Perft_Branch:
/*
	       push   rsi r14 r15
virtual at rsp
.movelist  rb sizeof.ExtMove*MAX_MOVES
.lend	   rb 0
end virtual
.localsize = ((.lend-rsp+15) and (-16))

	 _chkstk_ms   rsp, .localsize
		sub   rsp, .localsize

		lea   r15d, [rcx-1]
		xor   r14, r14
		lea   rdi, [.movelist]
		mov   rsi, rdi
		cmp   ecx, 1
		 ja   .DepthN
*/
Perft_Branch.movelist = 0
Perft_Branch.localsize = sizeof.ExtMove*MAX_MOVES + Perft_Branch.movelist
Perft_Branch.localsize = (Perft_Branch.localsize + 15) & -16

        stp  x26, x30, [sp, -16]!
        stp  x24, x25, [sp, -16]!
        sub  sp, sp, Perft_Branch.localsize

        sub  x25, x1, 1
       subs  x25, x1, 1
        mov  x24, 0
        add  x27, sp, Perft_Branch.movelist 
        add  x26, sp, Perft_Branch.movelist 
        bhi  Perft_Branch.DepthN

Perft_Branch.Depth1:
/*
	       call   Gen_Legal
		mov   rax, rdi
		sub   rax, rsi
		shr   eax, 3	      ; assume sizeof.ExtMove = 8
		add   rsp, .localsize
		pop   r15 r14 rsi
		ret
*/
         bl  Gen_Legal
        sub  x0, x27, x26
        lsr  x0, x0, 3

        add  sp, sp, Perft_Branch.localsize
        ldp  x24, x25, [sp], 16
        ldp  x26, x30, [sp], 16
        ret

Perft_Branch.DepthN:
/*
	       call   Gen_Legal
		xor   eax, eax
		mov   dword[rdi], eax

		mov   ecx, dword[rsi]
	       test   ecx, ecx
		 jz   .DepthNDone
*/
         bl  Gen_Legal
        str  wzr, [x27]
        ldr  w1, [x26]
        cbz  w1, Perft_Branch.DepthNDone
        
Perft_Branch.DepthNLoop:
/*
	       call   Move_GivesCheck
		mov   edx, eax
		mov   ecx, dword[rsi]
	       call   Move_Do__PerftGen_Branch
		mov   ecx, r15d
	       call   Perft_Branch
		add   r14, rax
		mov   ecx, dword[rsi]
		add   rsi, sizeof.ExtMove
	       call   Move_Undo
		mov   ecx, dword[rsi]
	       test   ecx, ecx
		jnz   .DepthNLoop
*/
         bl  Move_GivesCheck
        ldr  w1, [x26]
       strb  w0, [x21, State.givesCheck]
         bl  Move_Do__PerftGen_Branch
        mov  x1, x25
         bl  Perft_Branch
        add  x24, x24, x0
        ldr  w1, [x26]
        add  x26, x26, sizeof.ExtMove
         bl  Move_Undo
        ldr  w1, [x26]
       cbnz  w1, Perft_Branch.DepthNLoop

Perft_Branch.DepthNDone:
/*
		mov   rax, r14
		add   rsp, .localsize
		pop   r15 r14 rsi
		ret
*/
        mov  x0, x24
        add  sp, sp, Perft_Branch.localsize
        ldp  x24, x25, [sp], 16
        ldp  x26, x30, [sp], 16
        ret
