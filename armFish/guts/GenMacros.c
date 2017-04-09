/*
The macro GenAll used the following labels from GenPawnJmp and GenCastleJmp
.CheckProm
.PromDone
.CaptureEp
.CaptureEpDone
.CastlingOO
.CastlingOOO
.CastlingDone
*/


/*
macro shift_bb delta,b,t {
if delta eq DELTA_N
		shl   b, 8
else if delta eq DELTA_S
		shr   b, 8
else if delta eq DELTA_NE
		mov   t, not FileHBB
		and   b, t
		shl   b, 9
else if delta eq DELTA_SE
		mov   t, not FileHBB
		and   b, t
		shr   b, 7
else if delta eq DELTA_NW
		mov   t, not FileABB
		and   b, t
		shl   b, 7
else if delta eq DELTA_SW
		mov   t, not FileABB
		and   b, t
		shr   b, 9

else
	  display 'delta in shift_bb strange'
	  display 13,10
	  err
end if
}
*/
.macro ShiftBB delta, res, bb
 .if \delta == DELTA_N
        lsl  \res, \bb, 8
 .elseif \delta == DELTA_S
	lsr  \res, \bb, 8
 .elseif \delta == DELTA_NE
	and  \res, \bb, ~FileHBB
	lsl  \res, \res, 9
 .elseif \delta == DELTA_SE
	and  \res, \bb, ~FileHBB
	lsr  \res, \res, 7
 .elseif \delta == DELTA_NW
	and  \res, \bb, ~FileABB
	lsl  \res, \res, 7
 .elseif \delta == DELTA_SW
	and  \res, \bb, ~FileABB
	lsr  \res, \res, 9
 .else
  .error "delta in ShiftBB strange"
 .endif

.endm

/*
macro attacks_from_pawn color, res, square {

 if color eq White
		mov   res, qword[WhitePawnAttacks+8*square]
 else if color eq Black
		mov   res, qword[BlackPawnAttacks+8*square]
 else
	  display 'color in attacks_from_pawn strange'
	  display 13,10
	  err
 end if

}
*/
.macro AttacksFromPawn color, res, square, temp
 .if \color == White
        lea  \temp, WhitePawnAttacks
        ldr  \res, [\temp, \square, lsl 3]
 .elseif \color == Black
        lea  \temp, BlackPawnAttacks
        ldr  \res, [\temp, \square, lsl 3]
 .else
  .error "color in AttacksFromPawn strange"
 .endif
.endm


/*
macro CastlingJmp Rights, JmpTrue, JmpFalse {
	; in: rbp  address of Pos
	;     r13  their pieces
	;     r14  all pieces
	; out eax =  0 if castling is illegal
	;     eax = -1 if castling is legal
	; assumed to have passed path test and rights test
local ..ksq_loop
		mov   rax, qword[rbp+Pos.typeBB+8*Pawn]
		 or   rax, qword[rbp+Pos.typeBB+8*King]
		and   rax, r13
	       test   rax, qword[rbp-Thread.rootPos+Thread.castling_kingpawns+8*(Rights)]
		jnz   JmpFalse
		mov   rdx, qword[rbp+Pos.typeBB+8*Knight]
		and   rdx, r13
	       test   rdx, qword[rbp-Thread.rootPos+Thread.castling_knights+8*(Rights)]
		jnz   JmpFalse
	      movzx   r11d, byte[rbp-Thread.rootPos+Thread.castling_ksqpath+8*(Rights)]
		mov   r10, qword[rbp+Pos.typeBB+8*Rook]
		 or   r10, qword[rbp+Pos.typeBB+8*Queen]
		and   r10, r13
		mov   r9, qword[rbp+Pos.typeBB+8*Bishop]
		 or   r9, qword[rbp+Pos.typeBB+8*Queen]
		and   r9, r13
	      movzx   eax, byte[rbp-Thread.rootPos+Thread.castling_rfrom+Rights]
		mov   rdx, r14
		btr   rdx, rax
	RookAttacks   rax, 56*(((Rights) and 2) shr 1)+(((Rights) and 1) xor 1)*(SQ_G1-SQ_C1)+SQ_C1, rdx, r8
	       test   rax, r10
		jnz   JmpFalse
	       test   r11d, r11d
		 jz   JmpTrue
*/
.macro CastlingJmp Rights, JmpTrue, JmpFalse
        ldr  x0, [x20, 8*Pawn]
        ldr  x4, [x20, 8*King]
        orr  x0, x0, x4
        and  x0, x0, x13
        ldr  x4, [x20, -Thread.rootPos + Thread.castling_kingpawns + 8*(\Rights)]
        tst  x0, x4
        bne  \JmpFalse
        ldr  x2, [x20, 8*Knight]
        and  x2, x2, x13
        ldr  x4, [x20, -Thread.rootPos + Thread.castling_knights + 8*(\Rights)]
        tst  x2, x4
        bne  \JmpFalse
       ldrb  w11, [x20, -Thread.rootPos + Thread.castling_ksqpath + 8*(\Rights)]
        ldr  x10, [x20, 8*Rook]      
        ldr  x4, [x20, 8*Queen]
        orr  x10, x10, x4
        and  x10, x10, x13
        ldr  x9, [x20, 8*Bishop]      
        ldr  x4, [x20, 8*Queen]
        orr  x9, x9, x4
        and  x9, x9, x13
       ldrb  w0, [x20, -Thread.rootPos + Thread.castling_rfrom + \Rights]
        mov  x4, 1
        lsl  x4, x4, x0
        bic  x2, x14, x4
        mov  x1, 56/2*((\Rights)&2)+((\Rights)&1)*(SQ_C1-SQ_G1)+SQ_G1
        RookAttacks x0, x1, x2, x8, x4
        tst  x0, x10
        bne  \JmpFalse
        cbz  w11, \JmpTrue
