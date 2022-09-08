<<<<<<< HEAD
; test1.asm
label:	; an unused label
	ldc 0
	ldc -5
	ldc +5
loop: br loop ; an infinite loop
br next	;offset should be zero
data 5
next:
    	ldc loop ; load code address
	ldc var1 ; forward ref
var1: data 0 ; a variabl
=======

	   ldc	0x1000
	   a2sp
	   adj -1
	   ldc result
	   stl 0
	   ldc count
	   ldnl 0
	   adj 1
	   call main
>>>>>>> 3a8594a21b3c8699d2f4afee63a3342f2ff423a3
