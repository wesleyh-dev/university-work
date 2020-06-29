/*********************************P1TEST.C*******************************
 *
 *	Test program for the modules ASL and procq (phase 1).
 *
 *	Produces progress messages in the array ``okbuf[]''
 *		and error messages in the array ``errbuf[]''.
 *		Aborts as soon as an error is detected.
 */

#include "../h/const.h"
#include "../h/types.h"

#include "../h/procq.e"
#include "../h/asl.e"

#define	MAXPROC	20

int proc_ctr = 0;

char okbuf[512];	/* sequence of progress messages */
char errbuf[128];	/* contains reason for failing */
char msgbuf[128];	/* nonrecoverable error message before shut down */
int sem[MAXPROC];
int onesem;
proc_t	*procp[MAXPROC], *p, *q, *q1, *q2, *q3, *firstproc, *lastproc, *midproc;
char *mp = okbuf;

proc_link ql;

main()
{
	register int i;
    int maxloop = 3;

	initProc();
	addokbuf("Initialized process queues   ");

	initSemd();
	addokbuf("Initialized semaphore list   ");

	for (i = 0; i < maxloop; i++) {
		procp[i] = allocProc();
		if (insertBlocked(&sem[i], procp[i]))
			adderrbuf("insertBlocked(0): unexpected TRUE   ");
		if (insertBlocked(&sem[i+1], procp[i]))
			adderrbuf("insertBlocked(1): unexpected TRUE   ");
		if (insertBlocked(&sem[i+2], procp[i]))
			adderrbuf("insertBlocked(2): unexpected TRUE   ");
	}
	addokbuf("ASL inserted ok   ");

	for (i = 0; i < maxloop; i++) {
		q1 = removeBlocked(&sem[i]);
		q2 = removeBlocked(&sem[i+1]);
		q3 = removeBlocked(&sem[i+2]);

        if ( q1 != q2 || q1 != q3 ) 
			adderrbuf("removeBlocked: removed procs are not the same");
		if (q1 == (proc_t *) ENULL)
			adderrbuf("removeBlocked: wouldn't remove   ");
		if (q1 != procp[i])
			adderrbuf("removeBlocked: removed wrong element   ");
	}
	addokbuf("ASL removed ok   ");
		
	for (i = 0; i < maxloop; i++) {
		procp[i] = allocProc();
		if (insertBlocked(&sem[i], procp[i]))
			adderrbuf("insertBlocked(3): unexpected TRUE   ");
		if (insertBlocked(&sem[i+1], procp[i]))
			adderrbuf("insertBlocked(4): unexpected TRUE   ");
		if (insertBlocked(&sem[i+2], procp[i]))
			adderrbuf("insertBlocked(5): unexpected TRUE   ");

		if (headBlocked(&sem[i+2]) != procp[i])
			adderrbuf("headBlocked(5): wrong process at the head of the queue   ");
	}

	q=procp[1];
	/* remove a process from the tail, middle and head of a queue */
	p = outBlocked(q);
	if (p != q)
		adderrbuf("outBlocked(1): couldn't remove from valid queue   ");

	p = outBlocked(q);
	if (p != (proc_t *) ENULL)
		adderrbuf("outBlocked: removed nonexistent process.  ");
	
	addokbuf("ASL outBlocked ok   ");
	for (i = 0; i < maxloop; i++) {
		q1 = removeBlocked(&sem[i]);
    	if ( q1 != procp[0] ) 
			adderrbuf("removeBlocked: did not remove procp[0]");
	}
	for (i = 2; i < maxloop+2; i++) {
		q1 = removeBlocked(&sem[i]);
    	if ( q1 != procp[2] ) 
			adderrbuf("removeBlocked: did not remove procp[2]");
	}
	for (i = 0; i < maxloop+2; i++) {
		q1 = removeBlocked(&sem[i]);
    	if ( q1 != (proc_t *) ENULL ) 
			adderrbuf("removeBlocked: removed nonexistent proc ");
	}
	addokbuf("ASL module ok   ");
}

addokbuf(sp)
register char *sp;
{
	while ((*mp++ = *sp++) != '\0')
		;
	mp--;
}


adderrbuf(sp)
register char *sp;
{
	register char *ep = errbuf;

	while ((*ep++ = *sp++) != '\0')
		;
/*
	HALT();
*/
        asm("	trap	#0");
}

/*
panic(s)
register char *s;
{
	register char *i=msgbuf;

	while ((*i++ = *s++) != '\0')
		;*/
/*
	HALT();
*/
/*         asm("	trap	#0");
}*/

