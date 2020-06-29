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
shm *block;

void clearmap() {
	int i = 0;
	while (i<N) {
		block->bitmap[i] = 0;
		i++;
	}
}

void clearperfs() { /* Clear the array of perfect numbers just in case :D */
	int i = 0;
	while(i<LIMIT) {
		block->perfs[i] = 0;
		i++;
	}
}

void clearprocs() { /* Clear the array of perfect numbers just in case :D */
	int i = 0;
	while(i<LIMIT) {
		block->pcs[i].pid = 0;
		block->pcs[i].tested = 0;
		block->pcs[i].skipped = 0;
		block->pcs[i].perfs = 0;
		i++;
	}
}

void addperfectnum(int perf) { /* Add a perfect number to found array using insertion sort */
	int i = 0;
	int j;
	int tmp;
	if (!block->perfs[i]) {
		block->perfs[i] = perf;
		return;
	}
	i = 1;
	while (1) {
		if (block->perfs[i]) i++;
		else break;
	}

	while ((i > 0) && (perf < block->perfs[i-1])) {
		block->perfs[i] = block->perfs[i-1];
		i--;
	}
	block->perfs[i] = perf;
}

int checkprocess() { /* Return -1 if process table is full, index of free position if not full */
	int i = 0;
	while(i<LIMIT) {
		if (!block->pcs[i].pid) return i;
		i++;
	}
	return -1;
}

void addnewprocess(int index, int pid) { /* Add a new 'compute' process to array to given position */
	proc new;
	new.pid = pid;
	new.perfs = 0;
	new.tested = 0;
	new.skipped = 0;
	block->pcs[index] = new;
}

void killmyself() { /* Deallocate shared memory then end this bitch for good */
	if (shmdt(block) == -1) {
		perror("shmdt");
		exit(errno);
	}
	if (shmctl(shmid,IPC_RMID,0) == -1) {
		perror("shmctl");
		exit(errno);
	}
	raise(SIGHUP);
}

void killall() { /* Got the kill command from report, send SIGINT to all open processes then killmyself */
	int i = 0;
	while (i<LIMIT) {
		if (block->pcs[i].pid) {
			kill(block->pcs[i].pid,SIGINT);
		}
		i++;
	}
	sleep(5);
	killmyself();
}

int main() {

	int shmid;
	int mqid;

	size_t size = (size_t)( N*sizeof(int) + 20*sizeof(int) + 20 *sizeof(proc) );
	if ((shmid=shmget(SHMKEY,size,IPC_CREAT | 0666)) == -1) { /* Allocate shared memory */
		perror("shmget");
		exit(errno);
	}
	if ((block=((shm *)shmat(shmid,0,0))) == (shm *)NULL) { /* Map to address space */
		perror("shmat");
		exit(errno);
	}
	if ((mqid=msgget(MQKEY,IPC_CREAT | 0666)) == -1) { /* Create message queue if necessary */
		perror("msgget");
		exit(errno);
	}

	clearmap();
	//clearperfs();

	while (1) {
		struct {
			long type;
			int buf;
		} msg;
		if (msgrcv(mqid,&msg,sizeof(msg.buf),0,0) == -1) {
			perror("msgrcv");
			exit(errno);
		} /* Message received */
		if (msg.type == PROCMSG) { /* Handle a process-related message */
			int newpid = msg.buf;
			int chk = checkprocess();
			if (chk >= 0) {
				addnewprocess(chk,newpid);
			} else {
				printf("PROCESS TABLE IS FULL, HANGING UP...\n");
				pid_t p2kill = newpid;
				kill(p2kill, SIGHUP);
			}
		}
		if (msg.type == PERFMSG) { /* Handle a perfect-related message */
			int newperf = msg.buf;
			addperfectnum(newperf);
		}
		if (msg.type == KILLMSG) { /* Got the kill all message from report */
			killall();
		}
	}
}

