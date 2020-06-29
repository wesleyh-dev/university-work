/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING 
A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - WESLEY HARMON
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct _seg {
        
	int bits[256];
        struct _seg *next,*prev;
}seg;

#define BITSPERSEG (8*256*sizeof(int))
#define SIZEOFBITS (256)
#define BITSPERINT (8*sizeof(int))

seg *head, *glbptr;
int N;

// Given and odd integer, determine which int within bits[] it corresponds to
int whichint(int j) {

        int wi = ((j/2)%BITSPERSEG)/BITSPERINT;
        return wi;
}

// Given an odd integer, determine which bit it corresponds to within bits[x]
int whichbit(int j) {

        int wb = ((j/2)%BITSPERSEG)%BITSPERINT;
        return wb;
}

// Given an odd integer, determine which segment it is in
seg* whichseg(int j){

        int segmnt;
        seg *pt;
        pt = head;
	segmnt = (j/2)/BITSPERINT;
        while(segmnt >= 256){
                pt = pt->next;
                segmnt -= 256;
        }
        return pt;
}

// Pass values one by one up to sqrtN
// Use input j as increment for marking non-primes
int marknonprime(int j) {

        int wi = whichint(j);
        int wb = whichbit(j);
        seg *sgmnt = whichseg(j);
        sgmnt->bits[wi] |= (1<<wb);
        return 0;
}

int testprime(int j) {

        int wi = whichint(j);
        int wb = whichbit(j);
        seg *sgmnt = whichseg(j);
        if((sgmnt->bits[wi] & (1<<wb)))
                return 1;

        return 0;
}

// Set bit maps to 0
void clearbits(seg *head, int segs) {

        int i, j;
        seg *pt;
        pt = head;

        for(i=0; i<segs; i++) {
                for(j=0; j<SIZEOFBITS; j++) {
                        pt->bits[j] = 0;
                }
                pt = pt->next;
        }
}

void goldbach() {

	int k, gi, sol;
	int lo, hi;
	while(scanf("%d", &k) != EOF) {
		sol = 0;
		gi = 3;
		while(gi <= k/2) {
			if(testprime(gi)==0) {
				if(testprime(k-gi)==0) {
					lo = gi;
					hi = k-gi;
					sol++;
				}
			}
			if(gi>=(k/2)-1) break;
			gi+=2;

		}
		printf("Largest %d = %d + %d out of %d solutions\n", k, lo, hi, sol);
	}
}

void main(int argc, char *argv[]) {

        seg *pt, *previous;
        int howmany, sqrtN, hm;

        if(argc == 2) sscanf(argv[1],"%d",&howmany);
        else scanf("%d",&howmany);

        // Find sqrt of input argument and remember it
        // Save original input as 'N'
        N = howmany;
        sqrtN = (int)(sqrt(howmany));

        // Calculate # of segments and allocate memory
        howmany = (howmany + BITSPERSEG-1) / BITSPERSEG;
        head = (seg *) malloc(sizeof(seg));
        pt = head;
        previous = head;
        for (hm=1; hm<howmany; hm++) {
                pt->next = (seg *) malloc(sizeof(seg));
                if(pt == head) {
                          pt = pt->next;
                          continue;
                }
                pt->prev = previous;
                pt = pt->next;
                previous = previous->next;
        }

        printf("Done allocating %d nodes \n", hm);

        // Set the full bitmap to all 0s
        pt = head;
        clearbits(pt, howmany);

        // Mark multiples of primes up through sqrtN
        int mi, mj;
        int count = 0;
        for(mi=3; mi<=sqrtN; mi+=2) {
                if (!testprime(mi)) {
			for(int mj=3; mj*mi<=N; mj+=2) {
                                marknonprime(mj*mi);
                        }
                }
        }

	// Count primes less than or equal N
        for(int i=3; i<=N; i+=2) {
                if (!testprime(i)) count++;
        }
        printf("The number of odd primes less than or equal to N is: %d\n", count);

	// Decomposition of even integers starts here and continues until EOF
	goldbach();


}
