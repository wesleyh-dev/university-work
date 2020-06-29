#include "header.h"

shm *block;
int shmid;
int mqid;
int n; /* The ultimate counter */
int pind;

void removeprocess(int pid) { /* Remove the process from shared table and terminate */
	int i = 0;
	while (i<LIMIT) {
		if (block->pcs[i].pid == pid) {
			block->pcs[i].pid = 0;
			block->pcs[i].perfs = 0;
			block->pcs[i].tested = 0;
			block->pcs[i].skipped = 0;
			break;
		}
		i++;
	}
	printf("Safely removed process with PID = %d\n",pid);
	exit(1);
}

int getindex() { /* Retrieve index of this process in the shared memory blocks pcs array */
	int id = 0;
	while (id<LIMIT) {
		if (block->pcs[id].pid == procmsg.pid) return id;
		id++;
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

void setbit(int j) {    /* Set the jth bit of bitmap */
	int i=whichint(j);
	int pos=whichbit(j);
	block->bitmap[i] |= (1<<pos);
	printf("COMPUTE Set a bit and the output is %d\n",block->bitmap[i]);
}

int testbit(int j) {    /* True if jth bit == 1, false means jth bit has not been tested */
	int i=whichint(j);
	int pos=whichbit(j);
	if ((block->bitmap[i] & (1<<pos)))
		return 1;

	return 0;
}

void perfect(int start) { /* Test if number is perfect */
	int i, sum;
	n = start;
	while (1) {
		sum=1;
		if (testbit(n)) { /* Skip this number because it has been tested */
			block->pcs[pind].skipped++;
		} else { /* Test this bit for perfectness */
			for (i=2;i<n;i++) {
				if (!(n%i)) sum+=i;
			}
			if (sum == n) {
				if (n == 1) /* DO NOTHING FOR NOW, probs the best solution rn */;
				else {
					perfmsg.perf = n;
					msgsnd(mqid,&perfmsg,sizeof(perfmsg.perf),0);
					setbit(n);
					block->pcs[pind].tested++;
				}
			}
		}

			n++;
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		perror("usage");
		exit(1);
	}

	if ((shmid=shmget(SHMKEY,sizeof(block),0660)) == -1) { /* Get shared memory ID */
		perror("shmget");
		exit(errno);
	}
	if ((block=((shm *)shmat(shmid,NULL,0))) == (shm *)NULL) { /* Map to address space */
		perror("shmat");
		exit(errno);
	}

	block->bitpt->bits[0] |= 1;

	if ((mqid=msgget(MQKEY,IPC_CREAT | 0660)) == -1) { /* Create message queue if necessary */
		perror("msgget");
		exit(errno);
	}

	procmsg.type = PROCMSG;
	procmsg.pid = (int)getpid();
	procmsg.perfs = 0;
	procmsg.tested = 0;
	procmsg.skipped = 0;
	msgsnd(mqid,&procmsg,4*sizeof(int),0); /* Send 'manage' initial process statistics */

	pind = getindex();
	perfmsg.type = PERFMSG;
	perfect(1);

	//perfect(atoi(argv[1]));	/* Assumes that the command line argument will be of type int!!! */

	//sleep(5);
	//removeprocess(procmsg.pid);


}