/*
..ksq_loop:
	      movzx   edx, byte[rbp-Thread.rootPos+Thread.castling_ksqpath+8*(Rights)+r11]
	RookAttacks   rax, rdx, r14, r8
	       test   rax, r10
		jnz   JmpFalse
      BishopAttacks   rax, rdx, r14, r8
	       test   rax, r9
		jnz   JmpFalse
		sub   r11d, 1
		jnz   ..ksq_loop
}
*/
CastlingJmp.ksq_loop\@:
        add  x7, x20, -Thread.rootPos + Thread.castling_ksqpath + 8*(\Rights)
       ldrb  w2, [x7, x11]
        RookAttacks  x0, x2, x14, x8, x4
        tst  x0, x10
        bne  \JmpFalse
        BishopAttacks  x0, x2, x14, x8, x4
        tst  x0, x9
        bne  \JmpFalse
        sub  x11, x11, 1
       cbnz  x11, CastlingJmp.ksq_loop\@
.endm



/*
macro generate_promotions Type, Delta, pon7, target
{
local .Outer,.OuterDone,.Inner,.InnerDone
 if Type eq QUIET_CHECKS
	      movzx   eax, byte [rbx+State.ksq]
		xor   ecx, ecx
		bts   rcx, rax
 end if

		mov   rsi, pon7
	   shift_bb   Delta, rsi, rdx
		and   rsi, target
		 jz   .OuterDone
*/
.macro GenProm Type, Delta, pon7, target
 .if \Type == QUIET_CHECKS
       ldrb  w0, [x21, State.ksq]
        mov  x1, 1
        lsl  x1, x1, x0        
 .endif
        ShiftBB  \Delta, x16, \pon7
       ands  x16, x16, \target
        beq  GenProm.OuterDone\@
/*
.Outer:
		bsf   rdx, rsi
 if (Type in <CAPTURES, EVASIONS, NON_EVASIONS>)
	       imul   eax, edx, 65
		add   eax, 64*64*(MOVE_TYPE_PROM+3) - 64*Delta
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
 end if
 if (Type in <QUIETS, EVASIONS, NON_EVASIONS>)
	       imul   eax, edx, 65
		add   eax, 64*64*(MOVE_TYPE_PROM+2) - 64*Delta
		mov   dword[rdi+0*sizeof.ExtMove], eax
		sub   eax, 64*64*(1)
		mov   dword[rdi+1*sizeof.ExtMove], eax
		sub   eax, 64*64*(1)
		mov   dword[rdi+2*sizeof.ExtMove], eax
		lea   rdi, [rdi+3*sizeof.ExtMove]
 end if
 if (Type eq QUIET_CHECKS)
	       imul   eax, edx, 65
	       test   rcx, qword[KnightAttacks+8*rdx]
		 jz   .InnerDone
		add   eax, 64*64*(MOVE_TYPE_PROM+0) - 64*Delta
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
 end if
*/
GenProm.Outer\@:
       rbit  x2, x16
        clz  x2, x2
 .if \Type == CAPTURES || \Type == EVASIONS || \Type == NON_EVASIONS
        add  x0, x2, x2, lsl 6
        mov  x4, 64*64*(MOVE_TYPE_PROM+3) - 64*\Delta
        add  x0, x0, x4
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
 .endif
 .if \Type == QUIETS || \Type == EVASIONS || \Type == NON_EVASIONS
        add  x0, x2, x2, lsl 6
        mov  x4, 64*64*(MOVE_TYPE_PROM+2) - 64*\Delta
        add  x0, x0, x4
        str  w0, [x27, 0*sizeof.ExtMove]
        sub  w0, w0, 64*64*(1)
        str  w0, [x27, 1*sizeof.ExtMove]
        sub  w0, w0, 64*64*(1)
        str  w0, [x27, 2*sizeof.ExtMove]
        add  x27, x27, 3*sizeof.ExtMove
 .endif
 .if \Type == QUIET_CHECKS
        add  x0, x2, x2, lsl 6
        lea  x7, KnightAttacks
        ldr  x4, [x7, x2, lsl 3]
        beq  GenProm.InnerDone\@
        mov  x4, 64*64*(MOVE_TYPE_PROM+0) - 64*\Delta
        add  x0, x0, x4
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
 .endif
/*
.InnerDone:
	       blsr   rsi, rsi, rax
		jnz   .Outer
.OuterDone:
*/
GenProm.InnerDone\@:
        sub  x4, x16, 1
       ands  x16, x16, x4
        bne  GenProm.Outer\@
GenProm.OuterDone\@:
.endm



