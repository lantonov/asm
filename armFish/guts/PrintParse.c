
StringLength:
// in:  x1 address of null terminated string
// out: x0 length
        mov  x0, -1
StringLength.Next:
        add  x0, x0, 1
       ldrb  w2, [x1, x0]
       cbnz  w2, StringLength.Next
        ret


PrintString.Next:
       strb  w0, [x27], 1
PrintString:
// in: x1 address of source string
// io: x27 string
       ldrb  w0, [x1], 1
       cbnz  w0, PrintString.Next
        ret


PrintFancy:
// in: x1 address of source string
//     x2 address of dword array
// io: x27 string with ie %x3 replaced by x1[3] ect
        stp  x29, x30, [sp, -16]!
        stp  x28, x26, [sp, -16]!
        stp  x24, x25, [sp, -16]!
        mov  x26, x1
        mov  x28, x2
        mov  x29, x3
PrintFancy.Loop:
       ldrb  w0, [x26], 1
        cmp  w0, '%'
        beq  PrintFancy.GotOne
        cbz  w0, PrintFancy.Done
       strb  w0, [x27], 1
          b  PrintFancy.Loop
PrintFancy.Done:
        ldp  x24, x25, [sp], 16
        ldp  x28, x26, [sp], 16
        ldp  x29, x30, [sp], 16
        ret
PrintFancy.GotOne:
       ldrb  w24, [x26], 1   
         bl  ParseInteger
        and  x0, x0, 31
        add  x1, x29, x0, lsl 4
        ld1  {V0.16b}, [x1]
        ldr  x0, [x28, x0, lsl 3]
        adr  x1, PrintHex
        adr  x2, PrintInt
        adr  x3, PrintUInt
        adr  x4, PrintDouble
        adr  x5, PrintDoubleVec
        adr  x6, PrintHexVec
        cmp  w24, 'i'
       csel  x1, x1, x2, ne
        cmp  w24, 'u'
       csel  x1, x1, x3, ne
        cmp  w24, 'd'
       csel  x1, x1, x4, ne
        cmp  w24, 'D'
       csel  x1, x1, x5, ne
        cmp  w24, 'X'
       csel  x1, x1, x6, ne
        blr  x1
          b  PrintFancy.Loop


CmpString:
// if beginning of string at x26 matches null terminated string at x1
// then advance x26 to end of match and return x0 = -1
// else return x0 = 0 and do nothing
        mov  x3, x26
CmpString.Next:
       ldrb  w0, [x1], 1
        cbz  w0, CmpString.Found
       ldrb  w2, [x3], 1
        cmp  w0, w2
        beq  CmpString.Next
CmpString.NoMatch:
        mov  x0, 0
        ret
CmpString.Found:
        mov  x26, x3
        mov  x0, -1
        ret


CmpStringCaseless:
// if beginning of string at x26 matches null terminated string at x1
// then advance x26 to end of match and return x0 = -1
// else return x0 = 0 and do nothing
        mov  x3, x26
CmpStringCaseless.Next:
       ldrb  w0, [x1], 1
        cbz  w0, CmpStringCaseless.Found
       ldrb  w2, [x3], 1
        ToLower w2
        cmp  w0, w2
        beq  CmpStringCaseless.Next
CmpStringCaseless.NoMatch:
        mov  x0, 0
        ret
CmpStringCaseless.Found:
        mov  x26, x3
        mov  x0, -1
        ret


SkipSpaces.Next:
        add  x26, x26, 1
SkipSpaces:
// io: x26
       ldrb  w0, [x26]
        cmp  w0, ' '
        beq  SkipSpaces.Next
        ret


ParseToken.Next:
        add  x26, x26, 1
       strb  w0, [x27], 1
ParseToken:
// write at most x1 characters of string at x26 to x27
       ldrb  w0, [x26]
       subs  x1, x1, 1
        blo  ParseToken.Done
        cmp  w0, '/'
        blo  ParseToken.Done
        cmp  w0, ':'
        blo  ParseToken.Next
        cmp  w0, 'A'
        blo  ParseToken.Done
        cmp  w0, '['
        blo  ParseToken.Next
        cmp  w0, '\'
        beq  ParseToken.Next
        cmp  w0, 'a'
        blo  ParseToken.Done
        cmp  w0, 128
        blo  ParseToken.Next
ParseToken.Done:
        ret
        


