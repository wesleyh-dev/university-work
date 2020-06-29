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

#define SHMKEY        (key_t)77579
#define MQKEY         (key_t)77579

#define BITSPERSEG    (8*256*sizeof(int))
#define BITSPERINT    (8*sizeof(int))
#define SIZEOFBITS    (256)

#define N             (int)( (pow(2,25)) / 32)

#define PROCMSG	      1
#define PERFMSG       2
#define RMPROCMSG     3

#define LIMIT	        20

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
