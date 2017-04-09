
Evaluate_Init:
/*
	       push  rbx rsi rdi
*/
        stp  x29, x30, [sp, -16]!
/*
		lea   rsi, [.MobilityBonus_Knight]
		lea   rdi, [MobilityBonus_Knight]
		mov   ecx, 9
	  rep movsd
		lea   rsi, [.MobilityBonus_Bishop]
		lea   rdi, [MobilityBonus_Bishop]
		mov   ecx, 14
	  rep movsd
		lea   rsi, [.MobilityBonus_Rook]
		lea   rdi, [MobilityBonus_Rook]
		mov   ecx, 15
	  rep movsd
		lea   rsi, [.MobilityBonus_Queen]
		lea   rdi, [MobilityBonus_Queen]
		mov   ecx, 28
	  rep movsd
*/
        lea  x0, MobilityBonus_Knight
        adr  x1, Evaluate_Init.MobilityBonus_Knight
        mov  x2, 4*9
         bl  MemoryCopy
        lea  x0, MobilityBonus_Bishop
        adr  x1, Evaluate_Init.MobilityBonus_Bishop
        mov  x2, 4*14
         bl  MemoryCopy
        lea  x0, MobilityBonus_Rook
        adr  x1, Evaluate_Init.MobilityBonus_Rook
        mov  x2, 4*15
         bl  MemoryCopy
        lea  x0, MobilityBonus_Queen
        adr  x1, Evaluate_Init.MobilityBonus_Queen
        mov  x2, 4*28
         bl  MemoryCopy
/*
		lea   rsi, [.Lever]
		lea   rdi, [Lever]
		mov   ecx, 8
	  rep movsd
		lea   rsi, [.ShelterWeakness]
		lea   rdi, [ShelterWeakness]
		mov   ecx, 8*8
	  rep movsd
		lea   rsi, [.StormDanger]
		lea   rdi, [StormDanger]
		mov   ecx, 4*8*8
	  rep movsd
*/
        lea  x0, Lever
        adr  x1, Evaluate_Init.Lever
        mov  x2, 4*8
         bl  MemoryCopy
        lea  x0, ShelterWeakness
        adr  x1, Evaluate_Init.ShelterWeakness
        mov  x2, 4*8*8
         bl  MemoryCopy
        lea  x0, StormDanger
        adr  x1, Evaluate_Init.StormDanger
        mov  x2, 4*4*8*8
         bl  MemoryCopy
/*
		lea   rdi, [ThreatBySafePawn]
		lea   rsi, [.ThreatBySafePawn]
		mov   ecx, 8
	  rep movsd
		lea   rsi, [.ThreatBySafePawn]
		mov   ecx, 8
	  rep movsd
		lea   rdi, [Threat_Minor]
		lea   rsi, [.Threat_Minor]
		mov   ecx, 8
	  rep movsd
		lea   rsi, [.Threat_Minor]
		mov   ecx, 8
	  rep movsd
		lea   rdi, [Threat_Rook]
		lea   rsi, [.Threat_Rook]
		mov   ecx, 8
	  rep movsd
		lea   rsi, [.Threat_Rook]
		mov   ecx, 8
	  rep movsd
*/
        lea  x0, ThreatBySafePawn
        adr  x1, Evaluate_Init.ThreatBySafePawn
        mov  x2, 4*8
         bl  MemoryCopy
        adr  x1, Evaluate_Init.ThreatBySafePawn
        mov  x2, 4*8
         bl  MemoryCopy
        lea  x0, Threat_Minor
        adr  x1, Evaluate_Init.Threat_Minor
        mov  x2, 4*8
         bl  MemoryCopy
        adr  x1, Evaluate_Init.Threat_Minor
        mov  x2, 4*8
         bl  MemoryCopy
        lea  x0, Threat_Rook
        adr  x1, Evaluate_Init.Threat_Rook
        mov  x2, 4*8
         bl  MemoryCopy
        adr  x1, Evaluate_Init.Threat_Rook
        mov  x2, 4*8
         bl  MemoryCopy

