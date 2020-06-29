#include <string.h>
#include <stdio.h>

#include "../../h/const.h"
#include "../../h/types.h"
#include "../../h/vpop.h"

/* hardware constants */
#define	NOSEGS			4	/* number of segments in seg. table */
#define	PRINT0ADDR		(devreg_t *)0x1450
					/* address of printer0's device
						registers  */

/* nucleus constants the test program needs to know */
#define CLOCKINTERVAL		100000L	/* interval to V clock semaphore */

/* constants to allow SYS calls */
#define	DO_CREATEPROC		SYS1	/* create process */
#define	DO_TERMINATEPROC	SYS2	/* terminate process */
#define	DO_SEMOP			SYS3	/* V or P a semaphore */
#define	DO_SPECTRAPVEC		SYS5	/* specify trap vectors for passup */
#define	DO_GETCPUTIME		SYS6	/* get cpu time used to date */
#define	DO_WAITCLOCK		SYS7	/* delay on the clock semaphore */
#define	DO_WAITIO		SYS8	/* delay on a io semaphore */

#define DO_PASSERN      passeren    /* P a semaphore */
#define DO_VERHOGEN     verhogen    /* V a semaphore */

#define	CREATENOGOOD		-1	/* to note result of DO_CREATEPROC */

/* just to be clear */
#define SEMAPHORE		int
#define NOLEAVES		4	/* number of leaves of p8 process tree */

register int r2 asm("%d2");
register int r3 asm("%d3");
register int r4 asm("%d4");

SEMAPHORE	pr_mut=1,		/* for mutual exclusion on printer */
		s[MAXPROC+1],		/* semaphore array */
		testsem=0,		/* for a simple test */
		startp2=0,		/* used to start p2 */
		endp2=0,		/* used to signal p2's demise */
		endp3=0,		/* used to signal p3's demise */
		blkp4=1,		/* used to block second incaration
						of p4 */
		synp4=0,		/* used to allow p4 incarnations to
						synhronize */
		endp4=0,		/* to signal demise of p4 */
		endp5=0,		/* to signal demise of p5 */
		endp6=0,		/* to signal demise of p6 */
		endp8=0,		/* to signal demise of p8 */
		endcreate=0,		/* for a p8 leaf to signal its creation */
		blkp8=0;		/* to block p8 */

state_t		p2state, p3state,	/* initial states */
		p4state, p5state,
		p6state, p7state,
		p8rootstate, child1state,
       		child2state, gchild1state,
		gchild2state, gchild3state,
		gchild4state;

state_t		pstat_n, mstat_n,	/* trap states for p5 */
		sstat_n, pstat_o,
		mstat_o, sstat_o;

int		p1p2synch=0;		/* to check on p1/p2 synchronization */

int		p8inc;			/* p8's incarnation number */

int		p4inc=1;		/* p4 incarnation number */

int		p7v=0;				/* semaphore p7 will V */

devreg_t 	*p_devrg=PRINT0ADDR;	/* address of printer0's device
						registers */

char		iong[12] = "i/o no good";

int		p2(),p3(),p4(),p5(),p5a(),p6(),p7(),p7a(),p5prog(),p5mm();
int		p5sys(),p8root(),child1(),child2(),p8leaf();

int     gchild1sem=0, 
        gchild2sem=0, 
        gchild3sem=0, 
        gchild4sem=0; 

int     gchild1(),gchild2(),gchild3(),gchild4();

vpop p8semops[4];

/* a procedure to print on the line printer */
print(msg)
	char *msg;			/* address of message to print */
{


	r4 = (int)&pr_mut;
	DO_PASSERN(r4);				/* P(pr_mut) */

	/* set up printer's device registers */
	p_devrg->d_amnt = strlen(msg);
	p_devrg->d_badd = msg;
	p_devrg->d_op = IOWRITE;		/* start i/o    */


	/* wait for the i/o to complete and check result */
	r2 = -1;
	r3 = HARDFAILURE;
	r4 = PRINT0;
	DO_WAITIO();				/* wait for i/o */
	if (r3 != NORMAL || r2 != strlen(msg))  {
		/* try to print out "i/o no good" */
		p_devrg->d_amnt = strlen(iong);
		p_devrg->d_badd = iong;
		p_devrg->d_op = IOWRITE;	/* print `iong' */
		DO_WAITIO();
	}

	r4 = (int)&pr_mut;
	DO_VERHOGEN(r4);				/* V(pr_mut) */
}