PrintUciMove:
/*
	       call   _PrintUciMove
		mov   qword[rdi], rax
		add   rdi, rdx
		ret
*/
        
        stp   x29, x30, [sp, -16]!
         bl  _PrintUciMove
        str  x0, [x27]
        add  x27, x27, x2
        ldp  x29, x30, [sp], 16
        ret

_PrintUciMove:
/*
	; in:  ecx  move
	;      edx  is chess960
	; out: rax  move string
	;      edx  byte length of move string  4 or 5 for promotions
		mov   r8d, ecx
		shr   r8d, 6
		and   r8d, 63	; r8d = from
		mov   r9d, ecx
		and   r9d, 63	; r9d = to
		mov   eax, 'NONE'
	       test   ecx, ecx
		 jz   .Return
		mov   eax, 'NULL'
		cmp   ecx, MOVE_NULL
		 jz   .Return

	; castling requires special attention
		cmp   r9d, r8d
		sbb   eax, eax
		mov   r10d, r9d
		and   r10d, 56
		lea   r10d, [r10+4*rax+FILE_G]
		shr   ecx, 12
		lea   eax, [ecx-MOVE_TYPE_CASTLE]
		 or   eax, edx
	      cmovz   r9d, r10d

		mov   edx, r9d
		and   r9d, 7
		and   edx, 56
		shl   edx, 5
		lea   eax, [rdx+r9+'a1']

		shl   eax, 16

		mov   edx, r8d
		and   r8d, 7
		and   edx, 56
		shl   edx, 5
		add   edx, r8d
		lea   eax, [rax+rdx+'a1']

		sub   ecx, MOVE_TYPE_PROM
		cmp   ecx, 4
		 jb   .Promotion
*/
       ubfx  x8, x1, 6, 6
        and  x9, x1, 63
        mov  w0, 'N' + ('O'<<8)
       movk  w0, 'N' + ('E'<<8), lsl 16
        cbz  x1, _PrintUciMove.Return
        mov  w0, 'N' + ('U'<<8)
       movk  w0, 'L' + ('L'<<8), lsl 16
        tst  w1, w1
        bmi  _PrintUciMove.Return
        cmp  x9, x8
       cset  x0, hi
        and  x10, x9, 56
        add  x10, x10, x0, lsl 2
        add  x10, x10, FILE_C
        lsr  x1, x1, 12
        cmp  x1, MOVE_TYPE_CASTLE
       ccmp  x2, 0, 0, eq
       csel  x9, x10, x9, eq
        
        mov  x0, 'a' + ('1'<<8)
        and  x2, x9, 56
        add  x0, x0, x2, lsl 5
        and  x9, x9, 7
        add  x0, x0, x9
        
        mov  x3, 'a' + ('1'<<8)
        and  x2, x8, 56
        add  x3, x3, x2, lsl 5
        and  x8, x8, 7
        add  x3, x3, x8

        add  x0, x3, x0, lsl 16
        sub  x1, x1, MOVE_TYPE_PROM
        cmp  x1, 4
        blo  _PrintUciMove.Promotion

_PrintUciMove.Return:
/*
		mov   edx, 4
		ret
*/
        mov  x2, 4
        ret
_PrintUciMove.Promotion:
/*
		shl   ecx, 3
		mov   edx, 'nbrq'
		shr   edx, cl
		and   edx, 0x0FF
		shl   rdx, 32
		 or   rax, rdx
		mov   edx, 5
		ret
*/
        adr  x3, _PrintUciMove.lookup
        ldr  w3, [x3, x1]
        add  x0, x0, x3, lsl 32
        mov  x2, 5
        ret

_PrintUciMove.lookup:
        .ascii "nbrq"


ParseUciMove:

/*
	; if string at rsi is a legal move, it is return in eax and rsi is advanced,
	;   othersize MOVE_NONE (0) is return and rsi is unchanged

	       push   rbx rdi rsi
virtual at rsp
  .moveList    rb sizeof.ExtMove*MAX_MOVES
  .lend rb 0
end virtual
.localsize = ((.lend-rsp+15) and (-16))

	 _chkstk_ms   rsp, .localsize
		sub   rsp, .localsize
*/
ParseUciMove.moveList = 0
ParseUciMove.localsize = sizeof.ExtMove*MAX_MOVES
ParseUciMove.localsize = (ParseUciMove.localsize+15) & -16

        stp  x21, x30, [sp, -16]!
        stp  x26, x27, [sp, -16]!
        sub  sp, sp, ParseUciMove.localsize
