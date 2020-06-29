#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

#define SHMKEY (key_t)77579
#define MQKEY (key_t)77579
#define BITSPERINT (8*sizeof(int))
#define N 1048576
#define PROCMSG	1
#define PERFMSG 2
#define KILLMSG 3
#define LIMIT	20
#define MAGIC 33554432

typedef struct _proc { /* Process with statistics */
	int pid;
	int perfs;
	int tested;
	int skipped;
} proc;

typedef struct _shm { /* Bitmap + array + processes */
	int bitmap[N];
	int perfs[20];
	proc pcs[20];
} shm;

int pind;
int n;
int shmid;
int mqid;
int mypid;
shm *block;

int getindex() { /* Retrieve index of this process in the shared memory block's pcs array */
	int id = 0;
	while (id<LIMIT) {
		if (block->pcs[id].pid == mypid) { return id; }
		id++;
	}
	return -1;
}

int whichint(int j) {	/* Which element within bits[]? */
	int i=(j-1)/BITSPERINT;
	return i;
}

int whichbit(int j) {   /* Which bit within bits[x]? */
	int b=(j-1)%BITSPERINT;
	return b;
}

void setbit(int j) {    /* Set the jth bit of bitmap */
	int i=whichint(j);
	int pos=whichbit(j);
	block->bitmap[i] |= (1<<pos);
}

int testbit(int j) {    /* True if jth bit == 1, false means jth bit has not been tested */
	int i=whichint(j);
	int pos=whichbit(j);
	if ((block->bitmap[i] & (1<<pos)))
		return 1;

	return 0;
}

void removeprocess(int pid) { /* Remove the process from shared table and terminate */
	block->pcs[pind].pid = 0;
	block->pcs[pind].perfs = 0;
	block->pcs[pind].tested = 0;
	block->pcs[pind].skipped = 0;
	raise(SIGHUP);
}

void sendprocmsg(int pid) { /* Tell manager that a new process has started, manager then files the process away or kills it with fire */
	struct {
		long type;
		int pid;
	} procmsg;
	procmsg.type = PROCMSG;
	procmsg.pid = pid;
	if (msgsnd(mqid,&procmsg,sizeof(procmsg.pid),0) == -1) {
		perror("msgsnd");
		exit(errno);
	} /* Send 'manage' initial process statistics */
}

void sendperfmsg(int n) { /* Tell manager that a perfect number has been found and update statistics */
	struct {
		long type;
		int perf;
	} perfmsg;
	perfmsg.type = PERFMSG;
	perfmsg.perf = n;
	if (msgsnd(mqid,&perfmsg,sizeof(perfmsg.perf),0) == -1) {
		perror("msgsnd");
		exit(errno);
	}
}

void killmyself() { /* Hit the starting point remove process and terminate */
	removeprocess(mypid);
	if (shmdt(block) == -1) {
		perror("shmdt");
		exit(errno);
	}
	raise(SIGHUP);
}

void perfect(int start) { /* Test if number is perfect */

	int i, sum;
	n = start;
	while (1) {
		if (n == MAGIC) { /* Wrap around to the start */
			n=1;
			continue;
		}
		sum = 1;
		if (!testbit(n)) { /* Number has NOT been tested */
			for (i=2;i*i<=n;i++){
				if (n%i == 0) {
					sum = sum + i + n/i;
				}
			}
			if (sum == n && n != 1) { /* Perfect number has been found */
				sendperfmsg(n);
				block->pcs[pind].perfs++;
			}
			setbit(n);
			block->pcs[pind].tested++;
		} else { /* Number has been tested, so skip! */
			block->pcs[pind].skipped++;
		}
		// if (n == start && (block->pcs[pind].tested > 0 || block->pcs[pind].skipped > 0) ) {
		// 	killmyself();
		// }
		n++;
	}
}



int main(int argc, char *argv[]) {

	if (argc == 1) {
		perror("usage");
		raise(SIGHUP);
	}

	size_t size = (size_t)( N*sizeof(int) + 20*sizeof(int) + 20 *sizeof(proc) );
	if ((shmid=shmget(SHMKEY,size,0666)) == -1) { /* Get shared memory ID */
		perror("shmget");
		exit(errno);
	}
	if ((block=((shm *)shmat(shmid,0,0))) == (shm *)NULL) { /* Map to address space */
		perror("shmat");
		exit(errno);
	}

	if ((mqid=msgget(MQKEY,0666)) == -1) { /* Create message queue if necessary */
		perror("msgget");
		exit(errno);
	}

	mypid = (int)getpid();
	sendprocmsg(mypid);

	pind = getindex();
	perfect((int)atoi(argv[1]));

}
