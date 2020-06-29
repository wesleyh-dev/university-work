#include "header.h"

typedef struct _seg { /* Bitmap */
	int bits[256];
	struct _seg *next;
} seg;

typedef struct _proc { /* Process statistics */
	int pid;
	int perfs;
	int tested;
	int skipped;
} proc;

struct { /* A new message containing process statistics */
	long type;
	int pid;
	int perfs;
	int tested;
	int skipped;
} procmsg;

struct { /* A message containing a new perfect number */
	long type;
	int perf;
} perfmsg;

struct {
	long type;
	int pid;
} rmprocmsg;

typedef struct _shm { /* Bitmap + array + processes */
	seg *bitpt;
	int perfs[20];
	proc pcs[20];
} shm;

seg *head;
shm *block;

void clearmap(seg *head, int n) { /* Initialize all values in bitmap to 0 */
	int i = 1;
	int j = 0;
	seg *pt = head;
	while(i<n) {
		while(j<SIZEOFBITS) {
			pt->bits[j] = 0;
			j++;
		}
		pt = pt->next;
		i++;
	}
}

void allocatemap(seg *head, int n) { /* Allocate memory for bitmap */
	seg *pt = head;
	int i = 1;
	while (i<n) {
		pt->next = (seg *)malloc(sizeof(seg));
		pt = pt->next;
		i++;
	}
}

int checkprocess() { /* Return -1 if process table is full, index of free position if not full */
	int i = 0;
	while(i<LIMIT) {
		if (!block->pcs[i].pid) return i;
		i++;
	}
	return -1;
}

void addprocess(proc new, int index) { /* Add a new 'compute' process to array to given position */
	block->pcs[index] = new;
	printf("Added a new process with PID = %d\n",block->pcs[index].pid);
}

void updateprocess(proc update, int i) {
	block->pcs[i].perfs = update.perfs;
	block->pcs[i].tested = update.tested;
	block->pcs[i].skipped = update.skipped;
}

void addperfectnum(int perf) { /* Add a perfect number to found array using insertion sort */
	int i = 0;
	int j, k;
	int tmp;
	if (!block->perfs[0]) block->perfs[0] = perf;
	else {
		while(i<LIMIT){
			if(perf > block->perfs[i]) {
				i++;
				continue;
			} else {
				j = i;
				while(1) {
					if(!block->perfs[j+1]) break;
					j++;
				}
				while(j>=i) {
					tmp = block->perfs[j];
					block->perfs[j+1] = tmp;
					j--;
				}
				block->perfs[i] = perf;
				break;
			}
		}
	}
}

int whichint(int j) {	/* Which element within bits[]? */
	int i=((j-1)%BITSPERSEG)/BITSPERINT;
	return i;
}

int whichbit(int j) {   /* Which bit within bits[x]? */
	int b=((j-1)%BITSPERSEG)%BITSPERINT;
	return b;
}

seg* whichshseg(int j){   /* Which segment of bitmap linked list? Return pointer to segment in SHARED MEMORY */
	int segmnt;
	seg *pt;
	pt=head;
	segmnt=(j-1)/BITSPERINT;
	while(segmnt >= 256){
	  pt=pt->next;
	  segmnt -= 256;
	}
	return pt;
}

void setbit(int j) {    /* Set the jth bit of bitmap */
	int i=whichint(j);
	int pos=whichbit(j);
	seg *sgmt=whichshseg(j);
	sgmt->bits[i] |= (1<<pos);
	printf("Set a bit and the output is %d\n",block->bitpt->bits[i]);
}

int testbit(int j) {    /* True if jth bit == 1, false means jth bit has not been tested */
	int i=whichint(j);
	int pos=whichbit(j);
	seg *sgmt=whichshseg(j);
	if ((sgmt->bits[i] & (1<<pos)))
		return 1;

	return 0;
}

int main() {
	int shmid;
	int mqid;
	head  = (seg *)malloc(sizeof(seg)); /* Pointer to head of bitmap list */
	block = (shm *)malloc(sizeof(shm)); /* Pointer to shared memory block */

	allocatemap(head,NUMOFNODES);
	clearmap(head,NUMOFNODES);

	block->bitpt = head; /* Set _seg pointer to head of bitmap linked list */
	setbit(3);

	if ((shmid=shmget(SHMKEY,sizeof(block),IPC_CREAT | 0660)) == -1) { /* Allocate shared memory */
		perror("shmget");
		exit(errno);
	}
	if ((block=((shm *)shmat(shmid,NULL,0))) == (shm *)NULL) { /* Map to address space */
		perror("shmat");
		exit(errno);
	}

	if ((mqid=msgget(MQKEY,IPC_CREAT | 0660)) == -1) { /* Create message queue if necessary */
		perror("msgget");
		exit(errno);
	}



	while (1) {
		struct {
			long type;
			int buf[4];
		} msg;
		msgrcv(mqid,&msg,sizeof(msg.buf),0,0); /* Message received */
		if (msg.type == PROCMSG) { /* Handle a process-related message */
			proc pc; /* Copy process statistics into new process */
			pc.pid = msg.buf[0];
			pc.perfs = msg.buf[1];
			pc.tested = msg.buf[2];
			pc.skipped = msg.buf[3];
			int chk;
			if ((chk = checkprocess()) >= 0) addprocess(pc, chk); /* Add new process to index */
			else { /* The process table is full */
				printf("Process table is full\n");
				pid_t p2kill = (pid_t)pc.pid;
				kill(p2kill, SIGKILL);
			}

		}
		if (msg.type == PERFMSG) { /* Handle a perfect-related message */
			perfmsg.perf = msg.buf[0];
			addperfectnum(perfmsg.perf);
		}
	}
}