/*                                                                   */
/*                 p1 -- the root process                            */
/*                                                                   */

/* Enable interrupts for p1a */

p1()
{
	  extern int p1a();

	  STST(&p2state);
      p2state.s_sr.ps_int = 0;
      p2state.s_pc = (int)p1a;
	  LDST(&p2state);
}

p1a()
{	



	r4 = (int)&testsem;
	DO_VERHOGEN(r4);					/* V(testsem)   */

	print("p1 v(testsem)");

	/* set up states of the other processes */

	/* set up p2's state */
	STST(&p2state);			/* create a state area             */
	p2state.s_sp -= PAGESIZE*2;	/* stack of p2 should sit above    */
	p2state.s_pc = (int)p2;		/* p2 starts executing function p2 */

	STST(&p3state);
	p3state.s_sp = p2state.s_sp - PAGESIZE/2;
	p3state.s_pc = (int)p3;

	STST(&p4state);
	p4state.s_sp = p3state.s_sp - PAGESIZE/2;
	p4state.s_pc = (int)p4;
	
	STST(&p5state);
	p5state.s_sp = p4state.s_sp - PAGESIZE/2 - PAGESIZE/2;	/* because there will be twp p4s */
	p5state.s_pc = (int)p5;

	STST(&p6state);
	p6state.s_sp = p5state.s_sp - PAGESIZE;
	p6state.s_pc = (int)p6;

	STST(&p7state);
	p7state.s_sp = p6state.s_sp - 2 * PAGESIZE;
	p7state.s_pc = (int)p7;

	STST(&p8rootstate);
	p8rootstate.s_sp = p7state.s_sp - 2 * PAGESIZE;
	p8rootstate.s_pc = (int)p8root;

	STST(&child1state);
	child1state.s_sp = p8rootstate.s_sp - 2 * PAGESIZE;
	child1state.s_pc = (int)child1;

	STST(&child2state);
	child2state.s_sp = child1state.s_sp - PAGESIZE/2;
	child2state.s_pc = (int)child2;


	STST(&gchild1state);
	gchild1state.s_sp = child2state.s_sp - PAGESIZE/2;
	gchild1state.s_pc = (int)gchild1;

	STST(&gchild2state);
	gchild2state.s_sp = gchild1state.s_sp - PAGESIZE/2;
	gchild2state.s_pc = (int)gchild2;

	STST(&gchild3state);
	gchild3state.s_sp = gchild2state.s_sp - PAGESIZE/2;
	gchild3state.s_pc = (int)gchild3;

	STST(&gchild4state);
	gchild4state.s_sp = gchild3state.s_sp - PAGESIZE/2;
	gchild4state.s_pc = (int)gchild4;

	/* create process p2 */
	r4 = (int)&p2state;
	DO_CREATEPROC();				/* start p2     */

	print("p2 was started");

	r4 = (int)&startp2;
	DO_VERHOGEN(r4);					/* V(startp2)   */


	r4 = (int)&p3state;
	DO_CREATEPROC();				/* start p3     */

	print("p3 is started");

	r4 = (int)&p4state;
	DO_CREATEPROC();				/* start p4     */

	r4 = (int)&p5state;
	DO_CREATEPROC(); 				/* start p5     */

	r4 = (int)&p6state;
	DO_CREATEPROC();				/* start p6	*/

	r4 = (int)&p7state;
	DO_CREATEPROC();				/* start p7	*/

	/* now for a more rigorous check of process termination */
	for (p8inc=0; p8inc<4; p8inc++) {
		if (p8inc)
			p8rootstate.s_sp -= PAGESIZE/2;
		r4 = (int)&p8rootstate;
		DO_CREATEPROC();

		if (r2 == CREATENOGOOD) {
			print("error in process termination");
			HALT();
		}

		r4 = (int)&endp8;
		DO_PASSERN(r4);
	}

	r4 = (int)&endp2;
	DO_PASSERN(r4);					/* P(endp2)     */

	/* make sure we really blocked */
	if (p1p2synch == 0)
		print("p1/p2 synchronization bad");

	r4 = (int)&endp3;
	DO_PASSERN(r4);					/* P(endp3)     */

	r4 = (int)&endp5;
	DO_PASSERN(r4);					/* P(endp5)     */

	print("p1 knows p5 ended");

	r4 = (int)&blkp4;
	DO_PASSERN(r4);					/* P(blkp4)     */

	r4 = (int)&endp6;
	DO_PASSERN(r4);					/* P(endp6)     */

	print("p1 finishes OK");
	r3=0;
	asm("divu	%d3,%d4");		/* 	r4 = r4/0, terminate p1 */

	/* should not reach this point, since p1 just got a program trap */
	print("p1 still alive after progtrap & no trap vector");
	HALT();					/* HALT !!!     */
}