/*
; generate_pawn_jmp generates targets for uncommon operations in pawn move gen
;  first we have promotions
;  then ep captures

macro generate_pawn_jmp Us, Type
{
local Them, TRank8BB, TRank7BB, TRank3BB, Up, Right, Left
local .b1, .b2, .eS, .pawnsNotOn7, .pawnsOn7, .enemies

.b1 equ r8
.b2 equ r9
.eS equ r10
.pawnsNotOn7 equ r11
.pawnsOn7 equ r12
.enemies  equ r13

match =White, Us
\{
	Them	 equ Black
	TRank8BB equ Rank8BB
	TRank7BB equ Rank7BB
	TRank3BB equ Rank3BB
	Up	 equ DELTA_N
	Right	 equ DELTA_NE
	Left	 equ DELTA_NW
\}


match =Black, Us \{
	Them	 equ White
	TRank8BB equ Rank1BB
	TRank7BB equ Rank2BB
	TRank3BB equ Rank6BB
	Up	 equ DELTA_S
	Right	 equ DELTA_SW
	Left	 equ DELTA_SE
\}
*/
.macro GenPawnJmp ParentLabel, Us, Type
 .if \Us == White
	Them	 = Black
	TRank8BB = Rank8BB
	TRank7BB = Rank7BB
	TRank3BB = Rank3BB
	Up	 = DELTA_N
	Right	 = DELTA_NE
	Left	 = DELTA_NW
 .elseif \Us == Black
	Them	 = White
	TRank8BB = Rank1BB
	TRank7BB = Rank2BB
	TRank3BB = Rank6BB
	Up	 = DELTA_S
	Right	 = DELTA_SW
	Left	 = DELTA_SE
 .else
  .error "bad color in GenPawnJmp"
 .endif
/*
	      align   8
.CheckProm:

    if (Type eq CAPTURES)
		mov   .eS, r14
		not   .eS
    end if
    if (Type eq EVASIONS)
		and   .eS, r15
    end if

generate_promotions   Type, Right,.pawnsOn7,.enemies
generate_promotions   Type, Left,.pawnsOn7,.enemies
generate_promotions   Type, Up,.pawnsOn7,.eS
		jmp   .PromDone

if (Type in <CAPTURES, EVASIONS, NON_EVASIONS>)
	      align   8
.CaptureEp:
		bsf   rax, .b1
		shl   eax, 6
		 or   eax, edx
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   .b1, .b1, rcx
		jnz   .CaptureEp
		jmp   .CaptureEpDone
end if
}
*/
 \ParentLabel\().CheckProm:
 .if \Type == CAPTURES
        mvn  x10, x14
 .endif
 .if \Type == EVASIONS
        and  x10, x10, x15
 .endif
        GenProm  \Type, Right, x12, x13
        GenProm  \Type, Left, x12, x13
        GenProm  \Type, Up, x12, x10
          b  \ParentLabel\().PromDone
 .if \Type == CAPTURES || \Type == EVASIONS || \Type == NON_EVASIONS
  \ParentLabel\().CaptureEp:
       rbit  x0, x8
        clz  x0, x0
        lsl  x0, x0, 6
        orr  x0, x0, x2
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x1, x8, 1
       ands  x8, x8, x1
        bne  \ParentLabel\().CaptureEp
          b  \ParentLabel\().CaptureEpDone
 .endif
.endm

/*
macro generate_pawn_moves Us, Type
{
local Them, TRank8BB, TRank7BB, TRank3BB, Up, Right, Left
local .b1, .b2, .eS, .pawnsNotOn7, .pawnsOn7, .enemies
local .SkipDCPawns, .SinglePush, .SinglePushDone, .DoublePush, .DoublePushDone
local .CaptureRight, .CaptureRightDone, .CaptureLeft, .CaptureLeftDone, .EpDone

match =White, Us
\{
	Them	 equ Black
	TRank8BB equ Rank8BB
	TRank7BB equ Rank7BB
	TRank3BB equ Rank3BB
	Up	 equ DELTA_N
	Right	 equ DELTA_NE
	Left	 equ DELTA_NW
\}


match =Black, Us \{
	Them	 equ White
	TRank8BB equ Rank1BB
	TRank7BB equ Rank2BB
	TRank3BB equ Rank6BB
	Up	 equ DELTA_S
	Right	 equ DELTA_SW
	Left	 equ DELTA_SE
\}


.b1 equ r8
.b2 equ r9
.eS equ r10
.pawnsNotOn7 equ r11
.pawnsOn7 equ r12
.enemies  equ r13
*/
.macro GenPawnMoves ParentLabel, Us, Type
 .if \Us == White
	Them	 = Black
	TRank8BB = Rank8BB
	TRank7BB = Rank7BB
	TRank3BB = Rank3BB
	Up	 = DELTA_N
	Right	 = DELTA_NE
	Left	 = DELTA_NW
 .elseif \Us == Black
	Them	 = White
	TRank8BB = Rank1BB
	TRank7BB = Rank2BB
	TRank3BB = Rank6BB
	Up	 = DELTA_S
	Right	 = DELTA_SW
	Left	 = DELTA_SE
 .else
  .error "bad Us in GenPawnMoves"
 .endif
 bb1 .req x8
 bb2 .req x9
 eS .req x10
 pawnsNotOn7 .req x11
 pawnsOn7 .req x12
 enemies  .req x13

//Display "GenPawnMoves \ParentLabel, \Us, \Type called\n"

