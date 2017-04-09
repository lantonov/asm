/*macro SliderBlockers result, sliders, s, pinners,\
		     pieces, pieces_color,\
		     b, snipers, snipersSqBB, t {

local ..YesPinners, ..NoPinners, ..MoreThanOne

	     Assert   e, result, 0, 'Assertion result=0 failed in slider_blockers'
	     Assert   e, pinners, 0, 'Assertion pinners=0 failed in slider_blockers'

		mov   snipers, qword[rbp+Pos.typeBB+8*Queen]
		mov   b, snipers
		 or   snipers, qword[rbp+Pos.typeBB+8*Rook]
		and   snipers, qword[RookAttacksPDEP+8*s]
		 or   b, qword[rbp+Pos.typeBB+8*Bishop]
		and   b, qword[BishopAttacksPDEP+8*s]
		 or   snipers, b
		shl   s#d, 6+3
		lea   s, [BetweenBB+s]
		and   snipers, sliders
		 jz   ..NoPinners
*/
.macro SliderBlockers result, sliders, sq, pinners, pieces, pieces_color, bb, snipers, snipersSqBB, tt, bishops, rooks, temp
        ldr  \snipers, [x20, 8*Queen]
        mov  \bb, \snipers

        orr  \snipers, \snipers, \rooks
        lea  \temp, RookAttacksPDEP
        ldr  \tt, [\temp, \sq, lsl 3]
        and  \snipers, \snipers, \tt

        orr  \bb, \bb, \bishops
        lea  \temp, BishopAttacksPDEP
        ldr  \tt, [\temp, \sq, lsl 3]
        and  \bb, \bb, \tt

        orr  \snipers, \snipers, \bb

        lea  \tt, BetweenBB
        add  \sq, \tt, \sq, lsl 9
       ands  \snipers, \snipers, \sliders
        beq  SliderBlockers.NoPinners\@
/*
..YesPinners:
	       blsi   snipersSqBB, snipers
		bsf   t, snipers
		mov   b, pieces
		and   b, qword[s+8*t]
		mov   t, pieces_color
		and   t, b
	      cmovz   snipersSqBB, t
		lea   t, [b-1]
	       test   t, b
		jnz   ..MoreThanOne
		 or   result, b
		 or   pinners, snipersSqBB
..MoreThanOne:
	       blsr   snipers, snipers, t
		jnz   ..YesPinners
..NoPinners:
}
*/
SliderBlockers.YesPinners\@:
       rbit  \tt, \snipers
        neg  \snipersSqBB, \snipers
        clz  \tt, \tt
        and  \snipersSqBB, \snipersSqBB, \snipers
        ldr  \bb, [\sq, \tt, lsl 3]
        and  \bb, \bb, \pieces
       ands  \tt, \bb, \pieces_color
       csel  \snipersSqBB, \tt, \snipersSqBB, eq
        sub  \tt, \bb, 1
        and  \tt, \tt, \bb
       cbnz  \tt, SliderBlockers.MoreThanOne\@
        orr  \result, \result, \bb
        orr  \pinners, \pinners, \snipersSqBB
SliderBlockers.MoreThanOne\@:
        sub  \tt, \snipers, 1
        and  \snipers, \snipers, \tt
       cbnz  \snipers, SliderBlockers.YesPinners\@
SliderBlockers.NoPinners\@:
.endm


