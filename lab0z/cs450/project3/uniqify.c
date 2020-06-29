#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define SIZEOFPIPE 35

int main () {

	FILE *srt;
	FILE *sup;
	int pfd0[2];
	int pfd1[2];
	char c[1];
	int n;
	int cnt;
	int status;
	char* buf;
	char* supbuf;
	char* cur;
	char* nxt;
		
	if (pipe(pfd0) == -1) {
		perror("pipe");
		exit(3);
	}
	if (pipe(pfd1) == -1) {
		perror("pipe");
		exit(3);
	}

	if (fork()) 
	{
		if (fork()) 
		{
			close(pfd0[0]);
			buf = (char*)malloc(sizeof(char)*SIZEOFPIPE);
			n = 0;
			strcpy(buf, "");
			/* Appears to write words of appropriate size to the pfd0[1] which is what we want */
			while ((c[0] = getc(stdin)) != EOF) 
			{
				if (isalpha(c[0])) {
					if (n >= 35) continue;
					c[0] = tolower(c[0]);
					strcat(buf, c);
					n++;
				} else if (!isalpha(c[0])) {
					if (n >= 5) {
						srt = fdopen(pfd0[1], "w");
						strcat(buf, "\n");
						if (strlen(buf) > 0) fputs(buf, srt);
						n = 0;
						printf("Length: %ld String: %s", strlen(buf), buf);
					}
					buf = (char*)malloc(sizeof(char)*SIZEOFPIPE);
				}
			}
			fclose(srt);
			close(pfd0[1]);

		} else 
		{
			// Suppress child code
			
			close(pfd1[1]);
			FILE *supstr = fdopen(pfd1[0], "r");
			char b[35];
			fgets(b, 35, supstr);
			printf("Length of b is: %ld", strlen(b));
			puts(b);
		}

	} else 
	{	
		dup2(pfd0[0], 0);
		close(pfd0[0]);
		dup2(pfd1[1], 1);
		close(pfd1[1]);		
		execl("sort", "sort", NULL);
		
	}

	wait(NULL);
	return 0;




}