/*
		mov   rax, qword[rbp+Pos.typeBB+8*Pawn]
		and   rax, qword[rbp+Pos.typeBB+8*Us]
		mov   .pawnsOn7, TRank7BB
	       andn   .pawnsNotOn7, .pawnsOn7, rax
		and   .pawnsOn7, rax

if (Type eq EVASIONS)
		mov   .enemies, qword[rbp+Pos.typeBB+8*Them]
		and   .enemies, r15
else if (Type eq CAPTURES)
		mov   .enemies, r15
else
		mov   .enemies, qword[rbp+Pos.typeBB+8*Them]
end if
*/
        ldr  x0, [x20, 8*Pawn]
        ldr  x4, [x20, 8*\Us]
        and  x0, x0, x4
        mov  pawnsOn7, TRank7BB
        bic  pawnsNotOn7, x0, pawnsOn7
        and  pawnsOn7, pawnsOn7, x0
 .if \Type == EVASIONS
        ldr  enemies, [x20, 8*Them]
        and  enemies, enemies, x15
 .elseif \Type == CAPTURES
        mov  enemies, x15
 .else
        ldr  enemies, [x20, 8*Them]
 .endif
/*
;Single and double pawn pushes, no promotions
 if (Type eq CAPTURES)
 else

    if (Type in <QUIETS,QUIET_CHECKS>)
		mov   .eS, r15
    else
		mov   .eS, r14
		not   .eS
    end if

		mov   .b1, .pawnsNotOn7
	   shift_bb   Up, .b1, rax
		and   .b1, .eS

		mov   .b2, TRank3BB
		and   .b2, .b1
	   shift_bb   Up, .b2, rax
		and   .b2, .eS

    if Type eq EVASIONS
		and   .b1, r15
		and   .b2, r15
    end if
*/
 .if \Type != CAPTURES
  .if \Type == QUIETS || \Type == QUIET_CHECKS
        mov  eS, x15
  .else
        mvn  eS, x14
  .endif
        ShiftBB  Up, bb1, pawnsNotOn7
        and  bb1, bb1, eS
        mov  bb2, TRank3BB
        and  bb2, bb2, bb1
        ShiftBB  Up, bb2, bb2
        and  bb2, bb2, eS
  .if \Type == EVASIONS
        and  bb1, bb1, x15
        and  bb2, bb2, x15
  .endif
/*
    if Type eq QUIET_CHECKS
	      movzx   edx, byte[rbx+State.ksq]
  attacks_from_pawn   Them, rax, rdx
		and   .b1, rax
		and   .b2, rax

		and   rdx, 7
		mov   rax, .pawnsNotOn7
		mov   rcx, qword [FileBB+8*rdx]
	       andn   rcx, rcx, .eS
		and   rax, qword[rbx+State.dcCandidates]
		 jz   .SkipDCPawns
	   shift_bb   Up, rax, rdx
		and   rax, rcx
		mov   rcx, TRank3BB
		and   rcx, rax
	   shift_bb   Up, rcx, rdx
		and   rcx, .eS
		 or   .b1, rax
		 or   .b2, rcx
    .SkipDCPawns:
	end if
	       test   .b1, .b1
		 jz   .SinglePushDone
    .SinglePush:
		bsf   rax, .b1
	       imul   eax, (1 shl 6) + (1 shl 0)
		sub   eax, (Up shl 6) + (0 shl 0)
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   .b1, .b1, rcx
		jnz   .SinglePush
    .SinglePushDone:
	       test   .b2, .b2
		 jz   .DoublePushDone
    .DoublePush:
		bsf   rax, .b2
	       imul   eax, (1 shl 6) + (1 shl 0)
		sub   eax, ((Up+Up) shl 6)
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   .b2, .b2, rcx
		jnz   .DoublePush
    .DoublePushDone:
 end if
*/
  .if \Type == QUIET_CHECKS
       ldrb  w2, [x21, State.ksq]
        AttacksFromPawn  Them, x0, x2, x7
        and  bb1, bb1, x0
        and  bb2, bb2, x0
        and  x2, x2, 7
        mov  x0, pawnsNotOn7
        lea  x7, FileBB
        ldr  x1, [x7, x2, lsl 3]
        bic  x1, eS, x1
        ldr  x4, [x21, State.dcCandidates]
       ands  x0, x0, x4
        beq  GenPawnMoves.SkipDCPawns
        ShiftBB  Up, x0, x0
        and  x0, x0, x1
        mov  x1, TRank3BB
        and  x1, x1, x0
        ShiftBB  Up, x1, x1
        and  x1, x1, eS
        orr  bb1, bb1, x0
        orr  bb2, bb2, x1
   GenPawnMoves.SkipDCPawns\@:        
  .endif
        cbz  bb1, GenPawnMoves.SinglePushDone\@
  GenPawnMoves.SinglePush\@:
       rbit  x0, bb1
        clz  x0, x0
        add  x0, x0, x0, lsl 6
        sub  x0, x0, Up << 6
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x1, bb1, 1
       ands  bb1, bb1, x1
        bne  GenPawnMoves.SinglePush\@
  GenPawnMoves.SinglePushDone\@:
        cbz  bb2, GenPawnMoves.DoublePushDone\@
  GenPawnMoves.DoublePush\@:
       rbit  x0, bb2
        clz  x0, x0
        add  x0, x0, x0, lsl 6
        sub  x0, x0, (Up+Up) << 6
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x1, bb2, 1
       ands  bb2, bb2, x1
        bne  GenPawnMoves.DoublePush\@
  GenPawnMoves.DoublePushDone\@:       
 .endif
