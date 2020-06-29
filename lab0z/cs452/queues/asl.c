/* This code is my own work, it was written without consulting code written by other students (Wesley Harmon) */

#include "../h/types.h"
#include "../h/const.h"
#include "../h/procq.e"
#include "../h/asl.h"

/* Declare/initialize variables */
semd_t semdTable[MAXPROC];
semd_t *semd_h;
semd_t *semdFree_h;


/* Insert the process table entry pointed to by p at the tail of the process queue
   associated with the semaphore whose address is semAdd. If the semaphore is currently
   not active (there is no descriptor for it in the ASL), allocate a new descriptor from 
   the free list, insert it in the ASL (at the appropriate position), and initialize
   all of the fields. If a new semaphore descriptor needs to be allocated and the free 
   list is empty, return TRUE. In all other cases return FALSE */
insertBlocked(int *semAdd, proc_t *p) {

	if (semd_h == (semd_t *) ENULL) {
		/* No active semaphores, trivial case  */		
		semd_t *as;
		if (semdFree_h == (semd_t *) ENULL) return 1;
		as = semdFree_h;
		semdFree_h = semdFree_h->s_next;
		if (semdFree_h != (semd_t *) ENULL) {
                        semdFree_h->s_prev = (semd_t *) ENULL;
                }

		as->s_next = (semd_t *) ENULL;
		as->s_prev = (semd_t *) ENULL;
		as->s_semAdd = semAdd;

		insertProc(&as->s_link, p);
		int i;
		for (i = 0; i < SEMMAX; i++) {
			if (p->semvec[i] == (int *) ENULL) break;
		}				
		p->semvec[i] = as->s_semAdd;	
		semd_h = as;	
		return 0;
	} 
	
	semd_t *pt;
	pt = semd_h;

	if (semAdd < pt->s_semAdd) { 	
		/* Insert at beginning */		
		semd_t *as;
		if (semdFree_h == (semd_t *) ENULL) return 1;
                as = semdFree_h;
                semdFree_h = semdFree_h->s_next;
		if (semdFree_h != (semd_t *) ENULL) {
                        semdFree_h->s_prev = (semd_t *) ENULL;
                }
		
		as->s_next = pt;
		as->s_prev = (semd_t *) ENULL;
		as->s_semAdd = semAdd;
		
		pt->s_prev = as;
		
		insertProc(&as->s_link, p);
                int i;
                for (i = 0; i < SEMMAX; i++) {
 	               if (p->semvec[i] == (int *) ENULL) break;
                }
		as->s_semAdd = semAdd;
                p->semvec[i] = as->s_semAdd;
		
		semd_h = as;
                return 0;
	}

	while (pt->s_next != (semd_t *) ENULL) {	
		if (pt->s_next->s_semAdd == semAdd) {
			pt = pt->s_next;
			insertProc(&pt->s_link, p);
	                int i;
        	        for (i = 0; i < SEMMAX; i++) {
                	       if (p->semvec[i] == (int *) ENULL) break;
                	}
                	p->semvec[i] = pt->s_semAdd;
                	return 0;
		} else if (pt->s_next->s_semAdd > semAdd) {
			semd_t *as;
			if (semdFree_h == (semd_t *) ENULL) return 1;
			as = semdFree_h;
			semdFree_h = semdFree_h->s_next;
			if (semdFree_h != (semd_t *) ENULL) {
        	                semdFree_h->s_prev = (semd_t *) ENULL;
	                }
			
			as->s_next = pt->s_next;
			as->s_prev = pt;
			pt->s_next->s_prev = as;
			pt->s_next = as;
			as->s_semAdd = semAdd;
			
			insertProc(&as->s_link, p);
                        int i;
                        for (i = 0; i < SEMMAX; i++) {
                               if (p->semvec[i] == (int *) ENULL) break;
                        }
                        p->semvec[i] = as->s_semAdd;
                        return 0;
		}
		pt = pt->s_next;
	}
	semd_t *as;
	if (semdFree_h == (semd_t *) ENULL) return 1;
        as = semdFree_h;
        semdFree_h = semdFree_h->s_next;
        if (semdFree_h != (semd_t *) ENULL) {
        	semdFree_h->s_prev = (semd_t *) ENULL;
        }
	/* Not found insert at the end */
	pt->s_next = as;					
	as->s_prev = pt;
	as->s_next = (semd_t *) ENULL;
	insertProc(&as->s_link, p);
        int i;
        for (i = 0; i < SEMMAX; i++) {
 		if (p->semvec[i] == (int *) ENULL) break;
        }
	as->s_semAdd = semAdd;
        p->semvec[i] = as->s_semAdd;
        return 0;

}


/* Search the ASL for a descriptor of this semaphore. If none is found, return ENULL.
   Otherwise, remove the first process table entry from the process queue of the 
   appropriate semaphore descriptor and return a pointer to it. If the process queue
   for this semaphore becomes empty, remove the descriptor from the ASL and insert
   it in the free list of semaphore descriptors. */