/*
		lea   rsi, [.PassedRank]
		lea   rdi, [PassedRank]
		mov   ecx, 8
	  rep movsd
		lea   rsi, [.PassedFile]
		lea   rdi, [PassedFile]
		mov   ecx, 8
	  rep movsd
*/
        lea  x0, PassedRank
        adr  x1, Evaluate_Init.PassedRank
        mov  x2, 4*8
         bl  MemoryCopy
        lea  x0, PassedFile
        adr  x1, Evaluate_Init.PassedFile
        mov  x2, 4*8
         bl  MemoryCopy
/*
		lea   rsi, [.QuadraticOurs]
		lea   rdi, [DoMaterialEval_Data]
		mov   ecx, 8*(6+6)
	  rep movsd
*/
        lea  x0, DoMaterialEval_Data
        adr  x1, Evaluate_Init.QuadraticOurs
        mov  x2, 4*8*(6+6)
         bl  MemoryCopy
/*
		lea   rdi, [KingFlank]
		mov   rax, (FileABB or FileBBB or FileCBB or FileDBB)
	      stosq
	      stosq
	      stosq
		shl   rax, 2
	      stosq
	      stosq
		shl   rax, 2
	      stosq
	      stosq
	      stosq
*/
        lea  x0, DoMaterialEval_Data
        mov  x1, FileABB | FileBBB | FileCBB | FileDBB
        str  x1, [x0], 8
        str  x1, [x0], 8
        str  x1, [x0], 8
        lsl  x1, x1, 2
        str  x1, [x0], 8
        str  x1, [x0], 8
        lsl  x1, x1, 2
        str  x1, [x0], 8
        str  x1, [x0], 8
        str  x1, [x0], 8
/*
        ; copy Knight, Bishop, Rook, and Queen all at once
                lea   rsi, [.Protector_Knight]
                lea   rdi, [Protector_Knight]
                mov   ecx, 4*8
          rep movsd
*/
        lea  x0, Protector_Knight
        adr  x1, Evaluate_Init.Protector_Knight
        mov  x2, 4*4*8
         bl  MemoryCopy
/*
		pop   rdi rsi rbx
		ret
*/
        ldp  x29, x30, [sp], -16
        ret

        .balign 4
Evaluate_Init.MobilityBonus_Knight:
 .word (-75 << 16) + (-76)
 .word (-57 << 16) + (-54)
 .word (- 9 << 16) + (-28)
 .word ( -2 << 16) + (-10)
 .word (  6 << 16) + (5)
 .word ( 14 << 16) + (12)
 .word ( 22 << 16) + (26)
 .word ( 29 << 16) + (29)
 .word ( 36 << 16) + (29)

Evaluate_Init.MobilityBonus_Bishop:
 .word (-48 << 16) + (-59)
 .word (-20 << 16) + (-23)
 .word (16 << 16) + (-3)
 .word (26 << 16) + (13)
 .word (38 << 16) + (24)
 .word (51 << 16) + (42)
 .word (55 << 16) + (54)
 .word (63 << 16) + (57)
 .word (63 << 16) + (65)
 .word (68 << 16) + (73)
 .word (81 << 16) + (78)
 .word (81 << 16) + (86)
 .word (91 << 16) + (88)
 .word (98 << 16) + (97)

Evaluate_Init.MobilityBonus_Rook:
 .word (-60 << 16) + (-77)
 .word (-26 << 16) + (-20)
 .word (-11 << 16) + (27)
 .word (-6 << 16) + (57)
 .word (-3 << 16) + (69)
 .word (-1 << 16) + (82)
 .word (10 << 16) + (109)
 .word (16 << 16) + (121)
 .word (24 << 16) + (131)
 .word (25 << 16) + (143)
 .word (32 << 16) + (155)
 .word (32 << 16) + (163)
 .word (43 << 16) + (167)
 .word (48 << 16) + (171)
 .word (56 << 16) + (173)