/* p2 -- semaphore and cputime-SYS test process */
p2()
{

	int		i,j;			/* just to waste time  */
	long		now1,now2;		/* times of day        */
	unsigned int	cpu_t1,cpu_t2;		/* cpu time used       */

	r4 = (int)&startp2;
	DO_PASSERN(r4);				/* P(startp2)   */

	print("p2 starts");

	/* initialize all semaphores in the s[] array */
	for (i=0; i<=MAXPROC; i++)
		s[i] = 0;

	/* V, then P, all of the semaphores in the s[] array */
	for (i=0; i<=MAXPROC; i++)  {
		r4 = (int)&s[i];
		DO_VERHOGEN(r4);			/* V(S[I]) */
		r4 = (int)&s[i];
		DO_PASSERN(r4);			/* P(S[I]) */
		if (s[i] != 0)
			print("p2 bad v/p pairs");
	}

	print("p2 v's successfully");

	/* test of SYS6 */

	STCK(&now1);				/* time of day   */
	DO_GETCPUTIME();			/* CPU time used */
	cpu_t1 = r2;

	/* delay for several milliseconds */
	for (i=1; i<=1500; i++)
		j = i;

	DO_GETCPUTIME();			/* CPU time used */
	cpu_t2 = r2;
	STCK(&now2);				/* time of day  */

	if (((now2 - now1) >= (cpu_t2 - cpu_t1)) &&
			((cpu_t2 - cpu_t1) >= 5000))
		print("p2 is OK");
	else  {
		if ((now2 - now1) < (cpu_t2 - cpu_t1))
			print ("more cpu time than real time");
		if ((cpu_t2 - cpu_t1) < 5000)
			print ("not enough cpu time went by");
		print("p2 blew it!");
	}

	p1p2synch = 1;				/* p1 will check this */

	r4 = (int)&endp2;
	DO_VERHOGEN(r4);				/* V(endp2)     */

	DO_TERMINATEPROC();			/* terminate p2 */

	/* just did a SYS2, so should not get to this point */
	print("p2 didn't terminate");
	HALT();					/* HALT!           */
}


/* p3 -- clock semaphore test process */
p3()
{

	long		time1, time2;
	unsigned int	cpu_t1,cpu_t2;		/* cpu time used       */
	int		i;

	time1 = 0;
	time2 = 0;

	/* loop until we are delayed at least half of clock V interval */
	while (time2-time1 < (CLOCKINTERVAL >> 1))  {
		STCK(&time1);			/* time of day     */
		DO_WAITCLOCK();
		STCK(&time2);			/* new time of day */
	}

	print("p3 - WAITCLOCK OK");

	/* now let's check to see if we're really charge for CPU
	   time correctly */
	DO_GETCPUTIME();
	cpu_t1 = r2;

	for (i=0; i<10; i++) {
		DO_WAITCLOCK();
    }
	
	DO_GETCPUTIME();
	cpu_t2 = r2;

	if (cpu_t2 - cpu_t1 < 1000)
		print("p3 - CPU time incorrectly maintained");
	else
		print("p3 - CPU time correctly maintained");


	r4 = (int)&endp3;
	DO_VERHOGEN(r4);				/* V(endp3)        */

	DO_TERMINATEPROC();			/* terminate p3    */

	/* just did a SYS2, so should not get to this point */
	print("p3 didn't terminate");
	HALT();					/* HALT            */
}