/*
		lea   rdi, [.moveList]
		mov   rbx, qword[rbp+Pos.state]
	       call   SetCheckInfo
	       call   Gen_Legal
		xor   eax, eax
	      stosd

		mov   ebx, dword[rsi]
	      movzx   eax, byte[rsi+4]
	    ToLower   eax
		mov   edx, ' '
		sub   edx, eax
		adc   rsi, 4
		sar   edx, 31
		and   eax, edx
		shl   rax, 32
		 or   rbx, rax
		lea   rdi, [.moveList-sizeof.ExtMove]
.CheckNext:
		add   rdi, sizeof.ExtMove
		mov   ecx, dword[rdi+ExtMove.move]
		xor   eax, eax
	       test   ecx, ecx
		 jz   .Failed
		mov   edx, dword[rbp+Pos.chess960]
	       call   _PrintUciMove	   ; string result is in rax
		cmp   rax, rbx
		jne   .CheckNext

		mov   eax, dword[rdi+ExtMove.move]
		add   rsp, .localsize
		pop   rdx rdi rbx	   ; move found - keep advanced value of rsi
		ret

.Failed:
		add   rsp, .localsize
		pop   rsi rdi rbx
		ret
*/

        add  x27, sp, ParseUciMove.moveList
        ldr  x21, [x20, Pos.state]
         bl  SetCheckInfo
         bl  Gen_Legal
        str  xzr, [x27]
        ldr  w21, [x26], 4
       ldrb  w0, [x26]
        ToLower w0
        cmp  w0, ' '
       cinc  x26, x26, hi
       csel  w0, w0, wzr, hi
        add  x21, x21, x0, lsl 32
        add  x27, sp, ParseUciMove.moveList-sizeof.ExtMove
ParseUciMove.CheckNext:
        add  x27, x27, sizeof.ExtMove
        ldr  w1, [x27, ExtMove.move]
        cbz  w1, ParseUciMove.Failed
        ldr  w2, [x20, Pos.chess960]
         bl  _PrintUciMove
        cmp  x0, x21
        bne  ParseUciMove.CheckNext
        ldr  w0, [x27, ExtMove.move]
        add  sp, sp, ParseUciMove.localsize
        ldp  x2, x27, [sp], 16
        ldp  x21, x30, [sp], 16
        ret
ParseUciMove.Failed:
        mov  w0, 0
        add  sp, sp, ParseUciMove.localsize
        ldp  x26, x27, [sp], 16
        ldp  x21, x30, [sp], 16
        ret


PrintSquare:
/*
		mov   al,'-'
		cmp   ecx, 64
		jae   .none
		mov   eax, ecx
		and   eax, 7
		add   eax, 'a'
	      stosb
		mov   eax, ecx
		shr   eax, 3
		add   eax, '1'
.none:
	      stosb
		ret
*/
PrintSquare:
        mov  w0, '-'
        cmp  x1, 64
        bhs  PrintSquare.none
        and  x0, x1, 7
        add  x0, x0, 'a'
       strb  w0, [x27], 1
        lsr  x0, x1, 3
        add  x0, x0, '1'
PrintSquare.none:
       strb  w0, [x27], 1
        ret        

ParseSquare:
/*
	; if string at rsi is a square return it
	;    else return 65 and don't change rsi
		mov   rdx, rsi
		xor   eax, eax
	      lodsb
		mov   ecx, eax
		cmp   al, '-'
		 je   .none
		sub   ecx, 'a'
		 js   .error
		cmp   ecx, 8
		jae   .error
		xor   eax, eax
	      lodsb
		sub   eax, '1'
		 js   .error
		cmp   eax, 8
		jae   .error
		lea   eax, [rcx+8*rax]
		ret
.none:
		mov   eax, 64
		ret
.error:
		mov   rsi, rdx
		mov   eax, 65
		ret
*/
        mov  x2, x26
       ldrb  w0, [x26], 1
        cmp  x0, '-'
        beq  ParseSquare.none
       subs  x1, x0, 'a'
        bmi  ParseSquare.error
        cmp  x1, 8
        bhs  ParseSquare.error
       ldrb  w0, [x26], 1
       subs  x0, x0, '1'
        bmi  ParseSquare.error
        cmp  x0, 8
        bhs  ParseSquare.error
        add  x0, x1, x0, lsl 3
        ret
