
Move_Undo:
/*
	; in: rbp  address of Pos
	;     rbx  address of State
	;     ecx  move

	       push   rsi

		mov   esi, dword[rbp+Pos.sideToMove]
		xor   esi, 1

		mov   r8d, ecx
		shr   r8d, 6
		and   r8d, 63	; r8d = from
		mov   r9d, ecx
		and   r9d, 63	; r9d = to
		shr   ecx, 12

	      movzx   r11d, byte[rbx+State.capturedPiece]      ; r11 = TO PIECE
	      movzx   r10d, byte[rbp+Pos.board+r9]	       ; r10 = FROM PIECE

		xor   edx, edx
		bts   rdx, r8
		bts   rdx, r9

		mov   eax, r10d
		and   eax, 7
*/
        ldr  w16, [x20, Pos.sideToMove]
        eor  x16, x16, 1
       ubfx  x8, x1, 6, 6
        and  x9, x1, 63
        lsr  x1, x1, 12

        add  x7, x20, Pos.board
       ldrb  w11, [x21, State.capturedPiece]
       ldrb  w10, [x7, x9]

        mov  x4, 1
        lsl  x2, x4, x8
        lsl  x4, x4, x9
        orr  x2, x2, x4

        and  x0, x10, 7
/*
		sub   rbx, sizeof.State
		mov   qword[rbp+Pos.state], rbx
		mov   dword[rbp+Pos.sideToMove], esi
		mov   byte[rbp+Pos.board+r8], r10l
		mov   byte[rbp+Pos.board+r9], r11l
		xor   qword[rbp+Pos.typeBB+8*rax], rdx
		xor   qword[rbp+Pos.typeBB+8*rsi], rdx

		cmp   ecx, MOVE_TYPE_PROM
		jae   .Special
*/
        sub  x21, x21, sizeof.State

        str  x21, [x20, Pos.state]
        str  w16, [x20, Pos.sideToMove]
        add  x7, x20, Pos.board
       strb  w10, [x7, x8]
       strb  w11, [x7, x9]
        ldr  x4, [x20, x0, lsl 3]
        eor  x4, x4, x2
        str  x4, [x20, x0, lsl 3]
        ldr  x4, [x20, x16, lsl 3]
        eor  x4, x4, x2
        str  x4, [x20, x16, lsl 3]

        cmp  x1, MOVE_TYPE_PROM
        bhs  Move_Undo.Special
/*
	      movzx   eax, byte[rbp+Pos.pieceIdx+r9]
		mov   byte[rbp+Pos.pieceList+rax], r8l
		mov   byte[rbp+Pos.pieceIdx+r8], al
		mov   eax, r11d 		; save a copy of captured piece

		and   r11d, 7
		jnz   .Captured

		pop   rsi
		ret
*/
        add  x7, x20, Pos.pieceIdx
       ldrb  w0, [x7, x9]
        add  x7, x20, Pos.pieceList
       strb  w8, [x7, x0]
        add  x7, x20, Pos.pieceIdx
       strb  w0, [x7, x8]
        mov  x0, x11
        and  x11, x11, 7
       cbnz  x11, Move_Undo.Captured
        ret

Move_Undo.Captured:
/*
		xor   esi, 1
		btr   rdx, r8
		 or   qword[rbp+Pos.typeBB+8*r11], rdx
		 or   qword[rbp+Pos.typeBB+8*rsi], rdx

	      movzx   ecx, byte[rbp+Pos.pieceEnd+rax]
		mov   byte[rbp+Pos.pieceIdx+r9], cl
		mov   byte[rbp+Pos.pieceList+rcx], r9l
		add   ecx, 1
		mov   byte[rbp+Pos.pieceEnd+rax], cl

		pop   rsi
		ret
*/
        eor  x16, x16, 1
        mov  x4, 1
        lsl  x4, x4, x8
        bic  x2, x2, x4
        ldr  x4, [x20, x11, lsl 3]
        orr  x4, x4, x2
        str  x4, [x20, x11, lsl 3]
        ldr  x4, [x20, x16, lsl 3]
        orr  x4, x4, x2
        str  x4, [x20, x16, lsl 3]

        add  x7, x20, Pos.pieceEnd
       ldrb  w1, [x7, x0]
        add  x7, x20, Pos.pieceIdx
       strb  w1, [x7, x9]
        add  x7, x20, Pos.pieceList
       strb  w9, [x7, x1]
        add  x1, x1, 1
        add  x7, x20, Pos.pieceEnd
       strb  w1, [x7, x0]
        ret