/* p4 -- termination test process */
p4()
{


	switch (p4inc) {
		case 1:
			print("first incarnation of p4 starts");
			p4inc++;
			break;
		case 2:
			print("second incarnation of p4 starts");
			break;
	}

	r4 = (int)&synp4;
	DO_VERHOGEN(r4);				/* V(synp4)     */

	r4 = (int)&blkp4;
	DO_PASSERN(r4);				/* P(blkp4)     */

	r4 = (int)&synp4;
	DO_PASSERN(r4);				/* P(synp4)     */

	/* start another incarnation of p4 running, and wait for  */
	/* a V(synp4). the new process will block at the P(blkp4),*/
	/* and eventually, the parent p4 will terminate, killing  */
	/* off both p4's.                                         */

	p4state.s_sp -= PAGESIZE/2;		/* give another page  */

	r4 = (int)&p4state;
	DO_CREATEPROC();			/* start a new p4    */

	r4 = (int)&synp4;
	DO_PASSERN(r4);				/* wait for it       */

	print("p4 is OK");

	r4 = (int)&endp4;
	DO_VERHOGEN(r4);				/* V(endp4)          */

	DO_TERMINATEPROC();			/* terminate p4      */

	/* just did a SYS2, so should not get to this point */
	print("p4 didn't terminate");
	HALT();					/* HALT            */
}


/* p5 -- SYS5 test process */
p5()
{

	state_t		tstate;		/* a state with translation on */
	int		x;		/* divided by 0 to test prog traps */
	int		seg_no;		/* to set up segment table     */

	print("p5 starts");

	/* set up higher level TRAP handlers (new areas) */
	STST(&pstat_n);
	pstat_n.s_pc = (int)p5prog;
	pstat_n.s_sp -= 60;
	STST(&mstat_n);
	mstat_n.s_pc = (int)p5mm;
	mstat_n.s_sp -= 120;
	STST(&sstat_n);
	sstat_n.s_pc = (int)p5sys;
	sstat_n.s_sp -= 180;

	/* specify trap vectors */
	r2 = PROGTRAP;
	r3 = (int)&pstat_o;
	r4 = (int)&pstat_n;
	DO_SPECTRAPVEC();

	r2 = MMTRAP;
	r3 = (int)&mstat_o;
	r4 = (int)&mstat_n;
	DO_SPECTRAPVEC();

	r2 = SYSTRAP;
	r3 = (int)&sstat_o;
	r4 = (int)&sstat_n;
	DO_SPECTRAPVEC();

	r2 = 1;
	r3 = 0;
	asm("divu %d3,%d2");		/* d2=d2/d3, use asm, otherwise 
					   a library function is called
					   to execute the division */	
        
	STST(&tstate);			/* cause a memory management trap   */
	tstate.s_crp = 0;		/* by loading a state with trans-   */
	tstate.s_sr.ps_m = TRUE;	/* lation on with an invalid  */
                                        /* CPU Root Pointer                 */
	LDST(&tstate);
}

/* second part of p5 - should be entered in user mode */
p5a()
{

	long		time1, time2;

	SYS9();				/* should be passed up */

	/* the first time through, we are in user mode */
	/* and the P should generate a program trap */
	r4 = (int)&endp4;
	DO_PASSERN(r4);			/* P(endp4)                         */

	/* do some delay to be reasonably sure p4 and its offspring are dead */
	time1 = 0;
	time2 = 0;
	while (time2-time1 < (CLOCKINTERVAL >> 1))  {
		STCK(&time1);
		DO_WAITCLOCK();
		STCK(&time2);
	}

	/* if p4 and offspring are really dead, this will increment blkp4 */
	r4 = (int)&blkp4;
	DO_VERHOGEN(r4);			/* P(blkp4) */

	r4 = (int)&endp5;
	DO_VERHOGEN(r4);			/* V(endp5) */

	r2 = PROGTRAP;			/* should cause a termination       */
	r3 = (int)&pstat_o;		/* since this has already been      */
	r4 = (int)&pstat_n;		/* done for PROGTRAPs               */
	DO_SPECTRAPVEC();

	/* should have terminated, so should not get to this point */
	print("p5 didn't terminate");
	HALT();				/* HALT            */
}

/* p5's program trap handler */
p5prog()
{
	switch(pstat_o.s_tmp.tmp_pr.pr_typ) {
	case ZERODIVIDE:
		print("divide by zero");
		pstat_o.s_r[3] = 1;		/* will not happen next time */
		break;
	case PRIVILEGE:
		print("privileged instruction");
		/* return to kernel mode */
		pstat_o.s_sr.ps_s = 1;
		pstat_o.s_pc = (int)p5a;	/* retry in kernel mode */
		break;
	default:
		print("other program trap");
	}

	/* reload old state */
	LDST(&pstat_o);
}