ParseSquare.none:
        mov  x0, 64
        ret
ParseSquare.error:
        mov  x26, x2
        mov  x0, 65
        ret


PrintHex:
// in: x0
// io: x27 string
        mov  w4, 16                
PrintHex.Next:
        ror  x0, x0, 64-4
        and  x1, x0, 15
        cmp  w1, 10
        add  w2, w1, '0'
        add  w3, w1, 'a'-10
       csel  w1, w2, w3, lo
       strb  w1, [x27], 1                
        sub  w4, w4, 1
       cbnz  w4, PrintHex.Next
        ret


PrintHexVec:
// in: v0 double
// io: x27 string
        stp  x29, x30, [sp, -16]!
        sub  sp, sp, 16
        st1  {v0.16b}, [sp]
        ldr  x0, [sp, 0]
         bl  PrintHex
        mov  w0, '_'
       strb  w0, [x27], 1
        ldr  x0, [sp, 8]
         bl  PrintHex
        add  sp, sp, 16       
        ldp  x29, x30, [sp], 16
        ret


PrintDoubleVec:
// in: v0 double
// io: x27 string
        stp  x29, x30, [sp, -16]!
        sub  sp, sp, 16
        st1  {v0.16b}, [sp]
        mov  w0, '('
       strb  w0, [x27], 1
        ldr  d0, [sp, 0]
         bl  PrintDouble
        mov  w0, ','
       strb  w0, [x27], 1
        ldr  d0, [sp, 8]
         bl  PrintDouble
        mov  w0, ')'
       strb  w0, [x27], 1
        add  sp, sp, 16       
        ldp  x29, x30, [sp], 16
        ret


PrintDouble:
// in: d0 double
// io: x27 string
        stp  x29, x30, [sp, -16]!
        mov  x1, 1000
      scvtf  d1, x1
       fmul  d0, d0, d1
     fcvtzs  x2, d0
        tst  x2, x2
        mov  w4, '-'
       strb  w4, [x27]
       cinc  x27, x27, lt
       cneg  x2, x2, lt
       sdiv  x0, x2, x1
       msub  x29, x0, x1, x2
         bl  PrintUInt
        mov  w4, '.'
       strb  w4, [x27], 1
        mov  x2, 100
       udiv  x4, x29, x2
       msub  x3, x4, x2, x29
        mov  x0, 10
        add  w4, w4, '0'
       strb  w4, [x27], 1
       udiv  x2, x3, x0
        add  w4, w2, '0'
       strb  w4, [x27], 1
       msub  x2, x2, x0, x3
        add  w2, w2, '0'
       strb  w2, [x27], 1
        ldp  x29, x30, [sp], 16
        ret


PrintInt:
// in: x0 signed integer
// io: x27 string
        tst  x0, x0
        mov  w1, '-'
       strb  w1, [x27]
       cinc  x27, x27, lt
       cneg  x0, x0, lt

PrintUInt:
// in: x0 unsigned integer
// io: x27 string
        sub  sp, sp, 64
        mov  x3, 0
        mov  x2, 10
PrintUInt.PushLoop:
       udiv  x1, x0, x2
       msub  x0, x1, x2, x0
       strb  w0, [sp, x3]
        add  x3, x3, 1
        mov  x0, x1
       cbnz  x1, PrintUInt.PushLoop
PrintUInt.PopLoop:
       subs  x3, x3, 1
       ldrb  w0, [sp, x3]
        add  w0, w0, '0'
       strb  w0, [x27], 1
        bhi  PrintUInt.PopLoop
        add  sp, sp, 64
        ret


ParseInteger:
// io: x26 string
// out: x0
       ldrb  w1, [x26]
        mov  x2, 0
        mov  x0, 0
        cmp  w1, '-'
        beq  ParseInteger.Neg
        cmp  w1, '+'
        beq  ParseInteger.Pos
          b  ParseInteger.Loop
ParseInteger.Neg:
        mvn  x2, x2
ParseInteger.Pos:
        add  x26, x26, 1
