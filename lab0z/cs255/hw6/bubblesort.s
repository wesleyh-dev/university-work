        xdef BubbleSort


BubbleSort:
******************************************************
* Write your bubble sort assembler subroutine here
*
*    BubbleSort: input  D0 = addres of int array
*                       D1 = N
******************************************************
	move.l #0,Done
	move.l #1,k
While:
	move.l Done,d2
	cmp.l #0,d2
	bne WhileEnd
	move.l #1,Done
	move.l #0,i
For:
	move.l i,d2
	move.l d1,d3
	sub.l k,d3
	cmp.l d3,d2
	bge ForEnd
If:
	move.l i,d2
	muls #4,d2
	move.l i,d3
	add.l #1,d3
	muls #4,d3
	movea.l d0,a0
	move.l 0(a0,d2),d2
	move.l 0(a0,d3),d3
	cmp.l d3,d2
	ble IfEnd
	move.l i,d2
	muls #4,d2
	move.l i,d3
	add.l #1,d3
	muls #4,d3
	movea.l d0,a0
	move.l 0(a0,d2),d4
	move.l d4,Help
	move.l 0(a0,d3),d5
	move.l d5,0(a0,d2)
	move.l Help,0(a0,d3)
	move.l #0,Done
IfEnd:
	move.l i,d2
	add.l #1,d2
	move.l d2,i
	bra For
ForEnd:
	move.l k,d2
	add.l #1,d2
	move.l d2,k
	bra While	
WhileEnd:
	rts

* *****************************************************************************
* If you need local variables, you can add variable definitions below this line
* *****************************************************************************
Done: ds.l 1
Help: ds.l 1
k:    ds.l 1
i:    ds.l 1

        end
