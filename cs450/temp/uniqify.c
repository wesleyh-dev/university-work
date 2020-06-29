#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define SIZEOFPIPE 35

int main (int argc, char* argv[]) {

  int pfd0[2];
  int pfd1[2];
  int pid0;
  int pid1;
  int status;

  if ((pipe(pfd0)) == -1) {
    perror("Pipe #1");
    exit(1);
  }
  if ((pipe(pfd1)) == -1) {
    perror("Pipe #2");
    exit(1);
  }
  // if (close(pfd0[1]) == -1) {
  //   perror("close");
  //   exit(1);
  // }
  //
  // if (close(pfd1[0]) == -1) {
  //   perror("close");
  //   exit(1);
  // }

  pid0 = fork();
  if (pid0 == 0) { /* Sort code */
      dup2(pfd0[0], 0);
      dup2(pfd1[1], 1);
      execlp("sort", "sort", (char *) NULL);
  }

  pid1 = fork();
  if (pid1 == 0) { 	/* Suppress code */
     close(pfd0[0]);
     close(pfd1[1]);
     FILE *p1 = fdopen(1, "w+");
     char *buf;
     char *prev;
     int n = 1;
     while ((buf = fgets(buf, SIZEOFPIPE, p1))) {
       if (strcmp(buf, prev) == 0) {
         n++;
       } else {
         printf("%d %s", n, buf);
         n = 1;
       }
       strcpy(prev, buf);
     }

  } else if (pid1 > 0) {
  	int c;
  	int n = 0;
  	FILE *wrtstream;
  	FILE *rdstream;
  	wrtstream = fdopen(1, "w+");
  	while ((c = getc(stdin)) != EOF) {
  		if (isalpha(c)) {
  			if (n >= 35) continue;
  			putc(tolower(c),wrtstream);
  			n++;
  		}
  		else if (!isalpha(c)) {
  			if (n <= 5) {
  				n = 0;
  				continue;
  			} else {
  				char *buf = fgets(buf, n, wrtstream);
          rdstream = fdopen(pfd0[0], "w");
          strcat(buf, "\n");
  				printf("%s", buf);
  				fputs(buf, rdstream);
  				n = 0;
  			}
  			wrtstream = fdopen(pfd0[1], "w+");
  		}
  	}
  }

  return 0;
}