/* p5's memory management trap handler */
p5mm()
{
	print("memory management trap");
	mstat_o.s_sr.ps_m = 0;		/* no more address translation  */
	mstat_o.s_sr.ps_s = 0;	/* but put us in user mode when */
					/* we return                    */
	mstat_o.s_pc = (int)p5a;  	/* return to p5a                */
	LDST(&mstat_o);
}

/* p5's SYS trap handler */
p5sys()
{
	switch (sstat_o.s_sr.ps_s) {
	case 0:
		print("high level SYS call from user mode process");
		break;
	case 1:
		print("high level SYS call from kernel mode process");
		break;
	}
	LDST(&sstat_o);
}

/*p6 -- high level syscall without initializing trap vector*/
p6()
{

	int i,j;
    char tmp[100];
    unsigned int p6systemtime;
    long p6cputime; 

	print("p6 starts");

	for (i=0;i<7;i++) {
		DO_GETCPUTIME();
		p6cputime=r2;
		STCK(&p6systemtime);
		sprintf(tmp,"p6  iteration: %1d , system time %7d , cpu-time %7d",i, p6systemtime, p6cputime);
		print(tmp);
		j=0;
		while(j< (i&1)*5000) j++;
	}

	r4 = (int)&endp6;   /* endp6 is needed for low TIMESLICE */
	DO_VERHOGEN(r4);

	SYS9();		/* should cause termination because p6 has no 
			  trap vector */

	print("p6 still alive after SYS9() without specifying trap vector");

	HALT();
}

/*p7 -- program trap without initializing passup vector*/
p7()
{

	char tmp[10];
	
	print("p7 starts");

	r4=(int) &p7v;
	while ( p7v < 10) {
		sprintf(tmp,"p7: %d ",p7v);
		print(tmp);
		r4=(int) &p7v;
		DO_VERHOGEN(r4);
	}
		
	r4 = 1;
	r3 = 0;
	asm("divu	%d3,%d4");
	
	print("p7 still alive after program trap with no trap vector");
	HALT();
}

/* p8root -- test of termination of subtree of processes              */
/* create a subtree of processes, wait for the leaves to block, signal*/
/* the root process, and then terminate                               */
p8root()
{
	int		grandchild;

	print("p8root starts");

	r4 = (int)&child1state;
	DO_CREATEPROC();

	r4 = (int)&child2state;
	DO_CREATEPROC();

    p8semops[0].op = LOCK;
    p8semops[0].sem = &gchild1sem; 
    p8semops[1].op = LOCK;
    p8semops[1].sem = &gchild2sem; 
    p8semops[2].op = LOCK;
    p8semops[2].sem = &gchild3sem; 
    p8semops[3].op = LOCK;
    p8semops[3].sem = &gchild4sem; 
    r3 = 4;
    r4 = (int)p8semops;
    DO_SEMOP();
	
	r4 = (int)&endp8;
	DO_VERHOGEN(r4);

	DO_TERMINATEPROC();
}

/*child1 & child2 -- create two sub-processes each*/

child1()
{

	print("child1 starts");
	
	r4 = (int)&gchild1state;
	DO_CREATEPROC();
	
	r4 = (int)&gchild2state;
	DO_CREATEPROC();

	r4 = (int)&blkp8;
	DO_PASSERN(r4);
}
child2()
{

	print("child2 starts");
	
	r4 = (int)&gchild3state;
	DO_CREATEPROC();
	
	r4 = (int)&gchild4state;
	DO_CREATEPROC();

	r4 = (int)&blkp8;
	DO_PASSERN(r4);
}

/*gchild -- code for leaf processes*/

gchild1()
{

	print("gchild1 starts");
	
	r4 = (int)&gchild1sem;
	DO_VERHOGEN(r4);

	r4 = (int)&blkp8;
	DO_PASSERN(r4);
}

gchild2()
{

	print("gchild2 starts");
	
	r4 = (int)&gchild2sem;
	DO_VERHOGEN(r4);

	r4 = (int)&blkp8;
	DO_PASSERN(r4);
}

gchild3()
{

	print("gchild3 starts");
	
	r4 = (int)&gchild3sem;
	DO_VERHOGEN(r4);

	r4 = (int)&blkp8;
	DO_PASSERN(r4);
}

gchild4()
{

	print("gchild4 starts");
	
	r4 = (int)&gchild4sem;
	DO_VERHOGEN(r4);

	r4 = (int)&blkp8;
	DO_PASSERN(r4);
}
