#include "../../h/const.h"
#include "../../h/types.h"
#include "../../h/vpop.h"
#include "../../h/util.h"

#include "../../h/procq.e"
#include "../../h/asl.e"

#include "../../h/trap.e"
#include "../../h/syscall.e"
#include "../../h/int.e"

register int r2 asm("%d2");
register int r3 asm("%d3");
register int r4 asm("%d4");

unsigned int stack;
state_t savearea;
proc_t *tail;

extern int p1();

static void init() {
	
	stack = savearea.s_sp;
	stack = stack - 2*PAGESIZE;
	initProc();
	initSemd();
	trapinit();
	intinit();
}


void schedule() {

	if (tail != (proc_t *) ENULL) {
		intschedule();
		LDST(&tail->p_s);
	} else {
		intdeadlock();
	}
}

void main() {
	
	STST(&savearea);
	init();
	proc_t proc1;
	state_t new;
	new.s_sp = stack;
	new.s_pc = (int)p1;
	new.s_sr.ps_s = 1;
	new.s_sr.ps_m = 0;
	new.s_sr.ps_int = 7;
	proc1.p_s = new;
	proc1.child = (proc_t *) ENULL;
	proc1.sibling = (proc_t *) ENULL;
	proc1.parent = (proc_t *) ENULL;
	tail = &proc1;
	schedule();	
}



