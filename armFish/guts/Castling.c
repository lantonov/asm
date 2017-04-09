
CastleOOLegal_White:
/*
	CastlingJmp   2*White+0, .Good, .Done
.Good:		 or   eax, -1
		ret
.Done:		xor   eax, eax
		ret
*/
        CastlingJmp  (2*White+0), CastleOOLegal_White.Good, CastleOOLegal_White.Done
CastleOOLegal_White.Good:
        mov  w0, -1
        ret
CastleOOLegal_White.Done:
        mov  w0, 0
        ret


CastleOOLegal_Black:
/*
	CastlingJmp   2*Black+0, .Good, .Done
.Good:		 or   eax, -1
		ret
.Done:		xor   eax, eax
		ret
*/
        CastlingJmp  (2*Black+0), CastleOOLegal_Black.Good, CastleOOLegal_Black.Done
CastleOOLegal_Black.Good:
        mov  w0, -1
        ret
CastleOOLegal_Black.Done:
        mov  w0, 0
        ret


CastleOOOLegal_White:
/*
	CastlingJmp   2*White+1, .Good, .Done
.Good:		 or   eax, -1
		ret
.Done:		xor   eax, eax
		ret
*/
        CastlingJmp  (2*White+1), CastleOOOLegal_White.Good, CastleOOOLegal_White.Done
CastleOOOLegal_White.Good:
        mov  w0, -1
        ret
CastleOOOLegal_White.Done:
        mov  w0, 0
        ret


CastleOOOLegal_Black:
/*
	CastlingJmp   2*Black+1, .Good, .Done
.Good:		 or   eax, -1
		ret
.Done:		xor   eax, eax
		ret
*/
        CastlingJmp  (2*Black+1), CastleOOOLegal_Black.Good, CastleOOOLegal_Black.Done
CastleOOOLegal_Black.Good:
        mov  w0, -1
        ret
CastleOOOLegal_Black.Done:
        mov  w0, 0
        ret