proc_t * removeBlocked(int *semAdd) {

	semd_t *pt;
	pt = semd_h;
	if (pt == (semd_t *) ENULL) return (proc_t *) ENULL;
	if (pt->s_semAdd == semAdd) {				
		/* semd_h is the target descriptor */
		proc_t *p = removeProc(&pt->s_link);
		if (p != (proc_t *) ENULL) {
			int i;
			for (i = 0; i < SEMMAX; i++) {
				if (p->semvec[i] == semAdd) break;
			}
			p->semvec[i] = (int *) ENULL;
		}
		
		if (pt->s_link.next == (proc_t *) ENULL) {
			semd_h = semd_h->s_next;
			pt->s_next = semdFree_h;
			pt->s_prev = (semd_t *) ENULL;
                        semdFree_h = pt;			
		}

		return p;
	}

	while (pt->s_next != (semd_t *) ENULL) {
		if (pt->s_semAdd == semAdd) {
			proc_t *p = removeProc(&pt->s_link);
			if (p != (proc_t *) ENULL) {
	                        int i;
        	                for (i = 0; i < SEMMAX; i++) {
                        	        if (p->semvec[i] == semAdd) break;
                	        }
                       		p->semvec[i] = (int *) ENULL;
                	}
			
			if (pt->s_link.next == (proc_t *) ENULL) {
				pt->s_prev->s_next = pt->s_next;
				pt->s_next->s_prev = pt->s_prev;
				pt->s_next = semdFree_h;
				pt->s_prev = (semd_t *) ENULL;
				semdFree_h = pt;
			}

			return p;
		}

		pt = pt->s_next;
	}

	if (pt->s_semAdd == semAdd) {
        	proc_t *p = removeProc(&pt->s_link);
        	if (p != (proc_t *) ENULL) {
                	int i;
                        for (i = 0; i < SEMMAX; i++) {
                        	if (p->semvec[i] == semAdd) break;
                        }
                        p->semvec[i] = (int *) ENULL;
                }

                if (pt->s_link.next == (proc_t *) ENULL) {
			pt->s_prev->s_next = (semd_t *) ENULL;
			pt->s_next = semdFree_h;
			pt->s_prev = (semd_t *) ENULL;
			semdFree_h = pt;                        
		}

		return p;
        }

	return (proc_t *) ENULL;
}


/* Remove the process table entry pointed to by p from the from the queues associated
   with the appropriate semaphore on the ASL. If the desired entry does not appear in 
   any of the process queues (an error condition), return ENULL. Otherwise, return p. */
proc_t * outBlocked(proc_t *p) {

	int i, count, actual;
	proc_t *res;
	semd_t *pt;
	pt = semd_h;
	count = 0;
	actual = 0;
	while (pt->s_next != (semd_t *) ENULL) {
		res = outProc(&pt->s_link, p);
		if (res == (proc_t *) ENULL) {
			count++;
		} else {
                        for (i = 0; i < SEMMAX; i++) {
                        	if (res->semvec[i] == pt->s_semAdd) break;
                        }
                        res->semvec[i] = (int *) ENULL;

			if (pt->s_link.next == (proc_t *) ENULL) {
                        	pt->s_prev->s_next = pt->s_next;
                        	pt->s_next->s_prev = pt->s_prev;
                        	pt->s_next = semdFree_h;
                        	pt->s_prev = (semd_t *) ENULL;
                        	semdFree_h = pt;
   			}
                }
		actual++;
		pt = pt->s_next;
	}
	res = outProc(&pt->s_link, p);
	if (res == (proc_t *) ENULL) {
                        count++;
        } else {
        	for (i = 0; i < SEMMAX; i++) {
        		if (res->semvec[i] == pt->s_semAdd) break;
        	}
        	res->semvec[i] = (int *) ENULL;

                if (pt->s_link.next == (proc_t *) ENULL) {
			semd_t *prev;
			prev = pt->s_prev;
                	prev->s_next = pt->s_next;
                	pt->s_next = semdFree_h;
                	pt->s_prev = (semd_t *) ENULL;
                	semdFree_h = pt;
                }
        }
	actual++;

	if (count == actual) return (proc_t *) ENULL;
	return p;
}


/* Return a pointer to the process table entry that is at the head of the process queue
   associated with semaphore semAdd. If the list is emptry, return ENULL. */
proc_t * headBlocked(int *semAdd) {

	semd_t *pt = semd_h;
	while (pt->s_next != (semd_t *) ENULL) {
		if (pt->s_semAdd == semAdd) {
			if (pt->s_link.next == (proc_t *) ENULL) return (proc_t *) ENULL;
			proc_t *p = headQueue(pt->s_link);
			return p;
		}
		pt = pt->s_next;
	}
	if (pt->s_semAdd == semAdd) {
		if (pt->s_link.next == (proc_t *) ENULL) return (proc_t *) ENULL;
		proc_t *p = headQueue(pt->s_link);
		return p;
	}
	
	return (proc_t *) ENULL;
}


/* Initialize the semaphore descriptor free list */

initSemd() {
	
	semd_t *prev, *next;
	
	int i;
	semdTable[0].s_prev = (semd_t *) ENULL;
	semdTable[0].s_next = &semdTable[1];
	semdTable[0].s_semAdd = (int *) ENULL;
	semdTable[0].s_link.index = ENULL;
	semdTable[0].s_link.next = (proc_t *) ENULL;	

	for (i = 1; i < MAXPROC-1; i++) {
		semdTable[i].s_prev = &semdTable[i-1];
		semdTable[i].s_next = &semdTable[i+1];
		semdTable[i].s_semAdd = (int *) ENULL;
		semdTable[i].s_link.index = ENULL;
	        semdTable[i].s_link.next = (proc_t *) ENULL;

	}
	
	semdTable[MAXPROC-1].s_prev = &semdTable[i-1];
        semdTable[MAXPROC-1].s_next = (semd_t *) ENULL;
        semdTable[MAXPROC-1].s_semAdd = (int *) ENULL;
	semdTable[MAXPROC-1].s_link.index = ENULL;
        semdTable[MAXPROC-1].s_link.next = (proc_t *) ENULL;

	semdFree_h = &semdTable[0];
	semd_h = (semd_t *) ENULL;	
}


/* This function will be used to determine if there are any semaphores on the ASL.
   Return FALSE if the ASL is empty or TRUE if not empty. */
headASL() {

	if (semd_h == (semd_t *) ENULL) return 0;
	else return 1;

}


