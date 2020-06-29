#include "../../h/const.h"
#include "../../h/types.h"
#include "../../h/vpop.h"

/* constants to allow SYS calls */
#define	DO_CREATEPROC		SYS1	/* create process */
#define	DO_TERMINATEPROC	SYS2	/* terminate process */
#define	DO_SEMOP			SYS3	/* V or P a semaphore */
#define	DO_SPECTRAPVEC		SYS5	/* specify trap vectors for passup */
#define	DO_GETCPUTIME		SYS6	/* get cpu time used to date */
#define	DO_WAITCLOCK		SYS7	/* delay on the clock semaphore */
#define	DO_WAITIO		SYS8	/* delay on a io semaphore */

#define	DO_PASSERN		passeren	/* P a semaphore */
#define	DO_VERHOGEN		verhogen	/* V a semaphore */

#define	CREATENOGOOD		-1	/* to note result of DO_CREATEPROC */

/* just to be clear */
#define SEMAPHORE		int
#define NOLEAVES		4	/* number of leaves of p8 process tree */

register int r2 asm("%d2");
register int r3 asm("%d3");
register int r4 asm("%d4");


SEMAPHORE	
		s[MAXPROC+1],		/* semaphore array */
		testsem=0,		/* for a simple test */
		startp2=0,		/* used to start p2 */
		endp2=0,		/* used to signal p2's demise */
		blkp4=1,		/* used to block second incaration
						of p4 */
		synp4=0,		/* used to allow p4 incarnations to
						synhronize */
		endp4=0,		/* to signal demise of p4 */
		endp5=0,		/* to signal demise of p5 */
		endp8=0,		/* to signal demise of p8 */
		endcreate=0,		/* for a p8 leaf to signal its creation */
		blkp8=0;		/* to block p8 */

int     gchild1sem=0, 
        gchild2sem=0, 
        gchild3sem=0, 
        gchild4sem=0; 

state_t		p2state, 	/* initial states */
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

char		iong[12] = "i/o no good";

int		p2(),p4(),p5(),p5a(),p6(),p7(),p7a(),p5prog(),p5mm();
int		p5sys(),p8root(),child1(),child2();
int     gchild1(),gchild2(),gchild3(),gchild4();


vpop p8semops[4];

#define MAXOKBUF 1000
char okbuf[MAXOKBUF];
char okbufov[14] = "okbufoverflow";
char *mp = okbuf;
/* a procedure to print on the line printer */
print(msg)
	char *msg;			/* address of message to print */
{
  if ((mp + 10) > (okbuf + MAXOKBUF - 1)) {
    msg = okbufov;
  }

  while ((*mp++ = *msg++)  != '\0');
  *(mp-1)='-';

  
}


/*                                                                   */
/*                 p1 -- the root process                            */
/*                                                                   */
p1()
{	

	DO_VERHOGEN((int *)&testsem);					/* V(testsem)   */

	print("p1 v(testsem)");

	/* set up states of the other processes */

	STST(&p8rootstate);
	p8rootstate.s_sp -= PAGESIZE*2;
	p8rootstate.s_pc = (int)p8root;

	STST(&child1state);
	child1state.s_sp = p8rootstate.s_sp - PAGESIZE/2;
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

	/* now for a more rigorous check of process termination */
	for (p8inc=0; p8inc<4; p8inc++) {
		r4 = (int)&p8rootstate;
		DO_CREATEPROC();

		if (r2 == CREATENOGOOD) {
			print("error in process termination");
			HALT();
		}

		r4 = (int)&endp8;
		DO_PASSERN(r4);
	}

	print("p1 finishes OK");
	r3 = 0;
	asm("divu %d3,%d4"); 		/* r4 = r4/0, terminate p1 */

	/* should not reach this point, since p1 just got a program trap */
	print("p1 still alive after progtrap & no trap vector");
	HALT();					/* HALT !!!     */
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
/*  these functions are in libutil.a

verhogen(i)
int *i;
{

    vpop semops[2];
  
	semops[0].op = UNLOCK;
	semops[0].sem = i;
	r3 = 1;
	r4 = (int)semops;
	DO_SEMOP();
}

passeren(i)
int *i;
{

    vpop semops[2];
  
	semops[0].op = LOCK;
	semops[0].sem = i;
	r3 = 1;
	r4 = (int)semops;
	DO_SEMOP();
}
*/
