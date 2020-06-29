/* link descriptor type */
typedef struct proc_link {

        int index;
        struct proc_t *next;

} proc_link;

/* process table entry type */
typedef struct proc_t {

        proc_link p_link[SEMMAX];       /* pointers to next entries */
        state_t p_s;                    /* processor state of the process */
        int qcount;                     /* number of queues containing this entry */
        int *semvec[SEMMAX];            /* vector of active semophores for this entry */
        /* TODO: other entries to be added by me */
	struct proc_t *next;		/* pointer to next proc_t entry in free list */

	/* NUCLEUS */
	unsigned int oldcpu_t;
	unsigned int newcpu_t;
	unsigned int cumcpu_t;
	struct proc_t *child;
	struct proc_t *sibling;
	struct proc_t *parent;
	
} proc_t;


