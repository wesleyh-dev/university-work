	xdef InsertList
	xdef Then
	xdef Else

InsertList:

* ********************************************************************
* Put your InsertList function here 
* ********************************************************************

* 8(a6) will contain e
* 12(a6) will contain h

* Prelude==============================	
	move.l a6,-(a7)
	move.l a7,a6
* =====================================

	move.l 12(a6),d0
	cmp.l #0,d0
	beq Then

* Compare VALUES and NOT the addresses!! Made this mistake initially.
	movea.l 12(a6),a0
	move.l 0(a0),d0
	movea.l 8(a6),a1
	move.l 0(a1),d1
	cmp.l d1,d0
	ble Else	

Then:
	movea.l 8(a6),a0
	move.l 12(a6),d0
	move.l d0,4(a0)
	move.l 8(a6),d0

* Postlude=============================
	move.l a6,a7
	move.l (a7)+,a6
* ======================================
	rts	

Else:
	movea.l 12(a6),a0
	move.l 4(a0),-(a7)
	move.l 8(a6),-(a7)
	bsr InsertList
	adda.l #8,a7
	movea.l 12(a6),a0
	move.l d0,4(a0)
	move.l 12(a6),d0
	
* Postlude==============================
	move.l a6,a7
	move.l (a7)+,a6
* ======================================
	rts


        end
