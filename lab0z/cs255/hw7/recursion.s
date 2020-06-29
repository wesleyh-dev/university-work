* ====================================================================
* Do not touch the following xdef:
* ====================================================================
        xdef F
	xdef ElseIf
	xdef If1
	xdef Else
	xdef For
	xdef EndFor


* **************************************************************************
* You can add more xdef directives above if you need more external labels
*
* - Remember that you can only add a label as a break point (stop location) 
*   in EGTAPI if the label has been xdef'ed !!!
*
* - And I am pretty sure you will need to use break point for debugging 
* - in this project... So add more xdef above to export your breakpoints
* **************************************************************************

F:

* ********************************************************************
* Put your (recursive) function F here 
*
* F receives the parameters i, j, k on the stack
* F returns the function value in register d0
*
* Make sure you use the "rts" instruction to return to the caller
* Also: Make sure you DE-ALLOCATE the local variables and restore a6
*       BEFORE you return to the caller !!!
* ********************************************************************
	move.l a6,-(a7)
	move.l a7,a6
	suba.l #8,a7

	move.l 16(a6),d0
	movel. 12(a6),d1
	cmp.l #0,d0
	ble If1
	cmp.l #0,d1
	bgt ElseIf
If1: 
	move.l a6,a7
	move.l (a7)+,a6
	move.l #-1,d0
	rts

ElseIf:
	move.l 16(a6),d0
	move.l 12(a6),d1
	add.l d1,d0
	move.l 8(a6),d1
	cmp.l d1,d0
	bge Else
	
	move.l 16(a6),d0
	move.l 12(a6),d1
	add.l d1,d0
	move.l a6,a7
	move.l (a7)+,a6
	rts

Else:
	move.l #0,-4(a6)
	move.l #1,-8(a6)

For:
	move.l -8(a6),d0
	move.l 8(a6),d1
	cmp.l d1,d0
	bge EndFor	
	move.l -8(a6),d0
	move.l 16(a6),d1
	move.l 12(a6),d2
	move.l 8(a6),d3
	sub.l d0,d1
	sub.l d0,d2
	sub.l #1,d3
	move.l d1,-(a7)
	move.l d2,-(a7)
	move.l d3,-(a7)


	bsr F
	adda.l #12,a7
	move.l -4(a6),d1
	add.l #1,d1
	add.l d0,d1
	move.l d1,-4(a6)

	move.l -8(a6),d0
	add.l #1,d0
	move.l d0,-8(a6)

	bra For

EndFor:
	move.l -4(a6),d0

	move.l a6,a7
	move.l (a7)+,a6
	rts


*====================================================================
* Don't add anything below this line...
* ====================================================================
        end
