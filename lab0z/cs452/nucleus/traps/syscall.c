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

creatproc(state_t *old) {
	
	proc_t *p = allocProc();
	if (p == (proc_t *) ENULL) {
		r2 = -1;
		return;
	} else {
		p->p_s = *old;
		if (tail->child == (proc_t *) ENULL) {
			/* tail has no children, bi-directional link created */
			tail->child = p;
			p->child = (proc_t *) ENULL;
			p->sibling = (proc_t *) ENULL;
			p->parent = tail;
		} else {
			/* tail has children, add proc p as last sibling with link to parent */
			proc_t *tmp = tail->child;
			while (tmp->sibling != (proc_t *) ENULL) {
				tmp = tmp->sibling;
			}
			tmp->sibling = p;
			tmp->parent = (proc_t *) ENULL;	
			p->child = (proc_t *) ENULL;
			p->sibling = (proc_t *) ENULL;
			p->parent = tail;		
		}
		tail = p;
		r2 = 0;
		return;
	}
}

/* Depth-first termination of tmp, tmp's children and siblings, if any */
killhelper(proc_t *tmp) {
	
	proc_t *res, *parent, *sibling;	
	if (tmp->child == (proc_t *) ENULL && tmp->sibling == (proc_t *) ENULL) {
		/* No children or siblings, base case */
		parent = tmp->parent;
		parent->child = (proc_t *) ENULL;
		tmp->parent = (proc_t *) ENULL;
		res = outBlocked(tmp);
		freeProc(tmp);
	} else {
		/* Kill all children recursively, terminate all of tmp's siblings (if any) and their progeny */
		killhelper(tmp->child);
		while (tmp->sibling != (proc_t *) ENULL) {
			sibling = tmp->sibling;
			tmp->sibling = (proc_t *) ENULL;
			if (sibling->child != (proc_t *) ENULL) {
				killhelper(sibling->child);
			}
			res = outBlocked(sibling);
			freeProc(sibling);
			tmp = sibling;
		}
		parent = tmp->parent;
		parent->child = (proc_t *) ENULL;
		tmp->parent = (proc_t *) ENULL;
		res = outBlocked(tmp);
		freeProc(tmp);
	}
}

killproc(state_t *old) {
	
	proc_t *res, *parent, *child, *tmp = tail;
	if (tmp->child == (proc_t *) ENULL) {
		/* Target has no children, trivial case */
		res = outBlocked(tmp);
		tail = tmp->parent;
		freeProc(tmp);		
	} else {
		/* Terminate all children, then terminate tmp appropriately */
		child = tmp->child;
		killhelper(child);
		if (tmp->sibling == (proc_t *) ENULL) {
			/* No siblings, terminate easily */
			parent = tmp->parent;
			parent->child = (proc_t *) ENULL;
			tmp->parent = (proc_t *) ENULL;
			tail = tmp->parent;
		} else {
			/* Target has siblings */
			parent = tmp->parent;
			parent->child = tmp->sibling;
			tmp->parent = (proc_t *) ENULL;
			tmp->sibling = (proc_t *) ENULL;
		}
		res = outBlocked(tmp);
		freeProc(tmp);		
	}
}

semop(state_t *old) {
	
	int i, no_ops = r3;
	vpop *semops;
	semops = (vpop *) r4;
	for (i = 0; i < no_ops; i++) {
		
	}
}

notused(state_t *old) {
	HALT();
}

trapstate(state_t *old) {
	
}

getcputime(state_t *old) {

}

trapsysdefault(state_t *old) {

}
