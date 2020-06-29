#include "../../h/const.h"
#include "../../h/types.h"
#include "../../h/vpop.h"
#include "../../h/util.h"

#include "../../h/procq.e"
#include "../../h/asl.e"

#include "../../h/main.e"
#include "../../h/syscall.e"
#include "../../h/int.e"

register int r2 asm("%d2");
register int r3 asm("%d3");
register int r4 asm("%d4");

state_t *prog_old;
state_t *prog_new;
state_t *mm_old;
state_t *mm_new;
state_t *sys_old;
state_t *sys_new;

void static trapsyshandler() {

}


void static trapmmhandler() {

	

}


void static trapproghandler() {

}

void trapinit() {

	prog_new->s_pc = (int) trapproghandler;	
	mm_new->s_pc = (int) trapmmhandler;
	sys_new->s_pc = (int) trapsyshandler;

	prog_old = (state_t *) BEGINTRAP;
	prog_new = (state_t *) (BEGINTRAP + sizeof(state_t));
	mm_old = (state_t *) (BEGINTRAP + 2*sizeof(state_t));
	mm_new = (state_t *) (BEGINTRAP + 3*sizeof(state_t));
	sys_old = (state_t *) (BEGINTRAP + 4*sizeof(state_t));
	sys_new = (state_t *) (BEGINTRAP + 5*sizeof(state_t));
	
	*(int *) 0x008 = (int)STLDMM;
	*(int *) 0x00c = (int)STLDADDRESS;
	*(int *) 0x010 = (int)STLDILLEGAL;
        *(int *) 0x014 = (int)STLDZERO;
        *(int *) 0x020 = (int)STLDPRIVILEGE;
        *(int *) 0x08c = (int)STLDSYS;
        *(int *) 0x94 = (int)STLDSYS9;
        *(int *) 0x98 = (int)STLDSYS10;
        *(int *) 0x9c = (int)STLDSYS11;
        *(int *) 0xa0 = (int)STLDSYS12;
        *(int *) 0xa4 = (int)STLDSYS13;
        *(int *) 0xa8 = (int)STLDSYS14;
        *(int *) 0xac = (int)STLDSYS15;
        *(int *) 0xb0 = (int)STLDSYS16;
        *(int *) 0xb4 = (int)STLDSYS17;
        *(int *) 0x100 = (int)STLDTERM0;
        *(int *) 0x104 = (int)STLDTERM1;
        *(int *) 0x108 = (int)STLDTERM2;
        *(int *) 0x10c = (int)STLDTERM3;
        *(int *) 0x110 = (int)STLDTERM4;
        *(int *) 0x114 = (int)STLDPRINT0;
        *(int *) 0x11c = (int)STLDDISK0;
        *(int *) 0x12c = (int)STLDFLOPPY0;
        *(int *) 0x140 = (int)STLDCLOCK;

}



