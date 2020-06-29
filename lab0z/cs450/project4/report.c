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
#include <string.h>

#define SHMKEY (key_t)77579
#define MQKEY (key_t)77579
#define BITSPERINT (8*sizeof(int))
#define N 1048576
#define PROCMSG	1
#define PERFMSG 2
#define KMSG 50
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

int shmid;
int mqid;
shm *block;

void report() {
	int i = 0;
	while (block->perfs[i]) {
		printf("%d\n", block->perfs[i]);
		i++;
	}
}

void sendkillcall(int i) {
	report();
	struct {
		long type;
		int m;
	} msg;
	msg.type = KMSG;
	msg.m = i;
	if ( msgsnd(mqid, &msg, sizeof(int), 0) == -1) {
		perror("msgsnd");
		exit(errno);
	}
	if (shmdt(block) == -1) {
		perror("shmdt");
		exit(errno);
	}
	raise(SIGHUP);
}

int main(int argc, char *argv[]) {

	int shmid;
	int mqid;

	size_t size = (size_t)( N*sizeof(int) + 20*sizeof(int) + 20 *sizeof(proc) );
	if ((shmid=shmget(SHMKEY,size,0666)) == -1) { /* Allocate shared memory */
		perror("shmget");
		exit(errno);
	}
	if ((block=((shm *)shmat(shmid,0,0))) == (shm *)NULL) { /* Map to address space */
		perror("shmat");
		exit(errno);
	}
	if ((mqid=msgget(MQKEY,0666)) == -1) { 
		perror("msgget");
		exit(errno);
	}

	if (argc == 1)  /* Perform a simple report */
		report();

	if (argc == 2) {
		int tst = strcmp(argv[1], "-k");
		if (tst == 1 || tst == -1) {
			perror("usage");
			raise(SIGHUP);
		} else {
			sendkillcall(0);
		}
	}


}