ParseInteger.Loop:     
       ldrb  w1, [x26]
       subs  x1, x1, '0'
        blo  ParseInteger.Done
        cmp  x1, 9
        bhi  ParseInteger.Done
        add  x26, x26, 1
        add  x0, x0, x0, lsl 2
        add  x0, x1, x0, lsl 1
          b  ParseInteger.Loop
ParseInteger.Done:      
        eor  x0, x0, x2
        sub  x0, x0, x2
        ret


ParseBoole:
/*
	; io: rsi string
	;out: rax = -1 if string starts with true
	;         = 0  otherwise
	; rsi is advanced if true or false is read

		 or   rax, -1
		mov   ecx, dword[rsi]
		add   rsi, 4
		cmp   ecx, 'true'
		 je   .done
		sub   rsi, 4
		xor   eax, eax
		cmp   ecx, 'fals'
		jne   .done
		cmp   byte[rsi+4], 'e'
		jne   .done
		add   rsi, 5
	.done:
		ret
*/
        stp  x29, x30, [sp, -16]!
        lea  x1, sz_true
         bl  CmpString
        cmp  w0, 0
        mov  x0, -1
        bne  ParseBoole.Done
        lea  x1, sz_false
         bl  CmpString
        mov  x0, 0
ParseBoole.Done:
        ldp  x29, x30, [sp], 16
        ret


GetLine:
// out: x0 = 0 if success, = -1 if failed
//      x1 length of string
//      x26 address of string
        stp  x29, x30, [sp,-16]!
        stp  x25, x28, [sp,-16]!
        stp  x23, x24, [sp,-16]!
        stp  x21, x22, [sp,-16]!
        lea  x29, ioBuffer
        ldr  w22, [x29, IOBuffer.tmp_i]
        ldr  w23, [x29, IOBuffer.tmp_j]
        ldr  x24, [x29, IOBuffer.inputBufferSizeB]
        ldr  x25, [x29, IOBuffer.inputBuffer]
        mov  x28, 0
GetLine.NextChar:
        cmp  x28, x24
        bhs  GetLine.Realloc
GetLine.ReallocRet:
        cmp  x22, x23
        bhs  GetLine.GetMore
GetLine.GetMoreRet:
        add  x0, x29, IOBuffer.tmpBuffer
       ldrb  w0, [x0, x22]
        add  x22, x22, 1
       strb  w0, [x25, x28]
        add  x28, x28, 1
        cmp  w0, ' '
        bhs  GetLine.NextChar
        mov  x0, 0
GetLine.Done:
        str  w22, [x29,IOBuffer.tmp_i]
        str  w23, [x29,IOBuffer.tmp_j]
        str  x24, [x29,IOBuffer.inputBufferSizeB]
        str  x25, [x29,IOBuffer.inputBuffer]
        mov  x26, x25
        mov  x1, x28
        ldp  x21, x22, [sp], 16
        ldp  x23, x24, [sp], 16
        ldp  x25, x28, [sp], 16
        ldp  x29, x30, [sp], 16
        ret
GetLine.GetMore:
        mov  x22, 0
        add  x1, x29, IOBuffer.tmpBuffer
        mov  x2, sizeof.IOBuffer.tmpBuffer
         bl  Os_ReadStdIn
        mov  x23, x0
        cmp  x0, 1
        bge  GetLine.GetMoreRet
GetLine.Failed:
        mov  x0, -1
        mov  x23, 0
          b  GetLine.Done
GetLine.Realloc:
        add  x0, x24, 4096
         bl  Os_VirtualAlloc
        mov  x23, x0
        mov  x1, x25
        mov  x2, x24
         bl  MemoryCopy
        mov  x0, x25
        mov  x1, x24
         bl  Os_VirtualFree
        mov  x25, x23
        add  x24, x24, 4096
          b  GetLine.ReallocRet


MemoryCopy.Next:
       ldrb  w3, [x1], 1
       strb  w3, [x0], 1
MemoryCopy:
// copy x2 bytes from x1 to x0
// advance x0 to the end of the block
// preserve every reg except x0, x1, x2
       subs  x2, x2, 1        
        bpl  MemoryCopy.Next
        ret


MemoryFill.Next:
       strb  w1, [x0], 1
MemoryFill:
// fill x2 bytes from the lower byte in x1 to x0
// advance x0 to the end of the block
// preserve every reg except x0, x1, x2
       subs  x2, x2, 1        
        bpl  MemoryFill.Next
        ret