Move_Undo.Special:
/*
		xor   edx, edx
		cmp   ecx, MOVE_TYPE_EPCAP
		 ja   .Castle

	      movzx   eax, byte[rbp+Pos.pieceIdx+r9]
		mov   byte[rbp+Pos.pieceList+rax], r8l
		mov   byte[rbp+Pos.pieceIdx+r8], al
		mov   eax, r11d 		; save a copy of captured piece

		 je   .EpCapture
*/
        mov  x2, 0
        cmp  x1, MOVE_TYPE_EPCAP
        bhi  Move_Undo.Castle
        add  x7, x20, Pos.pieceIdx
       ldrb  w0, [x7, x9]
        add  x7, x20, Pos.pieceList
       strb  w8, [x7, x0]
        add  x7, x20, Pos.pieceIdx
       strb  w0, [x7, x8]
        mov  x0, x11
        beq  Move_Undo.EpCapture

Move_Undo.Prom:
/*
	; change promoted piece back to pawn on r8d
		lea   eax, [8*rsi+Pawn]
		lea   ecx, [rcx-MOVE_TYPE_PROM+Knight]
		bts   rdx, r8
		 or   qword[rbp+Pos.typeBB+8*Pawn], rdx
		xor   qword[rbp+Pos.typeBB+8*rcx], rdx
		mov   byte[rbp+Pos.board+r8], al
		mov   byte[rbp+Pos.board+r9], r11l
*/
        mov  x0, Pawn
        add  x0, x0, x16, lsl 3
        add  x1, x1, -MOVE_TYPE_PROM + Knight
        mov  x4, 1
        lsl  x4, x4, x8
        orr  x2, x2, x4
        ldr  x4, [x20, 8*Pawn]
        orr  x4, x4, x2
        str  x4, [x20, 8*Pawn]
        ldr  x4, [x20, x1, lsl 3]
        eor  x4, x4, x2
        str  x4, [x20, x1, lsl 3]
        add  x7, x20, Pos.board
       strb  w0, [x7, x8]
       strb  w11, [x7, x9]
/*
	       push   rdi
		lea   ecx, [8*rsi+rcx]
	      movzx   edi, byte[rbp+Pos.pieceEnd+rcx]
		sub   edi, 1
	      movzx   edx, byte[rbp+Pos.pieceList+rdi]
	      movzx   eax, byte[rbp+Pos.pieceIdx+r8]
		mov   byte[rbp+Pos.pieceEnd+rcx], dil
		mov   byte[rbp+Pos.pieceIdx+rdx], al
		mov   byte[rbp+Pos.pieceList+rax], dl
		mov   byte[rbp+Pos.pieceList+rdi], 64
*/
        add  x1, x1, x16, lsl 3
        add  x7, x20, Pos.pieceEnd
       ldrb  w17, [x7, x1]
        sub  x17, x17, 1
        add  x7, x20, Pos.pieceList
       ldrb  w2, [x7, x17]
        add  x7, x20, Pos.pieceIdx
       ldrb  w0, [x7, x8]

        add  x7, x20, Pos.pieceEnd
       strb  w17, [x7, x1]
        add  x7, x20, Pos.pieceIdx
       strb  w0, [x7, x2]
        add  x7, x20, Pos.pieceList
       strb  w2, [x7, x0]
        mov  w4, 64
       strb  w4, [x7, x17]

        
/*
	      movzx   edx, byte[rbp+Pos.pieceEnd+8*rsi+Pawn]
		mov   byte[rbp+Pos.pieceIdx+r8], dl
		mov   byte[rbp+Pos.pieceList+rdx], r8l
		add   edx, 1
		mov   byte[rbp+Pos.pieceEnd+8*rsi+Pawn], dl
		mov   eax, r11d
		pop   rdi


		xor   edx, edx
		and   r11d, 7
		jnz   .PromCapture

		pop   rsi
		ret
*/
        add  x7, x20, x16, lsl 3
       ldrb  w2, [x7, Pos.pieceEnd + Pawn]
        add  x7, x20, Pos.pieceIdx
       strb  w2, [x7, x8]
        add  x7, x20, Pos.pieceList
       strb  w8, [x7, x2]
        add  x2, x2, 1
        add  x7, x20, x16, lsl 3
       strb  w2, [x7, Pos.pieceEnd + Pawn]
        mov  x0, x11

        mov  x2, 0
        and  x11, x11, 7
       cbnz  x11, Move_Undo.PromCapture
        ret

