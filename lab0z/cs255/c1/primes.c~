#include "header.h"
#include <stdio.h>
#define SetBit(A,k) ( A[(k/32)] |= (1 << (k%32)) )
#define IsPrime(A,k) ( A[(k/32)] & (1 << (k%32)) ) 

void clearAll(int N)
{
	int i;
	for (i = 0; i <= N/32; i++)
		prime[i] = 0;
}

void sieveOfE(int N)
{
	int i, j, k;
	clearAll(N);
	k = 2;
	while (k <= N) {
		for (i = k; i <= N; i++)
			if (IsPrime(prime, i) == 0) 
				break;

		for (j = 2*i; j <= N+1; j += i)
			SetBit(prime, j);

		k = i+1;
	}
}

int countPrimes(int N)
{
	int count, i;
	count = 0;
	for (i=2;i<=N;i++)
		if (IsPrime(prime, i) == 0)
			count++;

	return count;	
}

void printPrimes(int k1, int k2, int N)
{
	int count, i;
	count = 0;
	for (i = 2 ; i <= N; i++)
	{
		if (IsPrime(prime, i) == 0)
		{
			++count;
			if (k1<=count && count<=k2)
				printf("%d ", i);
			else if (count > k2) break;
		}
		
	}	


}



