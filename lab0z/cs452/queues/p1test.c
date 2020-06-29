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
proc_t	*procp[MAXPROC], *p, *q, *firstproc, *lastproc, *midproc;
char *mp = okbuf;

proc_link ql;

main()
{
	register int i;

	initProc();
	addokbuf("Initialized process queues   ");

	/* Check allocProc */
	for (i = 0; i < MAXPROC; i++) {
		if ((procp[i] = allocProc()) == (proc_t *) ENULL)
			adderrbuf("allocProc: unexpected ENULL   ");
	}
	if (allocProc() != (proc_t *) ENULL) {
		adderrbuf("allocProc: allocated more than MAXPROC entries   ");
	}
	addokbuf("allocProc ok   ");

	/* return the last 10 entries back to free list */
	for (i = 10; i < MAXPROC; i++)
		freeProc(procp[i]);
	addokbuf("freed 10 entries   ");

	/* create a 10-element process queue */
    ql.next = (proc_t *) ENULL;
    ql.index = ENULL;
	addokbuf("Inserting...   ");
	for (i = 0; i < 10; i++) {
		if ((q = allocProc()) == (proc_t *) ENULL)
			adderrbuf("allocProc: unexpected ENULL while insert   ");
		switch (i) {
		case 0:
			firstproc = q;
			break;
		case 5:
			midproc = q;
			break;
		case 9:
			lastproc = q;
			break;
		default:
			break;
		}
		insertProc(&ql, q);
	}
	addokbuf("inserted 10 elements   ");

	/* Check outProc and headQueue */
	if ((q=headQueue(ql)) != firstproc)
		adderrbuf("headQueue failed   ");
	q = outProc(&ql, firstproc);
	if (q == (proc_t *) ENULL || q != firstproc)
		adderrbuf("outProc failed on first entry   ");
	freeProc(q);
	q = outProc(&ql, midproc);
	if (q == (proc_t *) ENULL || q != midproc)
		adderrbuf("outProc failed on middle entry   ");
	freeProc(q);
	if (outProc(&ql, procp[0]) != (proc_t *) ENULL)
		adderrbuf("outProc failed on nonexistent entry   ");
	addokbuf("outProc ok   ");

	/* Check if removeProc and insertProc remove in the correct order */
	addokbuf("Removing...   ");
	for (i = 0; i < 8; i++) {
		if ((q = removeProc(&ql)) == (proc_t *) ENULL)
			adderrbuf("removeProc: unexpected ENULL   ");
		freeProc(q);
	}
	if (q != lastproc)
		adderrbuf("removeProc: failed on last entry   ");
	if (removeProc(&ql) != (proc_t *) ENULL)
		adderrbuf("removeProc: removes too many entries   ");
	for (i = 0; i < 10; i++)
		freeProc(procp[i]);
	addokbuf("insertProc and removeProc ok   ");
	addokbuf("process queue module ok      ");

	/* check ASL */
	initSemd();
	addokbuf("Initialized semaphore list   ");

	/* check removeBlocked and insertBlocked */
	for (i = 10; i < MAXPROC; i++) {
		procp[i] = allocProc();
		if (insertBlocked(&sem[i], procp[i]))
			adderrbuf("insertBlocked(1): unexpected TRUE   ");
	}
	for (i = 0; i < 10; i++) {
		procp[i] = allocProc();
		if (insertBlocked(&sem[i], procp[i]))
			adderrbuf("insertBlocked(2): unexpected TRUE   ");
	}

	/* check if semaphore descriptors are returned to free list */
	p = removeBlocked(&sem[11]);
	if (insertBlocked(&sem[11],p))
		adderrbuf("removeBlocked: fails to return to free list   ");

	if (insertBlocked(&onesem, procp[9]) == FALSE)
		adderrbuf("insertBlocked: inserted more than MAXPROC   ");
	for (i = 10; i< MAXPROC; i++) {
		q = removeBlocked(&sem[i]);
		if (q == (proc_t *) ENULL)
			adderrbuf("removeBlocked: wouldn't remove   ");
		if (q != procp[i])
			adderrbuf("removeBlocked: removed wrong element   ");
		if (insertBlocked(&sem[i-10], q))
			adderrbuf("insertBlocked(3): unexpected TRUE   ");
	}
	if (removeBlocked(&sem[11]) != (proc_t *) ENULL)
		adderrbuf("removeBlocked: removed nonexistent blocked proc   ");
	addokbuf("insertBlocked and removeBlocked ok   ");

	if (headBlocked(&sem[11]) != (proc_t *) ENULL)
		adderrbuf("headBlocked: nonENULL for a nonexistent queue   ");
	if ((q = headBlocked(&sem[9])) == (proc_t *) ENULL)
		adderrbuf("headBlocked(1): ENULL for an existent queue   ");
	if (q != procp[9])
		adderrbuf("headBlocked(1): wrong process returned   ");
	p = outBlocked(q);
	if (p != q)
		adderrbuf("outBlocked(1): couldn't remove from valid queue   ");
	q = headBlocked(&sem[9]);
	if (q == (proc_t *) ENULL)
		adderrbuf("headBlocked(2): ENULL for an existent queue   ");
	if (q != procp[19])
		adderrbuf("headBlocked(2): wrong process returned   ");
	p = outBlocked(q);
	if (p != q)
		adderrbuf("outBlocked(2): couldn't remove from valid queue   ");
	p = outBlocked(q);
	if (p != (proc_t *) ENULL)
		adderrbuf("outBlocked: removed same process twice.");
	if (headBlocked(&sem[9]) != (proc_t *) ENULL)
		adderrbuf("out/headBlocked: unexpected nonempty queue   ");
	addokbuf("headBlocked and outBlocked ok   ");
	addokbuf("ASL module ok   ");
	addokbuf("Successful mission Captain Kirk");
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