Evaluate_Init.MobilityBonus_Queen:
 .word (-39 << 16) + (-36)
 .word (-21 << 16) + (-15)
 .word (3 << 16) + (8)
 .word (3 << 16) + (18)
 .word (14 << 16) + (34)
 .word (22 << 16) + (54)
 .word (28 << 16) + (61)
 .word (41 << 16) + (73)
 .word (43 << 16) + (79)
 .word (48 << 16) + (92)
 .word (56 << 16) + (94)
 .word (60 << 16) + (104)
 .word (60 << 16) + (113)
 .word (66 << 16) + (120)
 .word (67 << 16) + (123)
 .word (70 << 16) + (126)
 .word (71 << 16) + (133)
 .word (73 << 16) + (136)
 .word (79 << 16) + (140)
 .word (88 << 16) + (143)
 .word (88 << 16) + (148)
 .word (99 << 16) + (166)
 .word (102 << 16) + (170)
 .word (102 << 16) + (175)
 .word (106 << 16) + (184)
 .word (109 << 16) + (191)
 .word (113 << 16) + (206)
 .word (116 << 16) + (212)

Evaluate_Init.Lever:
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (17 << 16) + (16)
 .word (33 << 16) + (32)
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)


Evaluate_Init.Doubled:
 .word (0 << 16) + (0)
 .word (18 << 16) + (38)
 .word (9 << 16) + (19)
 .word (6 << 16) + (12)
 .word (4 << 16) + (9)
 .word (3 << 16) + (7)
 .word (3 << 16) + (6)
 .word (2 << 16) + (5)


// ShelterWeakness and StormDanger are twice as big
// to avoid an anoying min(f,FILE_H-f) in ShelterStorm
Evaluate_Init.ShelterWeakness:
 .word 100, 20, 10, 46, 82,  86,  98, 0
 .word 116,  4, 28, 87, 94, 108, 104, 0
 .word 109,  1, 59, 87, 62,  91, 116, 0
 .word  75, 12, 43, 59, 90,  84, 112, 0
 .word  75, 12, 43, 59, 90,  84, 112, 0
 .word 109,  1, 59, 87, 62,  91, 116, 0
 .word 116,  4, 28, 87, 94, 108, 104, 0
 .word 100, 20, 10, 46, 82,  86,  98, 0

Evaluate_Init.StormDanger:
 .word 4, 73, 132, 46, 31 ,  0,0,0
 .word 1, 64, 143, 26, 13 ,  0,0,0
 .word 1, 47, 110, 44, 24 ,  0,0,0
 .word 0, 72, 127, 50, 31 ,  0,0,0
 .word 0, 72, 127, 50, 31 ,  0,0,0
 .word 1, 47, 110, 44, 24 ,  0,0,0
 .word 1, 64, 143, 26, 13 ,  0,0,0
 .word 4, 73, 132, 46, 31 ,  0,0,0

 .word 22,  45,  104, 62,  6 , 0,0,0
 .word 31,  30,   99, 39, 19 , 0,0,0
 .word 23,  29,   96, 41, 15 , 0,0,0
 .word 21,  23,  116, 41, 15 , 0,0,0
 .word 21,  23,  116, 41, 15 , 0,0,0
 .word 23,  29,   96, 41, 15 , 0,0,0
 .word 31,  30,   99, 39, 19 , 0,0,0
 .word 22,  45,  104, 62,  6 , 0,0,0

 .word  0,  0,   79, 23,  1 , 0,0,0
 .word  0,  0,  148, 27,  2 , 0,0,0
 .word  0,  0,  161, 16,  1 , 0,0,0
 .word  0,  0,  171, 22, 15 , 0,0,0
 .word  0,  0,  171, 22, 15 , 0,0,0
 .word  0,  0,  161, 16,  1 , 0,0,0
 .word  0,  0,  148, 27,  2 , 0,0,0
 .word  0,  0,   79, 23,  1 , 0,0,0

 .word  0, -290, -274, 57, 41 , 0,0,0
 .word  0,   60,  144, 39, 13 , 0,0,0
 .word  0,   65,  141, 41, 34 , 0,0,0
 .word  0,   53,  127, 56, 14 , 0,0,0
 .word  0,   53,  127, 56, 14 , 0,0,0
 .word  0,   65,  141, 41, 34 , 0,0,0
 .word  0,   60,  144, 39, 13 , 0,0,0
 .word  0, -290, -274, 57, 41 , 0,0,0