Move_Undo.PromCapture:
/*
		xor   esi, 1
		bts   rdx, r9
		 or   qword[rbp+Pos.typeBB+8*r11], rdx
		 or   qword[rbp+Pos.typeBB+8*rsi], rdx

	      movzx   ecx, byte[rbp+Pos.pieceEnd+rax]
		mov   byte[rbp+Pos.pieceIdx+r9], cl
		mov   byte[rbp+Pos.pieceList+rcx], r9l
		add   ecx, 1
		mov   byte[rbp+Pos.pieceEnd+rax], cl

		pop   rsi
		ret
*/
        eor  x16, x16, 1
        mov  x4, 1
        lsl  x4, x4, x9
        orr  x2, x2, x4
        ldr  x4, [x20, x11, lsl 3]
        orr  x4, x4, x2
        str  x4, [x20, x11, lsl 3]
        ldr  x4, [x20, x16, lsl 3]
        orr  x4, x4, x2
        str  x4, [x20, x16, lsl 3]

        add  x7, x20, Pos.pieceEnd
       ldrb  w1, [x7, x0]
        add  x7, x20, Pos.pieceIdx
       strb  w1, [x7, x9]
        add  x7, x20, Pos.pieceList
       strb  w9, [x7, x1]
        add  x1, x1, 1
        add  x7, x20, Pos.pieceEnd
       strb  w1, [x7, x0]
        ret

Move_Undo.EpCapture:
/*
	; place 8*rsi+Pawn on square rcx

		lea   ecx, [2*rsi-1]
		lea   ecx, [r9+8*rcx]
		xor   esi, 1
		bts   rdx, rcx
		 or   qword[rbp+Pos.typeBB+8*Pawn], rdx
		 or   qword[rbp+Pos.typeBB+8*rsi], rdx
		mov   byte[rbp+Pos.board+r9], 0
		mov   byte[rbp+Pos.board+rcx], r11l

	      movzx   eax, byte[rbp+Pos.pieceEnd+r11]
		mov   byte[rbp+Pos.pieceIdx+rcx], al
		mov   byte[rbp+Pos.pieceList+rax], cl
		add   eax, 1
		mov   byte[rbp+Pos.pieceEnd+r11], al
		pop   rsi
		ret
*/
        lsl  x1, x16, 1
        sub  x1, x1, 1
        add  x1, x9, x1, lsl 3
        eor  x16, x16, 1
        mov  x4, 1
        lsl  x4, x4, x1
        orr  x2, x2, x4
        ldr  x4, [x20, 8*Pawn]
        orr  x4, x4, x2
        str  x4, [x20, 8*Pawn]
        ldr  x4, [x20, x16, lsl 3]
        orr  x4, x4, x2
        str  x4, [x20, x16, lsl 3]
        add  x7, x20, Pos.board
       strb  wzr, [x7, x9]
       strb  w11, [x7, x1]
        add  x7, x20, Pos.pieceEnd
       ldrb  w0, [x7, x11]
        add  x7, x20, Pos.pieceIdx
       strb  w0, [x7, x1]
        add  x7, x20, Pos.pieceList
       strb  w1, [x7, x0]
        add  x0, x0, 1
        add  x7, x20, Pos.pieceEnd
       strb  w0, [x7, x11]
        ret

Move_Undo.Castle:
/*
	; r8 = kfrom
	; r9 = rfrom
	; ecx = kto
	; edx = rto
	; r10 = ourking
	; r11 = our rook
		bts   rdx, r8
		bts   rdx, r9
		xor   qword[rbp+Pos.typeBB+8*rax], rdx
		xor   qword[rbp+Pos.typeBB+8*rsi], rdx
*/
        mov  x4, 1
        lsl  x4, x4, x8
        orr  x2, x2, x4
        mov  x4, 1
        lsl  x4, x4, x9
        orr  x2, x2, x4
        ldr  x4, [x20, x0, lsl 3]
        eor  x4, x4, x2
        ldr  x4, [x20, x0, lsl 3]
        ldr  x4, [x20, x16, lsl 3]
        eor  x4, x4, x2
        ldr  x4, [x20, x16, lsl 3]
/*
		lea   r10d, [8*rsi+King]
		lea   r11d, [8*rsi+Rook]
		mov   edx, r8d
		and   edx, 56
		cmp   r9d, r8d
		sbb   eax, eax
		lea   ecx, [rdx+4*rax+FILE_G]
		lea   edx, [rdx+2*rax+FILE_F]

		mov   byte[rbp+Pos.board+rcx], 0
		mov   byte[rbp+Pos.board+rdx], 0
		mov   byte[rbp+Pos.board+r8], r10l
		mov   byte[rbp+Pos.board+r9], r11l
*/
        mov  x10, King
        add  x10, x10, x16, lsl 3
        mov  x11, Rook
        add  x11, x11, x16, lsl 3
        and  x2, x8, 56
        cmp  x9, x8
       cset  x0, hi
        add  x1, x2, x0, lsl 2
        add  x2, x2, x0, lsl 1
        add  x1, x1, FILE_C
        add  x2, x2, FILE_D
        add  x7, x20, Pos.board
       strb  wzr, [x7, x1]
       strb  wzr, [x7, x2]
       strb  w10, [x7, x8]
       strb  w11, [x7, x9]
