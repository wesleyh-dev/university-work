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

  if (argc < 2) {
    perror("Usage");
    exit(1);
  }

  if ((pipe(pfd0)) == -1) {
    perror("Pipe #1");
    exit(1);
  }
  if ((pipe(pfd1)) == -1) {
    perror("Pipe #2");
    exit(1);
  }
  if (close(pfd0[1]) == -1) {
    perror("close");
    exit(1);
  }
  if (close(pfd1[0]) == -1) {
    perror("close");
    exit(1);
  }

  pid0 = fork();
  if (pid0 == 0) { /* Sort code */
    dup2(pfd0[0], 0);
    dup2(pfd1[1], 1);
    execlp("sort", "sort", NULL);

  } else if (pid0 > 0) {

    pid1 = fork();
    if (pid1 == 0) {
      close(pfd0[0]);
      close(pfd1[1]);


    } else if (pid1 > 0) {
      waitpid(pid1, &status, 0);
      FILE *input = fopen(argv[1], "r");
      FILE *fd0 = fdopen(0, "w+");
      int c;
      int n = 0;
      while (!feof(input)) {
        c = fgetc(input);
        if (isalpha(c)) {
          putc(tolower(c), fd0);
          n++;
        }
        else if (!isalpha(c)) {
          FILE *pstream = fdopen(pfd0[0], "w+");
          char *buf;
          if (n > 5) {
            if (n > 35) {
              fgets(buf, 35, fd0);
              fputs(buf, pstream);
            }
            fgets(buf, n, fd0);
            fputs(buf, pstream);
          }
          while ( !isalpha(c = fgetc(input)) ) {}
          n = 0;
        }
      }



    }


  }


  return 0;
}
