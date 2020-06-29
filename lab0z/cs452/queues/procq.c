/* This code is my own work, it was written without consulting code written by other students (Wesley Harmon) */

#include "../h/types.h"
#include "../h/const.h"
#include "../h/procq.h"
#include "../h/asl.e"

/* Declaration/initialization of variables */
proc_t procTable[MAXPROC];
proc_t *procFree_h;


panic() {
	//HALT();
}

/* Insert element pointed to by p into the process queue where tp contains 
   the pointer/index to the tail (last element). Update the tail pointer accordingly. 
   If the process is already in SEMMAX queues, call the panic function. */
insertProc(proc_link *tp, proc_t *p) {
	
	if (p->qcount == SEMMAX) panic();
	int i;
	for (i = 0; i < SEMMAX; i++) {
		if (p->p_link[i].index == ENULL) break;
	}
	if (tp->next == (proc_t *) ENULL) {	
		/* trivial case of empty queue */	
		tp->index = i;
		tp->next = p;
		tp->next->qcount++;
		tp->next->p_link[tp->index].index = i;
		tp->next->p_link[tp->index].next = p;
	} else {
		int index = tp->next->p_link[tp->index].index;
        	proc_t *next = tp->next->p_link[tp->index].next;
        	p->p_link[i].index = index;
        	p->p_link[i].next = next;
        	tp->next->p_link[tp->index].index = i;
        	tp->next->p_link[tp->index].next = p;
		tp->index = i;
	        tp->next = p;
        	tp->next->qcount++;
	}

}

/* Remove first element from the process queue whose tail is pointed to by tp.
   Return ENULL if the queue was initially empty, otherwise return the pointer
   to the removed element. Update the pointer to the tail of queue if necessary. */
proc_t * removeProc(proc_link *tp) {
	
	int headind, tailind, newind;
	proc_t *head, *tail, *new;
	
	if (tp->next == (proc_t *) ENULL) return (proc_t *) ENULL;
	
	tail = tp->next;
	tailind = tp->index;
	head = tail->p_link[tailind].next;
	headind = tail->p_link[tailind].index;

	if (head == tp->next) {	
		/* tp is only element  */
		tail->p_link[tailind].next = (proc_t *) ENULL;
		tail->p_link[tailind].index = ENULL;			
		tp->next = (proc_t *) ENULL;
		tp->index = ENULL;
		head->qcount--;
		return head;		
	} else {
		/* queue has multiple elements; update queue and remove proc  */
		new = head->p_link[headind].next;
		newind = head->p_link[headind].index;
		tail->p_link[tailind].next = new;
		tail->p_link[tailind].index = newind;
		head->p_link[headind].next = (proc_t *) ENULL;
                head->p_link[headind].index = ENULL;
                head->qcount--;
		return head;
	}
}


/* Remove the process table entry pointed to by p from the queue whose tail is 
   pointed to by tp. Update the pointer to the tail of queue if necessary. 
   If the desired entry is not in the defined queue (an error condition) return ENULL.
   Otherwise, return p. Note that p can point to any element of the queue 
   (not necessarily the head element). */
proc_t * outProc(proc_link *tp, proc_t *p) {
	
	proc_t *tail = tp->next;
	int tailind = tp->index;
	proc_t *pt = tp->next;
	int index = tp->index;

	if (pt->p_link[index].next == (proc_t *) ENULL) {
		/* trying to remove from process that does not exist on queue */
		return (proc_t *) ENULL;
	}

	if (pt->p_link[index].next == pt) {
		/* only 1 element in queue */
		if (pt != p) return (proc_t *) ENULL;
		pt->p_link[index].next = (proc_t *) ENULL;
		pt->p_link[index].index = ENULL;
		tp->next = (proc_t *) ENULL;
		tp->index = ENULL;
		pt->qcount--;
		return pt;
	}

	proc_t *tmp, *out;
	int tmpind, outind;

	do {
		if (pt->p_link[index].next == p) {
			/* remove proc p from the queue */
			outind = pt->p_link[index].index;
			out = pt->p_link[index].next;
			if (out == tail) {
				/* update pointer to tail of queue */
				tp->next = pt;
				tp->index = index;
			}
			pt->p_link[index].next = out->p_link[outind].next;
			pt->p_link[index].index = out->p_link[outind].index;
			out->qcount--;
			out->p_link[outind].index = ENULL;
			out->p_link[outind].next = (proc_t *) ENULL;
			return p;
		}	
		tmp = pt->p_link[index].next;
		tmpind = pt->p_link[index].index;
		pt = tmp;
		index = tmpind;
		
	} while (pt != tp->next);
	
	return (proc_t *) ENULL;
}


/* Return ENULL if the procFree list is empty. Otherwise, remove and element from
   the procFree list and return a pointer to it. */
proc_t * allocProc() {

	if (procFree_h == (proc_t *) ENULL) return (proc_t *) ENULL;
	proc_t *old = procFree_h;
	proc_t *new = procFree_h->next;
	procFree_h = new;
	old->next = (proc_t *) ENULL;
	return old;

}


/* Return the element pointed to by p into the procFree list. */
freeProc(proc_t *p) {

	if (procFree_h == (proc_t *) ENULL) {	
		/* free list is empty, trivial case */	
		procFree_h = p;
		procFree_h->next = (proc_t *) ENULL;
	} else { 
		/* p is added to head of free list */					
		proc_t *temp = procFree_h;
		procFree_h = p;
		procFree_h->next = temp;
	}

}


/* Return a pointer to the process table entry at the head of the queue. The tail of
   the queue is pointed to by tp. */
proc_t * headQueue(proc_link tp) {
	
	return tp.next->p_link[tp.index].next;

}


/* Initialize the procFree list to contain all the elements of the array procTable. 
   Will be called only once during data structure initialization. */
initProc() {
	
	int i,j;
	for (i = 0; i < MAXPROC; i++) {
		for (j = 0; j < SEMMAX; j++) {
			procTable[i].p_link[j].index = ENULL;
			procTable[i].p_link[j].next = &procTable[i+1];
			procTable[i].semvec[j] = (int *) ENULL;
		}
		procTable[i].next = &procTable[i+1];	
	}
	for (i = 0; i < MAXPROC; i++) {
		procTable[MAXPROC-1].p_link[i].index = ENULL;
		procTable[MAXPROC-1].p_link[i].next = (proc_t *) ENULL;
	}
	for (i = 0; i < SEMMAX; i++) {
		procTable[MAXPROC-1].semvec[i] = (int *) ENULL;
	}
	procTable[MAXPROC-1].next = (proc_t *) ENULL;
	procFree_h = &procTable[0];

}



