/*
 if (Type eq EVASIONS)
		mov   rax, TRank8BB
	       test   .pawnsOn7, .pawnsOn7
		 jz   .PromDone
	       test   rax, r15
		jnz   .CheckProm
 else
		mov   rax, TRank8BB
	       test   .pawnsOn7, .pawnsOn7
		jnz   .CheckProm

 end if

.PromDone:
*/
 .if \Type == EVASIONS
        mov  x0, TRank8BB
        cbz  pawnsOn7, \ParentLabel\().PromDone
        tst  x0, x15
        bne  \ParentLabel\().CheckProm
 .else
        mov  x0, TRank8BB
       cbnz  pawnsOn7, \ParentLabel\().CheckProm
 .endif
 \ParentLabel\().PromDone:
/*
 if (Type in <CAPTURES, EVASIONS, NON_EVASIONS>)

		mov   .b1, .pawnsNotOn7
		mov   .b2, .pawnsNotOn7
	   shift_bb   Right, .b1, rax
	   shift_bb   Left, .b2, rax
		and   .b1, .enemies
		and   .b2, .enemies

	       test   .b1, .b1
		 jz   .CaptureRightDone
    .CaptureRight:
		bsf   rax, .b1
	       imul   eax, (1 shl 6) + (1 shl 0)
		sub   eax, (Right shl 6) + (0 shl 0)
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   .b1, .b1, rcx
		jnz   .CaptureRight
    .CaptureRightDone:

	       test   .b2, .b2
		 jz   .CaptureLeftDone
    .CaptureLeft:
		bsf   rax, .b2
	       imul   eax, (1 shl 6) + (1 shl 0)
		sub   eax, (Left shl 6) + (0 shl 0)
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   .b2, .b2, rcx
		jnz   .CaptureLeft
    .CaptureLeftDone:
*/
 .if \Type == CAPTURES || \Type == EVASIONS || \Type == NON_EVASIONS
        ShiftBB  Right, bb1, pawnsNotOn7
        ShiftBB  Left, bb2, pawnsNotOn7
        and  bb1, bb1, enemies
        and  bb2, bb2, enemies
        cbz  bb1, GenPawnMoves.CaptureRightDone\@
   GenPawnMoves.CaptureRight\@:
       rbit  x0, bb1
        clz  x0, x0
        add  x0, x0, x0, lsl 6
        sub  x0, x0, Right << 6
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x1, bb1, 1
       ands  bb1, bb1, x1
        bne  GenPawnMoves.CaptureRight\@
   GenPawnMoves.CaptureRightDone\@:
        cbz  bb2, GenPawnMoves.CaptureLeftDone\@
   GenPawnMoves.CaptureLeft\@:
       rbit  x0, bb2
        clz  x0, x0
        add  x0, x0, x0, lsl 6
        sub  x0, x0, Left << 6
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x1, bb2, 1
       ands  bb2, bb2, x1
        bne  GenPawnMoves.CaptureLeft\@
   GenPawnMoves.CaptureLeftDone\@:
/*
	      movzx   edx, byte[rbx+State.epSquare]
		lea   eax, [rdx-Up]
		cmp   edx, 64
		jae   .EpDone


	if (Type eq EVASIONS)
		 bt   r15, rax
		jnc   .EpDone
	end if

  attacks_from_pawn   Them, .b1, rdx
		 or   edx, MOVE_TYPE_EPCAP shl 12
		and   .b1, .pawnsNotOn7
		jnz   .CaptureEp
    .CaptureEpDone:

    .EpDone:

 end if

}
*/
       ldrb  w2, [x21, State.epSquare]
        sub  x0, x2, Up
        cmp  x2, 64
        bhs  GenPawnMoves.EpDone\@
  .if \Type == EVASIONS
        lsr  x4, x15, x0
       tbnz  x4, 0, GenPawnMoves.EpDone\@
  .endif
        AttacksFromPawn  Them, bb1, x2, x7
        orr  x2, x2, MOVE_TYPE_EPCAP << 12
       ands  bb1, bb1, pawnsNotOn7
        bne  \ParentLabel\().CaptureEp
  \ParentLabel\().CaptureEpDone:
  GenPawnMoves.EpDone\@:
 .endif
 .unreq bb1
 .unreq bb2
 .unreq eS
 .unreq pawnsNotOn7
 .unreq pawnsOn7
 .unreq enemies
.endm

/*
; generate moves Knight, Bishop, Rook, and Queen

macro generate_moves  Us, Pt, Checks
{
local  .Outer,.OuterDone,.Inner,.InnerDone


; this is the head of the loop
; we can either read from the piece lists
;  or pop bits from the bitboards

if PEDANTIC
		lea   r11, [rbp+Pos.pieceList+16*(8*Us+Pt)]
	      movzx   edx, byte[r11]
		cmp   edx, 64
		jae   .OuterDone
.Outer:

else
		mov   r11, qword[rbp+Pos.typeBB+8*Pt]
		and   r11, qword[rbp+Pos.typeBB+8*Us]
		 jz   .OuterDone
.Outer:
		bsf   rdx, r11
end if
*/
.macro GenMoves Us, Pt, Checks
//Display "GenMoves \Us, \Pt, \Checks called\n"
        add  x11, x20, Pos.pieceList+16*(8*\Us+\Pt)
       ldrb  w2, [x11]
        cmp  w2, 64
        bhs  GenMoves.OuterDone\@
 GenMoves.Outer\@:

/*
if Checks eq QUIET_CHECKS
		mov   r10, qword[rbx+State.checkSq+8*Pt]
		mov   rsi, qword[rbx+State.dcCandidates]
 if Pt eq Bishop
		mov   rax, qword[BishopAttacksPDEP+8*rdx]
		and   rax, r10
	       test   rax, r15
		 jz   .InnerDone
 else if Pt eq Rook
		mov   rax, qword[RookAttacksPDEP+8*rdx]
		and   rax, r10
	       test   rax, r15
		 jz   .InnerDone
 else if Pt eq Queen
		mov   rax, qword[BishopAttacksPDEP+8*rdx]
		 or   rax, qword[RookAttacksPDEP+8*rdx]
		and   rax, r10
	       test   rax, r15
		 jz   .InnerDone
 end if
		 bt   rsi, rdx
		 jc   .InnerDone
 if  Pt eq Knight
		mov   rsi, qword[KnightAttacks+8*rdx]
 else if Pt eq Bishop
      BishopAttacks   rsi, rdx, r14, rax
 else if Pt eq Rook
	RookAttacks   rsi, rdx, r14, rax
 else if Pt eq Queen
      BishopAttacks   rsi, rdx, r14, rax
	RookAttacks   r9, rdx, r14, rax
		 or   rsi, r9
 end if
else
*/
 .if \Checks == QUIET_CHECKS
        ldr  x10, [x21, State.checkSq + 8*\Pt]
        ldr  x16, [x21, State.dcCandidates]
  .if \Pt == Bishop
        lea  x7, BishopAttacksPDEP
        ldr  x0, [x7, x2, lsl 3]
        and  x0, x0, x10
        tst  x0, x15
        beq  InnerDone\@
  .elseif \Pt == Rook
        lea  x7, RookAttacksPDEP
        ldr  x0, [x7, x2, lsl 3]
        and  x0, x0, x10
        tst  x0, x15
        beq  InnerDone\@
  .elseif \Pt == Queen
        lea  x7, BishopAttacksPDEP
        ldr  x0, [x7, x2, lsl 3]
        lea  x7, RookAttacksPDEP
        ldr  x4, [x7, x2, lsl 3]
        orr  x0, x0, x4
        and  x0, x0, x10
        tst  x0, x15
        beq  InnerDone\@
  .endif
        lsr  x4, x16, x2
       tbnz  x4, 0, GenMoves.InnerDone\@
  .if \Pt == Knight
        lea  x7, KnightAttacks
        ldr  x16, [x7, x2, lsl 3]
  .elseif \Pt == Bishop
        BishopAttacks  x16, x2, x14, x0, x4
  .elseif \Pt == Rook
        RookAttacks  x16, x2, x14, x0, x4
  .elseif \Pt == Queen
        BishopAttacks  x16, x2, x14, x0, x4
        RookAttacks  x9, x2, x14, x0, x4
        orr  x16, x16, x9
  .endif
 .else
/*
 if  Pt eq Knight
		mov   rsi, qword[KnightAttacks+8*rdx]
 else if Pt eq Bishop
      BishopAttacks   rsi, rdx, r14, rax
 else if Pt eq Rook
	RookAttacks   rsi, rdx, r14, rax
 else if Pt eq Queen
      BishopAttacks   rsi, rdx, r14, rax
	RookAttacks   r9, rdx, r14, rax
		 or   rsi, r9
 end if

end if
*/
  .if \Pt == Knight
        lea  x7, KnightAttacks
        ldr  x16, [x7, x2, lsl 3]
  .elseif \Pt == Bishop
        BishopAttacks  x16, x2, x14, x0, x4
  .elseif \Pt == Rook
        RookAttacks  x16, x2, x14, x0, x4
  .elseif \Pt == Queen
        BishopAttacks  x16, x2, x14, x0, x4
        RookAttacks  x9, x2, x14, x0, x4
        orr  x16, x16, x9
  .endif
 .endif

/*
if Checks eq QUIET_CHECKS
		and   rsi, r10
end if

		shl   edx, 6
		and   rsi, r15
		 jz   .InnerDone
  .Inner:
		bsf   rax, rsi
		 or   eax, edx
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   rsi, rsi, rax
		jnz   .Inner
  .InnerDone:
*/
 .if \Checks == QUIET_CHECKS
        and  x16, x16, x10
 .endif
        lsl  x2, x2, 6
       ands  x16, x16, x15
        beq  GenMoves.InnerDone\@
 GenMoves.Inner\@:
       rbit  x0, x16
        clz  x0, x0
        orr  x0, x0, x2
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x0, x16, 1
       ands  x16, x16, x0
        bne  GenMoves.Inner\@
 GenMoves.InnerDone\@:

/*
; this is the exit of the loop
; we can either read from the piece lists
;  or pop bits from the bitboards

if PEDANTIC
		add   r11, 1
	      movzx   edx, byte[r11]
		cmp   edx, 64
		 jb   .Outer
 .OuterDone:

else
	       blsr   r11, r11, rax
		jnz   .Outer
 .OuterDone:

end if
}
*/
        add  x11, x11, 1
       ldrb  w2, [x11]
        cmp  w2, 64
        blo  GenMoves.Outer\@
 GenMoves.OuterDone\@:
.endm