/*
	       push   rdi

	      movzx   eax, byte[rbp+Pos.pieceIdx+rcx]
	      movzx   edi, byte[rbp+Pos.pieceIdx+rdx]
		mov   byte[rbp+Pos.pieceList+rax], r8l
		mov   byte[rbp+Pos.pieceList+rdi], r9l
		mov   byte[rbp+Pos.pieceIdx+r8], al
		mov   byte[rbp+Pos.pieceIdx+r9], dil
*/
        add  x7, x20, Pos.pieceIdx
       ldrb  w0, [x7, x1]
       ldrb  w17, [x7, x2]
        add  x7, x20, Pos.pieceList
       strb  w8, [x7, x0]
       strb  w9, [x7, x17]
        add  x7, x20, Pos.pieceIdx
       strb  w0, [x7, x8]
       strb  w17, [x7, x9]
/*
	; now move rook to the back of the list
	      movzx   eax, byte[rbp+Pos.pieceEnd+r11]
		sub   eax, 1
	      movzx   r10d, byte[rbp+Pos.pieceList+rax]
	       ;;xchg   byte[rbp+Pos.pieceList+rdi], byte[rbp+Pos.pieceList+rax]
	      movzx   r9d, byte[rbp+Pos.pieceList+rdi]
	      movzx   r11d, byte[rbp+Pos.pieceList+rax]
		mov   byte[rbp+Pos.pieceList+rdi], r11l
		mov   byte[rbp+Pos.pieceList+rax], r9l
	       ;;xchg   byte[rbp+Pos.pieceIdx+rdx], byte[rbp+Pos.pieceIdx+r12]
	      movzx   edi, byte[rbp+Pos.pieceIdx+r9]
	      movzx   r11d, byte[rbp+Pos.pieceIdx+r10]
		mov   byte[rbp+Pos.pieceIdx+r9], r11l
		mov   byte[rbp+Pos.pieceIdx+r10], dil
		pop   rdi
*/
        add  x7, x20, Pos.pieceEnd
       ldrb  w0, [x7, x11]
        sub  w0, w0, 1
        add  x7, x20, Pos.pieceList
       ldrb  w10, [x7, x0]
       ldrb  w9, [x7, x17]
       ldrb  w11, [x7, x0]
       strb  w11, [x7, x17]
       strb  w9, [x7, x0]
        add  x7, x20, Pos.pieceIdx
       ldrb  w17, [x7, x9]
       ldrb  w11, [x7, x10]
       strb  w11, [x7, x9]
       strb  w17, [x7, x10]
/*
		mov   rax, qword[rbp+Pos.typeBB+8*rsi]
		mov   r10, qword[rbp+Pos.typeBB+8*King]
		mov   r11, qword[rbp+Pos.typeBB+8*Rook]
*/
        ldr  x0, [x20, x16, lsl 3]
        ldr  x10, [x20, 8*King]
        ldr  x11, [x20, 8*Rook]        
//		btr   rax, rcx
        mov  x4, 1
        lsl  x4, x4, x1
        bic  x0, x0, x4
//		btr   rax, rdx
        mov  x4, 1
        lsl  x4, x4, x2
        bic  x0, x0, x4
//		bts   rax, r8
        mov  x4, 1
        lsl  x4, x4, x8
        orr  x0, x0, x4
//		bts   rax, r9
        mov  x4, 1
        lsl  x4, x4, x9
        orr  x0, x0, x4
//		btr   r10, rcx
        mov  x4, 1
        lsl  x4, x4, x1
        bic  x10, x10, x4
//		bts   r10, r8
        mov  x4, 1
        lsl  x4, x4, x8
        orr  x10, x10, x4
//		btr   r11, rdx
        mov  x4, 1
        lsl  x4, x4, x2
        bic  x11, x11, x4

//		bts   r11, r9
        mov  x4, 1
        lsl  x4, x4, x9
        orr  x11, x11, x4
/*
		mov   qword[rbp+Pos.typeBB+8*rsi], rax
		mov   qword[rbp+Pos.typeBB+8*King], r10
		mov   qword[rbp+Pos.typeBB+8*Rook], r11

		pop   rsi
		ret
*/
        str  x0, [x20, x16, lsl 3]
        str  x10, [x20, 8*King]
        str  x11, [x20, 8*Rook]
        ret