Evaluate_Init.ThreatBySafePawn:
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (176 << 16) + (139)
 .word (131 << 16) + (127)
 .word (217 << 16) + (218)
 .word (203 << 16) + (215)
 .word (0 << 16) + (0)

Evaluate_Init.Threat_Minor:
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (0 << 16) + (33)
 .word (45 << 16) + (43)
 .word (46 << 16) + (47)
 .word (72 << 16) + (107)
 .word (48 << 16) + (118)
 .word (0 << 16) + (0)

Evaluate_Init.Threat_Rook:
 .word (0 << 16) + (0)
 .word (0 << 16) + (0)
 .word (0 << 16) + (25)
 .word (40 << 16) + (62)
 .word (40 << 16) + (59)
 .word (0 << 16) + (34)
 .word (35 << 16) + (48)
 .word (0 << 16) + (0)



Evaluate_Init.PassedFile:
 .word (9 << 16) + (10)
 .word (2 << 16) + (10)
 .word (1 << 16) + (-8)
 .word (-20 << 16) + (-12)
 .word (-20 << 16) + (-12)
 .word (1 << 16) + (-8)
 .word (2 << 16) + (10)
 .word (9 << 16) + (10)


Evaluate_Init.PassedRank:
 .word 0
 .word (5 << 16) + (7)
 .word (5 << 16) + (14)
 .word (31 << 16) + (38)
 .word (73 << 16) + (73)
 .word (166 << 16) + (166)
 .word (252 << 16) + (252)
 .word 0



Evaluate_Init.Protector_Knight:
 .word (  0 << 16) + (  0)
 .word (  7 << 16) + (  9)
 .word (  7 << 16) + (  1)
 .word (  1 << 16) + (  5)
 .word (-10 << 16) + ( -4)
 .word ( -1 << 16) + ( -4)
 .word ( -7 << 16) + ( -3)
 .word (-16 << 16) + (-10)

Evaluate_Init.Protector_Bishop:
 .word (  0 << 16) + ( 0)
 .word ( 11 << 16) + ( 8)
 .word ( -7 << 16) + (-1)
 .word ( -1 << 16) + (-2)
 .word ( -1 << 16) + (-7)
 .word (-11 << 16) + (-3)
 .word ( -9 << 16) + (-1)
 .word (-16 << 16) + (-1)

Evaluate_Init.Protector_Rook:
 .word (  0 << 16) + ( 0)
 .word ( 10 << 16) + ( 0)
 .word ( -2 << 16) + ( 2)
 .word ( -5 << 16) + ( 4)
 .word ( -6 << 16) + ( 2)
 .word (-14 << 16) + (-3)
 .word ( -2 << 16) + (-9)
 .word (-12 << 16) + (-7)

Evaluate_Init.Protector_Queen:
 .word (  0 << 16) + ( 0)
 .word (  3 << 16) + (-5)
 .word (  2 << 16) + (-5)
 .word ( -4 << 16) + ( 0)
 .word ( -9 << 16) + (-6)
 .word ( -4 << 16) + ( 7)
 .word (-13 << 16) + (-7)
 .word (-10 << 16) + (-7)


Evaluate_Init.QuadraticOurs:
	.word 0, 1667,    0,    0,    0,	 0,    0,    0
	.word 0,	40,    2,    0,    0,	 0,    0,    0
	.word 0,	32,  255,   -3,    0,	 0,    0,    0
	.word 0,	 0,  104,    4,    0,	 0,    0,    0
	.word 0,  -26,   -2,   47,  105, -149,    0,    0
	.word 0, -185,   24,  122,  137, -134,    0,    0
Evaluate_Init.QuadraticTheirs:
	.word 0,	 0,    0,    0,    0,	 0,    0,    0
	.word 0,	36,    0,    0,    0,	 0,    0,    0
	.word 0,	 9,   63,    0,    0,	 0,    0,    0
	.word 0,	59,   65,   42,    0,	 0,    0,    0
	.word 0,	46,   39,   24,  -24,	 0,    0,    0
	.word 0,  101,  100,  -37,  141,  268,    0,    0