/*
; generate_jmp generates targets for uncommon operations in move gen
; first we do castling and then generate_pawn_jmp

macro generate_jmp  Us, Type
{
local .CastlingOODone, .CastlingOOGood, .CastlingOOOGood


if Type in <CAPTURES, EVASIONS>
else


	      align   8
.CastlingOO:

 if Type eq NON_EVASIONS

	CastlingJmp   (2*Us+0), .CastlingOOGood, .CastlingOODone
.CastlingOOGood:
		mov   eax, dword[rbp-Thread.rootPos+Thread.castling_movgen+4*(2*Us+0)]
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]

 else
  if Us eq White
	       call   CastleOOLegal_White
  else if Us eq Black
	       call   CastleOOLegal_Black
  end if

  if Type eq QUIET_CHECKS
		mov   ecx, dword[rbp-Thread.rootPos+Thread.castling_movgen+4*(2*Us+0)]
		mov   dword[rdi], ecx
	       test   eax, eax
		jnz   .CheckOOQuiteCheck
  else
		and   eax, sizeof.ExtMove
		mov   ecx, dword[rbp-Thread.rootPos+Thread.castling_movgen+4*(2*Us+0)]
		mov   dword[rdi], ecx
		add   rdi, rax
  end if
 end if

.CastlingOODone:
	      movzx   eax, byte[rbx+State.castlingRights]
		mov   rcx, qword[rbp-Thread.rootPos+Thread.castling_path+8*(2*Us+1)]
		and   eax, 2 shl (2*Us)
		xor   eax, 2 shl (2*Us)
		and   rcx, r14
		 or   rax, rcx
		jnz   .CastlingDone
*/
.macro GenCastlingJmp  ParentLabel, Us, Type
 .if \Type != CAPTURES && \Type != EVASIONS
  \ParentLabel\().CastlingOO:
  .if \Type == NON_EVASIONS
        CastlingJmp  (2*\Us+0), GenCastlingJmp.CastlingOOGood\@, GenCastlingJmp.CastlingOODone\@
   GenCastlingJmp.CastlingOOGood\@:
        ldr  w0, [x20, -Thread.rootPos + Thread.castling_movgen + 4*(2*\Us+0)]
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
  .else
   .if \Us == White
         bl  CastleOOLegal_White
   .elseif \Us == Black
         bl  CastleOOLegal_Black
   .endif

   .if \Type == QUIET_CHECKS
        ldr  w1, [x20, -Thread.rootPos + Thread.castling_movgen + 4*(2*\Us+0)]
        str  w1, [x27]
       cbnz  w0, GenCastlingJmp.CheckOOQuiteCheck\@
   .else
        and  w0, w0, sizeof.ExtMove
        ldr  w1, [x20, -Thread.rootPos + Thread.castling_movgen + 4*(2*\Us+0)]
        str  w1, [x27]
        add  x27, x27, x0
   .endif
  .endif
 GenCastlingJmp.CastlingOODone\@:
       ldrb  w0, [x21, State.castlingRights]
        ldr  x1, [x20, -Thread.rootPos + Thread.castling_path + 8*(2*\Us+1)]
        and  x0, x0, 2 << (2*\Us)
        eor  x0, x0, 2 << (2*\Us)
        and  x1, x1, x14
        orr  x0, x0, x1
       cbnz  x0, \ParentLabel\().CastlingDone
/*
.CastlingOOO:
	; check O-O-O

 if Type eq NON_EVASIONS
		CastlingJmp   (2*Us+1), .CastlingOOOGood, .CastlingDone
.CastlingOOOGood:
		mov   eax, dword[rbp-Thread.rootPos+Thread.castling_movgen+4*(2*Us+1)]
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
		jmp   .CastlingDone
 else
  if Us eq White
	       call   CastleOOOLegal_White
  else if Us eq Black
	       call   CastleOOOLegal_Black
  end if
  if Type eq QUIET_CHECKS
		mov   ecx, dword[rbp-Thread.rootPos+Thread.castling_movgen+4*(2*Us+1)]
	       test   eax, eax
		mov   dword[rdi], ecx
		jnz   .CheckOOOQuiteCheck
		jmp   .CastlingDone
  else
		and   eax, sizeof.ExtMove
		mov   ecx, dword[rbp-Thread.rootPos+Thread.castling_movgen+4*(2*Us+1)]
		mov   dword[rdi], ecx
		add   rdi, rax
		jmp   .CastlingDone
  end if
 end if
*/
  \ParentLabel\().CastlingOOO:
  .if \Type == NON_EVASIONS
        CastlingJmp  (2*\Us+1), GenCastlingJmp.CastlingOOOGood\@, \ParentLabel\().CastlingDone
   GenCastlingJmp.CastlingOOOGood\@:
        ldr  w0, [x20, -Thread.rootPos + Thread.castling_movgen + 4*(2*\Us+1)]
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
          b  \ParentLabel\().CastlingDone
  .else
   .if \Us == White
         bl  CastleOOOLegal_White
   .elseif Us == Black
         bl  CastleOOOLegal_Black
   .endif
   .if \Type == QUIET_CHECKS
        ldr  w1, [x20, -Thread.rootPos + Thread.castling_movgen + 4*(2*\Us+1)]
        str  w1, [x27]
       cbnz  w0, GenCastlingJmp.CheckOOOQuiteCheck\@
          b  \ParentLabel\().CastlingDone
   .else
        and  w0, w0, sizeof.ExtMove
        ldr  w1, [x20, -Thread.rootPos + Thread.castling_movgen + 4*(2*\Us+1)]
        str  w1, [x27]
        add  x27, x27, x0
          b  \ParentLabel\().CastlingDone
   .endif
  .endif
