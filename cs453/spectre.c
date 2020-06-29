#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void f(char *s)
{
	char buf[510];
	strcpy(buf, s);
}


int main(int argc, char *argv[])
{
	if (argc < 2)
		exit(1);

	uid_t uid = getuid();
	uid_t euid = geteuid();

	printf("running as uid = %u, euid = %u\n", uid, euid);

	f(argv[1]);

	return 0;
}

