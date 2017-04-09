SetCheckInfo:
/*
	; in: rbp  address of Pos
	;     rbx  address of State

	       push   rsi rdi r12 r13 r14 r15
*/
SetCheckInfo.AfterPrologue:
/*
		mov   esi, dword[rbp+Pos.sideToMove]
		mov   r15, qword[rbp+Pos.typeBB+8*rsi]
		xor   esi, 1
		mov   r14, qword[rbp+Pos.typeBB+8*rsi]
		shl   esi, 6+3
		mov   r13, r15		; r13 = our pieces
		mov   r12, r14		; r12 = their pieces
		mov   rdi, r15
		 or   rdi, r14		; rdi = all pieces
		and   r15, qword[rbp+Pos.typeBB+8*King]
		and   r14, qword[rbp+Pos.typeBB+8*King]
		bsf   r15, r15		; r15 = our king
		bsf   r14, r14		; r14 = their king
*/
        ldr  w16, [x20, Pos.sideToMove]
        ldr  x4, [x20, 8*King]
        ldr  x13, [x20, x16, lsl 3]
        eor  x16, x16, 1
        ldr  x12, [x20, x16, lsl 3]
        and  x14, x12, x4
        and  x15, x13, x4
        eor  x17, x12, x13
       rbit  x14, x14
       rbit  x15, x15
        clz  x14, x14
        clz  x15, x15

SetCheckInfo.go:
/*
ProfileInc SetCheckInfo

		mov   byte[rbx+State.ksq], r14l

		mov   rax, qword[WhitePawnAttacks+rsi+8*r14]
		mov   rdx, qword[KnightAttacks+8*r14]
		mov   qword[rbx+State.checkSq+8*Pawn], rax
		mov   qword[rbx+State.checkSq+8*Knight], rdx
		shr   esi, 6+3
      BishopAttacks   rax, r14, rdi, r8
	RookAttacks   rdx, r14, rdi, r8
		xor   r11, r11
		mov   qword[rbx+State.checkSq+8*Bishop], rax
		mov   qword[rbx+State.checkSq+8*Rook], rdx
		 or   rax, rdx
		mov   qword[rbx+State.checkSq+8*Queen], rax
		mov   qword[rbx+State.checkSq+8*King], r11
*/
        lea  x6, KnightAttacks
        add  x7, x6, WhitePawnAttacks-KnightAttacks
        add  x7, x7, x16, lsl 6+3
        add  x10, x21, x16, lsl 3
       strb  w14, [x21, State.ksq]
        ldr  x0, [x7, x14, lsl 3]
        ldr  x2, [x6, x14, lsl 3]
        str  x0, [x21, State.checkSq+8*Pawn]
        str  x2, [x21, State.checkSq+8*Knight]
        BishopAttacks x0, x14, x17, x8, x4
        RookAttacks x2, x14, x17, x8, x4
        mov  x11, 0
        str  x0, [x21, State.checkSq+8*Bishop]
        str  x2, [x21, State.checkSq+8*Rook]
        orr  x0, x0, x2
        str  x0, [x21, State.checkSq+8*Queen]
        str  xzr, [x21, State.checkSq+8*King]
/*
	; for their king
		xor   eax, eax
     SliderBlockers   rax, r13, r14, r11,\
		      rdi, r12,\
		      rcx, rdx, r8, r9
		mov   qword[rbx+State.pinnersForKing+8*rsi], r11
		mov   qword[rbx+State.blockersForKing+8*rsi], rax
		and   rax, r13
		mov   qword[rbx+State.dcCandidates], rax
*/
        mov  x0, 0
        ldr  x3, [x20, 8*Bishop]
        ldr  x4, [x20, 8*Rook]
        SliderBlockers  x0, x13, x14, x11, x17, x12, x1, x2, x8, x9, x3, x4, x5
        str  x11, [x10, State.pinnersForKing]
        str  x0, [x10, State.blockersForKing]
        and  x0, x0, x13
        str  x0, [x21, State.dcCandidates]
/*
	; for our king
		xor   r11, r11
		xor   esi, 1
		xor   eax, eax
     SliderBlockers   rax, r12, r15, r11,\
		      rdi, r13,\
		      rcx, rdx, r8, r9
		mov   qword[rbx+State.pinnersForKing+8*rsi], r11
		mov   qword[rbx+State.blockersForKing+8*rsi], rax
		and   rax, r13
		mov   qword[rbx+State.pinned], rax

		pop   r15 r14 r13 r12 rdi rsi
		ret
*/
        eor  x16, x16, 1
        add  x10, x21, x16, lsl 3
        mov  x11, 0
        mov  x0, 0
//Display "calling SliderBlockers\n"
//Display "result = %x0\n"
//Display "sliders = %x12\n"
//Display "sq = %x15\n"
//Display "pinners = %x11\n"
//Display "pieces = %x17\n"
//Display "pieces_color = %x13\n"
        SliderBlockers  x0, x12, x15, x11, x17, x13, x1, x2, x8, x9, x3, x4, x5
//Display "SliderBlockers returned  %x0 %x11\n"

        str  x11, [x10, State.pinnersForKing]
        str  x0, [x10, State.blockersForKing]
        and  x0, x0, x13
        str  x0, [x21, State.pinned]
        ret