/*
 if Type eq QUIET_CHECKS
		      align   8
.CheckOOQuiteCheck:
	       call   Move_GivesCheck
		and   eax, 8
		add   rdi, rax
		jmp   .CastlingOODone
	      align   8
.CheckOOOQuiteCheck:
	       call   Move_GivesCheck
		and   eax, 8
		add   rdi, rax
		jmp   .CastlingDone
 end if
end if
  generate_pawn_jmp   Us, Type
}
*/
  .if \Type == QUIET_CHECKS
   GenCastlingJmp.CheckOOQuiteCheck\@:
         bl  Move_GivesCheck
        cbz  w0, GenCastlingJmp.CastlingOODone\@
        add  x27, x27, sizeof.ExtMove
          b  GenCastlingJmp.CastlingOODone\@
   GenCastlingJmp.CheckOOOQuiteCheck\@:
         bl  Move_GivesCheck
        cbz  w0, GenCastlingJmp.CastlingOODone\@
        add  x27, x27, sizeof.ExtMove
          b  \ParentLabel\().CastlingDone
  .endif
 .endif
.endm



/*
macro generate_all  Us, Type
{
local .KingMoves, .KingMovesDone;, .CastlingDone, ;,  .CastlingOO, .CastlingOOO

generate_pawn_moves   Us, Type
     generate_moves   Us, Knight, Type
     generate_moves   Us, Bishop, Type
     generate_moves   Us, Rook, Type
     generate_moves   Us, Queen, Type

if Type in <CAPTURES, EVASIONS>
else

	      movzx   r9d, byte[rbx+State.castlingRights]

		mov   r10, qword[rbp-Thread.rootPos+Thread.castling_path+8*(2*Us+0)]
		mov   r11, qword[rbp-Thread.rootPos+Thread.castling_path+8*(2*Us+1)]
		and   r10, r14
		and   r11, r14
end if
*/
.macro GenAll ParentLabel, Us, Type
//Display "GenAll \ParentLabel, \Us, \Type called\n"
        GenPawnMoves \ParentLabel, \Us, \Type
        GenMoves  \Us, Knight, \Type
        GenMoves  \Us, Bishop, \Type
        GenMoves  \Us, Rook, \Type
        GenMoves  \Us, Queen, \Type
 .if \Type != CAPTURES && \Type != EVASIONS
       ldrb  w9, [x21, State.castlingRights]
        ldr  x10, [x20, -Thread.rootPos + Thread.castling_path + 8*(2*\Us+0)]
        ldr  x11, [x20, -Thread.rootPos + Thread.castling_path + 8*(2*\Us+1)]
        and  x10, x10, x14
        and  x11, x11, x14
 .endif

/*
if Type in <QUIET_CHECKS, EVASIONS>
else
		mov   rsi, qword[rbp+Pos.typeBB+8*King]
		and   rsi, qword[rbp+Pos.typeBB+8*Us]
		bsf   rdx, rsi
		mov   rcx, qword[KingAttacks+8*rdx]
		shl   edx, 6
		and   rcx, r15
		 jz   .KingMovesDone
 .KingMoves:
		bsf   rax, rcx
		 or   eax, edx
		mov   dword[rdi], eax
		lea   rdi, [rdi+sizeof.ExtMove]
	       blsr   rcx, rcx, r8
		jnz   .KingMoves
 .KingMovesDone:
end if
*/
 .if \Type != QUIET_CHECKS && \Type != EVASIONS
        ldr  x16, [x20, 8*King]
        ldr  x4, [x20, 8*\Us]
        and  x16, x16, x4
       rbit  x2, x16
        clz  x2, x2
        lea  x7, KingAttacks
        ldr  x1, [x7, x2, lsl 3]
        lsl  x2, x2, 6
       ands  x1, x1, x15
        beq  GenAll.KingMovesDone\@
  GenAll.KingMoves\@:
       rbit  x0, x1
        clz  x0, x0
        orr  x0, x0, x2
        str  w0, [x27]
        add  x27, x27, sizeof.ExtMove
        sub  x8, x1, 1
       ands  x1, x1, x8
        bne  GenAll.KingMoves\@
  GenAll.KingMovesDone\@:
 .endif
/*
if Type in <CAPTURES, EVASIONS>
else
;                 check for castling
;                  since this is rare, the castling functions are included at the end in generate_jmp
		mov   edx, r9d
		and   r9d, 1 shl (2*Us)
		xor   r9d, 1 shl (2*Us)
		and   edx, 2 shl (2*Us)
		xor   edx, 2 shl (2*Us)
		mov   r13, qword[rbp+Pos.typeBB+8*(Us xor 1)]
		 or   r9, r10
		 jz   .CastlingOO
		 or   rdx, r11
		 jz   .CastlingOOO
.CastlingDone:
end if
}
*/
 .if \Type != CAPTURES && \Type != EVASIONS
        mov  x2, x9
        and  x9, x9, 1 << (2*\Us)
        eor  x9, x9, 1 << (2*\Us)
        and  x2, x2, 2 << (2*\Us)
        eor  x2, x2, 2 << (2*\Us)
        ldr  x13, [x20, 8*(\Us^1)]
        orr  x9, x9, x10
        cbz  x9, \ParentLabel\().CastlingOO
        orr  x2, x2, x11
        cbz  x2, \ParentLabel\().CastlingOOO
  \ParentLabel\().CastlingDone:
 .endif
//Display "GenAll \ParentLabel, \Us, \Type exiting\n"
.endm

