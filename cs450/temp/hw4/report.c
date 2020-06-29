#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define KEY (key_t)666
#define N 1100000
#define setBit(map, k) ( map[(k/32)] |= (1 << (k%32)) )
#define clearBit(map, k) ( map[(k/32)] &= ~(1 << (k%32)) )	/* Set the kth bit to 0 */
#define testBit(map, k) ( map[(k/32)] & (1 << (k%32)) )	/* Returns 1 if bit tested is 1 */



void updateprocess(int pid) { /* Update a processes statistics */

}

int main(int argc, char argv[]) {
	
	
}
